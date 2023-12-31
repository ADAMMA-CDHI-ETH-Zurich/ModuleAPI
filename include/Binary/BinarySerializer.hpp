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
#include "BinaryData.hpp"
#include "ClassFactory/ClassFactory.hpp"

#include "Serialization/Serializer.hpp"
#include "TypeChecking/TypeCheckingFunctions.hpp"
#include "Reflection/ReflectionManager.hpp"

namespace claid
{
    class BinarySerializer : public Serializer<BinarySerializer>
    {
    

        public:
            
            std::string getReflectorName()
            {
                return "BinarySerializer";
            } 
            
            BinaryData* binaryData = nullptr;

            BinarySerializer()
            {
                
            }

            template<typename T>
            void callFloatOrDouble(const char* property, T& member)
            {
                this->binaryData->store(member);
            }   

            // Also includes any variants of signed, unsigned, short, long, long long, ...
            template<typename T>
            void callInt(const char* property, T& member)
            {
                this->binaryData->store(member);
            }

            void callBool(const char* property, bool& member)
            {
                this->binaryData->store(member);
            }

            // Why template? Because we can have signed and unsigned char.
            template<typename T>
            void callChar(const char* property, T& member)
            {
                this->binaryData->store(member);
            }

            void callString(const char* property, std::string& member)
            {
                this->binaryData->storeString(member);
            }

            template<typename T>
            void callBeginClass(const char* property, T& member)
            {
                
            }

            template<typename T>
            void callEndClass(const char* property, T& member)
            {

            }


            template<typename T>
            void callPointer(const char* property, T*& member)
            {
                if(member == nullptr)
                {
                    // What to do with nullptrs? Do not serialize?
                    CLAID_THROW(Exception, "Error, BinarySerializer can not serialize member " << property << "."

                    << "The member is a pointer of type " << TypeChecking::getCompilerSpecificCompileTypeNameOfClass<T>() << ", but the value of the pointer is null.");
                    return;
                }

                // In the following, we check whether serialization is implemented for member.
                // Note, that T might not match the members real type, as member might be polymorphic.
                // Thus, we can not use className to check whether a PolymorphicReflector was registered (see below).

                // Note, that this check is only necessary for SERIALIZERS.
                // The serializer needs to make sure it stores the exact className.
                // The deserializer deserializes the data for the object (member) based on this className.
                // Thus, the deserializer relies on the serializer to store the correct className.

                // We only need the rttiType for checking whether the type of member has implemented serialization.
                // Cannot use className to check whether the type is registered to ClassFactory and PolymorphicReflector,
                // because getClassName is a virtual function. If a type is derived from a base class AND 
                // implements serialization (registered to ClassFactory and PolymorphicReflector), then getClassName()
                // provides the correct type. However, if the derived type does NOT implement serialization,
                // getClassName returns the className of the base type, which would lead into storing the wrong
                // class identifier in the binary data.
                std::string rttiTypeString = TypeChecking::getCompilerSpecificRunTimeNameOfObject(*member);

                if(!ClassFactory::ClassFactory::getInstance()->isFactoryRegisteredForRTTITypeName(rttiTypeString))
                {
                    CLAID_THROW(claid::Exception, "BinarySerializer failed to serialize object to binary. Member \"" << property << "\" is a pointer/polymorphic object of RTTI type \"" << rttiTypeString << "\". However, no PolymorphicReflector was registered for type \"" << rttiTypeString << "\". Was PORTAIBLE_SERIALIZATION implemented for this type?");
                }

                // If there is a factory available for data type with given RTTI string (see above), then the className will
                // be the correct name of the Polymorphic class.
                std::string className = ClassFactory::getInstance()->getClassNameOfObject(*member);

                UntypedReflector* untypedReflector;
                if (!ReflectionManager::getInstance()->getReflectorForClass(className, this->getReflectorName(), untypedReflector))
                {
                    CLAID_THROW(claid::Exception, "BinarySerializer failed to serialize object to binary. Member \"" << property << "\" is a pointer/polymorphic type with it's class specified as \"" << className << "\". However, no PolymorphicReflector was registered for class \"" << className << "\". Was PORTAIBLE_SERIALIZATION implemented for this type?");
                }

                // Store class name
                this->binaryData->storeString(className);

                untypedReflector->invoke(static_cast<void*>(this), static_cast<void*>(member));               
            }
            
            template<typename T>
            void callSharedPointer(const char* property, T& member)
            {
                typedef typename T::element_type BaseTypeT;
                BaseTypeT* ptr = member.get();

                if(ptr == nullptr)
                {
                    // What to do with nullptrs? Do not serialize?
                    CLAID_THROW(Exception, "Error, BinarySerializer can not serialize member " << property << "."
                    << "The member is a shared_ptr of type " << TypeChecking::getCompilerSpecificCompileTypeNameOfClass<T>() << ", but the value of the pointer is null");
                    return;
                }

                this->callPointer<BaseTypeT>(property, ptr);
            }

            template<typename T>
            void callEnum(const char* property, T& member)
            {
                int32_t m = static_cast<int32_t>(member);
                this->callInt(property, m);
            }

            void count(const std::string& name, int32_t& count)
            {
                this->binaryData->store(count);
            }

            void countElements(int32_t& count)
            {
                this->binaryData->store(count);
            }

            void beginSequence()
            {
                // Do nothing
            }

            void itemIndex(const size_t index)
            {
                // Do nothing
            }
            
            void endSequence()
            {
                // Do nothing
            }
            
            void write(const char* data, size_t size)
            {
                this->binaryData->store(data, size);
            }



            template <typename T>
            typename std::enable_if<!std::is_arithmetic<T>::value>::type
            serialize(T& obj, BinaryData* targetContainer)
            {
                this->binaryData = targetContainer;
                this->binaryData->clear();

                invokeReflectOnObject("", obj);
            }

            template <typename T>
            typename std::enable_if<std::is_arithmetic<T>::value>::type
            serialize(T& obj, BinaryData* targetContainer)
            {
                this->binaryData = targetContainer;
                this->binaryData->clear();

                // Store data type string in order to check it during deserialization.
                // This is crucial and needs to platform independent.
                std::string name = TypeChecking::getCompilerIndependentTypeNameOfClass<T>();
                this->binaryData->storeString(name);


                this->binaryData->store(obj);
            }

            template<typename T>
            void onInvocationStart(T& obj)
            {
                // Store data type string in order to check it during deserialization.
                // This is crucial and needs to platform independent.
                std::string name = TypeChecking::getCompilerIndependentTypeNameOfClass<T>();
                this->binaryData->storeString(name);
            }


            void enforceName(std::string& name, int idInSequence = 0)
            {
                // Some serializers, like BinarySerializer, might not store the members name (i.e., property parameters),
                // as it is not necessary to be known in the binary data).
                // For some cases, however, it might be necessary to store such strings in the serialized data nevertheless,
                // as it might be needed for deserialization etc.
                // Thus, this function allows to make sure the string "name" is explicitly stored.
                
                this->binaryData->storeString(name);
            }

            virtual bool getByteRepresentationOfSerializedData(std::vector<char>& data)
            {
                printf("bla\n");
                if(this->binaryData == nullptr)
                {
                    data.clear();
                }
                else
                {
                    // Is this safe ?
                    data = binaryData->getData();
                }
                return true;

            }

    };
}

