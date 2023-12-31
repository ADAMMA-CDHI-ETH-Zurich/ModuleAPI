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

#include "ChannelData.hpp"
#include "Binary/BinaryData.hpp"
#include "XML/XMLNode.hpp"
#include "Exception/Exception.hpp"
#include "Serialization/AbstractSerializer.hpp"
#include <mutex>
#include "ClassFactory/ClassFactory.hpp"
#include "Reflection/UntypedReflector.hpp"
#include "Reflection/ReflectionManager.hpp"
#include "Binary/BinaryDeserializer.hpp"
namespace claid
{
   
    class ChannelDataBase;
    template<typename T>
    class ChannelData;

    struct ChannelBufferElement
    {
        // ChannelData is used by typed channels,
        // BinaryData is used by untyped channels.
        // They can be converted to each other, as ChannelData
        // needs to be binary de-/serializable.
        // So, why store both in one struct then, isn't it overhead?
        // Typed channels also allow to read binary data. Whenever
        // binary data shall be read from a channel, the data at the given
        // timestamp needs to be serialized. Thus, if multiple different processes
        // want to read the binary data, it always would need to be serialized again.
        // Thus, whenever the corresponding data of the typed channel is serialized,
        // we store the serialized data aswell.

        protected:
            TaggedDataBase header;
            TaggedData<BinaryData> binaryData;

            // If data is available at all (either binary or typed)
            bool dataAvailable = false;

            // If binary data is available (either set directly or typed data has been serialized).
            bool binaryDataAvailable = false;
            std::mutex mutex;

            std::string dataTypeName;
    

        public:


            ChannelBufferElement(std::string dataTypeName) : dataAvailable(false), dataTypeName(dataTypeName)
            {

            }

            ChannelBufferElement(TaggedData<BinaryData> binaryData, std::string dataTypeName) : binaryData(binaryData), dataAvailable(true), binaryDataAvailable(true), dataTypeName(dataTypeName)
            {
                this->header = binaryData.getHeader();
            }
            
            virtual ~ChannelBufferElement()
            {
            }

            void lock()
            {
                this->mutex.lock();
            }

            void unlock()
            {
                this->mutex.unlock();
            }

            TaggedDataBase getHeader()
            {
                return this->header;
            }

            virtual TaggedData<BinaryData> getBinaryData()
            {
                std::unique_lock<std::mutex> (this->mutex);
                if(!this->dataAvailable)
                {
                    CLAID_THROW(Exception, "Error! Tried to get binary data from ChannelBufferElement (untyped), but no data was ever set (no data available).");
                }

                // This does not copy data, since TaggedData uses a shared_ptr internally.
                TaggedData<BinaryData> data = this->binaryData;

                return data;
            }

            bool isDataAvailable() const
            {
                return this->dataAvailable;
            }

            virtual bool canSerializeToXML() const
            {
                // Cannot serialize to XML, because 
                // we are an untyped ChannelBufferElement.
                return false;
            }

            virtual std::shared_ptr<XMLNode> toXML()
            {
                CLAID_THROW(Exception, "Cannot serialize data to XML.\n"
                << "The channel's buffer (ChannelBuffer) is untyped.\n"
                << "Can only serialize, if there is at least one typed channel instance \n"
                << "available for the targeted channel with a given channel ID in this process.\n"
                << "Make sure there is at least one typed publisher or subscriber for this channel"
                << "in the current instance of CLAID.");
            }

