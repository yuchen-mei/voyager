#pragma once

#include "test/common/operations/Common.h"

template <typename T>
inline T* slice(std::any input_ptr, const std::vector<int> shape, uint64_t dim,
                uint64_t start, uint64_t end, uint64_t step) {
  dim = dim < 0 ? dim + shape.size() : dim;
  int num_elements = (end - start) / step;

  const auto inputs = std::any_cast<T*>(input_ptr);

  const int size = get_size(shape);
  T* outputs = new T[size];

  // std::vector<int> start_indices(shape.size(), 0);
  // std::vector<int> end_indices(shape.size(), 0);

  // for (size_t i = 0; i < shape.size(); ++i) {
  //   if (i == dim) {
  //     start_indices[i] = start;
  //     end_indices[i] = end;
  //   } else {
  //     start_indices[i] = 0;
  //     end_indices[i] = shape[i];
  //   }
  // }

  // for (int y_cnt = start_indices[2]; y_cnt < end_indices[2]; ++y_cnt) {
  //   for (int x_cnt = start_indices[3]; x_cnt < end_indices[3]; ++x_cnt) {
  //     for (int c_cnt = start_indices[1]; c_cnt < end_indices[1]; ++c_cnt) {
  //       for (int k_cnt = start_indices[0]; k_cnt < end_indices[0]; ++k_cnt) {
  //         std::vector<int> indices(shape.size(), 0);
  //         indices[0] = k_cnt;
  //         indices[1] = c_cnt;
  //         indices[2] = y_cnt;
  //         indices[3] = x_cnt;

  //         // int out_addr = y_cnt * end_indices[2] * end_indices[1] * end_indices[0] +
  //         //                x_cnt * end_indices[1] * end_indices[0] +
  //         //                c_cnt * end_indices[0] + k_cnt;

  //         // int in_addr = y_cnt * shape[2] * shape[1] * shape[0] +
  //         //               x_cnt * shape[1] * shape[0] +
  //         //               c_cnt * shape[0] + k_cnt;
          
  //         int out_addr = k_cnt * end_indices[1] * end_indices[2] * end_indices[3] +
  //                        c_cnt * end_indices[2] * end_indices[3] +
  //                        y_cnt * end_indices[3] + x_cnt;
  //         int in_addr = k_cnt * shape[1] * shape[2] * shape[3] +
  //                        c_cnt * shape[2] * shape[3] +
  //                        y_cnt * shape[3] + x_cnt;
          
  //         outputs[out_addr] = inputs[in_addr];
  //       }
  //     }
  //   }
  // }

  for (int i = 0; i < size; i++) {
    std::vector<int> indices(shape.size(), 0);
    int index = i;
    for (int j = shape.size() - 1; j >= 0; --j) {
      if (j == dim) {
        indices[j] = start + (index % num_elements) * step;
        index /= num_elements;
      } else {
        indices[j] = index % shape[j];
        index /= shape[j];
      }
    }

    int flat_index = get_flat_index(indices, shape);
    outputs[i] = inputs[flat_index];
  }

  return outputs;
}

template <typename T>
inline T* slice(std::any input_ptr, const codegen::OpOverload op) {
  if (op.target() != "slice") {
    return std::any_cast<T*>(input_ptr);
  }

  const auto input = op.kwargs().at("input").tensor();
  const auto shape = get_shape(input);

  uint64_t start = op.kwargs().at("start").int_value();
  uint64_t end = op.kwargs().at("end").int_value();
  uint64_t step = op.kwargs().at("step").int_value();
  uint64_t dim = op.kwargs().at("dim").int_value();

  dim = dim < 0 ? dim + shape.size() : dim;
  end = end > shape[dim] ? shape[dim] : end;

  return slice<T>(input_ptr, shape, dim, start, end, step);
}

template <typename T>
inline T* pad(std::any input_ptr, const std::vector<int> shape,
                const std::vector<int> pad_shape, bool XY_pad) {

  const auto inputs = std::any_cast<T*>(input_ptr);

  const int k = shape[0];
  const int c = shape[1];
  const int y = shape[2];
  const int x = shape[3];

  int pad_y_0, pad_y_1, pad_x_0, pad_x_1, pad_c;

  if (XY_pad) {
    pad_y_0 = pad_shape[0];
    pad_y_1 = pad_shape[1];
    pad_x_0 = pad_shape[2];
    pad_x_1 = pad_shape[3];
    pad_c = 0;
  } else {
    pad_y_0 = 0;
    pad_y_1 = 0;
    pad_x_0 = 0;
    pad_x_1 = 0;
    pad_c = pad_shape[5];
  }

  const int size = k * (c + pad_c) * (y + pad_y_0 + pad_y_1) * (x + pad_x_0 + pad_x_1);

  T* outputs = new T[size];

  for (int y_cnt = 0; y_cnt < y + pad_y_0 + pad_y_1; ++y_cnt) {
    for (int x_cnt = 0; x_cnt < x + pad_x_0 + pad_x_1; ++x_cnt) {
      for (int c_cnt = 0; c_cnt < c + pad_c; ++c_cnt) {
        for (int k_cnt = 0; k_cnt < k; ++k_cnt) {
          // int output_index = y_cnt * (x + pad_x_0 + pad_x_1) * (c + pad_c) * k +
          //                    x_cnt * (c + pad_c) * k +
          //                    c_cnt * k +
          //                    k_cnt;
          int output_index = k_cnt * (c + pad_c) * (y + pad_y_0 + pad_y_1) * (x + pad_x_0 + pad_x_1) +
                             c_cnt * (y + pad_y_0 + pad_y_1) * (x + pad_x_0 + pad_x_1) +
                             y_cnt * (x + pad_x_0 + pad_x_1) +
                             x_cnt;
          if (y_cnt < pad_y_0 || y_cnt >= y + pad_y_0 ||
              x_cnt < pad_x_0 || x_cnt >= x + pad_x_0 ||
              c_cnt >= c) {
            outputs[output_index] = T::zero(); // Padding value
          } else {
            // int input_index = (y_cnt - pad_y_0) * x * c * k +
            //                   (x_cnt - pad_x_0) * c * k +
            //                   c_cnt * k +
            //                   k_cnt;
            int input_index = k_cnt * c * y * x +
                               c_cnt * y * x +
                               (y_cnt - pad_y_0) * x +
                               (x_cnt - pad_x_0);

            outputs[output_index] = inputs[input_index];
          }
        }
      }
    }
  }

  return outputs;
}

template <typename T>
inline T* pad(std::any input_ptr, const codegen::OpOverload op) {

  const auto input = op.kwargs().at("input").tensor();
  const auto shape = get_shape(input);

  const auto pad_shape_field = op.kwargs().at("pad").int_list().values();

  std::vector<int> pad_shape(pad_shape_field.begin(), pad_shape_field.end());

  bool XY_pad = pad_shape.size() == 4;

  return pad<T>(input_ptr, shape, pad_shape, XY_pad);
}
