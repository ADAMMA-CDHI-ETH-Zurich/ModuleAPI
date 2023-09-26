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
#include "Network/Socket/Client/SocketClient.hpp"


#ifdef _WIN32
    #include "SocketClientWindows_impl.hpp"
#else
    #include "SocketClientUnix_impl.hpp"
#endif

namespace claid
{
    bool Network::SocketClient::write(BinaryData& data)
    {
        // First, we write the size (number of bytes) of the data.
        int32_t size = data.getNumBytes();

        std::vector<char> tmp(sizeof(int32_t));

        *reinterpret_cast<int32_t*>(tmp.data()) = size;

        Logger::printfln("Writing header %u", size);
        if (!writeBytes(tmp))
            return false;

        Logger::printfln("Writing data %u", size);
        // Now write the data itself.
        if (!writeBytes(data.getData()))
            return false;

        return true;
    }

    bool Network::SocketClient::read(BinaryData& data)
    {
        // We read the number of bytes first
        int32_t* dataNumBytes;
        std::vector<char> byteBuffer;
        // Read data size

        if (!this->readBytes(byteBuffer, sizeof(int32_t)))
        {
            return false;
        }

        dataNumBytes = reinterpret_cast<int32_t*>(byteBuffer.data());
        Logger::printfln("Read header, continuing to read %d bytes", *dataNumBytes);

        // Now read the data
        data.resize(*dataNumBytes);

        if (!this->readBytes(data.getData(), *dataNumBytes))
        {
            return false;
        }

        return true;
        }
    }