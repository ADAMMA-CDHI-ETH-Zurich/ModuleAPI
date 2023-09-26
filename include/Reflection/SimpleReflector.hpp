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
#include "RecursiveReflector.hpp"
#include "RunTime/RunTime.hpp"
namespace claid
{
    class SimpleReflector : public RecursiveReflector<SimpleReflector>
    {
        public:
            template<typename T>
            void callFloatOrDouble(const char* property, T& member)
            {
                printf("I am float, val is %f\n", member);

                RunTime runtime;
                printf("And runtime val is %d\n",     claid::RunTime::getInstance()->getVal());
            }   

            // Also includes any variants of signed, unsigned, short, long, long long, ...
            template<typename T>
            void callInt(const char* property, T& member)
            {
                printf("I am int, val is <%s>%d</%s>\n", property, member, property);
            }

            void callBool(const char* property, bool& member)
            {
                printf("I am int, val is <%s>%d</%s>\n", property, member, property);
            }

            template<typename T>
            void callString(const char* property, T& member)
            {
                printf("I am string, val is %s %s\n", member.c_str(), property);
            }

            template<typename T>
            void callBeginClass(const char* property, T& member)
            {

            }

            template<typename T>
            void callEndClass(const char* property, T& member)
            {

            }
            
    };
}