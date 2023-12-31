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

#include "XMLVal.hpp"
#include "Exception/Exception.hpp"
namespace claid
{

    struct XMLNumericVal : public XMLVal
    {


        XMLNumericVal(std::shared_ptr<XMLNode> parent, std::string name, std::string value) : XMLVal(parent, name, value)
        {

        }

        virtual ~XMLNumericVal()
        {
            
        }

        template<typename T>
        static std::string toString(const T& val)
        {
            return std::to_string(val);
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
            this->value = value;
            //parseFromString(this->value, string);
            return true;
        }

        virtual void contentAsString(std::string& string)
        {
            string = this->value;
        }
    
        #define XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(type, func) \
        static void parseFromString(const char* property, type& member, const std::string& string) \
        { \
            try\
            {\
                member = func(string); \
            }\
            catch(const std::exception& e) \
            {\
                CLAID_THROW(claid::Exception, "Failed to parse value from XML node \"" << property << "\". Cannot convert " << string << " to variable of type " << std::string(#type) << ". " \
                << "Possibly, the specified string is invalid or the value is too big for the specified data type.\n" \
                << "Original exception was: " << e.what()); \
            }\
        } \



    

        // template <typename T = int, only_if <eq <NumericType, long>{}, T> = 0>
        // void parseFromString(long& value, const std::string& string) 
        // { 
        //     value = std::stoi(string); 
        // }
        XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(short, std::stoi)
        XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(int, std::stoi)
        XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(long, std::stoi)
        XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(long long, std::stoi)

        XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(unsigned short, std::stoi)
        XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(unsigned int, std::stoul)
        XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(unsigned long, std::stoul)
        XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(unsigned long long, std::stoull)

        XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(float, std::stof)
        XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(double, std::stod)
        XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(long double, std::stold)

    

    };
}



// template <bool C, typename T = void>
// using only_if = typename std::enable_if <C, T>::type;

// template <typename A, typename B>
// using eq = typename std::is_same <A, B>::type;

// #define XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(type, func) \
// template <typename T = int, only_if <eq <NumericType, type>{}, T> = 0>\
// type parseFromString(type& value, const std::string& string) \
// { \
//     value = func(string); \
// } \



// // template <typename T = int, only_if <eq <NumericType, long>{}, T> = 0>
// // void parseFromString(long& value, const std::string& string) 
// // { 
// //     value = std::stoi(string); 
// // } 

// XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(int, std::stoi)
// XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(long, std::stoi)
// XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(long long, std::stoi)
// XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(unsigned int, std::stoul)
// XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(unsigned long, std::stoul)
// XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(unsigned long long, std::stoull)
// XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(float, std::stof)
// XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(double, std::stod)
// XMLNUMERICVAL_GENERATE_PARSE_FROM_STRING(long double, std::stold)