            virtual bool applySerializerToData(std::shared_ptr<AbstractSerializer> serializer, bool addHeader = false)
            {

                // The ChannelBuffer is untyped.
                // Hence, we have to deserialize the data from the binary format first, and apply the serializer afterwards.
                // Therefore, we have to look up the corresponding untyped reflectors, both for the BinaryDeserializer as well as the serializer we shall apply.

                std::string dataTypeName = this->dataTypeName;
                std::string reflectorName = serializer->getReflectorName();

                std::shared_ptr<void> untypedInstance = ClassFactory::getInstance()->getNewUntypedInstance(dataTypeName);
                if(untypedInstance == nullptr)
                {
                    CLAID_THROW(claid::Exception, "Cannot apply serializer to untyped data received on channel.\n"
                    << "The data is of type \"" << dataTypeName << "\", which is unknown to the class factory of this instance of CLAID. This might happen due to one of the following reasons:\n"
                    << "1st The class was not registered to the CLAID serialization system using REGISTER_SERIALIZATION(...). Please make sure that you register the class appropriately.\n"
                    << "2nd The data was received from a remotely connected instance of CLAID, that defined a new data type (e.g., in a separate package), which is not known to this instance of CLAID.\n"
                    << "Make sure that if you want to send data between multiple instances of CLAID, both instances including the packages that contain the corresponding data type.\n"
                    << "If you defined a data type outside of a package, then make sure that this definition is available (at compile time) for both the local and remote instance of CLAID.\n"
                    << "TO SOLVE THIS ISSUE: Either register the class using REGISTER_SERIALIZATION (...) OR let another Module subscribe to the corresponding channel in a typed manner, i.e.: subscribe<" << dataTypeName << ">");
                }

                UntypedReflector* untypedBinaryDeserializer;
                if (!ReflectionManager::getInstance()->getReflectorForClass(dataTypeName, "BinaryDeserializer", untypedBinaryDeserializer))
                {
                    CLAID_THROW(claid::Exception, "Cannot apply serializer to untyped data received on channel.\n"
                    << "The data is of type \"" << dataTypeName << "\", which is unknown to this instance of CLAID. This might happen due to one of the following reasons:\n"
                    << "1st The class was not registered to the CLAID serialization system using REGISTER_SERIALIZATION(...). Please make sure that you register the class appropriately.\n"
                    << "2nd The data was received from a remotely connected instance of CLAID, that defined a new data type (e.g., in a separate package), which is not known to this instance of CLAID.\n"
                    << "Make sure that if you want to send data between multiple instances of CLAID, both instances including the packages that contain the corresponding data type.\n"
                    << "If you defined a data type outside of a package, then make sure that this definition is available (at compile time) for both the local and remote instance of CLAID.");
                }

                std::shared_ptr<BinaryDataReader> reader = std::make_shared<BinaryDataReader>(this->binaryData.value());
                BinaryDeserializer binaryDeserializer(reader);
                
                untypedBinaryDeserializer->invoke(static_cast<void*>(&binaryDeserializer), untypedInstance.get());             
                printf("Apply 6");

                UntypedReflector* untypedSerializer;
                if (!ReflectionManager::getInstance()->getReflectorForClass(dataTypeName, reflectorName, untypedSerializer))
                {
                //      CLAID_THROW(claid::Exception, "Cannot apply serializer to untyped data received on channel.\n"
                //     << "It was tried to apply a serializer of type \"" << reflectorName << "\" to data type \"" << dataTypeName << "\", however the serializer does not support this data type."
                //     << "Make sure that the reflector was registered for that data type. If it is a standard serializer, such as BinarySerializer or XMLSerializer, make sure you register your data type using REGISTER_SERIALIZATION(...).");
                // 
                    return false;
                }
                untypedSerializer->invoke(static_cast<void*>(serializer.get()), untypedInstance.get());   

                if(addHeader)
                {
                    addDataHeaderAsMemberUsingSerializer(serializer);
                }

                return true;
            }

            void addDataHeaderAsMemberUsingSerializer(std::shared_ptr<AbstractSerializer> serializer)
            {
                std::string dataTypeName = ClassFactory::getInstance()->getClassNameOfObject(this->header);
                std::string reflectorName = serializer->getReflectorName();

                UntypedReflector* untypedSerializer;
                if (!ReflectionManager::getInstance()->getReflectorForClass(dataTypeName, reflectorName, untypedSerializer))
                {
                     CLAID_THROW(claid::Exception, "Cannot apply serializer to header of type \"" << dataTypeName << "\" of untyped data received on channel.\n"
                    << "It was tried to apply a serializer of type \"" << reflectorName << "\" to the header of data received in an untyped channel. This might happen if you implemented a custom header (inheriting from TaggedDataBase)"
                    << "and forgot to add it to CLAID's serialization system using REGISTER_SERIALIZATION(...)");
                }
                untypedSerializer->invokeMember("header", static_cast<void*>(serializer.get()), static_cast<void*>(&this->header));            
            }

            



    };
}
