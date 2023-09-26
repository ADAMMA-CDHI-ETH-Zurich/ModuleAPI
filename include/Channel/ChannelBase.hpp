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

#include <string>
#include <stdint.h>

#include "ChannelBufferBase.hpp"
namespace claid
{
    class ChannelBase
    {
        public:
            virtual intptr_t getChannelDataTypeUniqueIdentifier() = 0;
            virtual std::string getChannelDataTypeName() = 0;

            ChannelBase()
            {
                
            }

            ChannelBase(std::string channelID) : channelID(channelID)
            {

            }

            virtual ~ChannelBase()
            {

            }

            const std::string& getChannelID() const
            {
                return this->channelID;
            }

            virtual size_t getNumPublishers() = 0;
            virtual size_t getNumSubscribers() = 0;

            void writeBinaryData(const BinaryData& data);
            void readBinaryData(BinaryData& data);

            bool isTyped() const
            {
                return this->channelBuffer->isTyped();
            }


        protected:
            std::vector<ChannelSubscriberBase*> channelSubscribers;
            std::string channelID;        

            size_t numPublishers = 0;
            size_t numSubscribers = 0;

            ChannelBufferBase* channelBuffer;
    };
}