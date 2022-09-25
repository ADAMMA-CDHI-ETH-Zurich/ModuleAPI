#pragma once


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <stdint.h> 
#include <stdio.h> 

#include <stdlib.h> 

#include <string.h> 
#include <functional>

#include "Network/Socket/Client/SocketClient.hpp"

//#include <thread.h>
namespace portaible
{
	namespace Network
	{
		class SocketServer
		{

			public:
			
				enum SocketServerErrorType
				{
					SOCKET_CREATION_FAILED, // socket(...) failed
					SET_SOCK_OPT_FAILED, // setsockopt(...) failed
					BIND_FAILED, // bind(...) failed
					LISTEN_FAILED, // listen(...) failed
					ACCEPT_FAILED, // accept(...) failed
					ERROR_WSASTARTUP_FAILED,
					GET_ADDRESS_INFO_FAILED, // getaddrinfo(...) failed
				};

				struct SocketServerError
				{
					SocketServerErrorType errorType = SOCKET_CREATION_FAILED;
					int additionalErrorID = 0; // Error ID of the function that throws the error, e.g. WSAStartup fails with WSA_INVALID_PARAMETER
					std::string errorString = "";
				
					SocketServerError()
					{

					}

					SocketServerError(SocketServerErrorType errorType, int additionalErrorID = -1) : errorType(errorType), additionalErrorID(additionalErrorID)
					{

					}

					std::string errorTypeAsString()
					{
						std::vector<std::string> map = {"SOCKET_CREATION_FAILED", 
											"SET_SOCK_OPT_FAILED", 
											"BIND_FAILED", 
											"LISTEN_FAILED", 
											"ACCEPT_FAILED"};

						int idx = static_cast<int>(this->errorType);

						if(idx < 0 || idx >= map.size())
							return "Unknown Error";
						return map[idx];
					}
				};

				std::vector<SocketClient*> acceptedClients;

			private:
				int serverSocket;
				
				bool connected = false;
				int port;

				SocketServerError lastError;



			public:
				
			//	void close();
				bool bindTo(size_t port);
				bool accept(SocketClient& client);

				SocketServerError getLastError();


				SocketServer();
				~SocketServer();
		};


	}
}