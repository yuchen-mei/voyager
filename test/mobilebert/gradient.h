#pragma once

#include <map>
#include <string>
#include <vector>

#include "test/common/VerificationTypes.h"

std::map<std::string, std::string> gradientParamsMapping{
    {"classifier_weight", "classifier_weight"},
    {"classifier_bias", ""},
    {"output_bottleneck_LayerNorm_weight", "outputLayerNorm"},
    {"output_bottleneck_LayerNorm_bias", ""},
    {"output_bottleneck_dense_weight", "outputBottleneck"},
    {"output_bottleneck_dense_bias", ""},
    {"output_LayerNorm_weight", "bottleneckLayerNorm"},
    {"output_LayerNorm_bias", ""},
    {"output_dense_weight", "ffn2"},
    {"output_dense_bias", ""},
    {"intermediate_dense_weight", "outputBottleneck"},
    {"intermediate_dense_bias", ""},
    {"ffn_2_output_LayerNorm_weight", "bottleneckLayerNorm"},
    {"ffn_2_output_LayerNorm_bias", ""},
    {"ffn_2_output_dense_weight", "ffn2"},
    {"ffn_2_output_dense_bias", ""},
    {"ffn_2_intermediate_dense_weight", "outputBottleneck"},
    {"ffn_2_intermediate_dense_bias", ""},
    {"ffn_1_output_LayerNorm_weight", "bottleneckLayerNorm"},
    {"ffn_1_output_LayerNorm_bias", ""},
    {"ffn_1_output_dense_weight", "ffn2"},
    {"ffn_1_output_dense_bias", ""},
    {"ffn_1_intermediate_dense_weight", "outputBottleneck"},
    {"ffn_1_intermediate_dense_bias", ""},
    {"ffn_0_output_LayerNorm_weight", "bottleneckLayerNorm"},
    {"ffn_0_output_LayerNorm_bias", ""},
    {"ffn_0_output_dense_weight", "ffn2"},
    {"ffn_0_output_dense_bias", ""},
    {"ffn_0_intermediate_dense_weight", "outputBottleneck"},
    {"ffn_0_intermediate_dense_bias", ""},
    {"attention_output_LayerNorm_weight", "bottleneckLayerNorm"},
    {"attention_output_LayerNorm_bias", ""},
    {"attention_output_dense_weight", "outputAttention"},
    {"attention_output_dense_bias", ""},
    {"attention_self_value_weight", "vProjection"},
    {"attention_self_value_bias", ""},
    {"attention_self_query_weight", "qkProjection"},
    {"attention_self_query_bias", ""},
    {"attention_self_key_weight", "qkProjection"},
    {"attention_self_key_bias", ""},
    {"bottleneck_attention_LayerNorm_weight", "bottleneckLayerNorm"},
    {"bottleneck_attention_LayerNorm_bias", ""},
    {"bottleneck_attention_dense_weight", "inputBottleneck"},
    {"bottleneck_attention_dense_bias", ""},
    {"bottleneck_input_LayerNorm_weight", "bottleneckLayerNorm"},
    {"bottleneck_input_LayerNorm_bias", ""},
    {"bottleneck_input_dense_weight", "inputBottleneck"},
    {"bottleneck_input_dense_bias", ""},
};

