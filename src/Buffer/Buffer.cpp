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