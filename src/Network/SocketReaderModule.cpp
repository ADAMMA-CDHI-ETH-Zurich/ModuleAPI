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
#include "Network/SocketReaderModule.hpp"
#include "Binary/BinaryDeserializer.hpp"
namespace claid
{
namespace Network
{
       
    
    SocketReaderModule::SocketReaderModule() : socketClient(nullptr)
    {

    }
            
    SocketReaderModule::SocketReaderModule(SocketClient* socketClient) : 
                                            socketClient(socketClient)
    {

    }
    
    
    void SocketReaderModule::initialize()
    {
        // start might be called by another module / from another thread.
        // Thus, we make sure that we switch to our thread (of this module).
        this->active = true;
        this->stopped = false;
        this->callInModuleThread(&SocketReaderModule::run, this);
    }

    void SocketReaderModule::run()
    {
        if(this->socketClient == nullptr)
        {
            CLAID_THROW(Exception, "Failed to start SocketReaderModule: SocketClient is nullptr " << __FILE__ << " line " << __LINE__);
        }

        while(this->active)
        {
            BinaryData binaryData;
            if(!this->socketClient->read(binaryData))
            {
                if(!this->active)
                {
                    // Module has been stopped, thus we ignore the error.
                    break;
                }
                postError<ErrorReadFromSocketFailed>();
                this->active = false;
                break;
            }
            Logger::printfln("read %d bytes", binaryData.getNumBytes());
            // BinaryData is a message -> deserialize to message?
            BinaryDeserializer deserializer;
            RemoteConnection::Message message;
            try
            {
                std::shared_ptr<BinaryDataReader> reader = std::make_shared<BinaryDataReader>(binaryData);
                deserializer.deserialize(message, reader);
                this->messageReceivedChannel.post(message);
            }
            catch (const Exception& e)
            {
                postError<ErrorReadFromSocketFailed>();
                this->active = false;
                break;
            }
        }
        Logger::printfln("Reader loop exited");
        this->stopped = true;
    }
              
    void SocketReaderModule::setMessageReceivedChannel(Channel<RemoteConnection::Message> messageReceivedChannel)
    {
        this->messageReceivedChannel = messageReceivedChannel;
    }
    
    
    void SocketReaderModule::setErrorChannel(Channel<RemoteConnection::Error> errorChannel)
    {
        this->errorChannel = errorChannel;
    }

    void SocketReaderModule::stop()
    {
        Logger::printfln("ReaderModule::stop %d %d", this->stopped, this->active);
        if(!this->stopped)
        {
            this->stopped = true;
            this->active = false;
        }
        Logger::printfln("ReaderModule::end stop %d %d", this->stopped, this->active);
    }

    bool SocketReaderModule::isStopped()
    {
        return this->stopped;
    }

    void SocketReaderModule::terminate()
    {
        Logger::printfln("SocketReaderModule has terminated");
        this->messageReceivedChannel.unpublish();
        this->errorChannel.unpublish();
    }

    
   
}
}