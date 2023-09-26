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
#include "Network/Socket/Server/ClientAcceptModule.hpp"

namespace claid
{
    namespace Network
    {
        const std::string ClientAcceptModule::CLIENT_ACCEPT_CHANNEL = "ClientAcceptChannel";
        const std::string ClientAcceptModule::ERROR_CHANNEL = "ErrorChannel";


        void ClientAcceptModule::initialize()
        {
            this->clientAcceptChannel = this->publishLocal<SocketClient>(CLIENT_ACCEPT_CHANNEL);
        }

        Channel<SocketClient> ClientAcceptModule::subscribeToClientAcceptChannel(ChannelSubscriber<SocketClient> subscriber)
        {
            return this->subscribeLocal<SocketClient>(CLIENT_ACCEPT_CHANNEL, subscriber);
        }
        
        Channel<RemoteConnection::Error> ClientAcceptModule::subscribeToErrorChannel(ChannelSubscriber<RemoteConnection::Error> subscriber)
        {
            return this->subscribeLocal<RemoteConnection::Error>(ERROR_CHANNEL, subscriber);
        }

        void ClientAcceptModule::start(SocketServer* server)
        {
            this->server = server;
            this->active = true;
            // Start might be called by another thread. Make sure we switch to our own thread.
            this->callInModuleThread(&ClientAcceptModule::run, this);
        }


        void ClientAcceptModule::run()
        {
            Logger::printfln("ClientAcceptModule::run");
            while(this->active)
            {
                SocketClient client;

                if(!this->server->accept(client))
                {
                    // TODO: POST ERROR ACCEPT FAILED
                    CLAID_THROW(Exception, "Failed to accept client");
                }
                Logger::printfln("Post");
                this->clientAcceptChannel.post(client);
            }
        }
    }
}