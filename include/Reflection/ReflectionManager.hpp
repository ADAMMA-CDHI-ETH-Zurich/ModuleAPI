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

#include "Utilities/Singleton.hpp"
#include "MacroUtilities/MakeUniqueVariableName.hpp"
#include "TypedReflector.hpp"

#include <map>
#include <string>
namespace claid
{
	class ReflectionManager : public Singleton<ReflectionManager>
	{
		private:

			std::map<std::string /* Class name */, std::map<std::string, UntypedReflector*> /* Available Reflectors for class */> reflectors;
		public:

			template<typename Class, typename Reflector>
			void registerReflectorForClass()
			{
				TypedReflector<Class, Reflector>* reflector = new TypedReflector<Class, Reflector>();

				std::string className = TypeChecking::getCompilerIndependentTypeNameOfClass<Class>();

				// Create an instance of the reflector in order to retrieve it's name..
				Reflector r;
				std::string reflectorName = r.getReflectorName();

				Logger::printfln("Registering reflector %s for class %s", reflectorName.c_str(), className.c_str());


				UntypedReflector* untypedReflector = static_cast<UntypedReflector*>(reflector);
				reflectors[className].insert(std::make_pair(reflectorName, untypedReflector));
			}

			// template<typename Class, typename Reflector>
			// void getReflectorForClass()
			// {

			// }

			template<typename Class, typename Reflector>
			bool getReflectorForClass(UntypedReflector*& reflector)
			{
				std::string className = TypeChecking::getCompilerIndependentTypeNameOfClass<Class>();

				// Create an instance of the reflector in order to get its name..
				Reflector r;
				std::string reflectorName = r.getReflectorName();

				return getReflectorForClass(className, reflectorName, reflector);
			}

			bool getReflectorForClass(const std::string& className, const std::string& reflectorName, UntypedReflector*& reflector)
			{
		
				auto it = this->reflectors.find(className);

			

				if (it == this->reflectors.end())
				{
					return false;
				}
		
				
				auto it2 = it->second.find(reflectorName);
				
				if(it2 == it->second.end())
				{
					return false;
				}

				reflector = it2->second;

				return true;
			}
	};

	template<typename ObjectType, typename Reflector>
	class ReflectionManagerRegistrar
	{
	public:
		ReflectionManagerRegistrar()
		{
			ReflectionManager::getInstance()->template registerReflectorForClass<ObjectType, Reflector>();
		}
	};
}


// #define DECLARE_POLYMORPHIC_REFLECTOR(className, Reflector, ReflectorName) \
//      static claid::PolymorphicReflector::RegisterHelper<className, Reflector> polymorphicReflectorRegistrar##ReflectorName;


#define REGISTER_REFLECTOR_FOR_CLASS(Class, Reflector, ReflectorName)\
	namespace\
	{\
		static volatile claid::ReflectionManagerRegistrar<Class, Reflector> reflectorRegistrar##ReflectorName;\
	}
	