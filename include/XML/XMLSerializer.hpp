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
#include "XMLNode.hpp"
#include "XMLNumericVal.hpp"

#include "Serialization/Serializer.hpp"
#include "TypeChecking/TypeCheckingFunctions.hpp"
#include "Reflection/ReflectionManager.hpp"
#include "Reflection/Reflect.hpp"

#include <stack>
namespace claid
{
    class XMLSerializer : public Serializer<XMLSerializer>
    {
        private:

        public:
            std::shared_ptr<XMLNode> root = nullptr;
            std::shared_ptr<XMLNode> currentNode = nullptr;

            std::stack<std::shared_ptr<XMLNode>> nodeStack;

            std::string getReflectorName()
            {
                return "XMLSerializer";
            } 


            XMLSerializer()
            {
                this->root = std::shared_ptr<XMLNode>(new XMLNode(nullptr, "root"));
                this->currentNode = root;
            }
        
            ~XMLSerializer()
            {
                
            }

            EmptyReflect(XMLSerializer)

            template<typename T>
            void callFloatOrDouble(const char* property, T& member)
            {
                this->currentNode->addChild(
                    std::static_pointer_cast<XMLNode>(
                        std::shared_ptr<XMLNumericVal>(
                            new XMLNumericVal(this->currentNode, property, XMLNumericVal::toString<T>(member)))));
            }   

            // Also includes any variants of signed, unsigned, short, long, long long, ...
            template<typename T>
            void callInt(const char* property, T& member)
            {
                this->currentNode->addChild(
                    std::static_pointer_cast<XMLNode>(
                        std::shared_ptr<XMLNumericVal>(
                            new XMLNumericVal(this->currentNode, property, XMLNumericVal::toString<T>(member)))));
            }

            void callBool(const char* property, bool& member)
            {
                this->currentNode->addChild(
                    std::static_pointer_cast<XMLNode>(
                        std::shared_ptr<XMLVal>(
                            new  XMLVal(this->currentNode, property, member ? "true" : "false"))));
            }

            // Why template? Because we can have signed and unsigned char.
            template<typename T>
            void callChar(const char* property, T& member)
            {
                std::string str = "";
                str += member;

                this->currentNode->addChild(
                    std::static_pointer_cast<XMLNode>(
                        std::shared_ptr<XMLVal>(
                            new XMLVal(this->currentNode, property, str))));
            }

            template<typename T>
            void callString(const char* property, T& member)
            {
                this->currentNode->addChild(
                    std::static_pointer_cast<XMLNode>(
                        std::shared_ptr<XMLVal>(
                            new XMLVal(this->currentNode, property, member))));
            }

            template<typename T>
            void callBeginClass(const char* property, T& member)
            {
                std::shared_ptr<XMLNode> node = std::shared_ptr<XMLNode>(new XMLNode(currentNode, property));
                this->nodeStack.push(this->currentNode);
                this->currentNode->addChild(node);
                this->currentNode = node;
            }

            template<typename T>
            void callEndClass(const char* property, T& member)
            {
                this->currentNode = this->nodeStack.top();
                this->nodeStack.pop();
            }


            template<typename T>
            void callPointer(const char* property, T*& member)
            {
                if(member == nullptr)
                {
                    // What to do with nullptrs? Do not serialize?
                    CLAID_THROW(Exception, "Error, XMLSerializer can not serialize member " << property << "."
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
                    CLAID_THROW(claid::Exception, "XMLSerializer failed to serialize object to XML. Member \"" << property << "\" is a pointer/polymorphic object of type \"" << rttiTypeString << "\". However, no PolymorphicReflector was registered for type \"" << rttiTypeString << "\". Was PORTAIBLE_SERIALIZATION implemented for this type?");
                }

                std::string className = ClassFactory::getInstance()->getClassNameOfObject(*member);

                UntypedReflector* untypedReflector;
                if (!ReflectionManager::getInstance()->getReflectorForClass(className, this->getReflectorName(), untypedReflector))
                {
                    CLAID_THROW(claid::Exception, "XMLSerializer failed to deserialize object from XML. Member \"" << property << "\" is a pointer type with it's class specified as \"" << className << "\". However, no PolymorphicReflector was registered for class \"" << className << "\". Was PORTAIBLE_SERIALIZATION implemented for this type?");
                }

                untypedReflector->invoke(static_cast<void*>(this), static_cast<void*>(member));
                this->currentNode->setAttribute("class", className);
            }
            
            template<typename T>
            void callSharedPointer(const char* property, T& member)
            {
                typedef typename T::element_type BaseTypeT;
                BaseTypeT* ptr = member.get();


                if(ptr == nullptr)
                {
                    // What to do with nullptrs? Do not serialize?
                    CLAID_THROW(Exception, "Error, XMLSerializer can not serialize member " << property << "."
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
                // Do nothing
            }

            void countElements(int32_t& count)
            {
                // Do nothing
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

            }

            template <typename T>
            typename std::enable_if<!std::is_arithmetic<T>::value>::type
            serialize(const std::string& name, T& obj)
            {
                // std::string name = TypeChecking::getCompilerSpecificCompileTypeNameOfClass<T>();
                // std::shared_ptr<XMLNode> node = std::shared_ptr<XMLNode>(new XMLNode(currentNode, name));
                // this->currentNode->addChild(node);
                // this->currentNode = node;

                invokeReflectOnObject(name.c_str(), obj);
            }

            template <typename T>
            typename std::enable_if<std::is_arithmetic<T>::value>::type
            serialize(const std::string& name, T& obj)
            {
                this->root = std::shared_ptr<XMLNode>(new XMLNode(nullptr, "root"));
                this->currentNode = root;
                this->member(name.c_str(), obj, "");
            }

            template<typename T>
            void onInvocationStart(T& obj)
            {
                this->root = std::shared_ptr<XMLNode>(new XMLNode(nullptr, "root"));
                this->currentNode = root;
            }

            void forceReset()
            {
                
            }

            std::shared_ptr<XMLNode> getXMLNode()
            {
                return this->root;
            }

            void enforceName(std::string& name, int idInSequence = 0)
            {
                // Some serializers, like BinarySerializer, might not store the members name (i.e., property parameters),
                // as it is not necessary to be known in the binary data).
                // For some cases, however, it might be necessary to store such strings in the serialized data nevertheless,
                // as it might be needed for deserialization etc.
                // Thus, this function allows to make sure the string "name" is explicitly stored.
                

                // Note, that as the XMLSerializer stores names of members everytime anyways, this function is not needed
                // for XMLSerializer and -Deserialize.
            }

            virtual void writeDataToFile(const std::string& path, std::ofstream& file)
            {
                std::string str;
                this->root->toString(str);
                file << str;
            }
    };
}

