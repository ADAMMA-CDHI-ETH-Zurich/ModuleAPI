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
#include <sstream>
namespace claid
{
    class CSVSerializer : public Serializer<CSVSerializer>
    {
        private:
            std::stringstream header;
            std::stringstream data;

            template<typename T>
            void addValue(std::string name, const T& value)
            {
                if(header.str() == "")
                {
                    header << name;
                }
                else
                {
                    header << "," << name;
                }

                if(data.str() == "")
                {
                    data << value;
                }
                else
                {
                    data << "," << value;
                }
            }

        public:
            EmptyReflect(CSVSerializer)

            CSVSerializer()
            {

            }

            CSVSerializer(const CSVSerializer& other)
            {
                this->data << other.data.str();
                this->header << other.header.str();
            }

            CSVSerializer& operator=(const CSVSerializer& other)
            {
                this->data << other.data.str();
                this->header << other.header.str();
                return *this;
            }

            std::string getReflectorName()
            {
                return "CSVSerializer";
            }


            template<typename T>
            void callFloatOrDouble(const char* property, T& member)
            {
                addValue(property, member);
            }   

            // Also includes any variants of signed, unsigned, short, long, long long, ...
            template<typename T>
            void callInt(const char* property, T& member)
            {
                addValue(property, member);
            }

            void callBool(const char* property, bool& member)
            {
                addValue(property, member ? "true" : "false");
            }

            // Why template? Because we can have signed and unsigned char.
            template<typename T>
            void callChar(const char* property, T& member)
            {
                addValue(property, member);
            }

            template<typename T>
            void callString(const char* property, T& member)
            {
                addValue(property, member);
            }

            template<typename T>
            void callBeginClass(const char* property, T& member)
            {
                
            }

            template<typename T>
            void callEndClass(const char* property, T& member)
            {
                
            }


            template<typename T>
            void callPointer(const char* property, T*& member)
            {
                CLAID_THROW(Exception, "Pointers currently not supported by CSVSerializer");
            }
            
            template<typename T>
            void callSharedPointer(const char* property, T& member)
            {
                CLAID_THROW(Exception, "Shared_ptrs currently not supported by CSVSerializer");
            }

            template<typename T>
            void callEnum(const char* property, T& member)
            {
                int32_t m = static_cast<int32_t>(member);
                this->callInt(property, m);
            }

            void count(const std::string& name, int32_t& count)
            {
                // Do nothing
            }

            void countElements(int32_t& count)
            {
                // Do nothing
            }

            void beginSequence()
            {
                // Do nothing
            }

            void itemIndex(const size_t index)
            {
                // Do nothing
            }

            void endSequence()
            {
                // Do nothing
            }
            
            void write(const char* data, size_t size)
            {

            }


            template<typename T>
            void onInvocationStart(T& obj)
            {
                this->forceReset();
            }

            template<typename T>
            void onInvocationEnd(T& obj)
            {
                
            }

  
            void enforceName(std::string& name, int idInSequence = 0)
            {
                // Some serializers, like BinarySerializer, might not store the members name (i.e., property parameters),
                // as it is not necessary to be known in the binary data).
                // For some cases, however, it might be necessary to store such strings in the serialized data nevertheless,
                // as it might be needed for deserialization etc.
                // Thus, this function allows to make sure the string "name" is explicitly stored.
            }

            void writeHeaderToFile(const std::string& path, std::ofstream& file)
            {
                file << this->header.str() << "\n";
                file.flush();

            }

            void writeDataToFile(const std::string& path, std::ofstream& file)
            {
                file << this->data.str() << "\n";
            }

            void forceReset()
            {
                this->header.str("");
                this->data.str("");
            }
    };
}