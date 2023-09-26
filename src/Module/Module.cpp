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
// For remaining implementations of Module, see Module_impl.hpp
// (was done in separate _impl file, in order to solve cycle dependencies..)

#include "RunTime/RunTime.hpp"

namespace claid
{
    // Somehow compiler does not like the constructor in the Module_imp .. soooo
    Module::Module()
    {
        this->channelManager = &CLAID_RUNTIME->channelManager;
    }
}
// Yes, we do not use the REGISTER_MODULE macro here, because class "Module" should not be loadable by itself (it's just a base class!).
REGISTER_TO_CLASS_FACTORY(claid::Module) 
REGISTER_REFLECTOR_FOR_CLASS(claid::Module, claid::XMLDeserializer, XMLDeserializer) 
REGISTER_REFLECTOR_FOR_CLASS(claid::Module, claid::XMLSerializer, XMLSerializer) 
REGISTER_REFLECTOR_FOR_CLASS(claid::Module, claid::BinarySerializer, BinarySerializer)
REGISTER_REFLECTOR_FOR_CLASS(claid::Module, claid::BinaryDeserializer, BinaryDeserializer)