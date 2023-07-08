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
#include <numeric>
#include <algorithm>

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
    cout << vec << endl;
    cout << "First item=" << vec.front() << " or " << vec[0] << endl;
    vec.resize(8, 3.3);
    cout << vec << endl;

    //############################################################
    cout << endl << "-> list" << endl << endl;

    // Lists are similar to vectors. Difference is that memory is not contiguous
    // + easy insert or delete item, even in the middle of the list
    // + memory not pre-allocated (you pay what you need)
    // - hard to access any item from the list
    // - memory overhead (pointers to next and previous items)
    list<int> l(3, 2);      // Create liste of fixed size with default value
    cout << l << endl;
    l.push_back(5);         // Add item to end of list
    cout << l << endl;
    l.erase(l.begin());     // Remove list item
    cout << l << endl;

    //############################################################

    cout << endl << "-> array" << endl << endl;

    // Array can be similar to vector, to the difference that its size is static
    // and may be allocated on the stack (whereas vector is always on the heap).
    // Arrays are more limited than vectors but can be more efficient, especially
    // for small sizes.
    array<int, 3> ar;
    cout << ar << endl;
    ar.fill(2);
    cout << ar << endl;

    //############################################################

    cout << endl << "-> stack" << endl << endl;

    // Stack: First in / Last out
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
    cout << q << endl;
    q.pop();
    cout << q << endl;

    //############################################################

    cout << endl << "-> deque" << endl << endl;

    // Double-ended queue: a queue that you can access from both ends
    deque<int> dq(5, 3);
    cout << dq << endl;
    dq.push_front(1);   // Insert before 1st item
    dq.push_back(9);    // Add at end
    dq.insert(dq.begin()+2, 4);     // Insert before 2nd position starting from beginning
    dq.insert(dq.end()-3, 7);       // Insert before 3rd position starting from the end
    cout << dq << endl;

    //############################################################

    cout << endl << "-> set" << endl << endl;

    // Sets can't contain duplicate items
    set<double> s;
    s.emplace(2.1);
    s.emplace(3.2);
    s.emplace(2.1);
    cout << s << endl;

    //############################################################

    cout << endl << "-> map" << endl << endl;

    // Each item can be accessed using its key. Key can be of any type and must be unique.
    map<string, int> m;
    m["foo"] = 3;
    m["bar"] = 5;
    cout << m << endl;

    cout << "Searching for a given key:" << endl;
    auto it {m.find("foo")};
    if (it != m.end())
        cout << it->second << endl;
    else
        cout << "Item not found" << endl;

    cout << "Counting occurences of a given key:" << endl;
    cout << m.count("baz") << endl;


    //############################################################
    cout << endl << "Operations on container" << endl;
    cout << "=======================" << endl;
    cout << "Accumulate" << endl;
    cout << "----------" << endl;

    cout << "Accumulate: sum of each item of the container" << endl;
    cout << "-> accumulate (default is addition) = " << accumulate(vec.begin(), vec.end(), 0.0) << endl;
    cout << "But also possible to indicate operation to perform" << endl;
    cout << "-> accumulate with multiplication: " << accumulate(vec.begin(), vec.end(), 1.0, std::multiplies<>{}) << endl;

    cout << "It's also possible to tons of things with accumulate by giving any function" << endl;
    cout << "-> " << accumulate(vec.begin(), vec.end(), std::string(),
        [](const auto previous, const auto item){
        if (previous.empty())
                return std::to_string(item);
        else
                return previous + " " + std::to_string(item);
        }) << endl;
    cout << "same but with different implem:" << endl;
    cout << "-> " << accumulate(vec.begin()+1, vec.end(), std::to_string(vec.front()),
                [](const auto previous, const auto item){return previous + " " + std::to_string(item);}) << endl;

    //############################################################
    cout << endl << "Reduce (only since C++17)" << endl;
    cout         << "-------------------------" << endl;
    cout << "Reduce works similarly to accumulate with following differences:" << endl;
    cout << "- Initial value and return type of operation shall be of same type as iterable" << endl;
    cout << "- Operation can be executed in any order" << endl;
    cout << "-> " << reduce(vec.begin(), vec.end()) << endl;

    cout << endl << "Can be customized with any operation, provided, return value is the same type" << endl;
    cout << "Example to search max value in a container:" << endl;
    cout << reduce(vec.begin(), vec.end(), 0.0, [](auto prev, auto item){return max(prev, item);}) << endl;;
    cout << "Max value searched using STL function max_element:" << endl;
    cout << *max_element(vec.begin(), vec.end()) << endl;


    //############################################################
    cout << endl << "Other operations" << endl;
    cout << "----------------" << endl;
    cout << endl << "Here is a list" << endl;
    l.push_back(3);
    cout << l << endl;

    cout << endl << "Containers can be sorted, either using std::sort or built-in sort method" << endl;
    l.sort();
    cout << "sorted list: " << l << endl;

    cout << endl << "Containers can be reversed, either using std::reverse or built-in reverse method" << endl;
    l.reverse();
    cout << "Reversed list:" << l << endl;

    cout << endl << "Transform: apply any operation to " << endl;;
    transform(l.begin(), l.end(), l.begin(), [](auto in){return in*2;});
    cout << l << endl;

    cout << endl << "fill: fill all items with given value" << endl;
    std::list<double> l2(5);
    cout << l2 << endl;
    std::fill(l2.begin(), l2.end(), 3.0);
    cout << l2 << endl;
    cout << endl << "fill_n: fill n elements with given value" << endl;
    std::fill_n(l2.begin(), 2, 5.0);
    cout << l2 << endl;

    cout << endl << "find: return iterator to first item found equal to searched value" << endl;
    if (find(l2.begin(), l2.end(), 3.0) != l2.end())
        cout << "-> Item found" << endl;
    else
        cout << "Item not found";

    cout << endl << "find_if/find_if_not: return iterator to first item where operation returns true/false" << endl;
    if (find_if(l2.begin(), l2.end(), [](auto in){return ((int)in)%2;}) != l2.end())
        cout << "-> Item found" << endl;
    else
        cout << "Item not found";


    return 0;
}
