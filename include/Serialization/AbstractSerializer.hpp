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
#include "TypeChecking/TypeCheckingFunctions.hpp"
#include "Exception/Exception.hpp"
#include <fstream>

namespace claid
{
    class AbstractSerializer
    {
        public:
            virtual std::string getReflectorName() = 0;
            virtual ~AbstractSerializer()
            {

            }

            // Not all Serializers might require a header.
            // A header exists only once per file.
            // E.g., in a CSV, a header would be written only once at the start.
            virtual void writeHeaderToFile(const std::string& path, std::ofstream& file)
            {
            }

            virtual void writeDataToFile(const std::string& path, std::ofstream& file)
            {
                CLAID_THROW(Exception, "Serializer " << this->getReflectorName() << "was used to serialize some data.\n"
                << "It was requested to store the serialized data in file \"" << path << "\", however serializer "
                << this->getReflectorName() << " does not support writing serialized data to a file.");
            }

            virtual void forceReset()
            {

            }
            
    };
}