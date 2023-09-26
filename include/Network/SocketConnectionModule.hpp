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

#include "RemoteConnection/ConnectionModule.hpp"
#include "Network/Socket/Client/SocketClient.hpp"
#include "Network/SocketReaderModule.hpp"
namespace claid
{
    namespace Network
    {
        class SocketConnectionModule : public RemoteConnection::ConnectionModule
        {
            private:
                SocketClient socketClient;
                SocketReaderModule readerModule;

                bool started = false;

                // Message internally only holds references to header and data,
                // thus passing it by copy is fine. We do it because the message
                // received in onSendMessage is const.
                void sendMessage(RemoteConnection::Message message);
                void setup();

            public:
                SocketConnectionModule(SocketClient socketClient);
                void start();
                void stop();

        };
    }
}