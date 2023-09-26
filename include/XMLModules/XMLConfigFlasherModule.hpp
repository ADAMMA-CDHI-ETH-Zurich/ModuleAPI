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
#include "XMLConfigFlashDescription.hpp"
#include "XMLConfigFlashedModulesDescription.hpp"
#include "XMLFlasherUniqueIdentifier.hpp"
namespace claid
{
    class XMLConfigFlasherModule : public Module
    {

        private:
            XMLFlasherUniqueIdentifier uniqueIdentifier;
            bool uniqueIdentifierSet = false;

            std::string xmlChannelName;
            Channel<XMLConfigFlashDescription> xmlChannel;

            std::string uniqueIdentifierChannelName;
            Channel<XMLFlasherUniqueIdentifier> uniqueIdentifierChannel;

            std::string errorChannelName;
            Channel<std::string> errorChannel;

            std::string flashedModulesChannelName;
            Channel<XMLConfigFlashedModulesDescription> flashedModulesChannel;


        public:

            XMLConfigFlasherModule();
 
            void initialize();

            void onXMLDataReceived(ChannelData<XMLConfigFlashDescription> data);
            void onUniqueIdentifierReceived(ChannelData<XMLFlasherUniqueIdentifier> uniqueIdentifier);


            Reflect(XMLConfigFlasherModule, 
                reflectMember(xmlChannelName);
                reflectMember(uniqueIdentifierChannelName);
                reflectMember(errorChannelName);
                reflectMember(flashedModulesChannelName);
            )
    };
}