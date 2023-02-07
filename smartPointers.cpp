#include <iostream>
#include <memory>

using namespace std;


void foo(unique_ptr<int> ptr)
{
    cout << "Function foo with unique pointer at adress: " << ptr.get() << " / value=" << *ptr << endl;
}



int main()
{
    unique_ptr<int> a {make_unique<int>(5)};
    auto b {make_unique<int>(10)};
    cout << b.get() << endl;
    cout << "Unique pointer at adress: " << a.get() << " / value=" << *a << endl;
    foo(move(a));

    cout << "Unique pointer at adress: " << a.get() << endl;




}
