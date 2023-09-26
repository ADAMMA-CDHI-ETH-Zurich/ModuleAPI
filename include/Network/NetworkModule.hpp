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

#include "Utilities/Time.hpp"

#include "RunTime/RunTime.hpp"
#include "Binary/BinaryData.hpp"
#include "RemoteConnection/Message/Message.hpp"
#include "RemoteConnection/Error/Error.hpp"
#include "Network/Error/NetworkErrors.hpp"
#include "Network/NetworkStateChangeRequest.hpp"
#include "Reflection/Reflect.hpp"
namespace claid
{
    namespace Network
    {
        class NetworkModule : public Module
        {    
            protected:
                Channel<NetworkStateChangeRequest> networkStateChangeRequestChannel;
                std::string listenForNetworkChangeRequestsOn;

                virtual void initialize()
                {
                    if(this->listenForNetworkChangeRequestsOn != "")
                    {
                        this->networkStateChangeRequestChannel = 
                            this->subscribe<NetworkStateChangeRequest>(
                                    this->listenForNetworkChangeRequestsOn, &NetworkModule::onNetworkStateChangeRequestedCallback, this);
                    }
                }

                virtual void onNetworkStateChangeRequested(const NetworkStateChangeRequest& networkStateChangeRequest)
                {
                    
                }

            private:
                void onNetworkStateChangeRequestedCallback(ChannelData<NetworkStateChangeRequest> data)
                {
                    this->onNetworkStateChangeRequested(data->value());
                }

            
                

            public: 

                Reflect(NetworkModule,
                    reflectMemberWithDefaultValue(listenForNetworkChangeRequestsOn, std::string(""));
                )
          
                virtual ~NetworkModule()
                {

                }         
        
        };
    }

    
}