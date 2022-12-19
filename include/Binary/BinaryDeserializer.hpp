#pragma once
#include "BinaryData.hpp"
#include "BinaryDataReader.hpp"
#include "Serialization/Serializer.hpp"
#include "Serialization/Deserializer.hpp"
#include "TypeChecking/TypeCheckingFunctions.hpp"

namespace claid
{
    class BinaryDeserializer : public Deserializer<BinaryDeserializer>
    {
    
        private:
            BinaryDataReader binaryDataReader;

        public:
            

            BinaryDeserializer()
            {
                
            }

           

            template<typename T>
            void callFloatOrDouble(const char* property, T& member)
            {
                this->binaryDataReader.read(member);
            }   

            // Also includes any variants of signed, unsigned, short, long, long long, ...
            template<typename T>
            void callInt(const char* property, T& member)
            {
                this->binaryDataReader.read(member);
            }

            void callBool(const char* property, bool& member)
            {
                this->binaryDataReader.read(member);
            }

            // Why template? Because we can have signed and unsigned char.
            template<typename T>
            void callChar(const char* property, T& member)
            {
                this->binaryDataReader.read(member);
            }

            void callString(const char* property, std::string& member)
            {
                this->binaryDataReader.readString(member);
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
                std::string className;
                this->binaryDataReader.readString(className);

                // Check if ClassFactory is registered for className.
                if (!ClassFactory::ClassFactory::getInstance()->isFactoryRegisteredForClass(className))
                {
                    CLAID_THROW(claid::Exception, "BinaryDeserializer failed to deserialize object from binary data. Class \"" << className << "\" was not registered to ClassFactory and is unknown.");
                } 

                member = ClassFactory::ClassFactory::getInstance()->getNewInstanceAndCast<T>(className);

                PolymorphicReflector::WrappedReflectorBase<BinaryDeserializer>* polymorphicReflector;
                if (!PolymorphicReflector::PolymorphicReflector<BinaryDeserializer>::getInstance()->getReflector(className, polymorphicReflector))
                {
                    CLAID_THROW(claid::Exception, "BinaryDeserializer failed to deserialize object from binary. Member \"" << property << "\" is a pointer type with it's class specified as \"" << className << "\". However, no PolymorphicReflector was registered for class \"" << className << "\". Was PORTAIBLE_SERIALIZATION implemented for this type?");
                }

                polymorphicReflector->invoke(*this, static_cast<void*>(member));               
            }
            
            template<typename T>
            void callSharedPointer(const char* property, T& member)
            {
                typedef typename T::element_type BaseTypeT;
                BaseTypeT* pointer;
                this->callPointer<BaseTypeT>(property, pointer);
                member = T(pointer, std::default_delete<BaseTypeT>());
            }

            template<typename T>
            void callEnum(const char* property, T& member)
            {
                size_t m;
                this->callInt(property, m);
                member = static_cast<T>(m);
            }

            void count(const std::string& name, size_t& count)
            {
                this->binaryDataReader.read(count);
            }

            void countElements(size_t& count)
            {
                this->binaryDataReader.read(count);
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

            void read(char*& data, size_t size)
            {
                this->binaryDataReader.readBytes(data, size);
            }

            

            template <typename T>
            typename std::enable_if<!std::is_arithmetic<T>::value>::type
            deserialize(T& obj, BinaryDataReader& binaryDataReader)
            {
                this->binaryDataReader = binaryDataReader;

                // Read data type string and check if it matches the data type of obj.
                std::string name = TypeChecking::getCompilerIndependentTypeNameOfClass<T>();
                std::string storedName;


                this->binaryDataReader.readString(storedName);

                if(name != storedName)
                {
                    CLAID_THROW(Exception, "Error, failed to deserialize object from binary data because of mismatching types."
                                                "The data type of the object is " << name << " but the serialized data is of type " << storedName);
                }

                invokeReflectOnObject(obj);
            }

            template <typename T>
            typename std::enable_if<std::is_arithmetic<T>::value>::type
            deserialize(T& obj, BinaryDataReader& binaryDataReader)
            {
                this->binaryDataReader = binaryDataReader;

                // Read data type string and check if it matches the data type of obj.
                std::string name = TypeChecking::getCompilerIndependentTypeNameOfClass<T>();
                std::string storedName;


                this->binaryDataReader.readString(storedName);

                if(name != storedName)
                {
                    CLAID_THROW(Exception, "Error, failed to deserialize object from binary data because of mismatching types."
                                                "The data type of the object is " << name << " but the serialized data is of type " << storedName);
                }

                this->binaryDataReader.read(obj);
            }

            void enforceName(std::string& name, int idInSequence = 0)
            {
                // Some serializers, like BinarySerializer, might not store the members name (i.e., property parameters),
                // as it is not necessary to be known in the binary data).
                // For some cases, however, it might be necessary to store such strings in the serialized data nevertheless,
                // as it might be needed for deserialization etc.
                // Thus, this function allows to make sure the string "name" is explicitly stored.
                
                this->binaryDataReader.readString(name);
            }

    };
}