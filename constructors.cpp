#include <iostream>

using namespace std;


class Demo {
public:
    // Explicitely indicate to use default constructor
    Demo() = default;

    Demo(int a, int b) : m_a(a), m_b(b)
    {}

    // Constructor with only 1 parameter declared as EXPLICIT
    explicit Demo(int a) : Demo(a, 0)
    {}

    // Copy constructor marked as DEFAULT
    Demo(Demo const &) = default;

    /* Assignment operator marked as DELETED
     * This means it can't be used.
     * In fact, any method could be marked as deleted.
     * But it really makes sense for:
     * - copy constructor
     * - assignment operator
     * - inherited methods that shouldn't be called for a given subclass
     */
    Demo& operator=(Demo const&) = delete;

    void display()
    {
        cout << "a = " << m_a << " / b = " << m_b << endl;
    }

    // Equality operator
    bool operator== (Demo obj)
    {
        return (m_a == obj.m_a &&
                m_b == obj.m_b);
    }

private:
    /* If all members are always initialized with constant value,
     * they can be initialized with braces directly in class declaration.
     * If one member has to be initialized differently acording to a value
     * passed as constructor parameter, it has to be initialized in the constructor
     * directly, not here. Then all parameters should be initialized in the
     * constructor to avoid mixing ways of intializing parameters
     */
    int m_a {0};
    int m_b {0};
};




int main()
{
    // Create first object, using default constructor
    Demo obj1;
    obj1.display();

    // Create second object using COPY constructor
    auto obj2 {Demo(1)};
    obj2.display();


    // Comparison is performed using obj2 of type Demo and newly object of type Demo created juste here
    // Both objects of type Demo, comparison can be achieved
    if (obj2 == Demo(1))
        cout << "Same" << endl;
    else
        cout << "Different" << endl;

    /* Comparison is performed using obj2 of type Demo and an integer. Program would try
     * to implicitely convert that int into Demo type. This could be achieved since there is
     * one constructor with one integer parameter.
     * But this constructor is declared as explicit, that means it can't be used
     * for implicit conversions.
     **/
    // if (obj2 == 1)
    //     cout << "Same" << endl;
    // else
    //     cout << "Different" << endl;



   /* Assignment can be done since assignment operator has been deleted
    */
    // obj1 = obj2;

}
