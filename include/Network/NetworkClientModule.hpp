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

#include "RemoteConnection/ConnectionLink.hpp"
#include "RemoteConnection/RemoteModule/RemoteModule.hpp"
#include "RemoteConnection/RemoteConnectedEntity.hpp"
#include "RemoteConnection/Error/ErrorRemoteRuntimeOutOfSync.hpp"
#include "RemoteConnection/Error/ErrorConnectionTimeout.hpp"
#include "Network/SocketConnectionModule.hpp"
#include "Network/NetworkModule.hpp"

namespace claid
{
    namespace Network
    {
        class NetworkClientModule : public NetworkModule
        {

            public:
                NetworkClientModule()
                {
                }

                NetworkClientModule(std::string ip, int port)
                {
                    this->address = ip + std::string(":") + std::to_string(port);
                }


            template<typename Reflector>
            void reflect(Reflector& r)
            {
                REFLECT_BASE(r, NetworkModule)
                r.member("ConnectTo", this->address, "");
                r.member("TimeoutMsWhenTryingToConnect", this->timeoutInMs, "How long to wait for a response when (trying to) connect to a server.", static_cast<size_t>(3));
                r.member("TryToReconnectAfterMs", this->tryToReconnectAfterMs, "If the connection is lost (or could not be established), after how many ms should we try to reconnect? Set to 0 if reconnect should not happen.", static_cast<size_t>(1500));
                r.member("disabled", this->disabled, "If true, network connectivity is disabled. Can be reenabled by sending NetworkStateChange request on the corresponding channel", false);
            }

            private:
                std::string address;
                size_t timeoutInMs = 3;
                size_t tryToReconnectAfterMs = 200;

                bool connected = false;
                bool disabled = false;

                RemoteConnection::RemoteConnectedEntity* remoteConnectedEntity = nullptr;

                Channel<RemoteConnection::Error> errorChannel;
                
                void getIPAndPortFromAddress(const std::string& address, std::string& ip, int& port);
                void initialize();

                bool connectToServer();
                void onConnectedSuccessfully(SocketClient socketClient);

                void onErrorReceived(RemoteConnection::RemoteConnectedEntity* remoteConnectedEntity, ChannelData<RemoteConnection::Error> error);

                template<typename T>
                void callError()
                {
                    RemoteConnection::Error error;
                    error.set<T>();
                    
                    // Make sure to call onError asynchronous. That way, we avoid recursive calls of onError.
                    // (E.g.: If error is ErrorConnectToAdressFailed, then we might try to reconnect. If that fails again,
                    // it would call onError again, and so on and so forth resulting in unbound recursion, which would eventually lead
                    // to a stack overflow).
                    this->callInModuleThread<NetworkClientModule, RemoteConnection::RemoteConnectedEntity*, RemoteConnection::Error>(&NetworkClientModule::onError, this, this->remoteConnectedEntity, error);
                }

                void registerFunctionToPeriodicallyTryToReconnect();
                void unregisterFunctionToPeriodicallyTryToReconnect();

                void onError(RemoteConnection::RemoteConnectedEntity* remoteConnectedEntity, RemoteConnection::Error error);

                void onConnectionLost(RemoteConnection::RemoteConnectedEntity* remoteConnectedEntity);
                void tryToReconnect();

                void onNetworkStateChangeRequested(const NetworkStateChangeRequest& networkStateChangeRequest);

                void enableNetworkConnection();
                void disableNetworkConnection();
        };

    }
}