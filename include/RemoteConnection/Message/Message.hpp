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

#include <memory.h>
#include "RemoteConnection/Message/MessageHeader/MessageHeader.hpp"
#include "RemoteConnection/Message/MessageData/MessageData.hpp"
#include "Serialization/Serialization.hpp"
#include "Binary/BinaryData.hpp"

namespace claid
{
    namespace RemoteConnection
    {
        struct Message
        {
            std::shared_ptr<MessageHeader> header;
            std::shared_ptr<MessageData> data;

            template<typename HeaderType, typename DataType>
            static Message CreateMessage()
            {
                Message message;
                // Logger::printfln("Creating namess %s with data %s", 
                // TypeChecking::getCompilerSpecificCompileTypeNameOfClass<HeaderType>().c_str(),
                // TypeChecking::getCompilerSpecificCompileTypeNameOfClass<DataType>().c_str());

                message.header = 
                    std::static_pointer_cast<MessageHeader>(std::shared_ptr<HeaderType>(new HeaderType));

                message.data = 
                    std::static_pointer_cast<MessageData>(std::shared_ptr<DataType>(new DataType));
                
                return message;
            }


            template<typename Reflector>
            void reflect(Reflector& r)
            {
                r.member("Header", header, "");
                r.member("Data", data, "");
            }
        };
    }
}