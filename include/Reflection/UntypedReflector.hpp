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

namespace claid
{
	class UntypedReflector
	{
		public:
			virtual void invoke(void* reflector, void* obj) = 0;

			template<typename Reflector, typename Class>
			void invoke(Reflector& reflector, Class& object)
			{
				return invoke(static_cast<void*>(reflector), static_cast<void*>(object));
			}

			virtual void invokeMember(const char* memberName, void* reflector, void* obj) = 0;

			template<typename Reflector, typename Class>
			void invokeMember(const char* memberName, Reflector& reflector, Class& object)
			{
				return invokeMember(memberName, static_cast<void*>(reflector), static_cast<void*>(object));
			}

			virtual void invokeMemberWithDefaultValue(const char* memberName, void* reflector, void* obj, void* defaultValue) = 0;

			template<typename Reflector, typename Class>
			void invokeMemberWithDefaultValue(const char* memberName, Reflector& reflector, Class& object, Class& defaultValue)
			{
				return invokeMemberWithDefaultValue(memberName, static_cast<void*>(reflector), static_cast<void*>(object), static_cast<void*>(defaultValue));
			}
	};
}