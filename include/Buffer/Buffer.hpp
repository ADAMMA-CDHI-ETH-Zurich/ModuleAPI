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