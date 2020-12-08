#include <type_traits>
#include <iostream>
#include "TupleBuilder.h"

template<class T, class U>
struct A
{
    using MemType = U;
    U T::* pMember;
};

template<class T, class U>
struct B
{
    using MemType = U;
    U T::* pMember;
};

template<class T, class U>
struct C
{
    using MemType = U;
    U T::* pMember;
};

struct ConcreteType
{
    int i;
    double d;
    char c;
};

template<class>
struct IsA : std::false_type {};

template<class T, class U>
struct IsA<A<T,U>> : std::true_type {};

int main()
{
    auto t1 = std::make_tuple(A{&ConcreteType::i},  B{&ConcreteType::c}, A{&ConcreteType::d});
    auto t2 = BuildFromOther([](auto& item)-> auto requires IsA<std::decay_t<decltype(item)>>::value {
        return C{item.pMember};
    }, t1);

    std::cout << std::tuple_size_v<decltype(t2)> << std::endl;

    return 0;
}
