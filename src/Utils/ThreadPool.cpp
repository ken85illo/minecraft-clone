#include "ThreadPool.hpp"

// NOTE: This is not entirely my own code
// See: https://www.geeksforgeeks.org/cpp/thread-pool-in-cpp/

ThreadPool::ThreadPool(size_t numOfThreads) {
    // Creating worker threads
    for(size_t i = 0; i < numOfThreads; ++i) {
        m_threads.emplace_back([this] {
            while(true) {
                std::function<void()> task;
                {
                    // Locking the queue so that data
                    // can be shared safely
                    std::unique_lock<std::mutex> lock(m_queueMutex);

                    // Waiting until there is a task to
                    // execute or the pool is stopped
                    m_cv.wait(lock, [this] { return !m_tasks.empty() || m_stop; });

                    // exit the thread in case the pool
                    // is stopped and there are no tasks
                    if(m_stop && m_tasks.empty())
                        return;

                    // Get the next task from the queue
                    task = std::move(m_tasks.front());
                    m_tasks.pop();
                    ++m_activeTasks;
                }

                task();

                {
                    std::unique_lock<std::mutex> lock(m_queueMutex);
                    --m_activeTasks;
                }
                m_cv.notify_all();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        // Lock the queue to update the stop flag safely
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_stop = true;
    }

    // Notify all threads
    m_cv.notify_all();

    // Joining all worker threads to ensure they have
    // completed their tasks
    for(auto& thread : m_threads)
        thread.join();
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_tasks.emplace(std::move(task));
    }
    m_cv.notify_one();
}

void ThreadPool::wait() {
    std::unique_lock<std::mutex> lock(m_queueMutex);
    m_cv.wait(lock, [this] { return m_tasks.empty() && m_activeTasks == 0; });
}
