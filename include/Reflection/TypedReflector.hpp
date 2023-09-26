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
#include "UntypedReflector.hpp"


namespace claid
{
	template<typename ObjectType, typename Reflector>
	class TypedReflector : public UntypedReflector
	{

		virtual void invoke(void* reflector, void* obj) 
		{
			this->invokeTyped(*static_cast<Reflector*>(reflector), *static_cast<ObjectType*>(obj));
		}

		virtual void invokeTyped(Reflector& r, ObjectType& obj)
		{
			r.invokeReflectorOnNonPrimitiveType(obj);
		}

		virtual void invokeMember(const char* memberName, void* reflector, void* obj)
		{
			return this->invokeMemberTyped(memberName, *static_cast<Reflector*>(reflector), *static_cast<ObjectType*>(obj));
		}

		virtual void invokeMemberTyped(const char* memberName, Reflector& r, ObjectType& obj)
		{
			r.member(memberName, obj, "");
		}

		virtual void invokeMemberWithDefaultValue(const char* memberName, void* reflector, void* obj, void* defaultValue)
		{
			return this->invokeMemberWithDefaultValueTyped(memberName, *static_cast<Reflector*>(reflector), *static_cast<ObjectType*>(obj), *static_cast<ObjectType*>(defaultValue));
		}

		virtual void invokeMemberWithDefaultValueTyped(const char* memberName, Reflector& r, ObjectType& obj, ObjectType& defaultValue)
		{
			r.member(memberName, obj, "", defaultValue);
		}

	};
}