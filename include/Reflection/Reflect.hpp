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

#include "VariableWithGetterSetter.hpp"
#define EmptyReflect(Class)\
    template<typename Reflector>\
    void reflect(Reflector& reflector)\
    {\
        typedef Class THIS_CLASS;\
    }\

#define Reflect(Class, code)\
    template<typename Reflector>\
    void reflect(Reflector& reflector)\
    {\
        typedef Class THIS_CLASS;\
        code\
    }\

#define CustomReflectForReflector(Class, Reflector, code)\
    template<>\
    void reflect<Reflector>(Reflector& reflector)\
    {\
        typedef Class THIS_CLASS;\
        code\
    }

#define CustomReflectForReflectorNonNamespace(Class, Reflector, code)\
    template<>\
    void Class::reflect<Reflector>(Reflector& reflector)\
    {\
        typedef Class THIS_CLASS;\
        code\
    }

#define SplitReflect()\
    template<typename Reflector>\
    void reflect(Reflector& reflector)\
    {\
        splitReflectInType(reflector, *this);\
    }\


#define ReflectRead(Class, code)\
    template<typename Reflector>\
    void reflectRead(Reflector& reflector)\
    {\
        typedef Class THIS_CLASS;\
        code\
    }\

#define ReflectWrite(Class, code)\
    template<typename Reflector>\
    void reflectWrite(Reflector& reflector)\
    {\
        typedef Class THIS_CLASS;\
        code\
    }\

#define reflectMember(mem) reflector.member(#mem, mem, ""); reflector.pointer_to_member(#mem, &THIS_CLASS::mem)
#define reflectGetterSetter(mem, getter, setter) reflector.member(#mem, getter, setter);
#define reflectMemberWithDefaultValue(mem, defaultValue) reflector.member(#mem, mem, "", defaultValue); reflector.pointer_to_member(#mem, &THIS_CLASS::mem)
#define reflectFunction(function) reflector.pointer_to_function(#function, &THIS_CLASS::function)
