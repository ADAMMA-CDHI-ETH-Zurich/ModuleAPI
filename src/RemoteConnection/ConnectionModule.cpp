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

#include "RemoteConnection/ConnectionModule.hpp"

namespace claid
{
namespace RemoteConnection
{
    const std::string ConnectionModule::RECEIVE_CHANNEL = "ReceiveChannel";
    const std::string ConnectionModule::SEND_CHANNEL = "SendChannel";
    const std::string ConnectionModule::ERROR_CHANNEL = "ErrorChannel";

    ConnectionModule::ConnectionModule()
    {

    }
                
    // When someone wants to send a message via the remote connection.
    // Called by external module.
    void ConnectionModule::onSendMessage(ChannelData<Message> message)
    {
        this->sendMessage(message->value());
    }

    void ConnectionModule::initialize()
    {
        this->sendChannel = this->subscribeLocal<Message>(SEND_CHANNEL, &ConnectionModule::onSendMessage, this);
        this->receiveChannel = this->publishLocal<Message>(RECEIVE_CHANNEL);
        this->errorChannel = this->publishLocal<Error>(ERROR_CHANNEL);
        this->setup();
    }

    void ConnectionModule::terminate()
    {
        this->sendChannel.unsubscribe();
        this->receiveChannel.unpublish();
        this->errorChannel.unpublish();
    }

    Channel<Message> ConnectionModule::subscribeToReceiveChannel(ChannelSubscriber<Message> channelSubscriber)
    {
        return subscribeLocal<Message>(RECEIVE_CHANNEL, channelSubscriber);
    }

    Channel<Message> ConnectionModule::registerToReceiveChannel()
    {
        return publishLocal<Message>(RECEIVE_CHANNEL);
    }


    Channel<Message> ConnectionModule::registerToSendChannel()
    {
        return publishLocal<Message>(SEND_CHANNEL);
    }

    Channel<Error> ConnectionModule::subscribeToErrorChannel(ChannelSubscriber<Error> channelSubscriber)
    {
        return subscribeLocal<Error>(ERROR_CHANNEL, channelSubscriber);
    }

    Channel<Error> ConnectionModule::registerToErrorChannel()
    {
        return publishLocal<Error>(ERROR_CHANNEL);
    }

    void ConnectionModule::setup()
    {

    }



    
    
}
}
