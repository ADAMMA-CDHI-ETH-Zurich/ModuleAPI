#pragma once
#include "ClassFactory/ClassFactory.hpp"
#include "Reflection/ReflectionManager.hpp"
#include "XML/XMLSerializer.hpp"
#include "XML/XMLDeserializer.hpp"
#include "Binary/BinarySerializer.hpp"
#include "Binary/BinaryDeserializer.hpp"
#include "CSV/CSVSerializer.hpp"

#include "AdditionalSerialization.hpp"
#include "RunTime/RunTime.hpp"
// Only use the 4 serializers that come with the ModuleAPI (XMLSerializer, XMLDeserializer, BinarySerializer, BinaryDeserializer).
// Necessary for Modules, for example, which should only use the standard de-/serializers.
// #define DECLARE_SERIALIZATION_STANDARD_SERIALIZERS_ONLY(className) \
//     DECLARE_POLYMORPHIC_REFLECTOR(className, claid::XMLSerializer, XMLSerializer)\
//     DECLARE_POLYMORPHIC_REFLECTOR(className, claid::XMLDeserializer, XMLDeserializer)\
//     DECLARE_POLYMORPHIC_REFLECTOR(className, claid::BinarySerializer, BinarySerializer)\
//     DECLARE_POLYMORPHIC_REFLECTOR(className, claid::BinaryDeserializer, BinaryDeserializer)\

#define REGISTER_SERIALIZATION_STANDARD_SERIALIZERS_ONLY(className)\
    REGISTER_REFLECTOR_FOR_CLASS(className, claid::XMLSerializer, XMLSerializer)\
    REGISTER_REFLECTOR_FOR_CLASS(className, claid::XMLDeserializer, XMLDeserializer)\
    REGISTER_REFLECTOR_FOR_CLASS(className, claid::BinarySerializer, BinarySerializer)\
    REGISTER_REFLECTOR_FOR_CLASS(className, claid::BinaryDeserializer, BinaryDeserializer)\
    REGISTER_REFLECTOR_FOR_CLASS(className, claid::CSVSerializer, CSVSerializer)\



// #define DECLARE_SERIALIZATION(className) \
//     DECLARE_CLASS_FACTORY(className)\
//     DECLARE_SERIALIZATION_STANDARD_SERIALIZERS_ONLY(className) \
//     DECLARE_ADDITIONAL_DE_SERIALIZERS(className) // Serializers that might not always be available (e.g., JavaNativeSetter and Getter).

#define REGISTER_VECTOR_SERIALIZATION(className)\
    REGISTER_TO_CLASS_FACTORY(std::vector<className>)\
    REGISTER_SERIALIZATION_STANDARD_SERIALIZERS_ONLY(std::vector<className>)\
    REGISTER_ADDITIONAL_DE_SERIALIZERS(std::vector<className>)

#define REGISTER_SERIALIZATION(className)\
    REGISTER_TO_CLASS_FACTORY(className)\
    REGISTER_SERIALIZATION_STANDARD_SERIALIZERS_ONLY(className)\
    REGISTER_ADDITIONAL_DE_SERIALIZERS(className)

