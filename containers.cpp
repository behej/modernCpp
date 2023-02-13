#include <iostream>
#include <list>
#include <array>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <set>
#include <map>
#include <iterator>

#include "metaprogramming.h"



using namespace std;



int main()
{
    cout << "Several types of container" << endl;
    cout << "==========================" << endl;    
    cout << "-> vector" << endl << endl;

    // vector is a collection of items stored in contiguous memory location.
    // + can access any item from the vector
    // + no memory overhead
    // - adding a new element may imply to relocate the entire vector
    // - inserting an item is costly (must shift everything)
    // - memory is pre-allocated (there may be waste)
    vector<double> vec(5, 1.2);
    cout << vec;
    cout << "First item=" << vec.front() << " or " << vec[0] << endl;
    vec.resize(8, 3.3);
    cout << vec;

    //############################################################
    cout << endl << "-> list" << endl << endl;

    // Lists are similar to vectors. Difference is that memory is not contiguous
    // + easy insert or delete item, even in the middle of the list
    // + memory not pre-allocated (you pay what you need)
    // - hard to access any item from the list
    // - memory overhead (pointers to next and previous items)
    list<int> l(3, 2);      // Create liste of fixed size with default value
    cout << l;
    l.push_back(5);         // Add item to end of list
    cout << l;
    l.erase(l.begin());     // Remove list item
    cout << l;

    //############################################################

    cout << endl << "-> array" << endl << endl;

    // Array can be similar to vector, to the difference that its size is static
    // and may be allocated on the stack (whereas vector is always on the heap).
    // Arrays are more limited than vectors but can be more efficient, especially
    // for small sizes.
    array<int, 3> ar;
    cout << ar;
    ar.fill(2);
    cout << ar;

    //############################################################

    cout << endl << "-> stack" << endl << endl;

    // Stack: First in / Last ou
    stack<int> st;
    cout << "Stack just created. Is it empty ? " << boolalpha << st.empty() << endl;
    st.push(3);
    st.push(5);
    cout << "Pushed 2 items. Size is now " << st.size() << endl;
    cout << "Top item is " << st.top() << " but item still in place. Size is still " << st.size() << endl;
    st.pop();
    cout << "Pop the first item, size is now " << st.size() << endl;

    //############################################################

    cout << endl << "-> queue" << endl << endl;

    // Queue: First in / First out
    queue<float> q;
    q.push(1.2);
    q.push(3.3);
    q.push(5.6);
    cout << q;
    q.pop();
    cout << q;

    //############################################################

    cout << endl << "-> deque" << endl << endl;

    // Double-ended queue: a queue that you can access from both ends
    deque<int> dq(5, 3);
    cout << dq;
    dq.push_front(1);   // Insert before 1st item
    dq.push_back(9);    // Add at end
    dq.insert(dq.begin()+2, 4);     // Insert before 2nd position starting from beginning
    dq.insert(dq.end()-3, 7);       // Insert before 3rd position starting from the end
    cout << dq;

    //############################################################

    cout << endl << "-> set" << endl << endl;

    // Sets can't contain duplicate items
    set<double> s;
    s.emplace(2.1);
    s.emplace(3.2);
    s.emplace(2.1);
    cout << s;

    //############################################################

    cout << endl << "-> map" << endl << endl;

    // Each item can be accessed using its key. Key can be of any type and must be unique.
    map<string, int> m;
    m["foo"] = 3;
    m["bar"] = 5;
    cout << m;

    cout << "Searching for a given key:" << endl;
    auto it {m.find("foo")};
    if (it != m.end())
        cout << it->second << endl;
    else
        cout << "Item not found" << endl;

    cout << "Counting occurences of a given key:" << endl;
    cout << m.count("baz") << endl;



    return 0;
}
