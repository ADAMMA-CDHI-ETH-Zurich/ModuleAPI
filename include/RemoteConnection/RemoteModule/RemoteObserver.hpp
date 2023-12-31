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
#include "RemoteConnection/Message/MessageHeader/MessageHeaderChannelData.hpp"
#include "RemoteConnection/Message/MessageHeader/MessageHeaderKeepAlive.hpp"
#include "RemoteConnection/Message/MessageHeader/MessageHeaderKeepAliveResponse.hpp"

#include "RemoteConnection/Message/MessageData/MessageDataString.hpp"
#include "RemoteConnection/Message/MessageData/MessageDataBinary.hpp"
#include "RemoteConnection/Message/MessageData/MessageDataEmpty.hpp"


#include "Utilities/VariadicTemplateHelpers.hpp"
#include "RemoteConnection/RemoteModule/Observer.hpp"
#include <map>

namespace claid
{
    namespace RemoteConnection
    {
        // Observes everything we do locally (i.e. channels 
        // that were (un)published or (un)subscribed within the RunTime (global ChannelManager)).
        class RemoteObserver : public Observer
        {
            private:
                
                // This holds the references used to subscribe to the channels with the given ID, 
                // when it was subscribed to by the remotely running RunTime for the first time.
                // Only for the first subscription, we need to have a callback (in order to be able to send
                // the data to the remote RunTime whenever new data was published to the channel running in this local RunTime).
                // In other words: If (a Module in) the remote RunTime subscribes to a channel, we will be notified.
                // We then check if we already subscribed to that channel aswell. If no, we subscribe it and register a callback function,
                // so that whenever data is published to the channel in this local RunTime, we can send it to the remote RunTime.
                // If yes, then we also subscribe to the channel (in order to mirror the amount of subscriptions in the remote RunTime,
                // make sure we have the same amount of subscribers in all the connected RunTimes), but do not register a callback, as it is
                // not necessary to send the data multiple times.
                std::map<std::string, Channel<Untyped>> subscribedChannelsWithCallback;

                // See comment above. This map holds the references used to subscribe to the channels with the given ID WITHOUT a registered callbacks.
                std::multimap<std::string, Channel<Untyped>> subscribedChannels;

                // This holds the references used to publish the channels with the given IDs.
                // Whenever a channel was published in the remote RunTime, we publish it aswell, in order to match/mirror the number of channels published
                // in the other framework.
                std::multimap<std::string, Channel<Untyped>> publishedChannels;
                
                ChannelManager* globalChannelManager = nullptr;

                static const std::string IS_DATA_RECEIVED_FROM_REMOTE_TAG;

                // Interval in which a keep alive message is sent via the remote connection.
                // A reply to that message needs to be received before the next message is sent,
                // otherwise the connection is considered dead.
                static const uint32_t KEEP_ALIVE_INTERVAL_MILLISECONDS;
                bool keepAliveResponseReceived = false;
                bool hasKeepAliveMessageBeenSend = false;

                void onMessageReceived(ChannelData<Message> message);
                void onChannelUpdateMessage(const MessageHeaderChannelUpdate& header, const MessageDataString& data);
                void onChannelDataMessage(const MessageHeaderChannelData& header, const MessageDataBinary& data);            

                void onChannelSubscribed(const std::string& channelID);
                void onChannelPublished(const std::string& channelID);
                void onChannelUnsubscribed(const std::string& channelID);
                void onChannelUnpublished(const std::string& channelID);

                void onNewLocalDataInChannelThatRemoteRunTimeHasSubscribedTo(std::string channelID, ChannelData<Untyped> binaryData);            
                void sendMessage(Message message);

                void onChannelDataReceivedFromRemoteRunTime(const std::string& targetChannel, TaggedData<BinaryData>& data);

                void initialize();
                void terminate();
            

                void setIsDataReceivedFromRemoteRunTime(TaggedData<BinaryData>& data);
                bool isDataReceivedFromRemoteRunTime(TaggedData<BinaryData> data);

