#ifndef METAPROGRAMMING_H
#define METAPROGRAMMING_H

#include <vector>
#include <list>
#include <array>
#include <queue>
#include <deque>
#include <set>
#include <map>
#include <ostream>
#include <type_traits>


/*************************************
 * BASE PRINCIPLE
 * Templates are defined below. There are cases where a given template should be enabled or disabled.
 * Sometimes, template can be specialized (as done for map and queue) to match a given type because compiler
 * will pick the most specialized template first.
 * But sometimes, a template must be disable another way.
 * This other way is called SFINAE (Substitution Failure Is Not An Error).
 * Using metaprogramming instruction, compiler fails to substitute a template and then tries to pick another one.
 * Main instruction used is 'enable_if'
 *
 * ENABLE_IF
 * enable_if<bool cond, typename T = void>
 * if 'cond' is true, then enable_if has a member 'type' equal to T.
 * Type can then be used in function/method definition.
 * Condition can be buit using utility meta-programming functions from STL (such as is_base_of, is_integral,
 * is_array, etc.) or building our own templatized functions.
 *
 * IMPORTANT
 * enable_if and metaprogramming have been introduced with C++11
 * it's kinda obsolete since C++17 with the introduction of 'if constexpr'
 * it's even more obsolete since C++20 with the introduction of 'concepts'
 * **********************************/


// Default templatized metaprogramming function. Matches all types unless a more specialized function exists
// Returns false to discard function.
template<typename T>
struct is_type_container {
  static const bool value = false;
};

// Metaprogramming function specialized for vectors. Is templatized too to match vectors of any type.
// Returns true to enable function for vectors.
template<typename T>
struct is_type_container<std::vector<T>> {
  static const bool value = true;
};

// enable for lists
template<typename T>
struct is_type_container<std::list<T>> {
  static const bool value = true;
};

// enable for arrays
template<typename T, std::size_t n>
struct is_type_container<std::array<T, n>> {
  static const bool value = true;
};

// ennable for deque
template<typename T>
struct is_type_container<std::deque<T>> {
  static const bool value = true;
};


// enable for sets
template<typename T>
struct is_type_container<std::set<T>> {
  static const bool value = true;
};



/**
 * Operator<< is overloaded for containers. To match several containers
 * overload is templatized. But simply overloading operator<< for types T
 * also overrides basic implem for buitin types such as int, float, etc
 * that we still need.
 * In order to prevent that and limit overload to the only types we need,
 * operator<< overload is activated in certains conditions using SFINAE
 * (Substitution Failure Is Not An Error) with enable_if. If criteria is not
 * met, substitution with the template fails. Compiler then tries to find
 * another template that suceeds.
 * Criteria is given by a custom struct that we can specialize for each
 * typename to consider (see above).
 *
 * Note:
 * Similar behaviour could have been achieved using built-in metaprogramming
 * functions instead of building a custom one.
 * is_base_of<std::vector<T>, U>::value || is_base_of<std::list<T>, U>::value
 * In this case, that option would have been less readable and less maintanable.
 */
template<typename T>
typename std::enable_if<is_type_container<T>::value, std::ostream&>::type
operator<<(std::ostream& os, const T& container) {
    os << "Size: " << container.size() << std::endl;
    os << "Items: ";
    for (const auto item : container)
        os << item << " ";
    os << std::endl;
    return os;
}

/**
 * @brief Template specialization: Print size and items of containers of type 'queue'.
 * It is not possible to iterate over queue. So local copy is created (copy of parameters)
 * Each item is accessed using 'front' and is removed with 'pop' to access next item.
 * Sequence is repeated until queue is empty
 */
template<typename T,typename U>
std::ostream& operator<<(std::ostream& os, std::queue<T, U> container) {
    os << "Size: " << container.size() << std::endl;
    os << "Items: ";
    while (!container.empty()) {
        os << container.front() << " ";
        container.pop();
    }
    os << std::endl;

    return os;
}

/**
 * @brief Template specialization: Print all items of 'map'.
 */
template<typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::map<K, V> &m) {
    os << "Size: " << m.size() << std::endl;
    os << "Items: ";
    for (const auto item : m)
        os << item.first << "=" << item.second << " ";
    os << std::endl;
    return os;
}



#endif // METAPROGRAMMING_H
