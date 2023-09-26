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
#include "Getter.hpp"
#include "Setter.hpp"
#include <string>
namespace claid
{
    

    template<typename T>
    class VariableWithGetterSetter
    {

        private:
            Getter<T> getter;
            Setter<T> setter;
            std::string variableName;
            const T* defaultValue;

     

        public:

            typedef typename Getter<T>::GetterValueType GetterValueType;
            typedef T ValueType;
           

            VariableWithGetterSetter(std::string variableName, 
            Getter<T> getter, Setter<T> setter,
            const T* defaultValue = nullptr) : variableName(variableName), getter(getter), setter(setter), defaultValue(defaultValue)
            {
                
            }

            VariableWithGetterSetter& operator=(const T& other)
            {
                setter(other);
                return *this;
            }

            GetterValueType get() const
            {
                return getter.get();
            }

            operator GetterValueType() const
            {
                return this->get();
            }

            template<typename Reflector>
            void reflect(Reflector& r)
            {
                splitReflectInType(r, *this);
            }

            template<typename Reflector>
            void reflectRead(Reflector& r)
            {
                T tmp;
                if(defaultValue != nullptr)
                {   
                    r.member(this->variableName.c_str(), tmp, "", *defaultValue);
                }
                else
                {
                    r.member(this->variableName.c_str(), tmp, "");
                }
                setter(tmp);
            }

            template<typename Reflector>
            void reflectWrite(Reflector& r)
            {
                T tmp = getter;
                if(defaultValue != nullptr)
                {   
                    r.member(this->variableName.c_str(), tmp, "", *defaultValue);
                }
                else
                {
                    r.member(this->variableName.c_str(), tmp, "");
                }            
            }

          
    };

  

}