                void sendPeriodicKeepAliveMessage();
                void onKeepAliveMessage(const MessageHeaderKeepAlive& header, const MessageDataEmpty& data);
                void onKeepAliveResponse(const MessageHeaderKeepAliveResponse& header, const MessageDataEmpty& data);

                template<typename Header, typename Data, typename Class, typename... Args>
                bool callFunctionIfSignatureMatches(const Message& message, void (Class::*f)(const Header& header, const Data& data, Args...), Class* obj, Args... args)
                {
                    const MessageHeader& messageHeader = *message.header.get();
                    const MessageData& messageData = *message.data.get();

                    if(messageHeader.is<Header>())
                    {
                        const Header& typedHeader = *messageHeader.as<Header>();

                        if(messageData.is<Data>())
                        {
                            const Data& typedData = *messageData.as<Data>();
                            std::function<void (const Header&, const Data&, Args...)> function = VariadicTemplateHelpers::bind_with_variadic_placeholders<Class, const Header&, const Data&, Args...>(f, obj);

                            function(typedHeader, typedData, args...);
                            return true;
                        }
                    }
                    return false;
                }

                template<typename Header, typename Data, typename Class, typename... Args>
                bool callFunctionIfSignatureMatchesThrowExceptionIfWrongData(const Message& message, void (Class::*f)(const Header& header, const Data& data, Args...), Class* obj, Args... args)
                {
                    const MessageHeader& messageHeader = *message.header.get();
                    const MessageData& messageData = *message.data.get();

                    if(messageHeader.is<Header>())
                    {
                        const Header& typedHeader = *messageHeader.as<Header>();

                        if(messageData.is<Data>())
                        {
                            const Data& typedData = *messageData.as<Data>();
                            std::function<void (const Header&, const Data&, Args...)> function = VariadicTemplateHelpers::bind_with_variadic_placeholders<Class, const Header&, const Data&, Args...>(f, obj);

                            function(typedHeader, typedData, args...);
                            return true;
                        }
                        else
                        {
                            CLAID_THROW(Exception, "Error! Received Message with header type \"" << ClassFactory::getInstance()->getClassNameOfObject(messageHeader) << "\" and expected data "
                            << "to be of type \"" << ClassFactory::getInstance()->getClassName<Data>() << "\", but got data with type \"" << ClassFactory::getInstance()->getClassNameOfObject(messageData) << "\".");
                        }
                    }
                    return false;
                }


                                
                template<typename T>
                Channel<T> subscribe(const std::string& channelID)
                {
                    verifySafeAccessToChannels(channelID);
                    return this->globalChannelManager->subscribe<T>(channelID, this->getUniqueIdentifier());
                }

                template<typename T, typename Class>
                Channel<T> subscribe(const std::string& channelID,
                                void (Class::*f)(ChannelData<T>), Class* obj)
                {
                    verifySafeAccessToChannels(channelID);
                    std::function<void (ChannelData<T>)> function = std::bind(f, obj, std::placeholders::_1);
                    return subscribe(channelID, function); 
                }

                template<typename T>
                Channel<T> subscribe(const std::string& channelID, std::function<void (ChannelData<T>)> function)
                {
                    verifySafeAccessToChannels(channelID);
                    // runtime::getChannel(channelID).subscribe()
                    ChannelSubscriber<T> channelSubscriber(this->runnableDispatcherThread, function);
                    return this->globalChannelManager->subscribe<T>(channelID, channelSubscriber, this->getUniqueIdentifier());
                }

                template<typename T>
                Channel<T> subscribe(const std::string& channelID, ChannelSubscriber<T> channelSubscriber)
                {
                    verifySafeAccessToChannels(channelID);
                    return this->globalChannelManager->subscribe(channelID, channelSubscriber, this->getUniqueIdentifier());
                }

                template<typename T>
                Channel<T> publish(const std::string& channelID)
                {
                    verifySafeAccessToChannels(channelID);
                    return this->globalChannelManager->publish<T>(channelID, this->getUniqueIdentifier());
                }


            public:

                RemoteObserver(ChannelManager* globalChannelManager);

                ChannelSubscriber<Message> getSubscriberForReceptionOfMessages();


        };
    }
}