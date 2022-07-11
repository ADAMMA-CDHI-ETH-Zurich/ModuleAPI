#pragma once

#include "TaggedData.hpp"

namespace portaible
{
    template<typename T>
    class ChannelRead
    {
        private:
            TaggedData<T> data;
            const TaggedData<T>& internalValue() const
            {
                return data;
            }

            bool valid;


            // The channel buffer that holds the TaggedData that we are using.
            // More specifically: Data contains a shared_ptr<T> with the underlying data.
            // 
            ChannelBuffer<T>* holderBuffer;

        public:

            static ChannelRead InvalidChannelRead()
            {
                ChannelRead channelRead;
                channelRead.valid = false;
                return channelRead;
            }

            ChannelRead() : valid(false)
            {

            }


            ChannelRead(TaggedData<T>& data) : data(data), valid(true)
            {

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

            const Time& getTimestamp() const
            {
                return this->data.timestamp;
            }

            bool isValid() const
            {
                return this->valid;
            }
    };
}