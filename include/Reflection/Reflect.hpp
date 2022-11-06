#pragma once


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

#define SplitReflect(code)\
    template<typename Reflector>\
    void reflect(Reflector& reflector)\
    {\
        splitReflectInType(r, *this);\
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
#define reflectFunction(function) reflector.pointer_to_function(#function, &THIS_CLASS::function)