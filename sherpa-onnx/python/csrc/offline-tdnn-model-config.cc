// sherpa-onnx/python/csrc/offline-tdnn-model-config.cc
//
// Copyright (c)  2023  Xiaomi Corporation

#include "sherpa-onnx/csrc/offline-tdnn-model-config.h"

#include <string>
#include <vector>

#include "sherpa-onnx/python/csrc/offline-tdnn-model-config.h"

namespace sherpa_onnx {

void PybindOfflineTdnnModelConfig(py::module *m) {
  using PyClass = OfflineTdnnModelConfig;
  py::class_<PyClass>(*m, "OfflineTdnnModelConfig")
      .def(py::init<const std::string &>(), py::arg("model"))
      .def_readwrite("model", &PyClass::model)
      .def("__str__", &PyClass::ToString);
}

}  // namespace sherpa_onnx