std::map<std::string, SimplifiedParams> gradientParams{
    // FIXME: This should be a FC or matmul?
    // (16 x 1) * (1 x 512)
    {"classifier_weight",
     {
         0,                                          // INPUT_OFFSET
         0,                                          // WEIGHT_OFFSET
         10 * 1024,                                  // OUTPUT_OFFSET
         false,                                      // TRANSPOSE
         {{1, 1, 1, 1, 1, 1}, {1, 32, 1, 1, 1, 1}},  // LOOPS
         {0, 5},                                     // INPUTX
         {1, 4},                                     // INPUTY
         {3, 0},                                     // REDUCTION
         {2, 1},                                     // WEIGHT
         3,                                          // fxIndex
         2,                                          // fyIndex
         {5, 5},                                     // weightReuseIndex
         1,                                          // stride
         false,                                      // replication
         false,                                      // RELU
         false,                                      // bias
         30 * 1024,                                  // BIAS_OFFSET
         false,                                      // residual
         40 * 1024,                                  // RESIDUAL_OFFSET
         false,                                      // maxpool
         false,                                      // avgpool
         false,                                      // SOFTMAX
         false,                                      // FC
         false,                                      // NO_NORM
         false,                                      // weight
         false,                                      // ATTENTION_SCALING
         false,                                      // STORE_IN_ACC
         false,                                      // ACC_FROM_ACC
         false,                                      // INPUT_TRANSPOSE
         false,                                      // SPLIT_HEAD
         false,                                      // CONCAT_HEAD
         false,                                      // SOFTMAX_GRAD
         false,                                      // NO_NORM_GRAD
         false,                                      // RELU_GRAD
         false,                                      // WEIGHT_PERMUTE
         false,                                      // WEIGHT_ADDITION,
         false,                                      // CROSS_ENTROPY_LOSS_GRAD
         false,                                      // MSE_LOSS_GRAD
         false,                                      // BCE_LOGITS_LOSS_GRAD
         false,                                      // GRADIENT_CLIPPING
     }},

    // elementwise multiplication of matrices of size:
    // (128 x 512) * (128 x 512)
    {"outputLayerNorm",
     {
         0,                                           // INPUT_OFFSET
         0,                                           // WEIGHT_OFFSET
         10 * 1024,                                   // OUTPUT_OFFSET
         false,                                       // TRANSPOSE
         {{4, 1, 1, 8, 1, 1}, {32, 1, 1, 1, 1, 32}},  // LOOPS
         {0, 5},                                      // INPUTX
         {1, 4},                                      // INPUTY
         {2, 0},                                      // REDUCTION
         {3, 1},                                      // WEIGHT
         3,                                           // fxIndex
         2,                                           // fyIndex
         {4, 5},                                      // weightReuseIndex
         1,                                           // stride
         false,                                       // replication
         false,                                       // RELU
         false,                                       // bias
         30 * 1024,                                   // BIAS_OFFSET
         false,                                       // residual
         40 * 1024,                                   // RESIDUAL_OFFSET
         false,                                       // maxpool
         false,                                       // avgpool
         false,                                       // SOFTMAX
         false,                                       // FC
         false,                                       // NO_NORM
         false,                                       // weight
         false,                                       // ATTENTION_SCALING
         false,                                       // STORE_IN_ACC
         false,                                       // ACC_FROM_ACC
         false,                                       // INPUT_TRANSPOSE
         false,                                       // SPLIT_HEAD
         false,                                       // CONCAT_HEAD,
         false,                                       // SOFTMAX_GRAD
         true,                                        // NO_NORM_GRAD
     }},

    // output bottleneck
    // (128 x 128) x (128 x 512)
    {"outputBottleneck",
     {
         131072,                                     // INPUT_OFFSET
         225280,                                     // WEIGHT_OFFSET
         0,                                          // OUTPUT_OFFSET
         false,                                      // TRANSPOSE
         {{8, 1, 8, 1, 1, 1}, {8, 4, 1, 1, 1, 16}},  // LOOPS
         {0, 5},                                     // INPUTX
         {1, 4},                                     // INPUTY
         {3, 0},                                     // REDUCTION
         {2, 1},                                     // WEIGHT
         3,                                          // fxIndex
         2,                                          // fyIndex
         {5, 5},                                     // weightReuseIndex
         1,                                          // stride
         false,                                      // replication
         false,                                      // RELU
         false,                                      // bias
         30 * 1024,                                  // BIAS_OFFSET
         false,                                      // residual
         40 * 1024,                                  // RESIDUAL_OFFSET
         false,                                      // maxpool
         false,                                      // avgpool
         false,                                      // SOFTMAX
         false,                                      // FC
         false,                                      // NO-NORM
         false,                                      // weight
         false,                                      // ATTENTION_SCALING
         false,                                      // STORE_IN_ACC
         false,                                      // ACC_FROM_ACC
         true,                                       // INPUT_TRANSPOSE
         false,                                      // SPLIT_HEAD
         false,                                      // CONCAT_HEAD
     }},

    // FFN 2
    // (512 x 128) x (128 x 128)
    {"ffn2",
     {
         0,                                           // INPUT_OFFSET
         159744,                                      // WEIGHT_OFFSET
         131072,                                      // OUTPUT_OFFSET
         false,                                       // TRANSPOSE
         {{16, 1, 1, 1, 1, 1}, {8, 8, 1, 1, 1, 32}},  // LOOPS
         {0, 5},                                      // INPUTX
         {1, 4},                                      // INPUTY
         {3, 0},                                      // REDUCTION
         {2, 1},                                      // WEIGHT
         3,                                           // fxIndex
         2,                                           // fyIndex
         {5, 5},                                      // weightReuseIndex
         1,                                           // stride
         false,                                       // replication
         false,                                       // RELU
         false,                                       // bias
         30 * 1024,                                   // BIAS_OFFSET
         false,                                       // residual
         40 * 1024,                                   // RESIDUAL_OFFSET
         false,                                       // maxpool
         false,                                       // avgpool
         false,                                       // SOFTMAX
         false,                                       // FC
         false,                                       // NO-NORM
         false,                                       // weight
         false,                                       // ATTENTION_SCALING
         false,                                       // STORE_IN_ACC
         false,                                       // ACC_FROM_ACC
         true,                                        // INPUT_TRANSPOSE
         false,                                       // SPLIT_HEAD
         false,                                       // CONCAT_HEAD
     }},

    {"outputAttention",
     {
         0,                                          // INPUT_OFFSET
         77824,                                      // WEIGHT_OFFSET
         131072,                                     // OUTPUT_OFFSET
         false,                                      // TRANSPOSE
         {{4, 1, 4, 1, 1, 1}, {8, 2, 1, 1, 1, 32}},  // LOOPS
         {0, 5},                                     // INPUTX
         {1, 4},                                     // INPUTY
         {3, 0},                                     // REDUCTION
         {2, 1},                                     // WEIGHT
         3,                                          // fxIndex
         2,                                          // fyIndex
         {5, 5},                                     // weightReuseIndex
         1,                                          // stride
         false,                                      // replication
         false,                                      // RELU
         false,                                      // bias
         30 * 1024,                                  // BIAS_OFFSET
         false,                                      // residual
         40 * 1024,                                  // RESIDUAL_OFFSET
         false,                                      // maxpool
         false,                                      // avgpool
         false,                                      // SOFTMAX
         false,                                      // FC
         false,                                      // NO-NORM
         false,                                      // weight
         false,                                      // ATTENTION_SCALING
         false,                                      // STORE_IN_ACC
         false,                                      // ACC_FROM_ACC
         true,                                       // INPUT_TRANSPOSE
         false,                                      // SPLIT_HEAD
         false,                                      // CONCAT_HEAD
         false,                                      // SOFTMAX_GRAD;
         false,                                      // NO_NORM_GRAD;
         false,                                      // RELU_GRAD;
         true,                                       // WEIGHT_PERMUTE;
     }},

    // (128 x 128) * (128 x 512)
    {"vProjection",
     {
         0,                                          // INPUT_OFFSET
         0,                                          // WEIGHT_OFFSET
         131072,                                     // OUTPUT_OFFSET
         false,                                      // TRANSPOSE
         {{4, 1, 4, 1, 1, 1}, {8, 8, 1, 1, 1, 32}},  // LOOPS
         {0, 5},                                     // INPUTX
         {1, 4},                                     // INPUTY
         {3, 0},                                     // REDUCTION
         {2, 1},                                     // WEIGHT
         3,                                          // fxIndex
         2,                                          // fyIndex
         {5, 5},                                     // weightReuseIndex
         1,                                          // stride
         false,                                      // replication
         false,                                      // RELU
         false,                                      // bias
         30 * 1024,                                  // BIAS_OFFSET
         false,                                      // residual
         40 * 1024,                                  // RESIDUAL_OFFSET
         false,                                      // maxpool
         false,                                      // avgpool
         false,                                      // SOFTMAX
         false,                                      // FC
         false,                                      // NO-NORM
         false,                                      // weight
         false,                                      // ATTENTION_SCALING
         false,                                      // STORE_IN_ACC
         false,                                      // ACC_FROM_ACC
         true,                                       // INPUT_TRANSPOSE
         false,                                      // SPLIT_HEAD
         true,                                       // CONCAT_HEAD
     }},

    // TODO: Add unit test (no head concatenation)
    // Self-attention output
    // (128 x 128) x (128 x 128)
    {"qkProjection",
     {
         0,                                          // INPUT_OFFSET
         77824,                                      // WEIGHT_OFFSET
         131072,                                     // OUTPUT_OFFSET
         false,                                      // TRANSPOSE
         {{4, 1, 4, 1, 1, 1}, {8, 2, 1, 1, 1, 32}},  // LOOPS
         {0, 5},                                     // INPUTX
         {1, 4},                                     // INPUTY
         {3, 0},                                     // REDUCTION
         {2, 1},                                     // WEIGHT
         3,                                          // fxIndex
         2,                                          // fyIndex
         {5, 5},                                     // weightReuseIndex
         1,                                          // stride
         false,                                      // replication
         false,                                      // RELU
         false,                                      // bias
         30 * 1024,                                  // BIAS_OFFSET
         false,                                      // residual
         40 * 1024,                                  // RESIDUAL_OFFSET
         false,                                      // maxpool
         false,                                      // avgpool
         false,                                      // SOFTMAX
         false,                                      // FC
         false,                                      // NO-NORM
         false,                                      // weight
         false,                                      // ATTENTION_SCALING
         false,                                      // STORE_IN_ACC
         false,                                      // ACC_FROM_ACC
         true,                                       // INPUT_TRANSPOSE
         false,                                      // SPLIT_HEAD
         true,                                       // CONCAT_HEAD
     }},

    // elementwise multiplication of matrices of size:
    // (128 x 128) * (128 x 128)
    {"bottleneckLayerNorm",
     {
         0,                                           // INPUT_OFFSET
         0,                                           // WEIGHT_OFFSET
         10 * 1024,                                   // OUTPUT_OFFSET
         false,                                       // TRANSPOSE
         {{1, 1, 1, 1, 1, 1}, {8, 8, 1, 1, 1, 128}},  // LOOPS
         {0, 5},                                      // INPUTX
         {1, 4},                                      // INPUTY
         {2, 0},                                      // REDUCTION
         {3, 1},                                      // WEIGHT
         3,                                           // fxIndex
         2,                                           // fyIndex
         {4, 5},                                      // weightReuseIndex
         1,                                           // stride
         false,                                       // replication
         false,                                       // RELU
         false,                                       // bias
         30 * 1024,                                   // BIAS_OFFSET
         false,                                       // residual
         40 * 1024,                                   // RESIDUAL_OFFSET
         false,                                       // maxpool
         false,                                       // avgpool
         false,                                       // SOFTMAX
         false,                                       // FC
         false,                                       // NO_NORM
         false,                                       // weight
         false,                                       // ATTENTION_SCALING
         false,                                       // STORE_IN_ACC
         false,                                       // ACC_FROM_ACC
         false,                                       // INPUT_TRANSPOSE
         false,                                       // SPLIT_HEAD
         false,                                       // CONCAT_HEAD
         false,                                       // SOFTMAX_GRAD
         true,                                        // NO_NORM_GRAD
     }},

    // (128 x 128) * (128 x 512)
    {"inputBottleneck",
     {
         0,                                          // INPUT_OFFSET
         0,                                          // WEIGHT_OFFSET
         131072,                                     // OUTPUT_OFFSET
         false,                                      // TRANSPOSE
         {{4, 1, 4, 1, 1, 1}, {8, 8, 1, 1, 1, 32}},  // LOOPS
         {0, 5},                                     // INPUTX
         {1, 4},                                     // INPUTY
         {3, 0},                                     // REDUCTION
         {2, 1},                                     // WEIGHT
         3,                                          // fxIndex
         2,                                          // fyIndex
         {5, 5},                                     // weightReuseIndex
         1,                                          // stride
         false,                                      // replication
         false,                                      // RELU
         false,                                      // bias
         30 * 1024,                                  // BIAS_OFFSET
         false,                                      // residual
         40 * 1024,                                  // RESIDUAL_OFFSET
         false,                                      // maxpool
         false,                                      // avgpool
         false,                                      // SOFTMAX
         false,                                      // FC
         false,                                      // NO-NORM
         false,                                      // weight
         false,                                      // ATTENTION_SCALING
         false,                                      // STORE_IN_ACC
         false,                                      // ACC_FROM_ACC
         true,                                       // INPUT_TRANSPOSE
         false,                                      // SPLIT_HEAD
         false,                                      // CONCAT_HEAD
     }},
};

