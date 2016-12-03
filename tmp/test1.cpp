#include <iostream>
#include <type_traits>

template<typename B, typename D>
struct IsBase {
    using Yes = struct { char x[1]; };
    using No = struct { char x[2]; };
    constexpr static Yes test(const B*);
    constexpr static No test(...); 

    static const bool value = sizeof(test(static_cast<D*>(0))) == sizeof(Yes);
};

template<typename T1, typename T2>
struct IsSame { static const bool value = false; };

template<typename T>
struct IsSame<T, T> { static const bool value = true; };
 
using namespace std;

struct Base {};
struct Derived : public Base {}; 
struct NotDerived {};

template<typename B, typename D>
constexpr bool isDerived() {
    return IsBase<B, D>::value;
}

template<typename U, typename V>
constexpr bool isSame() {
    return IsSame<U, V>::value;
}

#define VAL(x) #x "=" << (x) << ", "
int main() {
    Base b;
    Derived d;
    
    static_assert(isDerived<Base, Derived>(), "C1 is NOT base of C2");
    static_assert(isDerived<Base, NotDerived>(), "C1 is NOT base of C2");
    static_assert(isDerived<Derived, Base>(), "C1 is NOT base of C2");

    static_assert(isSame<Base, Base>(), "C1 is NOT same as C2");
    static_assert(isSame<Derived, Derived>(), "C1 is NOT same as C2");
    static_assert(isSame<NotDerived, NotDerived>(), "C1 is NOT same as C2");
    static_assert(isSame<Derived, NotDerived>(), "C1 is NOT same as C2");
    static_assert(isSame<Base, Derived>(), "C1 is NOT same as C2");
}