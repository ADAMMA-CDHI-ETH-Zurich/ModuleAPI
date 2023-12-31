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


#include "Logger/Logger.hpp"
#include "Binary/BinaryData.hpp"

#include <stdlib.h>

#include <stdio.h>

#include <string.h>
#include <iostream>
#include <vector>

//#include <thread.h>

namespace claid
{
	namespace Network
	{
		class SocketClient
		{
			public:
				enum SocketClientErrorType
				{
					ERROR_WSASTARTUP_FAILED, // If WSAStartup(...) fails
					ERROR_CONNECT_FAILED, // If connect(...) fails
					ERROR_INETPTON_INVALID_IP_ADRESS, // If given IP is not a valid IPv4 dotted-decimal string or a valid IPv6 address string
					ERROR_INETPTON_FAILED, // If inet_pton(...) fails with specific error..
					ERROR_SOCKET_CREATION_FAILED, // If socket() function fails
					ERROR_WRITE_FAILED,
					ERROR_READ_FAILED,
					ERROR_READ_NUM_BYTES_ZERO, // if SocketClient::readyBytes was called with numBytes = 0
				};

				struct SocketClientError
				{
					SocketClientErrorType errorType;
					int additionalErrorID; // Error ID of the function that throws the error, e.g. WSAStartup fails with WSA_INVALID_PARAMETER
				};

			private:
				
				bool connected = false;


				SocketClientError lastError;
				


			public:
				int sock;

				void close();

				bool connectTo(std::string address, int port, size_t timeoutInMs = 3);

				// See SocketClient.cpp for implementation of these two functions,
				// not the __imple.hpp files.
				bool write(BinaryData& data);
				bool read(BinaryData& data);

				// Implemented in platform specific parts
				bool writeBytes(std::vector<char>& byteBuffer);
				bool readBytes(std::vector<char>& byteBuffer, size_t numBytes);

		
		
				bool isConnected();

				SocketClientError getLastError();

				static bool isValidIP4Adress(const std::string& string);

				SocketClient();
				SocketClient(int sock);
				~SocketClient();
		};
	}
}
