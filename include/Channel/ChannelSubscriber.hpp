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
#include "RunnableDispatcherThread/RunnableDispatcherThread.hpp"
#include "RunnableDispatcherThread/FunctionRunnableWithParams.hpp"
#include "RunnableDispatcherThread/Subscriber.hpp"

#include <functional>

#include "ChannelData.hpp"
#include "mutex"
#include <iostream>
#include <deque>

namespace claid
{
    // forward declaration
    template<typename T>
    class TypedChannel;



    class ChannelSubscriberBase : public Subscriber
    {
        private:
            // ChannelSubscriber has to be copyable, but mutex is non copyable.
            // Thus, create mutex on heap.
            std::shared_ptr<std::mutex> mutex = nullptr;

        protected:
            void lockMutex()
            {
                if(this->mutex.get() == nullptr)
                {
                    this->mutex = std::shared_ptr<std::mutex>(new std::mutex());
                }

                this->mutex->lock();
            }

            void unlockMutex()
            {
                this->mutex->unlock();
            }

        public:
            ChannelSubscriberBase(std::shared_ptr<RunnableDispatcherThread> runnableDispatcherThread) : Subscriber(runnableDispatcherThread)
            {
            
            }
    };

    template<typename T>
    class ChannelSubscriber : public ChannelSubscriberBase
    {   

        private: 
            std::function<void(ChannelData<T>)> function;
            TypedChannel<T>* channel;
            
            std::shared_ptr<Runnable> runnable;

            
            std::deque<ChannelData<T>> channelDataQueue;

            


            void run()
            {
                this->lockMutex();
                ChannelData<T> channelData = this->channelDataQueue.front();
                size_t sizeBeforePop = this->channelDataQueue.size();
                this->channelDataQueue.pop_front();
                this->unlockMutex();

                this->function(channelData);
             }

        public:
            ChannelSubscriber(std::shared_ptr<RunnableDispatcherThread> runnableDispatcherThread,
                      std::function<void (ChannelData<T>)> function) : ChannelSubscriberBase(runnableDispatcherThread), function(function)
            {
                
            }

            ~ChannelSubscriber()
            {
                
            }

            void setChannel(TypedChannel<T>* channel)
            {
                this->channel = channel;
            }
            
            void signalNewDataIsAvailable()
            {
                this->lockMutex();
                ChannelData<T> data = channel->read();
                if(data.isValid())
                {
                    this->channelDataQueue.push_back(data);
                    Subscriber::signalNewDataIsAvailable();
                }
                else
                {
                }
                this->unlockMutex();
            }

            std::shared_ptr<Runnable> getRunnable()
            {
                std::shared_ptr<
                    FunctionRunnableWithParams<void>> functionRunnable =
                            std::make_shared<FunctionRunnableWithParams<void>>();


                functionRunnable->bind(&ChannelSubscriber::run, this);

                return std::static_pointer_cast<Runnable>(functionRunnable);
            }


             
    };  
}
