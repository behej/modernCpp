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
    /**
     * @brief Default CTor
     * Variable are initialized
     * Memory is allocated
     */
    MyClass() : m_a(0), m_ptr(new double(3.2)) {
        cout << ">> Default CTor" << endl;
    }

    /**
     * @brief Dtor
     * Memory dynamically allocated is freed
     */
    ~MyClass() {
        cout << ">> DTor" << endl;
        if (m_ptr != nullptr)
            delete m_ptr;
    }

    /**
     * @brief Copy CTor
     * @param other Object to copy
     * Variable is initialized with same value than other
     * Memory is allocated and initialized with same value that other
     */
    MyClass(const MyClass& other) :
        m_a(other.m_a), m_ptr(new double)
    {
        *m_ptr = *other.m_ptr;
        cout << ">> Copy CTor" << endl;
    }

    /**
     * @brief Move CTor
     * @param other Object to move
     * Variable is initialized with same value than other
     * Memory reference by other is moved and now belong to this object
     */
    MyClass(MyClass&& other) :
        m_a(other.m_a), m_ptr(other.m_ptr)
    {
        other.m_ptr = nullptr;  // Other should not reference memory space anymore, since it doesn't belong to it anymore
        cout << ">> Move CTor" << endl;
    }

    /**
     * @brief Affectation operator
     * @param other Other object to copy
     * Variable is set to same value than other
     * Memory previously reference is freed
     * Memory is then newly allocated with value of other
     */
    MyClass& operator=(const MyClass& other) {
        cout << ">> Assignment operator" << endl;
        if (this != &other) {       // Check if dest different than source
            m_a = other.m_a;
            delete m_ptr;       // Old memory is freed
            m_ptr = new double(*other.m_ptr);   // New memory is allocated, content is copied
        }
        return *this;
    }

    /**
     * @brief Move assignment operator
     * @param other other object to move
     * Variable is set to same value than other
     * Memory previously reference is freed
     * Memory reference by other is moved and now belong to this object
     */
    MyClass& operator=(MyClass&& other) {
        cout << ">> Move assignment operator" << endl;
        if (this != &other) {       // Check if dest different than source
            m_a = other.m_a;
            delete  m_ptr;      // Old memory is freed
            m_ptr = other.m_ptr;    // Pointer now points t memory of other
            other.m_ptr = nullptr;  // Other should not reference memory space anymore, since it doesn't belong to it anymore
        }
        return *this;
    }

    void display() {
        if (m_ptr != nullptr) {
            cout << "Addr: " << this << " / Val: " << m_a << endl;
            cout << "pointer: " << m_ptr << " / Val: " << *m_ptr << endl;
        }
        else {
            cout << "Object is invalid - probably moved" << endl;
        }
    }

private:
    int m_a;
    double* m_ptr;
};

MyClass baz() {
    cout << ">> baz" << endl;
    MyClass a;
    a.display();
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
    obj1.display();
    cout << "MyClass obj2{obj1}" << endl;
    MyClass obj2 {obj1};
    obj2.display();
    cout << "MyClass obj3 {move(obj1)}" << endl;
    MyClass obj3 {move(obj1)};
    cout << "obj1:" << endl;
    obj1.display();     // Object1 has been moved. It shouldn't be used anymore
    cout << "obj3:" << endl;
    obj3.display();
    cout << "/!\\ From now on, obj1 has been moved. I'm no more responsible of it and I shouldn't use it anymore" << endl;
    cout << "MyClass obj4(baz()} - baz() creating and returning an object" << endl;
    MyClass obj4 {baz()};
    obj4.display();
    cout << "obj5 = obj2" << endl;
    MyClass obj5;
    obj5.display();
    obj5 = obj2;
    obj2.display();
    obj5.display();
    cout << "obj2 = baz()" << endl;
    obj2 = baz();   // Use of move assignment operator. Without it, compiler would use copy assignment which may be less efficient.
    obj2.display();

}
