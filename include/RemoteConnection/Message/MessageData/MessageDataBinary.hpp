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
#include "Serialization/Serialization.hpp"
#include "Binary/BinaryData.hpp"
#include "RemoteConnection/Message/MessageData/MessageData.hpp"

namespace claid
{
    namespace RemoteConnection
    {
        // Can be used for any data types.
        // Data is set and get by the appropriate functions and de-/serialized automatically.
        // Theoretically, we could always just use this for any data we sent.
        // The other data types (e.g. MessageDataString) are just created for convenience.
        struct MessageDataBinary : public MessageData
        {
            //DECLARE_SERIALIZATION(MessageDataBinary)
            BinaryData binaryData;

            template<typename Reflector>
            void reflect(Reflector& r)
            {
                r.member("Data", binaryData, "");
            }

            void setBinaryData(const BinaryData& binaryData)
            {
                this->binaryData = binaryData;
            }

            void getBinaryData(BinaryData& binaryData)
            {
                binaryData = this->binaryData;
            }

            template<typename T>
            void set(T& data)
            {
                BinarySerializer serializer;
                serializer.serialize(data, &binaryData);
               
            }

            template<typename T>
            void get(T& data)
            {
                BinaryDeserializer deserializer;
                std::shared_ptr<BinaryDataReader> reader = std::make_shared<BinaryDataReader>(this->binaryData);

                deserializer.deserialize(data, reader); 
            }
            
        };
    }
}