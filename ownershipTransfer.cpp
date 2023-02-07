#include <iostream>
#include <memory>

using namespace std;


class A
{
public:
    A() {
        cout << "CTor" << endl;
    }

    ~A() {
        cout << "DTor" << endl;
    }
};



class B
{
public:
    B() {
        cout << "create B" << endl;
    };
    ~B() {
        cout << "delete B" << endl;
    }

    void setA(unique_ptr<A> &&p_a) {
        m_a = move(p_a);
    }

private:
    unique_ptr<A> m_a {nullptr};
};



int main(int argc, char *argv[])
{
    B b {};

    {
        unique_ptr<A> a {make_unique<A>()};
        b.setA(move(a));
        cout << "presque fin" << endl;

    }
    cout << "fin" << endl;

    return 0;
}
