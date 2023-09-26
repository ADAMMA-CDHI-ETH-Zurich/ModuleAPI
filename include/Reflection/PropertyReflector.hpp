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

namespace claid
{
    class PropertyReflector : public RecursiveReflector<PropertyReflector>
    {


    public:

        std::string getReflectorName()
        {
            return "PropertyReflector";
        }
   

        template<typename T>
        void callFloatOrDouble(const char* property, T& member)
        {
            if (this->defaultValueCurrentlySet())
            {
                member = this->getCurrentDefaultValue<T>();
            }
        }

        // Also includes any variants of signed, unsigned, short, long, long long, ...
        template<typename T>
        void callInt(const char* property, T& member)
        {
            if (this->defaultValueCurrentlySet())
            {
                member = this->getCurrentDefaultValue<T>();
            }
        }

        void callBool(const char* property, bool& member)
        {
            if (this->defaultValueCurrentlySet())
            {
                member = this->getCurrentDefaultValue<bool>();
            }        
        }

        template<typename T>
        void callString(const char* property, T& member)
        {
            if (this->defaultValueCurrentlySet())
            {
                member = this->getCurrentDefaultValue<T>();
            }
        }

        template<typename T>
        void callBeginClass(const char* property, T& member)
        {
            
        }

        template<typename T>
        void callEndClass(const char* property, T& member)
        {
        }

        template<typename T>
        void callPointer(const char* property, T*& member)
        {
            if (this->defaultValueCurrentlySet())
            {
                member = this->getCurrentDefaultValue<T>();
            }
        }


        template<typename T>
        void reflect(std::string name, T& obj)
        {
            //this->callAppropriateFunctionBasedOnType(name.c_str(), obj);
        }
      
    };
}