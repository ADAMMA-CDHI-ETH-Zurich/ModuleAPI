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
#include <vector>
#include "Reflection/SplitReflectInType.hpp"
#include "Exception/Exception.hpp"
#include <iostream>
#include <fstream>
#include <mutex>

namespace claid
{
    class BinaryData
    {
        
       
        public:
            BinaryData()
            {
            }

            BinaryData(std::vector<char>& data) : data(data)
            {

            }

            virtual ~BinaryData()
            {
                
            }

            template<typename Reflector>
            void reflect(Reflector& r)
            {
                splitReflectInType(r, *this);
            }

            template<typename Reflector>
            void reflectRead(Reflector& r)
            {
                int32_t bytes;
                r.member("NumBytes", bytes, "");

                this->resize(bytes);

                char* dataPtr = this->getRawData();
                r.read(dataPtr, bytes);
            }

            template<typename Reflector>
            void reflectWrite(Reflector& r)
            {
                int32_t bytes = this->data.size();
                r.member("NumBytes", bytes, "");

                r.write(this->getRawData(), bytes);
            }

            // Should only be enabled for primitive types and byte (which we defined ourselves).
            template <typename T>
            typename std::enable_if<std::is_arithmetic<T>::value>::type // type of enable_if is void, if value is true, if not specified otherwise. If false, then type does not exist (see implementation of enable_if).
            store(const T& value)
            {
                int32_t size;
                const char* binaryData = toBinary<T>(value, size);

                this->store(binaryData, size);
            }

            void store(const char* data, int32_t size)
            {
                const char* ptr = data;
                this->data.insert(this->data.end(), data, data + size);
            }

            void storeString(const std::string& value)
            {
                // Store length of string
                int32_t size = value.size();
                this->store(size);

                // Store characters
                for(int32_t i = 0; i < size; i++)
                {
                    this->store(value[i]);
                }
            }

            void resize(int32_t size)
            {
                int32_t index = 0;
                this->data.resize(size);      
            }

            char* getRawData()
            {
                return this->data.data();
            }

            const char* getConstRawData() const
            {
                return this->data.data();
            }

            std::vector<char>& getData()
            {
                return this->data;
            }

            int32_t getNumBytes() const
            {
                return this->data.size();
            }

            int32_t size() const
            {
                return this->getNumBytes();
            }

            void clear()
            {
                this->data.clear();
            }

            void loadFromFile(const std::string& path)
            {
                   
                std::fstream file(path, std::ios::in | std::ios::binary);
                if(!file.is_open())
                {
                    CLAID_THROW(claid::Exception, "Error, cannot load binary data from \"" << path << "\".\n"
                    << "Could not open File for writing.");
                }

                file.seekg(0, std::ios::end);
                int32_t numBytes = file.tellg();
                file.seekg(0, std::ios::beg);

                this->resize(numBytes);

                file.read(this->data.data(), numBytes);
            }

            void saveToFile(const std::string& path) const
            {
                std::fstream file(path, std::ios::out | std::ios::binary);
                if(!file.is_open())
                {
                    CLAID_THROW(claid::Exception, "Error, cannot save binary data to \"" << path << "\".\n"
                    << "Could not open File for writing.");
                }

                file.write(this->data.data(), this->data.size());
            }

            void append(const BinaryData& data)
            {
                const char* ptr = data.getConstRawData();
                this->store(ptr, data.getNumBytes());
            }
     

        private:

            std::vector<char> data;

            template<typename T>
            static const char* toBinary(const T& value, int32_t& size)
            {
                size = sizeof(T);
                return reinterpret_cast<const char*>(&value);
            }           
    };
}

