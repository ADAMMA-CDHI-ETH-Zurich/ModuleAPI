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

#include <memory>
#include "TaggedDataBase.hpp"
#include "Reflection/ReflectBase.hpp"
#include <stdint.h>
#include <iostream>
#include <algorithm>
namespace claid
{
    template<typename T>
    class TaggedData : public TaggedDataBase
    {
        private:
            std::shared_ptr<T> data;
       
         

        public:

            template<typename Reflector>
            void reflect(Reflector& r)
            {
                REFLECT_BASE(r, TaggedDataBase);
                r.member("Data", this->data, "");
            }

            TaggedData() {}

            TaggedData(const T& data) : TaggedDataBase(Time::now(), 0)
            {
                this->data = std::shared_ptr<T>(new T(data));
            }
    
            TaggedData(const T& data, const Time& timestamp, uint64_t sequenceID = 0) : TaggedDataBase(timestamp, sequenceID)
            {
                // Copy is necessary.
                this->data = std::shared_ptr<T>(new T(data));
            }

            TaggedData(std::shared_ptr<T>& data) : TaggedDataBase(Time::now(), 0)
            {
                this->data = data;
            }

            TaggedData(std::shared_ptr<T>& data, const Time& timestamp, uint64_t sequenceID = 0) : TaggedDataBase(timestamp, sequenceID)
            {
                this->data = data;
            }

            // T value()
            // {
            //     return *this->data.get();
            // }

            const T& value() const
            {
                return *this->data.get();
            }

           

            // void toBinary()
            // {

            // }

            // uint64_t getUniqueIdentifier() const
            // {
            //     return reinterpret_cast<uint64_t>(data.get());
            // }

            TaggedDataBase getHeader()
            {
                return *static_cast<TaggedDataBase*>(this);
            }
    };
}