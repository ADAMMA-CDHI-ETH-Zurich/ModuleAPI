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
// #pragma once
// #include <type_traits>
// #include "AbstractReflector.hpp"
// #include "TypeChecking/TypeCheckingFunctions.hpp"

// namespace claid
// {

//     template<typename Type>
//     struct ____MISSING_REFLECT_METHOD_FOR_{
// 	static_assert(sizeof(Type)==0,
// 	    "Cannot find an (unambiguous) intrusive or nonintrusive reflect method for Type. "
// 	    "If Type is a class that you have implemented, then you should "
// 	    "implement its reflect() method.");
// 	static void invoke() {}
// };

//     // Has no reflect
//     template<typename Reflector, typename T, class Enable = void>
//     struct ReflectorInvoker 
//     {
//         static void call(Reflector& r, T& member) 
//         {
//            ____MISSING_REFLECT_METHOD_FOR_<T>::invoke();
//         }

//     }; 

//     // TODO fix for standard types
//     template<typename Reflector, typename T>
//     struct ReflectorInvoker<Reflector, T, typename std::enable_if<!std::is_const<T>::value && has_mem_reflect<T>::value>::type>
//     {
//         static void call(Reflector& r, T& member) 
//         {
//             member.reflect(r);
//         }

//     }; 

//     template<typename Reflector, typename T>
//     struct ReflectorInvoker<Reflector, T, typename std::enable_if<!std::is_const<T>::value && has_non_member_function_reflect<Reflector&, T&>::value>::type>
//     {
//         static void call(Reflector& r, T& member) 
//         {
//             reflect(r, member);
//         }

//     }; 

//      // if const, we have to cast the constness aways check again.
//     template<typename Reflector, typename T>
//     struct ReflectorInvoker<Reflector, T, typename std::enable_if<std::is_const<T>::value>::type>
//     {
//         static void call(Reflector& r, const T& member) 
//         {
//             typedef typename std::remove_const<T>::type NonConstType; 
//             NonConstType& non_const_member = *const_cast<NonConstType*>(&member);
//             ReflectorInvoker<Reflector, NonConstType>::call(r, non_const_member);
//         }

//     }; 


// }