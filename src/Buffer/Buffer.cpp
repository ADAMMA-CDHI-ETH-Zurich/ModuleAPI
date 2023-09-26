/***************************************************************************
* Copyright (C) 2023 ETH Zurich
* Core AI & Digital Biomarker, Acoustic and Inflammatory Biomarkers (ADAMMA)
* Centre for Digital Health Interventions
* 
* Authors: Patrick Langer
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*         http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
***************************************************************************/
#include "Buffer/Buffer.hpp"


REGISTER_SERIALIZATION(claid::Buffer)


#ifdef __JAVA_WRAPPERS__
    // Override reflect function above if the Reflector is JbindWrapperGenerator.
    // This allows us to create a custom Java wrapper for this data type.
    CustomReflectForReflectorNonNamespace(claid::Buffer, claid::JavaWrapper::JbindWrapperGenerator<claid::Buffer>,

    )
#endif

#ifdef __PYTHON_WRAPPERS__

// Override reflect function above if the Reflector is PybindWrapperGenerator.
// This allows us to create a custom python wrapper for this data type.
CustomReflectForReflectorNonNamespace(claid::Buffer, claid::PythonWrapper::PybindWrapperGenerator<claid::Buffer>,

    reflector.initPyClassIfNecessary(py::buffer_protocol());
    PythonWrapper::PybindWrapperGenerator<Buffer>::PyClass* pyClass;
    pyClass = reflector.getPyClass();
    pyClass->def_buffer([](Buffer &m) -> py::buffer_info {
        BufferInfo info = m.getBufferInfo();
        

        py::buffer_info pyBufferInfo = info.getPyBufferInfo(reinterpret_cast<void*>(m.getData()));
        Logger::printfln("Getting buffer %s", pyBufferInfo.format.c_str());
        return pyBufferInfo;
    });

    pyClass->def(py::init([](py::buffer b) {
        /* Request a buffer descriptor from Python */
        py::buffer_info info = b.request();

        BufferInfo bufferInfo;
        bufferInfo.fromPyBufferInfo(info);

        int32_t bytes = bufferInfo.getNumberOfBytes();
        std::shared_ptr<char> dataCopy(new char[bytes]);

        memcpy(dataCopy.get(), info.ptr, bytes);
        
        return Buffer(dataCopy, bufferInfo);
    }));


    reflectMember(bufferInfo);
)
#endif