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
#include "XMLNode.hpp"
#include <string>
#include <memory.h>

namespace claid
{
    class XMLVal : public XMLNode
    {
        protected:
                std::string value;

        public:



            XMLVal(std::shared_ptr<XMLNode> parent, const std::string& name, const std::string& value) : XMLNode(parent, name), value(value)
            {
            }

            virtual ~XMLVal()
            {
                
            }

            virtual void toString(std::string& string)
            {
                std::string openingTag;
                std::string closingTag;
                this->getOpeningTag(openingTag);
                this->getClosingTag(closingTag);
                string = this->getTabs() + openingTag + value + closingTag;
            }


            virtual bool fromString(const std::string& string)
            {
                this->value = string;
                //parseFromString(this->value, string);
                return true;
            }

            virtual void contentAsString(std::string& string)
            {
                string = this->value;
            }

            const std::string& getValue()
            {
                return this->value;
            }

    };
}