std::map<std::string, MemoryOffsets> gradientMemOffsets{
    {"classifier_weight",
     {
         0,
         6 * INTERMEDIATE_SIZE + 26 * HIDDEN_SIZE,
         12 * WEIGHT_INTERMEDIATE_SIZE + 7 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 24 * BIAS_HIDDEN_SIZE,
     }},
    {"classifier_bias",
     {
         0,  // unused
         0,
         12 * WEIGHT_INTERMEDIATE_SIZE + 23 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 24 * BIAS_HIDDEN_SIZE,
     }},
    {"output_bottleneck_LayerNorm_weight",
     {
         5 * INTERMEDIATE_SIZE + 26 * HIDDEN_SIZE,
         0,
         12 * WEIGHT_INTERMEDIATE_SIZE + 5 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 24 * BIAS_HIDDEN_SIZE,
     }},
    {"output_bottleneck_LayerNorm_bias",
     {
         0,  // unused
         0,
         12 * WEIGHT_INTERMEDIATE_SIZE + 6 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 24 * BIAS_HIDDEN_SIZE,
     }},
    {"output_bottleneck_dense_weight",
     {
         5 * INTERMEDIATE_SIZE + 25 * HIDDEN_SIZE,
         0,
         11 * WEIGHT_INTERMEDIATE_SIZE + 4 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 24 * BIAS_HIDDEN_SIZE,
     }},
    {"output_bottleneck_dense_bias",
     {
         0,  // unused
         0,
         11 * WEIGHT_INTERMEDIATE_SIZE + 4 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 24 * BIAS_HIDDEN_SIZE,
     }},

    {"output_LayerNorm_weight",
     {
         5 * INTERMEDIATE_SIZE + 24 * HIDDEN_SIZE,
         0,
         11 * WEIGHT_INTERMEDIATE_SIZE + 4 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 22 * BIAS_HIDDEN_SIZE,
     }},
    {"output_LayerNorm_bias",
     {
         0,  // unused
         0,
         11 * WEIGHT_INTERMEDIATE_SIZE + 4 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 23 * BIAS_HIDDEN_SIZE,
     }},
    {"output_dense_weight",
     {
         4 * INTERMEDIATE_SIZE + 24 * HIDDEN_SIZE,
         0,
         10 * WEIGHT_INTERMEDIATE_SIZE + 4 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 21 * BIAS_HIDDEN_SIZE,
     }},
    {"output_dense_bias",
     {
         0,  // unused
         0,
         11 * WEIGHT_INTERMEDIATE_SIZE + 4 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 21 * BIAS_HIDDEN_SIZE,
     }},
    {"intermediate_dense_weight",
     {
         4 * INTERMEDIATE_SIZE + 23 * HIDDEN_SIZE,
         0,
         9 * WEIGHT_INTERMEDIATE_SIZE + 3 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 21 * BIAS_HIDDEN_SIZE,
     }},
    {"intermediate_dense_bias",
     {
         0,
         0,
         10 * WEIGHT_INTERMEDIATE_SIZE + 3 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 21 * BIAS_HIDDEN_SIZE,
     }},
    {"ffn_2_output_LayerNorm_weight",
     {
         4 * INTERMEDIATE_SIZE + 22 * HIDDEN_SIZE,
         0,
         9 * WEIGHT_INTERMEDIATE_SIZE + 3 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 19 * BIAS_HIDDEN_SIZE,
     }},
    {"ffn_2_output_LayerNorm_bias",
     {
         0,
         0,
         9 * WEIGHT_INTERMEDIATE_SIZE + 3 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 20 * BIAS_HIDDEN_SIZE,
     }},
    {"ffn_2_output_dense_weight",
     {
         3 * INTERMEDIATE_SIZE + 22 * HIDDEN_SIZE,
         0,
         8 * WEIGHT_INTERMEDIATE_SIZE + 3 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 18 * BIAS_HIDDEN_SIZE,
     }},
    {"ffn_2_output_dense_bias",
     {
         0,
         0,
         9 * WEIGHT_INTERMEDIATE_SIZE + 3 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 18 * BIAS_HIDDEN_SIZE,
     }},
    {"ffn_2_intermediate_dense_weight",
     {
         3 * INTERMEDIATE_SIZE + 21 * HIDDEN_SIZE,
         0,
         7 * WEIGHT_INTERMEDIATE_SIZE + 2 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 18 * BIAS_HIDDEN_SIZE,
     }},
    {"ffn_2_intermediate_dense_bias",
     {
         0,
         0,
         8 * WEIGHT_INTERMEDIATE_SIZE + 2 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 18 * BIAS_HIDDEN_SIZE,
     }},
    {"ffn_1_output_LayerNorm_weight",
     {
         3 * INTERMEDIATE_SIZE + 20 * HIDDEN_SIZE,
         0,
         7 * WEIGHT_INTERMEDIATE_SIZE + 2 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 16 * BIAS_HIDDEN_SIZE,
     }},
    {"ffn_1_output_LayerNorm_bias",
     {
         0,
         0,
         7 * WEIGHT_INTERMEDIATE_SIZE + 2 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 17 * BIAS_HIDDEN_SIZE,
     }},
    {"ffn_1_output_dense_weight",
     {
         2 * INTERMEDIATE_SIZE + 20 * HIDDEN_SIZE,
         0,
         6 * WEIGHT_INTERMEDIATE_SIZE + 2 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 15 * BIAS_HIDDEN_SIZE,
     }},
    {"ffn_1_output_dense_bias",
     {
         0,
         0,
         6 * WEIGHT_INTERMEDIATE_SIZE + 2 * BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 15 * BIAS_HIDDEN_SIZE,
     }},
    {"ffn_1_intermediate_dense_weight",
     {
         0,
         0,
         5 * WEIGHT_INTERMEDIATE_SIZE + BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 15 * BIAS_HIDDEN_SIZE,
     }},
    {"ffn_1_intermediate_dense_bias",
     {
         2 * INTERMEDIATE_SIZE + 19 * HIDDEN_SIZE,
         0,
         5 * WEIGHT_INTERMEDIATE_SIZE + BIAS_INTERMEDIATE_SIZE +
             3 * WEIGHT_HIDDEN_SIZE + 15 * BIAS_HIDDEN_SIZE,
     }},
    {"ffn_0_output_LayerNorm_weight",
     {
         0,
         2 * INTERMEDIATE_SIZE + 18 * HIDDEN_SIZE,
         INTERMEDIATE_SIZE,
     }},
    {"ffn_0_output_dense",
     {
         0,
         INTERMEDIATE_SIZE + 18 * HIDDEN_SIZE,
         INTERMEDIATE_SIZE,
     }},
    {"ffn_0_intermediate_dense",
     {
         0,
         INTERMEDIATE_SIZE + 17 * HIDDEN_SIZE,
         INTERMEDIATE_SIZE,
     }},

    {"attention_output_LayerNorm",
     {
         0,
         INTERMEDIATE_SIZE + 16 * HIDDEN_SIZE,
         INTERMEDIATE_SIZE,
     }},
    {"attention_output_dense",
     {
         0,
         INTERMEDIATE_SIZE + 15 * HIDDEN_SIZE,
         INTERMEDIATE_SIZE,
     }},

    {"attention_self_value_layer_0",
     {
         0,
         0,
         INTERMEDIATE_SIZE,
     }},
    {"attention_self_key_layer_0",
     {
         0,
         INTERMEDIATE_SIZE + 4 * HIDDEN_SIZE,
         INTERMEDIATE_SIZE,
     }},
    {"attention_self_query_layer_0",
     {
         0,
         INTERMEDIATE_SIZE + 4 * HIDDEN_SIZE,
         INTERMEDIATE_SIZE,
     }},
    {"bottleneck_attention_LayerNorm_1",
     {
         0,
         INTERMEDIATE_SIZE + HIDDEN_SIZE,
         INTERMEDIATE_SIZE,
     }},
    {"bottleneck_attention_dense",
     {
         0,
         0,
         INTERMEDIATE_SIZE,
     }},
    {"bottleneck_input_LayerNorm",
     {
         0,
         INTERMEDIATE_SIZE,
         INTERMEDIATE_SIZE,
     }},
    {"bottleneck_input_dense",
     {
         0,
         0,
         INTERMEDIATE_SIZE,
     }},
    {"hidden_states_2",
     {
         0,
         5 * INTERMEDIATE_SIZE + 26 * HIDDEN_SIZE,
         INTERMEDIATE_SIZE,
     }},
};

