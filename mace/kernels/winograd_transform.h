// Copyright 2018 Xiaomi, Inc.  All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef MACE_KERNELS_WINOGRAD_TRANSFORM_H_
#define MACE_KERNELS_WINOGRAD_TRANSFORM_H_

#include <memory>
#include <vector>

#include "mace/core/future.h"
#include "mace/core/tensor.h"
#include "mace/kernels/activation.h"
#include "mace/kernels/conv_pool_2d_util.h"

#ifdef MACE_ENABLE_OPENCL
#include "mace/core/runtime/opencl/cl2_header.h"
#endif  // MACE_ENABLE_OPENCL

namespace mace {
namespace kernels {

struct WinogradTransformFunctorBase {
  WinogradTransformFunctorBase(const Padding &padding_type,
                               const std::vector<int> &paddings)
      : strides_({1, 1}),
        dilations_({1, 1}),
        padding_type_(padding_type),
        paddings_(paddings) {}

  const std::vector<int> strides_;    // [stride_h, stride_w]
  const std::vector<int> dilations_;  // [dilation_h, dilation_w]
  Padding padding_type_;
  std::vector<int> paddings_;
};

template <DeviceType D, typename T>
struct WinogradTransformFunctor : WinogradTransformFunctorBase {
  WinogradTransformFunctor(const Padding &padding_type,
                           const std::vector<int> &paddings)
      : WinogradTransformFunctorBase(padding_type, paddings) {}

  void operator()(const Tensor *input, Tensor *output, StatsFuture *future) {
    MACE_NOT_IMPLEMENTED;
  }
};

#ifdef MACE_ENABLE_OPENCL
template <typename T>
struct WinogradTransformFunctor<DeviceType::OPENCL, T>
    : WinogradTransformFunctorBase {
  WinogradTransformFunctor(const Padding &padding_type,
                           const std::vector<int> &paddings)
      : WinogradTransformFunctorBase(padding_type, paddings) {}

  void operator()(const Tensor *input, Tensor *output, StatsFuture *future);

  cl::Kernel kernel_;
  uint32_t kwg_size_;
  std::unique_ptr<BufferBase> kernel_error_;
  std::vector<index_t> input_shape_;
};
#endif  // MACE_ENABLE_OPENCL

struct WinogradInverseTransformFunctorBase {
  WinogradInverseTransformFunctorBase(const int batch,
                                      const int height,
                                      const int width,
                                      const ActivationType activation,
                                      const float relux_max_limit)
      : batch_(batch),
        height_(height),
        width_(width),
        activation_(activation),
        relux_max_limit_(relux_max_limit) {}

  const int batch_;
  const int height_;
  const int width_;
  const ActivationType activation_;
  const float relux_max_limit_;
};

template <DeviceType D, typename T>
struct WinogradInverseTransformFunctor : WinogradInverseTransformFunctorBase {
  WinogradInverseTransformFunctor(const int batch,
                                  const int height,
                                  const int width,
                                  const ActivationType activation,
                                  const float relux_max_limit)
      : WinogradInverseTransformFunctorBase(
            batch, height, width, activation, relux_max_limit) {}

  void operator()(const Tensor *input,
                  const Tensor *bias,
                  Tensor *output,
                  StatsFuture *future) {
    MACE_NOT_IMPLEMENTED;
  }
};

#ifdef MACE_ENABLE_OPENCL
template <typename T>
struct WinogradInverseTransformFunctor<DeviceType::OPENCL, T>
    : WinogradInverseTransformFunctorBase {
  WinogradInverseTransformFunctor(const int batch,
                                  const int height,
                                  const int width,
                                  const ActivationType activation,
                                  const float relux_max_limit)
      : WinogradInverseTransformFunctorBase(
            batch, height, width, activation, relux_max_limit) {}

  void operator()(const Tensor *input,
                  const Tensor *bias,
                  Tensor *output,
                  StatsFuture *future);

  cl::Kernel kernel_;
  uint32_t kwg_size_;
  std::unique_ptr<BufferBase> kernel_error_;
  std::vector<index_t> input_shape_;
};
#endif  // MACE_ENABLE_OPENCL

}  // namespace kernels
}  // namespace mace

#endif  // MACE_KERNELS_WINOGRAD_TRANSFORM_H_
