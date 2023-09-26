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

#include <type_traits>
// Invoke reflectWrite
template<typename Reflector, typename Type, typename Enable = void>
struct ClassReflectReadWriteInvoker 
{
	static void call(Reflector& r, Type& obj) 
	{
		// Cannot put this here, because windows MSCV is ... not so smart.
		// Somehow it evaluates the default type for all the types, meaning it would complain, for example,
		// about the BinarySerializer not having a "read" function, as used in reflectRead(...) of some types that use splitReflect() (e.g., BinaryData).
		// The BinarySerializer is a Serializer (obv.), ths isReadOnly is true. Therefore, reflectWrite would be called.
		// However, if we put reflectRead here, MSCV would still evaluate it..
		// clang and gcc, as usual, do not complain.
		// obj.reflectRead<Reflector>(r);
		obj.template reflectRead<Reflector>(r);
	}
}; 

// Invoke reflectRead
template<typename Reflector, typename Type>
struct ClassReflectReadWriteInvoker<Reflector, Type, typename std::enable_if<Reflector::isReadOnly>::type> 
{
	static void call(Reflector& r, Type& obj) 
	{
		obj.template reflectWrite<Reflector>(r);
	}
};

// // Invoke reflectRead
// template<typename Reflector, typename Type>
// struct ClassReflectReadWriteInvoker<Reflector, Type, typename std::enable_if<!Reflector::isReadOnly>::type> 
// {
// 	static void call(Reflector& r, Type& obj) 
// 	{
// 		obj.template reflectRead<Reflector>(r);
// 	}
// };


template<typename Reflector, typename Type>
inline void splitReflectInType(Reflector& r, Type& obj)
{
	ClassReflectReadWriteInvoker<Reflector, Type>::call(r, obj);
}