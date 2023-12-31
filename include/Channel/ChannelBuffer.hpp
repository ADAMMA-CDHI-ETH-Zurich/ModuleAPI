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

#include "Untyped.hpp"
#include "TaggedData.hpp"
#include "ChannelData.hpp"
#include "ChannelBufferBase.hpp"
#include "Channel/ChannelBufferElementTyped.hpp"
#include "Binary/BinaryData.hpp"
#include "TypeChecking/TypeCheckingFunctions.hpp"
#include <vector>
#include <mutex>
#include <thread>
#include <iostream>

namespace claid
{
    
    template<typename T>
    class ChannelBuffer;

    template<>
    class ChannelBuffer<Untyped> : public ChannelBufferBase
    {    

        public:
            ChannelBuffer() : ChannelBufferBase()
            {
                this->dataTypeName = "Untyped";
            }


            ChannelData<Untyped> getDataByIndex(size_t index)
            {
                std::shared_ptr<ChannelBufferElement>& element = this->getElement(index);
                ChannelData<Untyped> data(element->getHeader(), element);
                return data;
            }
            
            bool getLatest(ChannelData<Untyped>& latest)
            {
                return ChannelBufferBase::getLatest<Untyped>(this, latest);
            }
            // TODO Very likely this contains a bug
            bool getClosest(const Time& timestamp, ChannelData<Untyped>& closest)
            {
                return ChannelBufferBase::getClosest<Untyped>(this, timestamp, closest);
            }

            void getDataInterval(const Time& min, const Time& max, std::vector<ChannelData<Untyped> >& channelDataInterval)
            {
                ChannelBufferBase::getDataInterval<Untyped>(this, min, max, channelDataInterval);
            }

            virtual bool isTyped() const
            {
                return false;
            }

            
            
            template<typename NewType>
            ChannelBuffer<NewType>* type()
            {
                this->lockMutex();

                std::shared_ptr<ChannelBufferElement> newElements[MAX_CHANNEL_BUFFER_SIZE];

                for(size_t i = 0; i < MAX_CHANNEL_BUFFER_SIZE; i++)
                {
                    // In constructor, the binary data automatically gets deserialized.
                    // Note, that binary data is not copied, as TaggedData<BinaryData> uses a shared_ptr internally.
                    std::shared_ptr<ChannelBufferElementTyped<NewType>> 
                        typedElement;
                    
                    if(this->channelBufferElements[i]->isDataAvailable())
                    {   
                        typedElement = 
                        std::shared_ptr<ChannelBufferElementTyped<NewType>>(
                            new ChannelBufferElementTyped<NewType>(this->channelBufferElements[i]->getBinaryData()));
                    }
                    else
                    {
                        typedElement = 
                        std::shared_ptr<ChannelBufferElementTyped<NewType>>(
                            new ChannelBufferElementTyped<NewType>());
                    }
                     
                    newElements[i] = std::static_pointer_cast<ChannelBufferElement>(typedElement);
                }



                ChannelBuffer<NewType>* newBuffer = new ChannelBuffer<NewType>(newElements, this->currentIndex, this->numElements, this->dataTypeName);


                this->unlockMutex();

                return newBuffer;
            }          

            intptr_t getDataTypeIdentifier() const
            {
                return TypeChecking::getDataTypeUniqueIdentifier<Untyped>();
            }  
    };  

    template<typename T>
    class ChannelBuffer : public ChannelBufferBase
    {
        private:
            
            
            // Overriden from ChannelBufferBase
            std::shared_ptr<ChannelBufferElement> newChannelBufferElementFromBinaryData(TaggedData<BinaryData>& binaryData)
            {
                // Constructor automatically deserializes the data if possible.
                std::shared_ptr<ChannelBufferElementTyped<T>> typedElement(new ChannelBufferElementTyped<T>(binaryData));
              
                return std::static_pointer_cast<ChannelBufferElementTyped<T>>(typedElement);
            }

            std::shared_ptr<ChannelBufferElement> newChannelBufferElementFromTypedData(TaggedData<T>& data)
            {
                // Constructor automatically deserializes the data if possible.
                std::shared_ptr<ChannelBufferElementTyped<T>> typedElement(new ChannelBufferElementTyped<T>(data));
              
                return std::static_pointer_cast<ChannelBufferElementTyped<T>>(typedElement);
            }

        public:

         

            ChannelBuffer() : ChannelBufferBase()
            {
                this->dataTypeName = TypeChecking::getCompilerIndependentTypeNameOfClass<T>();
            }

            // Used by type function of ChannelBuffer<Untyped>
            ChannelBuffer(std::shared_ptr<ChannelBufferElement> channelBufferElements[MAX_CHANNEL_BUFFER_SIZE],
            size_t currentIndex,
            size_t numElements, std::string dataTypeName) 
            {

                for(size_t i = 0; i < MAX_CHANNEL_BUFFER_SIZE; i++)
                {
                    this->channelBufferElements[i] = channelBufferElements[i];
                }

                this->currentIndex = currentIndex;
                this->numElements = numElements;
                this->dataTypeName = dataTypeName;
            }  

            


            ChannelData<T> getDataByIndex(size_t index)
            {
                if(!this->getElement(index)->isDataAvailable())
                {
                    CLAID_THROW(Exception, "Error! Tried to access channel data at index " << index << " in ChannelBuffer (data type \""
                    << this->getDataTypeName() << "\", but data was not available.");
                }

                // Elements are typed.
                std::shared_ptr<ChannelBufferElementTyped<T>> 
                    typedElement = std::static_pointer_cast<ChannelBufferElementTyped<T>>(this->getElement(index));

                return ChannelData<T>(typedElement->getTypedData(), this->getElement(index));
            }
            

            void insert(TaggedData<T> data)
            {
                this->lockMutex();
                // Is this thread safe?
                // Yes.. as long as we create copies in getLatest, getClosest and getDataInterval.

                this->channelBufferElements[this->currentIndex] = newChannelBufferElementFromTypedData(data);
                // Do not convert to binary data / serialize here. It might not be needed.
                increaseIndex();

                this->unlockMutex();
            }

         

            bool getLatest(ChannelData<T>& latest)
            {
                return ChannelBufferBase::getLatest<T>(this, latest);
            }
            // TODO Very likely this contains a bug
            bool getClosest(const Time& timestamp, ChannelData<T>& closest)
            {
                return ChannelBufferBase::getClosest<T>(this, timestamp, closest);
            }

            void getDataInterval(const Time& min, const Time& max, std::vector<ChannelData<T> >& channelDataInterval)
            {
                ChannelBufferBase::getDataInterval<T>(this, min, max, channelDataInterval);
            }

            virtual bool isTyped() const
            {
                return true;
            }


            intptr_t getDataTypeIdentifier() const
            {
                std::string name = typeid(T).name();
                return TypeChecking::getDataTypeUniqueIdentifier<T>();
            }

    };
}

