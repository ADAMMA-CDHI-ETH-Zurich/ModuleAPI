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

#include "Channel.hpp"
#include "Untyped.hpp"
#include <map>

namespace claid
{


    class ChannelManager
    {
        private:
            const static std::string ON_CHANNEL_SUBSCRIBED_CHANNEL;
            const static std::string ON_CHANNEL_PUBLISHED_CHANNEL;
            const static std::string ON_CHANNEL_UNSUBSCRIBED_CHANNEL;
            const static std::string ON_CHANNEL_UNPUBLISHED_CHANNEL;

            std::map<std::string, std::shared_ptr<ChannelBase>> typedChannels;
            
            std::mutex channelMutex;

            std::shared_ptr<TypedChannel<std::string>> onChannelSubscribedChannel;
            std::shared_ptr<TypedChannel<std::string>> onChannelPublishedChannel;
            std::shared_ptr<TypedChannel<std::string>> onChannelUnsubscribedChannel;
            std::shared_ptr<TypedChannel<std::string>> onChannelUnpublishedChannel;

            bool accessToChannelsBlocked = false;

            template<typename T>
            typename std::enable_if<!std::is_same<T, Untyped>::value,bool>::type
            canCastChannel(std::shared_ptr<ChannelBase> channel)
            {              
                intptr_t a = channel->getChannelDataTypeUniqueIdentifier();
                intptr_t b = TypeChecking::getDataTypeUniqueIdentifier<T>();
                std::string name = channel->getChannelDataTypeName();
                std::string name2 = typeid(T).name();
                return channel->getChannelDataTypeUniqueIdentifier() == TypeChecking::getDataTypeUniqueIdentifier<T>();
            }

            template<typename T>
            typename std::enable_if<std::is_same<T, Untyped>::value,bool>::type
            canCastChannel(std::shared_ptr<ChannelBase> channel)
            {              
                // Every channel can be casted to Untyped channel.
                // Why, you ask? Even that TypedChannel<T> does not inherit from Channel<Untyped>?
                // Well, that's because TypedChannel<T> does not introduce any new member variables
                // compared to ChannelBase. In other words, TypedChannel<T> extends ChannelBase ONLY
                // by FUNCTIONS and NOT by variables.
                // Thus, TypedChannel<T> merely acts as a "view" to the underlying data.
                
                // But wait, doesn't that mean we could cast every TypedChannel<T> to every other
                // TypedChannel<T>? Well yes, theoretically. However, when you'd call post you might
                // end up inserting wrong data into the ChannelBuffer. That's why for 
                // TypedChannel<Untyped> (and Channel<Untyped>), the post function is disabled.
                return true;
            }

            template<typename T>
            std::shared_ptr<TypedChannel<T>> castChannel(std::shared_ptr<ChannelBase> channel)
            {
                return std::static_pointer_cast<TypedChannel<T>>(channel);
            }

            template<typename T>
            std::shared_ptr<TypedChannel<T>> registerNewChannel(const std::string& channelID)
            {
                std::shared_ptr<TypedChannel<T>> newChannel = std::shared_ptr<TypedChannel<T>>(new TypedChannel<T>(this, channelID));

                // Dont lock the mutex here, as it already get's locked in publish or subscribe functions.
                //std::unique_lock<std::mutex> lock(this->channelMutex);
                this->typedChannels.insert(std::make_pair(channelID, std::static_pointer_cast<ChannelBase>(newChannel)));

                return newChannel;       
            }

            void onChannelSubscribed(const std::string& channelID, uint64_t uniqueModuleID);
            void onChannelPublished(const std::string& channelID, uint64_t uniqueModuleID);
            void onChannelUnsubscribed(const std::string& channelID, uint64_t uniqueModuleID);
            void onChannelUnpublished(const std::string& channelID, uint64_t uniqueModuleID);

            // Explicitly forbid copying.
            ChannelManager(const ChannelManager&) = delete;

            template<typename T>
            bool isObserverChannel(Channel<T>& channel)
            {
                const std::vector<std::string> observerChannelIDs = {ON_CHANNEL_SUBSCRIBED_CHANNEL, ON_CHANNEL_PUBLISHED_CHANNEL, ON_CHANNEL_UNSUBSCRIBED_CHANNEL, ON_CHANNEL_UNPUBLISHED_CHANNEL};

                const std::string& channelID = channel.getChannelID();

                auto it = std::find(observerChannelIDs.begin(), observerChannelIDs.end(), channelID);

                return it != observerChannelIDs.end();
            }

            // Needed because we use stopObserving in function unsubscribe(), which can be evaluated 
            // for any given type. Therefore, t->unsubcribe(channel) results in an error, if 
            // T is not std::string (because observer channels just post data of type std::string, i.e. the channel ID).
            // But for such channels, stopObserving wouldn't be called anyways, because they are not one of the 4 observer channels.
            // I.e., isObserverChannel(channel is used before calling stopObserving).
            template<typename T, typename ChannelType>
            typename std::enable_if<std::is_same<T, std::string>::value>::type stopObservingHelper(Channel<T>& channel, std::shared_ptr<TypedChannel<ChannelType>>& t)
            {
                Logger::printfln("Unsubscribing observer %s", channel.getChannelID().c_str());

                t->unsubscribe(channel);
            }

