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

#include "ClassFactory/ClassFactoryBase.hpp"

#include <type_traits>
#include "Exception/Exception.hpp"
#include "TypeChecking/TypeCheckingFunctions.hpp"
namespace claid
{
	class ClassFactory;

	template<typename T>
	class ClassFactoryTyped : public ClassFactoryBase
	{
		private:
			template<typename U = T>
			typename std::enable_if<!std::is_abstract<U>::value, void*>::type
				getInstanceUntypedHelper()
			{
				return static_cast<void*>(new T);
			}

			template<typename U = T>
			typename std::enable_if<std::is_abstract<U>::value, void*>::type
				getInstanceUntypedHelper()
			{
				CLAID_THROW(Exception, "Error, cannot create instance of class \"" << TypeChecking::getCompilerSpecificCompileTypeNameOfClass<U>() << "\" at runtime using ClassFactory."
					<< "This class is an abstract class.");
			}

			template<typename U = T>
			typename std::enable_if<!std::is_abstract<U>::value, std::shared_ptr<void>>::type
				getInstanceUntypedAsSharedPtrHelper()
			{
				return std::static_pointer_cast<void>(std::make_shared<T>());
			}

			template<typename U = T>
			typename std::enable_if<std::is_abstract<U>::value, std::shared_ptr<void>>::type
				getInstanceUntypedAsSharedPtrHelper()
			{
				CLAID_THROW(Exception, "Error, cannot create instance of class \"" << TypeChecking::getCompilerSpecificCompileTypeNameOfClass<U>() << "\" at runtime using ClassFactory."
					<< "This class is an abstract class.");
			}

		public:

			void* getInstanceUntyped()
			{
				return getInstanceUntypedHelper<T>();
			}


			std::shared_ptr<void> getInstanceUntypedAsSharedPtr()
			{
				return getInstanceUntypedAsSharedPtrHelper<T>();
			}

			T* getInstance()
			{
				return new T;
			}

	};
}