#pragma once

#include "Network/Socket/Client/SocketClient.hpp"
#include "Network/Socket/Server/SocketServer.hpp"
#include "Network/Socket/Server/ClientAcceptModule.hpp"
#include "Network/SocketConnectionModule.hpp"
#include "Network/NetworkModule.hpp"

#include "RemoteConnection/ConnectionLink.hpp"
#include "RemoteConnection/RemoteModule/RemoteModule.hpp"
#include "RemoteConnection/RemoteConnectedEntity.hpp"


namespace portaible
{
    namespace Network
    {
        class NetworkServerModule : public NetworkModule
        {
            PORTAIBLE_MODULE(NetworkServerModule)
            private:


                std::vector<RemoteConnection::RemoteConnectedEntity*> remoteConnectedEntities;
                std::map<RemoteConnection::RemoteConnectedEntity*, Channel<RemoteConnection::Error>> errorChannels;

                Channel<RemoteConnection::Error> clientAcceptErrorChannel;
                Channel<SocketClient> clientAcceptChannel;

                ClientAcceptModule clientAcceptModule;
                SocketServer server;
                size_t port;

                void onClientAccepted(ChannelData<SocketClient> socketClient);
                void onClientAcceptError(ChannelData<RemoteConnection::Error> error);
                void onError(RemoteConnection::RemoteConnectedEntity* entity, ChannelData<RemoteConnection::Error> error);
                void initialize();

            public:
                template<typename Reflector>
                void reflect(Reflector& r)
                {
                    r.member("Port", this->port, "");
                }


        };
    }
}