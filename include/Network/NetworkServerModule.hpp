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

#include "Network/Socket/Client/SocketClient.hpp"
#include "Network/Socket/Server/SocketServer.hpp"
#include "Network/Socket/Server/ClientAcceptModule.hpp"
#include "Network/SocketConnectionModule.hpp"
#include "Network/NetworkModule.hpp"

#include "RemoteConnection/ConnectionLink.hpp"
#include "RemoteConnection/RemoteModule/RemoteModule.hpp"
#include "RemoteConnection/RemoteConnectedEntity.hpp"


namespace claid
{
    namespace Network
    {
        class NetworkServerModule : public NetworkModule
        {
            
            public:

                NetworkServerModule()
                {

                }
                
                NetworkServerModule(int port) : port(port)
                {
         
                }

                template<typename Reflector>
                void reflect(Reflector& r)
                {
                    REFLECT_BASE(r, NetworkModule)
                    r.member("Port", this->port, "");
                }

            private:
                
                size_t port;
                // We will push clients that connected to this Server to Channel /CLAID/LOCAL/OnNetworkClientConnected
                // and clients that disconnected to /CLAID/LOCAL/OnNetworkClientDisconnected
                // This can be useful for monitoring tools, e.g. to see externally connected RunTimes.
                // Note, that /CLAID/LOCAL is a hidden namespace, therefore these channels will not be mirrored to any 
                // remotely connected instance of CLAID.
                Channel<RemoteConnection::RemoteConnectedEntityUniqueIdentifier> onClientConnectedChannel;
                Channel<RemoteConnection::RemoteConnectedEntityUniqueIdentifier> onClientDisconnectedChannel;

                std::vector<RemoteConnection::RemoteConnectedEntity*> remoteConnectedEntities;
                std::map<RemoteConnection::RemoteConnectedEntity*, Channel<RemoteConnection::Error>> errorChannels;

                Channel<RemoteConnection::Error> clientAcceptErrorChannel;
                Channel<SocketClient> clientAcceptChannel;

                ClientAcceptModule clientAcceptModule;
                SocketServer server;

                void onClientAccepted(ChannelData<SocketClient> socketClient);
                void onClientAcceptError(ChannelData<RemoteConnection::Error> error);
                void onErrorReceived(RemoteConnection::RemoteConnectedEntity* entity, ChannelData<RemoteConnection::Error> error);
                void onError(RemoteConnection::RemoteConnectedEntity* entity, RemoteConnection::Error error);
                void initialize();

                void onClientLostConnection(RemoteConnection::RemoteConnectedEntity* entity);

               


        };
    }
}