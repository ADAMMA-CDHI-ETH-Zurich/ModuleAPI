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
#include "Reflection/RecursiveReflector.hpp"
#include "Serialization/AbstractDeserializer.hpp"

namespace claid
{
    template<typename Derived>
    class Deserializer : public RecursiveReflector<Derived>, public AbstractDeserializer
    { 
        public:
            static constexpr bool isReadOnly = false;

            // Empty function which can be overriden.
            // It is not used by all deserializers, so we do not require it to be there.
            template<typename MemberType, typename Type>
            void pointer_to_member(std::string name, Type MemberType::*ptr)
            {
               
            }

            template<typename Class, typename ReturnType, typename... Args>
            void pointer_to_function(std::string name, ReturnType (Class::*function)(Args...))
            {
                
            }

            template<typename ReturnType, typename ClassType, typename... Args>
            void pointer_to_function(std::string name, ReturnType (ClassType::*ptr)(Args...) const)
            {

            }
    };
}