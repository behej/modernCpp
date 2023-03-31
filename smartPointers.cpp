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
    // *B;  // Forbidden: Memory prviously referenced by B now belongs to A. B has been set to 0.

    /* Note:
     * Memory space previously referenced by A is automatically freed when overwriting it with B.
     * A now references address previously referenced by B
     * B now references 0
     * All of this happens thanks to the use of smart pointer
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


    cout << endl << "Shared pointer" << endl;
    cout << "==============" << endl;


}
