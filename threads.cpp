#include <chrono>
#include <exception>
#include <functional>
#include <future>
#include <iostream>
#include <semaphore>
#include <sstream>
#include <thread>
#include <type_traits>

#define NOT_USED(expr) (void)(expr)

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
std::string threadId()
{
    stringstream st;
    st << "Thread #" << std::hex << this_thread::get_id() << std::dec;
    return st.str();
}

/// Basic method that will loop a certain number of times, each loop
/// is only a sleep instruction.
template <typename T>
typename enable_if<is_duration<T>::value, void>::type
loopSleep(T delay, int count)
{
    cout << threadId() << " started - will loop " << count << " times" << endl;

    while (count--) {
        this_thread::sleep_for(delay);
        cout << threadId() << ": " << count << " remaining loops" << endl;
    }

    cout << threadId() << " ended" << endl;
}

/// Will call a function on a variable (using reference)
template <typename T1, typename T2>
typename enable_if<is_duration<T1>::value, void>::type
applyFunctionOnVar(T1 delay, int count, function<void(T2&)> f, T2& a)
{
    while (count--) {
        f(a);
        this_thread::sleep_for(delay);
    }
}

/// This function releases the synchro semaphore every second during 5s
/// Once finished, it releases the exit semaphore to indicate other thread to exit.
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

/// Fulfills (or not) promises after 2s delay
/// - set value for 1st promise
/// - explicitly raise exception for 2nd promise
/// - doesn't satisfy 3rd promise
void fulfillPromises(std::promise<int> prom1, std::promise<int> prom2, std::promise<int> prom3)
{
    NOT_USED(prom3);

    cout << threadId() << " Entering thread, wait for 2s" << endl;
    std::this_thread::sleep_for(2s);
    cout << "Fulfill promises -> end of thread" << endl;
    prom1.set_value(42);
    prom2.set_exception(std::make_exception_ptr(std::runtime_error("Promise #2 failed")));
}

int longComputation()
{
    this_thread::sleep_for(1000ms);
    return 3;
}

void incr(int& a)
{
    a++;
    cout << threadId() << ": incr: " << a << endl;
}

void decr(int& a)
{
    a--;
    cout << threadId() << ": decr: " << a << endl;
}

// MAIN
//======
int main()
{
    cout << "Start main thread #" << std::hex << this_thread::get_id() << std::dec << endl;

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
    thread thread_inc { applyFunctionOnVar<chrono::milliseconds, int>, 100ms, 5, incr, ref(a) };
    thread thread_dec { applyFunctionOnVar<chrono::milliseconds, int>, 100ms, 5, decr, ref(a) };
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

    cout << endl;
    cout << "Promises" << endl;
    cout << "========" << endl;
    cout << "Promise/future is a way to indicate that a value will later be made available." << endl;
    cout << "The promise is the objet to push value when evaluated." << endl;
    cout << "The future is the object where the value is retrieved. future.get is blocking until the promise is fulfilled." << endl
         << endl;

    // TODO diff between thread and async

    std::promise<int> satisfiedPromise;
    std::promise<int> exceptionPromise;
    std::promise<int> brokenPromise;
    std::future<int> satisfiedFuture { satisfiedPromise.get_future() };
    std::future<int> exceptionFuture { exceptionPromise.get_future() };
    std::future<int> brokenFuture { brokenPromise.get_future() };

    std::thread thread_future { fulfillPromises, move(satisfiedPromise), move(exceptionPromise), move(brokenPromise) };

    cout << "Thread started: waiting for futures:" << endl;
    {
        auto val { satisfiedFuture.get() };
        cout << "Promise is fulfilled. Future is able to retrieve the value and unblocks" << endl;
        cout << "- satified future -> value: " << val << endl;
    }

    try {
        cout << "Program explicitely raises an exception through the promise. Promise is not fulfilled and exception reaches the future." << endl;
        auto val { exceptionFuture.get() };
        NOT_USED(val);
    } catch (runtime_error e) {
        cout << "- exception future: " << e.what() << endl;
    }

    try {
        cout << "Promse is never fulfilled and destroyed. Future gets a 'broken promise' exception" << endl;
        auto val { brokenFuture.get() };
        NOT_USED(val);
    } catch (future_error e) {
        cout << "- broken promise: " << e.what() << endl;
    }

    thread_future.join(); // Necesary to synch with end of started thread
    cout << endl;

    cout << "Asynchronous functions" << endl;
    cout << "======================" << endl;
    cout << "Asynchronous functions are useful for computations that may take time." << endl;
    cout << "There is no guarantee that it will execute in its own thread" << endl;
    cout << "async will return its result through a 'future' object. future.get will block until method" << endl;
    cout << "called through async returns." << endl
         << endl;

    cout << "Start long computation asynchronously" << endl;
    auto var { std::async(longComputation) };
    cout << "Waiting for asynchronous computation to complete to retrieve result" << endl;
    auto val { var.get() };
    cout << "Asynchronous computation ended. Retrieved value: " << val << endl;

    return 0;
}
