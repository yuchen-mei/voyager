#pragma once

#include "test/toolchain/Common.h"

void map_matrix_vector_multiply(const codegen::Operation& param,
                                std::deque<BaseParams*>& mapped_params) {
  const auto op_list = get_op_list(param);
  const auto matrix_op = op_list[0];

  const auto input = matrix_op.kwargs().at("input").tensor();
  const auto output = get_op_outputs(param).back();

  bool is_matmul = matrix_op.target().find("matmul") != std::string::npos;
  std::string key = is_matmul ? "other" : "weight";
  const auto weight = matrix_op.kwargs().at(key).tensor();
  bool has_bias = matrix_op.kwargs().contains("bias");

  const auto weight_shape = get_shape(weight);
  int output_dim = weight_shape[0];
  int reduction_dim = weight_shape[1];

  int packing_factor = OC_DIMENSION / VECTOR_UNIT_WIDTH;

  VectorParams* vector_params = new VectorParams;
  VectorInstructionConfig* vector_instruction_config =
      new VectorInstructionConfig;

  // input is a vector of size reduction_dim
  vector_params->vector_fetch_0_offset = get_address(input);
  vector_params->vector_fetch_0_mode = 2;
  vector_params->vector_fetch_0_broadcast = 0b010000;

  int input_dtype = get_index_from_type_name<VU_INPUT_TYPES>(input.dtype());
  int input_dtype_width = get_type_width<VU_INPUT_TYPES>(input_dtype);
  int input_fetch_width = OC_DIMENSION * input_dtype_width;

  vector_params->vector_fetch_0_dtype = input_dtype;
  vector_params->vector_fetch_0_stride = OC_DIMENSION;
  vector_params->vector_fetch_0_burst_size = input_fetch_width / 8;
  vector_params->vector_fetch_0_num_beats = input_fetch_width / OC_PORT_WIDTH;
  vector_params->vector_fetch_0_packing_factor = packing_factor;

  for (int i = 0; i < 3; i++) {
    vector_params->vector_fetch_0_loops[0][i] = 1;
  }
  vector_params->vector_fetch_0_loops[1][0] = 1;
  vector_params->vector_fetch_0_loops[1][1] = output_dim;
  vector_params->vector_fetch_0_loops[1][2] = reduction_dim / OC_DIMENSION;

  // weight is a matrix of output_dim x reduction_dim
  vector_params->vector_fetch_1_offset = get_address(weight);
  vector_params->vector_fetch_1_mode = true;

  int weight_dtype = get_index_from_type_name<VU_INPUT_TYPES>(weight.dtype());
  int weight_dtype_width = get_type_width<VU_INPUT_TYPES>(weight_dtype);
  int weight_fetch_width = OC_DIMENSION * weight_dtype_width;

  vector_params->vector_fetch_1_dtype = weight_dtype;
  vector_params->vector_fetch_1_stride = OC_DIMENSION;
  vector_params->vector_fetch_1_burst_size = weight_fetch_width / 8;
  vector_params->vector_fetch_1_num_beats = weight_fetch_width / OC_PORT_WIDTH;
  vector_params->vector_fetch_1_packing_factor = packing_factor;

  for (int i = 0; i < 3; i++) {
    vector_params->vector_fetch_1_loops[0][i] = 1;
  }
  vector_params->vector_fetch_1_loops[1][0] = 1;
  vector_params->vector_fetch_1_loops[1][1] = output_dim;
  vector_params->vector_fetch_1_loops[1][2] = reduction_dim / OC_DIMENSION;

  for (int i = 0; i < 2; i++) {
    vector_params->vector_fetch_1_y_loop_idx[i] = 0;
    vector_params->vector_fetch_1_x_loop_idx[i] = 1;
    vector_params->vector_fetch_1_k_loop_idx[i] = 2;
  }

  // bias
  if (has_bias) {
    const auto bias = matrix_op.kwargs().at("bias").tensor();
    vector_params->vector_fetch_2_offset = get_address(bias);
    vector_params->vector_fetch_2_mode = true;
    vector_params->vector_fetch_2_broadcast = 0b011;

    const int bias_dtype =
        get_index_from_type_name<VU_INPUT_TYPES>(bias.dtype());
    const int bias_dtype_width = get_type_width<VU_INPUT_TYPES>(bias_dtype);
    const int bias_fetch_width = OC_DIMENSION * bias_dtype_width;

    vector_params->vector_fetch_2_dtype = bias_dtype;
    vector_params->vector_fetch_2_stride = OC_DIMENSION;
    vector_params->vector_fetch_2_burst_size = bias_fetch_width / 8;
    vector_params->vector_fetch_2_num_beats = bias_fetch_width / OC_PORT_WIDTH;
    vector_params->vector_fetch_2_packing_factor = packing_factor;

    for (int i = 0; i < 3; i++) {
      vector_params->vector_fetch_2_loops[0][i] = 1;
    }
    vector_params->vector_fetch_2_loops[1][0] = 1;
    vector_params->vector_fetch_2_loops[1][1] = 1;
    vector_params->vector_fetch_2_loops[1][2] = output_dim / OC_DIMENSION;

    for (int i = 0; i < 2; i++) {
      vector_params->vector_fetch_2_y_loop_idx[i] = 0;
      vector_params->vector_fetch_2_x_loop_idx[i] = 1;
      vector_params->vector_fetch_2_k_loop_idx[i] = 2;
    }
  }

  // output
  vector_params->vector_output_offset = get_address(output);
  vector_params->output_dtype =
      get_index_from_type_name<OUTPUT_DATATYPES>(output.dtype());

  for (int i = 0; i < 3; i++) {
    vector_params->output_loops[0][i] = 1;
  }
  vector_params->output_loops[1][0] = 1;
  vector_params->output_loops[1][1] = 1;
  vector_params->output_loops[1][2] = output_dim / OC_DIMENSION;

  for (int i = 0; i < 2; i++) {
    vector_params->output_y_loop_idx[i] = 0;
    vector_params->output_x_loop_idx[i] = 1;
    vector_params->output_k_loop_idx[i] = 2;
  }

  // Start reduction engine
  VectorInstructions inst0;
  inst0.op_type = VectorInstructions::reduction;
  inst0.inst_loop_count = VECTOR_UNIT_WIDTH / REDUCER_WIDTH;
  inst0.reduce_count = reduction_dim / REDUCER_WIDTH;
  inst0.reduce_op = VectorInstructions::radd;
  inst0.rdest = has_bias ? VectorInstructions::to_pipeline
                         : VectorInstructions::to_memory;
  vector_instruction_config->inst[0] = inst0;

  // Multiply input vector with weight matrix. Reducer takes
  // reduction_dim / OC_DIMENSION cycles to do the complete reduction,
  // and OC_DIMENSION cycles to fill up the entire vector
  VectorInstructions inst1;
  inst1.op_type = VectorInstructions::vector;
  inst1.inst_loop_count = reduction_dim;
  inst1.vector_op0_src0 = VectorInstructions::from_vector_fetch_0;
  inst1.vector_op0_src1 = VectorInstructions::from_vector_fetch_1;
  inst1.vector_op0 = VectorInstructions::op0_mul;
  inst1.vdest = VectorInstructions::to_reduce;
  vector_instruction_config->inst[1] = inst1;

  // inst2 - add bias, write out
  if (has_bias) {
    VectorInstructions inst2;
    inst2.op_type = VectorInstructions::vector;
    inst2.inst_loop_count = 1;
    inst2.vector_op0_src0 = VectorInstructions::from_vector_reducer;
    inst2.vector_op2_src1 = VectorInstructions::from_vector_fetch_2;
    inst2.vector_op2 = VectorInstructions::op2_add;
    inst2.vdest = VectorInstructions::to_output;
    vector_instruction_config->inst[2] = inst2;
  }

  vector_instruction_config->num_inst = has_bias ? 3 : 2;
  vector_instruction_config->config_loop_count = output_dim / VECTOR_UNIT_WIDTH;

  mapped_params.push_back(vector_params);
  mapped_params.push_back(vector_instruction_config);
}