            template<typename T, typename ChannelType>
            typename std::enable_if<!std::is_same<T, std::string>::value>::type stopObservingHelper(Channel<T>& channel, std::shared_ptr<TypedChannel<ChannelType>>& t)
            {
                // Do nothing, should never get called.
                // See comment above.
                CLAID_THROW(Exception, "Error, stopObservingCalled with a channel that is not one of the 4 observer channels. This should never happen and is a programming error!");
            }

            template<typename T>
            void stopObserving(Channel<T>& channel)
            {
                const std::string& channelID = channel.getChannelID();
       
                if(channelID == ON_CHANNEL_SUBSCRIBED_CHANNEL)
                {
                    stopObservingHelper(channel, onChannelSubscribedChannel);
                }
                else if(channelID == ON_CHANNEL_PUBLISHED_CHANNEL)
                {   
                    stopObservingHelper(channel, onChannelPublishedChannel);
                }
                else if(channelID == ON_CHANNEL_UNSUBSCRIBED_CHANNEL)
                {
                    stopObservingHelper(channel, onChannelUnsubscribedChannel);
                }
                else if(channelID == ON_CHANNEL_UNPUBLISHED_CHANNEL)
                {
                    stopObservingHelper(channel, onChannelUnpublishedChannel);
                }
                else
                {
                    CLAID_THROW(Exception, "Error, stopObserving() was called with a channel that has channelID \"" << channelID.c_str() << "\", which is not an observer channel.");
                }
            }
    
            bool isInHiddenNamespace(const std::string& channelID) const;

        public:

            ChannelManager();
            ~ChannelManager();

            // template<typename T>
            // publish

            // If T is untyped, we can't type.
            // Need to pass TypedChannel<Untyped> as type UntypedChannel, otherwise
            // we get a weird compiler error.
            template <typename T, typename UntypedChannel>
            typename std::enable_if<std::is_same<T, Untyped>::value>::type
            typeChannelIfPossible(std::shared_ptr<ChannelBase> channel)
            {
            }

            template <typename T, typename UntypedChannel>
            typename std::enable_if<!std::is_same<T, Untyped>::value>::type
            typeChannelIfPossible(std::shared_ptr<ChannelBase> channel)
            {
                // Attempting to type channel
                std::shared_ptr<UntypedChannel> untypedChannel = std::static_pointer_cast<UntypedChannel>(channel);
                untypedChannel->template type<T>();
            }


            template<typename T>
            std::shared_ptr<TypedChannel<T>> getMakeTypedChannel(const std::string& channelID)
            {
                auto it = typedChannels.find(channelID);
                if(it != typedChannels.end())
                {
                    std::shared_ptr<ChannelBase> channel = it->second;

                    // If channel is untyped, but T is another type then Untyped, we can type the channel
                    if(!channel->isTyped())
                    {
                       typeChannelIfPossible<T, TypedChannel<Untyped>>(channel);
                    }                    

                    if(canCastChannel<T>(channel))
                    {
                        // Safely cast ChannelBase to TypedChannel.
                        std::shared_ptr<TypedChannel<T>> typedChannel = castChannel<T>(channel);
                        return typedChannel;
                    }
                    else
                    {
                        CLAID_THROW(Exception, "Cannot cast channel \"" << channelID << "\"! The channel has type " << channel->getChannelDataTypeName() << ", however an access with type " << TypeChecking::getCompilerSpecificCompileTypeNameOfClass<T>() << " was inquired!" );
                    }
                    
                }
                else
                {
                    // Channel not found, create new.
                    std::shared_ptr<TypedChannel<T>> newChannel = registerNewChannel<T>(channelID);   
                    return newChannel;
                }

            }
            

            // uniqueModuleID can be used to identify which module has subscribed / published to a channel.
            // It is only used for the onChannel(Un)Subscribed, onChannel(Un)Published functions.
            // So far only has a use for the RemoteModule
            template<typename T>
            Channel<T> subscribe(const std::string& channelID, uint64_t uniqueModuleID)
            {
                std::unique_lock<std::mutex> lock(this->channelMutex);

                Channel<T> returnChannel;

                std::shared_ptr<TypedChannel<T>> channel = getMakeTypedChannel<T>(channelID);

                returnChannel = channel->subscribe(uniqueModuleID);                  
                this->onChannelSubscribed(channelID, uniqueModuleID);
                return returnChannel;
            }

  

