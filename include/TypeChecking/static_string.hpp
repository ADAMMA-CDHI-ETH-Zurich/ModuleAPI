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

#include <iostream>
#include <string>
#include <vector>

namespace claid
{
    namespace TypeChecking
    {
        
        // Compile time recursion to get length of a const c-string (char*) 
        // at compile time.
     
        template< size_t N >
        constexpr int constexpr_length_of_c_string( const char  (&var)[N] )
        {
        return N-1;
        }
        
       
        // Taken from https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c/56766138#56766138
        class static_string
        {
            const char* const p_;
            const std::size_t sz_;

        public:
            typedef const char* const_iterator;

            template <std::size_t N>
            constexpr static_string(const char(&a)[N]) noexcept
                : p_(a)
                , sz_(N-1)
                {}

            constexpr static_string(const char* p, std::size_t N) noexcept
                : p_(p)
                , sz_(N)
                {}

            constexpr const char*  data() const noexcept {return p_;}
            constexpr std::size_t size() const noexcept {return sz_;}

            constexpr const_iterator begin() const noexcept {return p_;}
            constexpr const_iterator end()   const noexcept {return p_ + sz_;}

            
            template< size_t N >
            constexpr bool isInNamespace( const char  (&ns)[N] ) noexcept
            {   
                for (int i = 0; i < constexpr_length_of_c_string(ns); ++i)
                {
                    if (p_[i] != ns[i])
                        return false;
                }
                return true;
            }

            constexpr char operator[](std::size_t n) const
            {
                return n < sz_ ? p_[n] : throw std::out_of_range("static_string");
            }

            const std::string toStdString() const
            {
                // p_ might not be 0 terminated at p[sz_].
                // Therefore, we make sure that the std::string has the correct length.
                return std::string(p_, sz_);
            }
        };
    }
}