#include <iostream>

using namespace std;


/*
DEFINITIONS
- lvalue: left value of operand. A variable with a name and a defined place in memory
- rvalue: right value of operand: A literal, unnamed, just a temporary value.
*/

/*
Overload summary
- defining foo(int) and foo(const int) at the same time will result in ambiguous calls
- defining foo(int) and foo(int&) at the same time will result in ambiguous calls
- defining foo(const int&&) is ok but doesn't make sense since purpose would probably to modify value
*/

void foo(int &a) {
    cout << ">> foo(int&)" << endl;
    a++;
}

void foo(const int &a) {
    cout << ">> foo(const int&)" << endl;
    // a++;     // Forbidden: a is const
}

void foo(int &&a) {
    cout << ">> foo(int&&)" << endl;
    a++;        // Possible, even if it refers to a literal
}


int bar() {
    int a {3};
    return a;
}


class MyClass
{
public:
    MyClass() : m_a(0) {
        cout << ">> Default CTor" << endl;
    }

    MyClass(const MyClass& other) :
        m_a(other.m_a)
    {
        cout << ">> Copy CTor" << endl;
    }

    MyClass(MyClass&& other) :
        m_a(other.m_a)
    {
        cout << ">> Move CTor" << endl;
    }

    MyClass& operator=(const MyClass& a) {
        cout << ">> Assignment operator" << endl;
        if (this != &a) {       // Check is dest different that source
            m_a = a.m_a;
        }
        return *this;
    }

    MyClass& operator=(MyClass&& a) {
        cout << ">> Move assignment operator" << endl;
        if (this != &a) {       // Check is dest different that source
            m_a = a.m_a;
        }
        return *this;
    }

private:
    int m_a;
};

MyClass baz() {
    cout << ">> baz" << endl;
    MyClass a;
    cout << ">> /baz" << endl;
    return a;
}



int main()
{
    int a {0};
    constexpr int b {1};

    cout << "foo(a) - 'a' being a variable" << endl;
    foo(a);         // non const int -> call of 1st definition of foo
    cout << "foo(b) - 'b' being a constexpr" << endl;
    foo(b);         // const int -> call of 2nd definition of foo
    cout << "foo(5)" << endl;
    foo(5);         // literal, temporary value (ie rvalue) -> call of 3rd definition of foo
    cout << "foo(a+b)" << endl;
    foo(a+b);       // literal, temporary value (ie rvalue) -> call of 3rd definition of foo
    cout << "foo(bar()) - bar() returning a literal value" << endl;
    foo(bar());     // temporary value -> call of 3rd definition of foo
    cout << "foo(move(a))" << endl;
    foo(move(a));   // a is moved to be considered as rvalue -> call of 3rd definition of foo
     cout << "foo(move(b))" << endl;
    foo(move(b));   // b is moved but is const -> call of 2nd definition of foo

    cout << "==========" << endl;

    cout << "MyClass obj1" << endl;
    MyClass obj1;
    cout << "MyClass obj2{obj1}" << endl;
    MyClass obj2 {obj1};
    cout << "MyClass obj3 {move(obj1)}" << endl;
    MyClass obj3 {move(obj1)};
    cout << "/!\\ From now on, obj1 has been moved. I'm no more responsible of it and I shouldn't use it anymore" << endl;
    cout << "MyClass obj4(baz()} - baz() creating and returning an object" << endl;
    MyClass obj4 {baz()};
    cout << "obj2 = obj1" << endl;
    MyClass obj5;
    obj5 = obj2;
    cout << "obj2 = baz()" << endl;
    obj2 = baz();

}
