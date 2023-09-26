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

#include <functional>

namespace claid
{
    

    template<class T>
    class Getter
    {
      
        private:
            std::function<T ()> getterFunction;

        public:

            typedef T GetterValueType;


            Getter(std::function<T()> getterFunction) : getterFunction(getterFunction)
            {

            }

            
            template<typename Class, typename... Params>
            Getter(T (Class::*getter)(Params...), Class* obj, Params... params) 
            {
                this->getterFunction = std::bind(getter, obj, params...);
            }

            T get() const
            {
                return getterFunction();
            }

            operator T() const
            {
                return this->get();
            }
   
   
    };

    
    
}