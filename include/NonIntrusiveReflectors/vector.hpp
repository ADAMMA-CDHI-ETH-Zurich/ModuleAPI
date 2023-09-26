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
#include <vector>
#include "Logger/Logger.hpp"

// Only difference between the two is, basically, that reflectWrite needs to resize the vector.
template<typename Reflector, typename T>
void reflectRead(Reflector& r, std::vector<T>& vector)
{
    r.beginSequence();
    int32_t count;
    // Gets count from the current data.
    r.count("item", count);

    vector = std::vector<T>(count);

    for(size_t i = 0; i < count; i++)
    {
        r.itemIndex(i);
        r.member("item", vector[i], "");
    }
    r.endSequence();
}

template<typename Reflector, typename T>
void reflectWrite(Reflector& r, std::vector<T>& vector)
{
    r.beginSequence();
    int32_t count = vector.size();
    r.count("item", count);

    for(size_t i = 0; i < vector.size(); i++)
    {
        T& val = vector[i];
        r.itemIndex(i);
        r.member("item", val, "");
    }
    r.endSequence();
}

// SplitReflect needs to be included after (!!) defining reflectRead and reflectWrite for the type ! 
#include "Reflection/SplitReflect.hpp"

template<typename Reflector, typename T>
void reflect(Reflector& r, std::vector<T>& vector)
{
    splitReflect(r, vector);
}

