#include <iostream>
#include <memory>

using namespace std;


void foo(unique_ptr<int> ptr)
{
    cout << "Function foo with unique pointer at adress: " << ptr.get() << " / value=" << *ptr << endl;
}



int main()
{
    cout << "Unique pointer" << endl;
    cout << "==============" << endl;

    // Create unique pointers to integers
    unique_ptr<int> A {make_unique<int>(5)};
    auto B {make_unique<int>(10)};
    cout << ">> Create unique pointers" << endl;
    cout << "Unique pointer 'a' at adress: " << A.get() << " / value=" << *A << endl;
    cout << "Unique pointer 'b' at adress: " << B.get() << " / value=" << *B << endl;

    // Copy is forbidden
    // A = B;   // Forbidden: uniq_pointer can't be copied.
    // foo(A);  // Use of uniq_pointer as function parameter is forbidden as well since it would copied to a local var

    // But uniq_pointer can be moved
    A = move(B);    // b is now moved to be stored in a. b shouldn't be accessed anymore
    cout << ">> Move unique pointers" << endl;
    cout << "Unique pointer 'A' at adress: " << A.get() << " / value=" << *A << endl;
    cout << "Unique pointer 'B' at adress: " << B.get() << endl;
    // *B;  // Forbidden: Memory previously referenced by B now belongs to A. B has been set to 0.

    /* Note:
     * Memory space previously referenced by A is automatically freed when overwriting it with B.
     * A now references address previously referenced by B
     * B now references 0
     * All of this happens thanks to the use of smart pointers
     */


    {
        auto C = make_unique<double>(3.2);
    }
    /* When reaching end of variable scope, variable is deleted.
     * There is no need to explicitely delete allocated memory,
     * smart pointer will do it for you.
     */

    cout << ">> Move A to function. It's now under its responsability." << endl;
    foo(move(A));
    cout << "Unique pointer at adress: " << A.get() << endl;

    cout << endl;
    cout << "Shared pointer" << endl;
    cout << "==============" << endl;

    shared_ptr<double> S2;
    {
        auto S1 {make_shared<double>(10.0)};
        cout << "Shared pointer 'S1' at address " << S1.get() << " / value: " << *S1 << endl;
        S2 = S1;
        cout << "Shared pointer 'S2' at address " << S2.get() << " / value: " << *S2 << endl;
        cout << S1.use_count() << " pointers refer to that memory location. Is unique ? " << boolalpha << S1.unique() << endl;
    }

    cout << endl;
    cout << "S1 went out of scope and is therefore deleted." << endl;
    cout << "But memory is still in use by S2, so pointer is not deleted. Use counter=" << S2.use_count() << " / unique ? " << S2.unique() << endl;
    cout << "Memory will be released when no pointer share that same memory location" << endl;



}
