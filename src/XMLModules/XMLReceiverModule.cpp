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
#include "XMLModules/XMLReceiverModule.hpp"
#include "XML/XMLDocument.hpp"

namespace claid
{
    XMLReceiverModule::XMLReceiverModule()
    {

    }

    XMLReceiverModule::XMLReceiverModule(std::string xmlChannelName, bool throwExceptionOnFailure) : xmlChannelName(xmlChannelName), throwExceptionOnFailure(throwExceptionOnFailure)
    {

    }

    void XMLReceiverModule::initialize()
    {
        this->xmlChannel = subscribe<std::string>(this->xmlChannelName, &XMLReceiverModule::onXMLDataReceived, this);
    }

    void XMLReceiverModule::onXMLDataReceived(ChannelData<std::string> data)
    {
        XMLDocument document;
        if(!document.loadFromString(data->value()))
        {
            if(this->throwExceptionOnFailure)
            {   
                CLAID_THROW(claid::Exception, "Error in XMLReceiverModule. Cannot load XML from string, data seems to be invalid:\n " << data->value());
            }
            else
            {
                Logger::printfln("Warning in XMLReceiverModule: Failed to load XML from string, data seems to be invalid:\n%s", data->value().c_str());
            }
        }
    }
}

REGISTER_MODULE(claid::XMLReceiverModule)