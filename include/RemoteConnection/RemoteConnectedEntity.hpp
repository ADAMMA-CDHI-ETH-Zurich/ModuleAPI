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

#include "RemoteConnection/RemoteConnectedEntityUniqueIdentifier.hpp"
#include "RemoteConnection/RemoteModule/RemoteModule.hpp"
#include "RemoteConnection/ConnectionModule.hpp"
#include "RemoteConnection/ConnectionLink.hpp"
#include "RemoteConnection/Error/Error.hpp"

namespace claid
{
    namespace RemoteConnection
    {
        // A RemoteConnectedEntity is "some entity" (whatsoever), that is connected to the current RunTime remotely.
        // A RemoteConnectedEntity can be communicated with using a ConnectionModule, and it's actions are mirrored
        // to the current RunTime by a RemoteModule. The RemoteModule and the ConnectionModule are connected using a ConnectionLink.

        class RemoteConnectedEntity
        {
            private:
                // Explicitly forbid copying.
                RemoteConnectedEntity(const RemoteConnectedEntity&) = delete;

                RemoteModule remoteModule;
                ConnectionModule* connectionModule;
                ConnectionLink link;

                RemoteConnectedEntity(ConnectionModule* connectionModule);

                bool started = false;

            public:
                RemoteConnectedEntity() = delete;
                ~RemoteConnectedEntity();

                // We use factory function to make sure that the RemoteConnectedEntity uses
                // a ConnectionModule that was created by itself. I.e.: We make sure we have ownership
                // over the ConnectionModule and it's not exposed to "the outside world".
                template<typename ConnectionModuleType, typename... arguments>
                static RemoteConnectedEntity* Create(arguments... args)
                {
                    Logger::printfln("Factory");
       
               
                    return new RemoteConnectedEntity(new ConnectionModuleType(args...));
                }

                void setup();
                void disintegrate();

                void start();
                void stop();

                Channel<Error> subscribeToErrorChannel(ChannelSubscriber<Error> channelSubscriber);
                Channel<Error> registerToErrorChannel();

                RemoteConnectedEntityUniqueIdentifier getUniqueIdentifier();


        };
    }
}