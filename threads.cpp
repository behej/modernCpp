#include <iostream>
#include <thread>
#include <type_traits>

using namespace std;

template <typename T>
struct is_duration {
    static const bool value = false;
};

template <typename Rep, typename Per>
struct is_duration<chrono::duration<Rep, Per>> {
    static const bool value = true;
};

template <typename T>
typename enable_if<is_duration<T>::value, void>::type
f1(int id, T delay, int count)
{
    cout << "F" << id << " started - will loop " << count << " times" << endl;

    while (count--) {
        this_thread::sleep_for(delay);
        cout << "F" << id << ": " << count << " remaining loops" << endl;
    }

    cout << "F1 ended" << endl;
}

int main()
{
    cout << "Start main" << endl;
    int threadCounter { 0 };
    thread t1 { f1<chrono::seconds>, threadCounter++, 1s, 5 };
    thread t2 { f1<chrono::seconds>, threadCounter++, 2s, 3 };

    cout << "Waiting for all thread to finish" << endl;
    t1.join();
    t2.join();

    cout << "End of main" << endl;
    return 0;
}
