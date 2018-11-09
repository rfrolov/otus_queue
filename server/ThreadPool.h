#pragma once

#include <queue>
#include <thread>
#include <condition_variable>
#include <functional>
#include <future>

/// Класс пула потоков.
struct ThreadPool {
    using func_t = std::function<void()>;

    explicit ThreadPool(size_t threads_num) :
            m_max_queue_size{threads_num * 3} {
        m_workers.reserve(threads_num);

        for (size_t i = 0; i < threads_num; ++i) {
            m_workers.emplace_back(
                    [this, i] {
                        for (;;) {
                            func_t task;
                            {
                                std::unique_lock<std::mutex> lock(m_queue_mutex);
                                m_condition.wait(lock, [this] { return m_stop || !m_task_queue.empty(); });
                                if (m_stop && m_task_queue.empty()) {
                                    return;
                                }
                                task = std::move(m_task_queue.front());
                                m_task_queue.pop();
                            }

                            task();
                        }
                    }
            );
        }
    }

    template<typename F, typename... Args>
    auto enqueue(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
        using return_type = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<return_type()>>(
                std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> res = task->get_future();

        for (;;) {
            std::unique_lock<std::mutex> lock(m_queue_mutex);

            if (m_stop) {
                throw std::runtime_error("enqueue on stopped ThreadPool");
            }

            if (m_task_queue.size() < m_max_queue_size) {
                m_task_queue.emplace([task]() { (*task)(); });
                break;
            }
            std::this_thread::yield();
        }
        m_condition.notify_one();
        return res;
    }


    void finish() {
        {
            std::unique_lock<std::mutex> lock(m_queue_mutex);
            m_stop = true;
        }
        m_condition.notify_all();
        for (std::thread &worker: m_workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    /// Деструктор.
    ~ThreadPool() {
        finish();
    }


private:
    std::vector<std::thread> m_workers{};
    std::mutex               m_queue_mutex{};
    std::condition_variable  m_condition{};
    std::queue<func_t>       m_task_queue{};
    bool                     m_stop{};
    size_t                   m_max_queue_size;
};
