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
#include "RunTime/RunTime.hpp"

namespace claid
{
    template<typename T>
    Channel<T> SubModule::subscribeLocal(const std::string& channelID)
    {
        std::string channelIDWithNamespace = this->addNamespacesToChannelID(channelID);
        verifySafeAccessToChannels(channelIDWithNamespace);
        return this->channelManager->subscribe<T>(channelIDWithNamespace, this->getUniqueIdentifier());
    }

    template<typename T, typename Class>
    Channel<T> SubModule::subscribeLocal(const std::string& channelID,
                    void (Class::*f)(ChannelData<T>), Class* obj)
    {
        verifySafeAccessToChannels(channelID);
        std::function<void (ChannelData<T>)> function = std::bind(f, obj, std::placeholders::_1);
        return subscribeLocal(channelID, function); 
    }

    template<typename T>
    Channel<T> SubModule::subscribeLocal(const std::string& channelID, std::function<void (ChannelData<T>)> function)
    {
        std::string channelIDWithNamespace = this->addNamespacesToChannelID(channelID);
        verifySafeAccessToChannels(channelIDWithNamespace);
        // runtime::getChannel(channelID).subscribe()
        ChannelSubscriber<T> channelSubscriber(this->runnableDispatcherThread, function);
        return this->channelManager->subscribe<T>(channelIDWithNamespace, channelSubscriber, this->getUniqueIdentifier());
    }

    template<typename T>
    Channel<T> SubModule::subscribeLocal(const std::string& channelID, ChannelSubscriber<T> channelSubscriber)
    {
        std::string channelIDWithNamespace = this->addNamespacesToChannelID(channelID);
        verifySafeAccessToChannels(channelIDWithNamespace);
        return this->channelManager->subscribe(channelIDWithNamespace, channelSubscriber, this->getUniqueIdentifier());
    }

    template<typename T>
    Channel<T> SubModule::publishLocal(const std::string& channelID)
    {
        std::string channelIDWithNamespace = this->addNamespacesToChannelID(channelID);
        verifySafeAccessToChannels(channelIDWithNamespace);
        return this->channelManager->publish<T>(channelIDWithNamespace, this->getUniqueIdentifier());
    }
}


namespace claid
{


template<typename T>
Channel<T> Module::subscribe(const std::string& channelID)
{
    std::string channelIDWithNamespace = this->addNamespacesToChannelID(channelID);
    verifySafeAccessToChannels(channelIDWithNamespace);
    return this->channelManager->subscribe<T>(channelIDWithNamespace, this->getUniqueIdentifier());
}

template<typename T, typename Class>
Channel<T> Module::subscribe(const std::string& channelID,
                void (Class::*f)(ChannelData<T>), Class* obj)
{
    verifySafeAccessToChannels(channelID);
    std::function<void (ChannelData<T>)> function = std::bind(f, obj, std::placeholders::_1);
    return subscribe(channelID, function); 
}

template<typename T>
Channel<T> Module::subscribe(const std::string& channelID, std::function<void (ChannelData<T>)> function)
{
    std::string channelIDWithNamespace = this->addNamespacesToChannelID(channelID);
    verifySafeAccessToChannels(channelIDWithNamespace);
    // runtime::getChannel(channelID).subscribe()
    ChannelSubscriber<T> channelSubscriber(this->runnableDispatcherThread, function);
    return this->channelManager->subscribe<T>(channelIDWithNamespace, channelSubscriber, this->getUniqueIdentifier());
}

template<typename T>
Channel<T> Module::subscribe(const std::string& channelID, ChannelSubscriber<T> channelSubscriber)
{
    std::string channelIDWithNamespace = this->addNamespacesToChannelID(channelID);
    verifySafeAccessToChannels(channelIDWithNamespace);
    return this->channelManager->subscribe(channelIDWithNamespace, channelSubscriber, this->getUniqueIdentifier());
}

template<typename T>
Channel<T> Module::publish(const std::string& channelID)
{
    std::string channelIDWithNamespace = this->addNamespacesToChannelID(channelID);
    verifySafeAccessToChannels(channelIDWithNamespace);
    return this->channelManager->publish<T>(channelIDWithNamespace, this->getUniqueIdentifier());
}
}