std::map<std::string, Files> gradientTestFiles{
    {"classifier_weight",
     {
         "mobilebert_encoder_layer_23_output_bottleneck_LayerNorm",
         "mobilebert_logits",
         "",
         "classifier_weight",
     }},
    {"classifier_bias",
     {
         "",
         "mobilebert_logits",
         "",
         "classifier_bias",
     }},
    {"output_bottleneck_LayerNorm_weight",
     {
         "output_bottleneck_residual",
         "output_bottleneck_LayerNorm",
         "",
         "output_bottleneck_LayerNorm_weight",
     }},
    {"output_bottleneck_LayerNorm_bias",
     {
         "",
         "output_bottleneck_LayerNorm",
         "",
         "output_bottleneck_LayerNorm_bias",
     }},
    {"output_bottleneck_dense_weight",
     {
         "output_LayerNorm",
         "output_bottleneck_dense",
         "",
         "output_bottleneck_dense_weight",
     }},
    {"output_bottleneck_dense_bias",
     {
         "",
         "output_bottleneck_dense",
         "",
         "output_bottleneck_dense_bias",
     }},

    {"output_LayerNorm_weight",
     {
         "output_residual",
         "output_LayerNorm",
         "",
         "output_LayerNorm_weight",
     }},
    {"output_LayerNorm_bias",
     {
         "",
         "output_LayerNorm",
         "",
         "output_LayerNorm_bias",
     }},
    {"output_dense_weight",
     {
         "intermediate_intermediate_act_fn",
         "output_dense",
         "",
         "output_dense_weight",
     }},
    {"output_dense_bias",
     {
         "",
         "output_dense",
         "",
         "output_dense_bias",
     }},
    {"intermediate_dense_weight",
     {
         "ffn_2_output_LayerNorm",
         "intermediate_dense",
         "",
         "intermediate_dense_weight",
     }},
    {"intermediate_dense_bias",
     {
         "",
         "intermediate_dense",
         "",
         "intermediate_dense_bias",
     }},

    {"ffn_2_output_LayerNorm_weight",
     {
         "ffn_2_output_residual",
         "ffn_2_output_LayerNorm",
         "",
         "ffn_2_output_LayerNorm_weight",
     }},
    {"ffn_2_output_LayerNorm_bias",
     {
         "",
         "ffn_2_output_LayerNorm",
         "",
         "ffn_2_output_LayerNorm_bias",
     }},
    {"ffn_2_output_dense_weight",
     {
         "ffn_2_intermediate_intermediate_act_fn",
         "ffn_2_output_dense",
         "",
         "ffn_2_output_dense_weight",
     }},
    {"ffn_2_output_dense_bias",
     {
         "ffn_2_intermediate_intermediate_act_fn",
         "ffn_2_output_dense",
         "",
         "ffn_2_output_dense_bias",
     }},
    {"ffn_2_intermediate_dense_weight",
     {
         "ffn_1_output_LayerNorm",
         "ffn_2_intermediate_dense",
         "",
         "ffn_2_intermediate_dense_weight",
     }},
    {"ffn_2_intermediate_dense_bias",
     {
         "",
         "ffn_2_intermediate_dense",
         "",
         "ffn_2_intermediate_dense_bias",
     }},

    {"ffn_1_output_LayerNorm_weight",
     {
         "ffn_1_output_residual",
         "ffn_1_output_LayerNorm",
         "",
         "ffn_1_output_LayerNorm_weight",
     }},
    {"ffn_1_output_LayerNorm_bias",
     {
         "",
         "ffn_1_output_LayerNorm",
         "",
         "ffn_1_output_LayerNorm_bias",
     }},
    {"ffn_1_output_dense_weight",
     {
         "ffn_1_intermediate_intermediate_act_fn",
         "ffn_1_output_dense",
         "",
         "ffn_1_output_dense_weight",
     }},
    {"ffn_1_output_dense_bias",
     {
         "ffn_1_intermediate_intermediate_act_fn",
         "ffn_1_output_dense",
         "",
         "ffn_1_output_dense_bias",
     }},
    {"ffn_1_intermediate_dense_weight",
     {
         "ffn_0_output_LayerNorm",
         "ffn_1_intermediate_dense",
         "",
         "ffn_1_intermediate_dense_weight",
     }},
    {"ffn_1_intermediate_dense_bias",
     {
         "",
         "ffn_1_intermediate_dense",
         "",
         "ffn_1_intermediate_dense_bias",
     }},

    {"ffn_0_output_LayerNorm",
     {
         "ffn_0_output_LayerNorm",
         "ffn_0_output_residual",
         "",
         "ffn_0_output_LayerNorm_weight",
     }},
    {"ffn_0_output_dense",
     {
         "ffn_0_output_dense",
         "ffn_0_intermediate_intermediate_act_fn",
         "",
         "ffn_0_output_dense_weight",
     }},
    {"ffn_0_intermediate_dense",
     {
         "ffn_0_intermediate_dense",
         "attention_output_LayerNorm",
         "",
         "ffn_0_intermediate_dense_weight",
     }},

    {"attention_output_LayerNorm",
     {
         "attention_output_LayerNorm",
         "attention_output_residual",
         "",
         "attention_output_LayerNorm_weight",
     }},
    {"attention_output_dense",
     {
         "attention_output_dense",
         "attention_self_context_layer_permuted",
         "",
         "attention_output_dense_weight",
     }},

    {"attention_self_value_layer_0",
     {
         "attention_self_value",
         "hidden_states",
         "",
         "attention_self_value_weight",
     }},
    {"attention_self_query_layer_0",
     {
         "attention_self_query_layer",
         "bottleneck_attention_LayerNorm",
         "",
         "attention_self_query_weight",
     }},
    {"attention_self_key_layer_0",
     {
         "attention_self_key_layer",
         "bottleneck_attention_LayerNorm",
         "",
         "attention_self_key_weight",
     }},

    {"bottleneck_attention_LayerNorm_1",
     {
         "bottleneck_attention_LayerNorm",
         "bottleneck_attention_dense",
         "",
         "bottleneck_attention_LayerNorm_weight",
     }},
    {"bottleneck_attention_dense",
     {
         "bottleneck_attention_dense",
         "hidden_states",
         "",
         "bottleneck_attention_dense_weight",
     }},

    {"bottleneck_input_LayerNorm",
     {
         "bottleneck_input_LayerNorm",
         "bottleneck_input_dense",
         "",
         "bottleneck_input_LayerNorm_weight",
     }},
    {"bottleneck_input_dense",
     {
         "bottleneck_input_dense",
         "hidden_states",
         "",
         "bottleneck_input_dense_weight",
     }},
    {"hidden_states_2",
     {
         "output_bottleneck_LayerNorm",
         "output_bottleneck_residual",
         "",
         "output_bottleneck_LayerNorm_weight",
     }},
};
