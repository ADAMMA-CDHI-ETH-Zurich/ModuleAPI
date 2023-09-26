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
#include "RemoteConnection/ConnectionLink.hpp"

namespace claid
{
namespace RemoteConnection
{
    void ConnectionLink::link(ConnectionModule* connection, RemoteModule* module)
    {
        printf("ConnectionLink::link");
        if(connection == nullptr)
        {
            CLAID_THROW(Exception, "Error, tried to link ConnectionModule and RemoteModule, but ConnectionModule is null.");
        }

        if(module == nullptr)
        {
            CLAID_THROW(Exception, "Error, tried to link ConnectionModule and RemoteModule, but RemoteModule is null.");
        }

        if(!connection->isInitialized())
        {
            CLAID_THROW(Exception, "Error, tried to link ConnectionModule of type " << connection->getModuleName() << " to RemoteModule"
            << " without ConnectionModule being initialized. Please call startModule before linkage");
        }

        if(!module->isInitialized())
        {
            CLAID_THROW(Exception, "Error, tried to link RemoteModule to ConnectionModule of type " << connection->getModuleName() 
            << " without RemoteModule being initialized. Please call startModule before linkage");
        }

        if(connection == nullptr)
        {
            CLAID_THROW(Exception, "Error: Tried to call ConnectionModule.link with a ConnectionModule that is null " << __FILE__ << __LINE__);
        }

        if(module == nullptr)
        {
            CLAID_THROW(Exception, "Error: Tried to call ConnectionModule.link with a RemoteModule that is null " << __FILE__ << __LINE__);
        }

        this->connection = connection;
        this->remoteModule = module;

        ChannelSubscriber<Message> subscriber = module->getSubscriberForReceptionOfMessages();


        this->remoteModule->setReceiveMessageChannel(connection->subscribeToReceiveChannel(subscriber));
        this->remoteModule->setSendMessageChannel(connection->registerToSendChannel());
        this->remoteModule->setErrorChannel(connection->registerToErrorChannel());
    }

    void ConnectionLink::unlink()
    {
        if(this->connection == nullptr)
        {
            CLAID_THROW(Exception, "Error: Tried to call ConnectionModule.link with a ConnectionModule that is null " << __FILE__ << __LINE__);
        }

        if(this->remoteModule == nullptr)
        {
            CLAID_THROW(Exception, "Error: Tried to call ConnectionModule.link with a RemoteModule that is null " << __FILE__ << __LINE__);
        }

        
        this->remoteModule->unsubscribeReceiveMessageChannel();
        this->remoteModule->unpublishSendMessageChannel();
        this->remoteModule->unpublishErrorChannel();
    }
}
}
