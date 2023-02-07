#include <iostream>
#include <list>
#include <array>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <set>
#include <map>
#include <numeric>
#include <iterator>

using namespace std;

template<class T>
void printContainer(const T& container)
{
    cout << "Size: " << container.size() << endl;

    cout << "Items: ";
    for (const auto item : container)
        cout << item << " ";
    cout << endl;
}

/**
 * @brief Print size and items of containers of type 'queue'.
 * It is not possible to iterate over queue. So local copy is created.
 * Each item is accessed using 'front' and is removed with 'pop' to access next item.
 * Sequence is repeated until queue is empty
 */
template<class T>
void printContainer(queue<T> queue)
{
    cout << "Size: " << queue.size() << endl;

    cout << "Items: ";
    while (!queue.empty()) {
        cout << queue.front() << " ";
        queue.pop();
    }
    cout << endl;
}

template<class K, class V>
void printContainer(map<K, V> m)
{
    cout << "Size: " << m.size() << endl;

    cout << "Items: ";
    for (const auto item : m)
        cout << item.first << "=" << item.second << " ";
    cout << endl;
}


int main()
{
    cout << "Several types of container" << endl;
    cout << "==========================" << endl;    
    cout << "-> vector" << endl << endl;

    // vector is a collection of items stored in contiguous memory location.
    // + can access any item from the vector
    // + no memory overhead
    // - adding a new element may imply to relocate the entire vector
    // - inserting an item is costly
    // - memory is pre-allocated (there may be waste)

    vector<double> vec(5, 1.2);
    printContainer(vec);
    cout << "First item=" << vec.front() << " or " << vec[0] << endl;
    vec.resize(8, 3.3);
    printContainer(vec);

    //############################################################
    cout << endl << "-> list" << endl << endl;

    // Lists are similar to vector. Difference is that memory is not contiguous
    // + easy insert or delete item, even in the middle of the list
    // + memory not pre-allocated (you pay what you need)
    // - hard to access any item from the list
    // - memory overhead (pointers to next and previous items)

    list<int> l(3, 2);      // Create liste of fixed size with default value
    printContainer(l);
    l.push_back(5);         // Add item to end of list
    printContainer(l);
    l.erase(l.begin()); // Remove list item
    printContainer(l);

    //############################################################

    cout << endl << "-> array" << endl << endl;

    array<int, 3> ar;
    printContainer(ar);
    ar.fill(2);
    printContainer(ar);

    //############################################################

    cout << endl << "-> stack" << endl << endl;

    stack<int> st;
    cout << "Stack just created. Is it empty ? " << (st.empty() ? "yes" : "no") << endl;
    st.push(3);
    st.push(5);
    cout << "Pushed 2 items. Size is now " << st.size() << endl;
    cout << "Top item is " << st.top() << " but item still in place. Size is still " << st.size() << endl;
    st.pop();
    cout << "Pop the first item, size is now " << st.size() << endl;

    //############################################################

    cout << endl << "-> queue" << endl << endl;

    queue<float> q;
    q.push(1.2);
    q.push(3.3);
    q.push(5.6);
    printContainer(q);
    q.pop();
    printContainer(q);

    //############################################################

    cout << endl << "-> deque" << endl << endl;

    deque<int> dq(5, 3);
    printContainer(dq);
    dq.push_front(1);   // Insert before 1st item
    dq.push_back(9);    // Add at end
    dq.insert(dq.begin()+2, 4);     // Insert before 2nd position starting from beginning
    dq.insert(dq.end()-3, 7);       // Insert before 3rd position starting from the end
    printContainer(dq);

    //############################################################

    cout << endl << "-> set" << endl << endl;

    // Sets can't contain duplicate items
    set<double> s;
    s.emplace(2.1);
    s.emplace(3.2);
    s.emplace(2.1);
    printContainer(s);

    //############################################################

    cout << endl << "-> map" << endl << endl;

    map<string, int> m;
    m["foo"] = 3;
    m["bar"] = 5;
    printContainer(m);

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