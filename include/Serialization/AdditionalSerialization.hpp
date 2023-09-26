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

// Add every additional De/Serializers or registration helpers 
// (in addition to the standard ones such as XMLDeSerializers and BinaryDeSerializers, 
// registration to ClassFactory), that you need here.


#if defined(__ANDROID__) || defined(__JAVA_WRAPPERS__)
    #include "JavaWrapper/JavaWrapperMaster.hpp"
#endif




#if defined(__ANDROID__) || defined(__JAVA_WRAPPERS__)
    #define REGISTER_JAVA_DE_SERIALIZERS(className) \
        REGISTER_JAVA_WRAPPER(className) 
#else
    #define REGISTER_JAVA_DE_SERIALIZERS(className) 
#endif 

#ifdef __PYTHON_WRAPPERS__
    #include "PythonWrapper/PythonWrapperMaster.hpp"
#endif


#ifdef __PYTHON_WRAPPERS__
    #define REGISTER_PYTHON_DE_SERIALIZERS(className) REGISTER_PYTHON_WRAPPER(className)
#else
    #define REGISTER_PYTHON_DE_SERIALIZERS(className)
#endif



#define REGISTER_ADDITIONAL_DE_SERIALIZERS(className) \
    REGISTER_JAVA_DE_SERIALIZERS(className)\
    REGISTER_PYTHON_DE_SERIALIZERS(className)
