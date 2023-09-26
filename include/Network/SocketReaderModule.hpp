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
#include "Network/Socket/Client/SocketClient.hpp"
#include "Network/Error/NetworkErrors.hpp"
namespace claid
{
    namespace Network
    {
        class SocketReaderModule : public SubModule
        {
            private:
                SocketClient* socketClient = nullptr;
                Channel<RemoteConnection::Message> messageReceivedChannel;
                Channel<RemoteConnection::Error> errorChannel;
                bool active;
                bool stopped;

                void run();

                template<typename T>
                void postError()
                {
                    RemoteConnection::Error error;
                    error.set<T>();
                    this->errorChannel.post(error);
                }

                void initialize();
                void terminate();

            public:
                SocketReaderModule();
                SocketReaderModule(SocketClient* client);

                void setMessageReceivedChannel(Channel<RemoteConnection::Message> messageReceivedChannel);
                void setErrorChannel(Channel<RemoteConnection::Error> errorChannel);


                void stop();

                bool isStopped();

                
        };
    }
}