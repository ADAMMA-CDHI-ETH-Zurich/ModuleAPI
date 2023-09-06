#pragma once 
#include "CLAID.hpp"

#include "BufferInfo.hpp"
#include "Reflection/SplitReflectInType.hpp"



namespace claid
{
    struct Buffer
    {
        //DECLARE_SERIALIZATION(Buffer)
        private:
            std::shared_ptr<char> data;
            BufferInfo bufferInfo;

        public:

            Buffer() : bufferInfo(BufferDataType::UINT8, {0})
            {
            }

            Buffer(std::shared_ptr<char> data, BufferDataType dataType, std::vector<int32_t> shape) : data(data)
            {
                this->bufferInfo = BufferInfo(dataType, shape);
            }

            Buffer(std::shared_ptr<char>& data, BufferInfo bufferInfo): data(data), bufferInfo(bufferInfo)
            {

            }

            SplitReflect()

            ReflectRead(Buffer,
                reflector.member("BufferInfo", bufferInfo, "");
                int32_t bytes = bufferInfo.getNumberOfBytes();

                this->allocateNew(bytes);
                
                char* ptr = this->getData();
                reflector.read(ptr, bytes);
            )

            ReflectWrite(Buffer,
                reflector.member("BufferInfo", bufferInfo, "");
                int32_t bytes = this->getNumberOfBytes();

                char* ptr = this->getData();
                reflector.write(reinterpret_cast<const char*>(ptr), bytes);
            )


            const BufferInfo& getBufferInfo()
            {
                return this->bufferInfo;
            }

            void allocateNew(const int32_t bytes)
            {
                this->data = std::shared_ptr<char>(new char[bytes]);
            }

            int32_t size() const;
            void setData();

            int32_t getNumberOfBytes() const
            {
                return this->bufferInfo.getNumberOfBytes();
            }

            char* getData()
            {
                return this->data.get();
            }




    };
}