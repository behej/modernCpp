#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>





// MAIN
//======
int main()
{
    int a {0};
    int b {0};
    std::vector<int> v {1, 2, 3};

    // Basic lambda function
    //======================
    // Simply get input variable and print it
    std::for_each(std::begin(v), std::end(v), [](int i){std::cout << i << ' ';});
    std::cout << std::endl;

    // Capture all variables by value
    // this lambda can't modify 'a'
    std::for_each(std::begin(v), std::end(v), [=](int i){std::cout << a << "/" << i << ' ';});
    std::cout << std::endl;

    // Capture variables by reference
    // Lambda can modify 'a'
    std::for_each(std::begin(v), std::end(v), [&](int i){a+=i;});
    std::cout << "a=" << a << std::endl;

    // Lambda can modify 'a' but can't modify 'b'
    std::for_each(std::begin(v), std::end(v), [&a, b](int i){a+=i;});

    // Lambda can modify everything but 'b'
    std::for_each(std::begin(v), std::end(v), [&, b](int i){a+=i;});


    // Named lambda function
    //======================
    // auto square = [](int in) -> int  {return in*in;};        // Old syntax when mandatory to indicate return type with the arrow
    auto square = [](auto in) {return in*in;};                  // New syntax where return type is automatically determined by the compiler
                                                                // Also note the 'auto' keyword for the input var, which allow us to use the lambda
                                                                // with any type (provided it supports the multiply operation)

    std::transform(std::begin(v), std::end(v), std::begin(v), square);


    // Templatized lambdas
    //====================

    // We may want to write such lambdas to execute with vectors of any type.
    // But it is also possible to call it with any type, for example simple integer
    // and in that last case, compilation will fail.
    auto lambda_sum1 = [](auto v) {
        int s = {0};
        for (auto i : v) {
            s += i;
        }
        return s;
    };

    // Templatized lambdas come to the rescue.
    // With following syntax, it is only possible to call it with vectors
    auto lambda_sum2 = []<typename T>(std::vector<T> v) {
        int s = {0};
        for (auto i : v) {
            s += i;
        }
        return s;
    };

    auto sum1 = std::invoke(lambda_sum1, v);
    auto sum2 = std::invoke(lambda_sum2, v);

    return 0;
}