            template<typename T>
            Channel<T> subscribe(const std::string& channelID, ChannelSubscriber<T> channelSubscriber, uint64_t uniqueModuleID)
            {
                Logger::printfln("Subscribing %s", channelID.c_str());

                std::unique_lock<std::mutex> lock(this->channelMutex);


                Channel<T> returnChannel;
                std::shared_ptr<TypedChannel<T>> channel = getMakeTypedChannel<T>(channelID);

                returnChannel = channel->subscribe(channelSubscriber, uniqueModuleID);

                if(!this->isInHiddenNamespace(channelID))
                {
                    this->onChannelSubscribed(channelID, uniqueModuleID);
                }             
                return returnChannel;
            }

            template<typename T>
            Channel<T> publish(const std::string& channelID, uint64_t uniqueModuleID)
            {
                Logger::printfln("Publishing %s", channelID.c_str());
                std::unique_lock<std::mutex> lock(this->channelMutex);

                Channel<T> returnChannel;
                std::shared_ptr<TypedChannel<T>> channel = getMakeTypedChannel<T>(channelID);

                returnChannel = channel->publish(uniqueModuleID);
           

                if(!this->isInHiddenNamespace(channelID))
                {
                    this->onChannelPublished(channelID, uniqueModuleID);
                }
                return returnChannel;
            }

            template<typename T>
            void unsubscribe(Channel<T>& channelObject)
            {
                std::unique_lock<std::mutex> lock(this->channelMutex);

                // Here we dont use getMakeTypedChannel because we use the already created Channel<T> object to unsubscribe.

                const std::string channelID = channelObject.getChannelID();
                Logger::printfln("Unsubscribing %s", channelID.c_str());

                auto it = typedChannels.find(channelID);

                if(it != typedChannels.end())
                {
                    std::shared_ptr<ChannelBase> channel = it->second;
                    if(canCastChannel<T>(channel))
                    {
                        std::shared_ptr<TypedChannel<T>> typedChannel = castChannel<T>(channel);
                        typedChannel->unsubscribe(channelObject);
                    }
                    else
                    {
                        CLAID_THROW(Exception, "Error, cannot unsubscribe from channel with ID" << channelID << 
                            "The type of the channel (" << channel->getChannelDataTypeName() << ") cannot be cast to " << TypeChecking::getCompilerSpecificCompileTypeNameOfClass<T>() << ".");
                    }
                }
                else if(this->isObserverChannel(channelObject))
                {
                    this->stopObserving(channelObject);
                }
                else
                {
                    CLAID_THROW(Exception, "Error, unsubscribe was called on a channel with channel ID " << channelID << ", which does not exist. This should never happen and most likely is a programming mistake");
                }
                if(!this->isInHiddenNamespace(channelID))
                {
                    this->onChannelUnsubscribed(channelID, channelObject.getPublisherSubscriberUniqueIdentifier());
                }
            }

            template<typename T>
            void unpublish(Channel<T>& channelObject)
            {
                std::unique_lock<std::mutex> lock(this->channelMutex);

                // Here we dont use getMakeTypedChannel because we use the already created Channel<T> object to unpublish.


                const std::string channelID = channelObject.getChannelID();

                auto it = typedChannels.find(channelID);

                if(it != typedChannels.end())
                {
                    std::shared_ptr<ChannelBase> channel = it->second;
                    if(canCastChannel<T>(channel))
                    {
                        std::shared_ptr<TypedChannel<T>> typedChannel = castChannel<T>(channel);
                        typedChannel->unpublish(channelObject);
                    }
                    else
                    {
                        CLAID_THROW(Exception, "Error, cannot unpublish channel with ID" << channelID << 
                            "The type of the channel (" << channel->getChannelDataTypeName() << ") cannot be cast to " << TypeChecking::getCompilerSpecificCompileTypeNameOfClass<T>() << ".");
                    }
                }
                else
                {
                    CLAID_THROW(Exception, "Error, unpublish was called on a channel with channel ID " << channelID << ", which does not exist. This should never happen and most likely is a programming mistake");
                }

                if(!this->isInHiddenNamespace(channelID))  
                {
                    this->onChannelUnpublished(channelID, channelObject.getPublisherSubscriberUniqueIdentifier());
                }
            }

            size_t getNumChannels();

            const std::string& getChannelNameByIndex(size_t id);


            bool hasChannelPublisher(const std::string& channelID);
            bool hasChannelSubscriber(const std::string& channelID);

                 
            Channel<std::string> observeSubscribedChannels(ChannelSubscriber<std::string> subscriber, uint64_t observerUniqueModuleID);
            Channel<std::string> observePublishedChannels(ChannelSubscriber<std::string> subscriber, uint64_t observerUniqueModuleID);
            Channel<std::string> observeUnsubscribedChannels(ChannelSubscriber<std::string> subscriber, uint64_t observerUniqueModuleID);
            Channel<std::string> observeUnpublishedChannels(ChannelSubscriber<std::string> subscriber, uint64_t observerUniqueModuleID);

            void getChannelIDs(std::vector<std::string>& channelIDs);

    };
}

