#pragma once
#include "Serialization/Serialization.hpp"

namespace claid
{
    namespace RemoteConnection
    {
        struct MessageHeader
        {

            virtual ~MessageHeader()
            {

            }

            template<typename Reflector>
            void reflect(Reflector& r)
            {
            }

            template<typename T>
            bool is() const
            {
                return dynamic_cast<const T*>(this) != nullptr;
            }

            template<typename T>
            T* as()
            {
                return static_cast<T*>(this);
            }

            template<typename T>
            const T* as() const
            {
                return static_cast<const T*>(this);
            }

            
        };
    }
}