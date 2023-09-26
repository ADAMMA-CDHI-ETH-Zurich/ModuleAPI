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
#include "CLAID.hpp"

namespace claid
{
    class XMLReceiverModule : public Module
    {

        private:
            std::string xmlChannelName;
            Channel<std::string> xmlChannel;

            // Specifies whether an exception shall be thrown if XML is invalid.
            bool throwExceptionOnFailure = false;

        public:


            XMLReceiverModule();
            XMLReceiverModule(std::string xmlChannelName, bool throwExceptionOnFailure);

            void initialize();

            void onXMLDataReceived(ChannelData<std::string> data);


            Reflect(XMLReceiverModule, 
                reflectMember(xmlChannelName);
                reflectMemberWithDefaultValue(throwExceptionOnFailure, false);
            )
    };
}