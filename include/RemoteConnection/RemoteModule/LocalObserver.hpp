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

#include "RunTime/RunTime.hpp"
#include "RemoteConnection/Message/Message.hpp"

#include "RemoteConnection/Message/MessageHeader/MessageHeaderChannelUpdate.hpp"
#include "RemoteConnection/Message/MessageData/MessageDataString.hpp"
#include "RemoteConnection/RemoteModule/Observer.hpp"


namespace claid
{
    namespace RemoteConnection
    {
        // Observes everything we do locally (i.e. channels 
        // that were (un)published or (un)subscribed within the RunTime (global ChannelManager)).
        class LocalObserver : public Observer
        {
            private:
                ChannelManager* observedChannelManager = nullptr;
      

                // Does not receive data from network, but is able to send data over network.
                Channel<std::string> subscribedChannel;
                Channel<std::string> publishedChannel;
                Channel<std::string> unsubscribedChannel;
                Channel<std::string> unpublishedChannel;

                void onChannelSubscribedCallback(ChannelData<std::string> channelID);
                void onChannelPublishedCallback(ChannelData<std::string> channelID);
                void onChannelUnsubscribedCallback(ChannelData<std::string> channelID);
                void onChannelUnpublishedCallback(ChannelData<std::string> channelID);

                void onChannelSubscribed(const std::string& channelID);
                void onChannelPublished(const std::string& channelID);
                void onChannelUnsubscribed(const std::string& channelID);
                void onChannelUnpublished(const std::string& channelID);

                void terminate();



                Message createChannelUpdateMessage(MessageHeaderChannelUpdate::UpdateType type, const std::string& string);
    

            public:
                LocalObserver();
                void observe(ChannelManager* manager);

        };
    }
}