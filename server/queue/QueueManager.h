#pragma once

#include "json/json.hpp"
#include "Queue.h"
#include <string>
#include <vector>
#include <future>

struct QueueManager {
    using json = nlohmann::json;


    static QueueManager &get_instance() {
        static QueueManager instance{};
        return instance;
    }

    QueueManager(const QueueManager &) = delete;
    QueueManager(const QueueManager &&) = delete;
    QueueManager &operator=(const QueueManager &) = delete;
    QueueManager &operator=(const QueueManager &&) = delete;


    std::future<json> get_list();
    std::future<json> create_queue(const std::string &queue_name);
    std::future<json> destroy_queue(const std::string &queue_name);
    std::future<json> add_message(const std::string &queue_name, const json &data);
    std::future<json> get_message(const std::string &queue_name);
    std::future<json> confirm_message(const std::string &queue_name, size_t id);

private:
    QueueManager() = default;
    auto find_queue(const std::string &name);

    static const size_t queues_limit = 10;
    std::vector<Queue> queues_{};
};


