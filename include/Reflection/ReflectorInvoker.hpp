// #pragma once
// #include <type_traits>
// #include "AbstractReflector.hpp"
// #include "TypeChecking/TypeCheckingFunctions.hpp"

// namespace claid
// {

//     template<typename Type>
//     struct ____MISSING_REFLECT_METHOD_FOR_{
// 	static_assert(sizeof(Type)==0,
// 	    "Cannot find an (unambiguous) intrusive or nonintrusive reflect method for Type. "
// 	    "If Type is a class that you have implemented, then you should "
// 	    "implement its reflect() method.");
// 	static void invoke() {}
// };

//     // Has no reflect
//     template<typename Reflector, typename T, class Enable = void>
//     struct ReflectorInvoker 
//     {
//         static void call(Reflector& r, T& member) 
//         {
//            ____MISSING_REFLECT_METHOD_FOR_<T>::invoke();
//         }

//     }; 

//     // TODO fix for standard types
//     template<typename Reflector, typename T>
//     struct ReflectorInvoker<Reflector, T, typename std::enable_if<!std::is_const<T>::value && has_mem_reflect<T>::value>::type>
//     {
//         static void call(Reflector& r, T& member) 
//         {
//             member.reflect(r);
//         }

//     }; 

//     template<typename Reflector, typename T>
//     struct ReflectorInvoker<Reflector, T, typename std::enable_if<!std::is_const<T>::value && has_non_member_function_reflect<Reflector&, T&>::value>::type>
//     {
//         static void call(Reflector& r, T& member) 
//         {
//             reflect(r, member);
//         }

//     }; 

//      // if const, we have to cast the constness aways check again.
//     template<typename Reflector, typename T>
//     struct ReflectorInvoker<Reflector, T, typename std::enable_if<std::is_const<T>::value>::type>
//     {
//         static void call(Reflector& r, const T& member) 
//         {
//             typedef typename std::remove_const<T>::type NonConstType; 
//             NonConstType& non_const_member = *const_cast<NonConstType*>(&member);
//             ReflectorInvoker<Reflector, NonConstType>::call(r, non_const_member);
//         }

//     }; 


// }