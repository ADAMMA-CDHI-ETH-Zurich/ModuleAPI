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

#include "RunTime/RunTime.hpp"
#include "RemoteConnection/Message/Message.hpp"
#include "RemoteConnection/Error/Error.hpp"

namespace claid
{
    namespace RemoteConnection
    {
        class Observer : public SubModule
        {
            protected:
                Channel<Message> sendMessageChannel;
                Channel<Error> errorChannel;
                bool sendMessageChannelSet = false;
                bool errorChannelSet = false;

                template<typename T>
                void postError()
                {
                    if(!errorChannelSet)
                    {
                        CLAID_THROW(Exception, "Error in Observer, cannot post error since error channel was not set before.");
                    }

                    Error error;
                    error.set<T>();
                    this->errorChannel.post(error);
                }

            public:
                
                void setSendMessageChannel(Channel<RemoteConnection::Message> sendMessageChannel) 
                {
                    printf("SendMessageChannelSet %s\n", this->getModuleName().c_str());
                    this->sendMessageChannel = sendMessageChannel;
                    this->sendMessageChannelSet = true;
                }

                void setErrorChannel(Channel<Error> errorChannel)
                {
                    this->errorChannel = errorChannel;
                    this->errorChannelSet = true;
                }

                void sendMessage(Message& message)
                {
                    if(!this->sendMessageChannelSet)
                    {
                        CLAID_THROW(Exception, "Error! Observer tried to send a message, however a channel to send messages was never set."
                        "Please make sure to call setSendMessageChannel of RemoteModule " << __FILE__ << " " << __LINE__);
                    }
                    Logger::printfln("SendMessageChannel post");
                    this->sendMessageChannel.post(message);
                }


        };
    }
}