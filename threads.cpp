#include <chrono>
#include <functional>
#include <iostream>
#include <semaphore>
#include <thread>
#include <type_traits>

using namespace std;

// meta programming
//=================
template <typename T>
struct is_duration : std::false_type {
};

template <typename Rep, typename Period>
struct is_duration<chrono::duration<Rep, Period>> : std::true_type {
};

// Global variables
//=================
counting_semaphore<1> synchroSemaphore { 0 }; // The parameter indicates the max value that may be reached by the semaphore
binary_semaphore exitSemaphore { 0 }; // binary semaphore is equivalent to counting_semaphore<1>

// Utility Functions
//===================
/// Basic method that will loop a certain number of times, each loop
/// is only a sleep instruction.
template <typename T>
typename enable_if<is_duration<T>::value, void>::type
loopSleep(T delay, int count)
{
    cout << "Thread #" << this_thread::get_id() << " started - will loop " << count << " times" << endl;

    while (count--) {
        this_thread::sleep_for(delay);
        cout << "Thread #" << this_thread::get_id() << ": " << count << " remaining loops" << endl;
    }

    cout << "Thread #" << this_thread::get_id() << " ended" << endl;
}

/// Will call a function on a variable (using reference)
template <typename T>
typename enable_if<is_duration<T>::value, void>::type
applyFunctionOnVar(T delay, int count, function<void(int&)> f, int& a)
{
    while (count--) {
        f(a);
        this_thread::sleep_for(delay);
    }
}

/// This function releases the synchro semaphore every second during 5s
/// Once finished, it releases the exit semaphore to indicate other thraead to exit.
void postingFunc()
{
    for (int i { 0 }; i < 5; i++) {
        sleep(1);
        cout << "Posting semaphore" << endl;
        synchroSemaphore.release();
    }
    exitSemaphore.release();
}

/// This function waits for the synchro semaphore
/// At each loop, it also checks the exit semaphore to detect if it has to stop.
void waitingFunc()
{
    auto start { chrono::high_resolution_clock::now() };
    while (!exitSemaphore.try_acquire()) {
        auto duration { chrono::high_resolution_clock::now() - start };
        cout << chrono::duration_cast<chrono::milliseconds>(duration).count() << "ms - Waiting" << endl;
        synchroSemaphore.acquire();
        duration = chrono::high_resolution_clock::now() - start;
        cout << chrono::duration_cast<chrono::milliseconds>(duration).count() << "ms - Got it !" << endl;
    }
    cout << "Exiting waiting thread" << endl;
}

void incr(int& a)
{
    a++;
    cout << "Thread #" << this_thread::get_id() << ": incr: " << a << endl;
}

void decr(int& a)
{
    a--;
    cout << "Thread #" << this_thread::get_id() << ": decr: " << a << endl;
}

// MAIN
//======
int main()
{
    cout << "Start main thread #" << this_thread::get_id() << endl;

    cout << endl;
    cout << "Basic threads: starting 2 threads that will run in parallel" << endl;
    cout << "===========================================================" << endl;
    // Starting 2 threads that will execute the same function, but with different parameters
    thread t1 { loopSleep<chrono::milliseconds>, 100ms, 5 };
    thread t2 { loopSleep<chrono::milliseconds>, 200ms, 3 };

    cout << "Waiting for all threads to finish" << endl;
    t1.join();
    t2.join();

    cout << "All threads have ended" << endl;
    cout << endl;

    cout << "Thread interference: Starting 2 threads that will modify the same variable simultaneously" << endl;
    cout << "=========================================================================================" << endl;
    cout << "Order is not predictable and not reproduceable." << endl;
    cout << "A thread can interrupt the other one." << endl;
    cout << "This means, if shared variable in not protected by a mutex, a thread can change the value" << endl;
    cout << "while the other thread is using it. This can lead to inconsistencies if value changes while beeing used." << endl;

    int a { 0 };
    thread thread_inc { applyFunctionOnVar<chrono::milliseconds>, 100ms, 5, incr, ref(a) };
    thread thread_dec { applyFunctionOnVar<chrono::milliseconds>, 100ms, 5, decr, ref(a) };
    thread_inc.join();
    thread_dec.join();
    cout << endl;

    cout << "Thread synchronisation" << endl;
    cout << "======================" << endl;
    cout << "One thread is constantly waiting for the semaphore to be released." << endl;
    cout << "The semaphore is released by the 2nd thread every second." << endl;
    cout << "It is important for the waiting thread to terminate to have a second semaphore" << endl;
    cout << "indicated whether the loop should end. That second semaphore is released by 2nd thread" << endl;
    cout << "when it finishes" << endl;
    thread thread_waiting { waitingFunc };
    thread thread_posting { postingFunc };
    thread_waiting.join();
    thread_posting.join();

    return 0;
}
