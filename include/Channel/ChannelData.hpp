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

#include "TaggedData.hpp"
#include "Untyped.hpp"
#include "Binary/BinaryData.hpp"
#include "Channel/ChannelBufferElement.hpp"



#include "Binary/BinarySerializer.hpp"
#include "Binary/BinaryDeserializer.hpp"

#include "XML/XMLSerializer.hpp"
#include "Reflection/Reflect.hpp"
#include "Serialization/Serializer.hpp"

namespace claid
{
    // Forward declaration
 
    struct ChannelBufferElement;

    // Not a DataBase, its just the base class for ChannelData ;)
    class ChannelDataBase
    {
        protected:
            bool valid;

            

            ChannelDataBase(bool valid) : valid(valid)
            {

            }

        public:
            virtual ~ChannelDataBase()
            {
                
            }

            bool isValid() const
            {
                return this->valid;
            }

            virtual TaggedData<BinaryData> getBinaryData() = 0;
            virtual std::shared_ptr<XMLNode> headerToXML() = 0;
            virtual std::shared_ptr<XMLNode> toXML() = 0;
            virtual bool canSerializeToXML() = 0;
            virtual bool applySerializerToData(std::shared_ptr<AbstractSerializer> serializer, bool addHeader = false) = 0;

    };

    template<typename T>
    class ChannelData;

    template<>
    class ChannelData<Untyped> : public ChannelDataBase
    {

        private:
            // Why not store TaggedData<BinaryData> ?
            // Because we want to serialize only when we really need it.
            // If someone subscribed to a channel untyped, maybe he just wants
            // to retrieve information about individual elements but not necessarily
            // serialize them. If the BinaryData is required, it can be retrieved using
            // getBinaryData().
            TaggedDataBase header;     

            // The ChannelBufferElement that our data belongs to.
            // This makes sure that the ChannelBufferElement is not deleted as long as an instance
            // of ChannelData still holds it.
            // Can also be used to retrieve binaryData.
            std::shared_ptr<ChannelBufferElement> channelBufferElement;   

        public:

            static ChannelData InvalidChannelData()
            {
                ChannelData channelData;
                channelData.valid = false;
                return channelData;
            }

            ChannelData() : ChannelDataBase(false)
            {

            }

            ChannelData(TaggedDataBase header, 
                std::shared_ptr<ChannelBufferElement> channelBufferElement) : ChannelDataBase(true), header(header), channelBufferElement(channelBufferElement)
            {
            }

            const Time& getTimestamp() const
            {
                return this->header.timestamp;
            }

            const uint64_t getSequenceID()
            {
                return this->header.sequenceID;
            }

            TaggedData<BinaryData> getBinaryData() 
            {
                return channelBufferElement->getBinaryData();
            }

            virtual TaggedDataBase getHeader()
            {
                return this->header;
            }

            std::shared_ptr<XMLNode> headerToXML()
            {
                XMLSerializer serializer;
                serializer.serialize("header", this->header);
                return serializer.getXMLNode();
            }

            std::shared_ptr<XMLNode> toXML()
            {
                return this->channelBufferElement->toXML();
            }

            bool canSerializeToXML()
            {
                // When ChannelData is untyped, we can only
                // serialize to XML if the bufferElement is typed.
                return this->channelBufferElement->canSerializeToXML();
            }

            bool applySerializerToData(std::shared_ptr<AbstractSerializer> serializer, bool addHeader = false)
            {
                return this->channelBufferElement->applySerializerToData(serializer, addHeader);
            }



     
        // ChannelData(ChannelBuffer<T>* holderBuffer, TaggedData<T>& taggedData) : ChannelDataBase(true), holderBuffer(holderBuffer), taggedData(taggedData)
        // {
        //     holderBuffer->serialize();
        // }
    };

    // Template specialization
    template<typename T>
    class ChannelData : public ChannelDataBase
    {
        private:
            TaggedData<T> taggedData;
            const TaggedData<T>& internalValue() const
            {
                return taggedData;
            }

            // The ChannelBufferElement that our data belongs to.
            // This makes sure that the ChannelBufferElement is not deleted as long as an instance
            // of ChannelData still holds it.
            // Can also be used to retrieve binaryData.
            std::shared_ptr<ChannelBufferElement> channelBufferElement;   

        public:

            static ChannelData InvalidChannelData()
            {
                ChannelData channelData;
                channelData.valid = false;
                return channelData;
            }

            ChannelData() : ChannelDataBase(false)
            {
            }


            ChannelData(TaggedData<T> taggedData, 
                std::shared_ptr<ChannelBufferElement> channelBufferElement) : ChannelDataBase(true), taggedData(taggedData), channelBufferElement(channelBufferElement)
            {
            }

            


            TaggedData<BinaryData> getBinaryData() 
            {
                return channelBufferElement->getBinaryData();
            }

            TaggedDataBase getHeader()
            {
                return *static_cast<TaggedDataBase*>(&taggedData);
            }

            const Time& getTimestamp() const
            {
                return this->taggedData.timestamp;
            }

            const uint64_t getSequenceID()
            {
                return this->taggedData.sequenceID;
            }

            operator const TaggedData<T>&() const 
            { 
                return this->internalValue(); 
            }


            const TaggedData<T>& operator*() const 
            { 
                return this->internalValue(); 
            }


            const TaggedData<T>* operator->() const 
            { 
                return &this->internalValue();
            }

            std::shared_ptr<XMLNode> headerToXML()
            {
                XMLSerializer serializer;
                TaggedDataBase header = this->getHeader();
                serializer.serialize("header", header);
                return serializer.getXMLNode();
            }

            std::shared_ptr<XMLNode> toXML()
            {
                if(!this->valid)
                {
                    CLAID_THROW(Exception, "Error, cannot serialize ChannelData to XML, data not valid");
                }
                return this->channelBufferElement->toXML();
            }

            bool canSerializeToXML()
            {
                // When ChannelData is typed, we can always serialize to XML.
                return true;
            }

            bool applySerializerToData(std::shared_ptr<AbstractSerializer> serializer, bool addHeader = false)
            {
                return this->channelBufferElement->applySerializerToData(serializer, addHeader);
            }


            
    };
}