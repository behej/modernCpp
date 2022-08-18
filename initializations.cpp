#include <iostream>

using namespace std;

class MyClass
{
public:
    MyClass() = default;
    MyClass(int a) : m_a(a)
    {}

private:
    int m_a {0};
};


// auto for return type C++11 syntax
auto myFunction1() -> int {
    return 3;
}

// auto for return type C++14 syntax
auto myFunction2() {
    return 3;
}


// return type can't be determined when writing the code because it depends
// on types T1 and T2.
// decltype allows to define it.
// ----
// NOTE: in this example, decltype is not really useful because indicating the return type
// after the arrow is not necessary anymore with C++14
template<class T1, class T2>
auto myTemplate1(T1 p1, T2 p2) -> decltype(p1 * p2) {
    decltype(p1 * p2) result = {p1 * p2};
    return result;
}


int main()
{
    /* Braces initilizations
     * =====================
     * Initializations can now be done using curly braces.
     * This syntax is to be prefered since its forbids narrowing.
     */
    int a {3};
    int b = 3.5;        // Narrowing happens here: 3.5 is changed to 3 because var b is an int -> NOT GOOD
    // int c {3.5};     // Error: type mismatch, value is not silently converted
    // unsigned char d {256};   // Error: 256 can't fit in a char. Whereas d=256 would compile and lead to unexpected value

    // Also work with classes
    MyClass obj1 {MyClass()};
    MyClass obj2 {};
    MyClass obj3 {MyClass(4)};
    MyClass obj4 {4};
    auto obj5 {MyClass()};

    /* auto keyword
     * ============
     * Let compiler automatically determine type of variable.
     * Very useful when using iterators, where writing exactly the type
     * can be painful without bringing any interest.
     * According to CppCon, auto keyword is also interesting
     * when variable should stick to the same type of another variable
     * of track another var.
     */

    // Constants init
    auto var1 {3};
    auto var2 {3.};
    auto var3 {int(3)};
    /* PROS:
     * -No unintialized var: auto without init value won't work
     * -No narrowing: var is directly the same type as init value
     * -Better performance: no implicit conversion will occur
     * CONS:
     * -No completely suitable if var must be of one given type (muse use last example's syntax
     */

    // Stick to same type of value provided by a funtion/class
    auto var4 {myFunction1()};
    /* PROS:
     * -var automatically adapts to required type
     * -more robust to future changes: if myFunction evolves and changes its return type,
     *  no need to rework var and we are sure no unwanted conversion will occur
     * -Developper will focus more on algotithm and less on syntax
     * CONS:
     * -Doesn't apply if var must be of given type
     */


    /* const vs. constexpr
     *====================
     * const: for variables that shouldn't change once initialized
     * constexpr: for variable that could be defined at compile time and never supposed to change
     */
    const int var5 {myFunction1()};
    // var5 = 5;        // ERROR: can't change a const value

    constexpr int var6 {5};
    // var6 = 5;        // ERROR: can't change a const value

    // constexpr int var7 {myFunction1()};      // ERROR: variable can't be defined at compile time


    /* decltype
     * =========
     * Allows to deduce the type of an expression.
     * Especially useful if can't be determined when writing the code (templates)
     */
    decltype(3) var7 = 3.2;     // decltype to an int, so var7 will be of type int
    decltype(myFunction2()) var8 {5};   // decltype applied to a function

    auto var9 {myTemplate1(3, 2.)};     // var9 will be float since decltype in template will return a float
    auto var10 {myTemplate1(3, 2)};     // var10 will be int since decltype in template will return a int
    // int var11 {myTemplate1(3, 2.)};  // ERROR: decltype in template will return float whereas var11 is int

}
