#include "QueueManager.h"

auto QueueManager::find_queue(const std::string &name) {
    return std::find_if(queues_.begin(), queues_.end(), [&name](auto &it) { return it.get_name() == name; });
}

std::future<QueueManager::json> QueueManager::get_list() {
    std::promise<json> promise;
    json               js{};

    js["is_ok"]      = true;
    js["queue_list"] = json{};

    for (auto &queue:queues_) {
        js["queue_list"].emplace_back(queue.get_name());
    }

    promise.set_value(js);
    return promise.get_future();
}

std::future<QueueManager::json> QueueManager::create_queue(const std::string &queue_name) {
    std::promise<json> promise;
    json               js{};
    if (queues_.size() >= queues_limit) {
        js["is_ok"] = false;
        js["error"] = "queue limit";
    } else {
        if (find_queue(queue_name) != queues_.end()) {
            js["is_ok"] = false;
            js["error"] = "queue already exist";
        } else {
            js["is_ok"] = true;
            queues_.emplace_back(queue_name);
        }
    }

    promise.set_value(js);
    return promise.get_future();
}

std::future<QueueManager::json> QueueManager::destroy_queue(const std::string &queue_name) {
    std::promise<json> promise;
    json               js{};
    auto               queue = find_queue(queue_name);

    if (queue == queues_.end()) {
        js["is_ok"] = false;
        js["error"] = "the queue doesn't exist";
    } else {
        queues_.erase(queue);
        js["is_ok"] = true;
    }

    promise.set_value(js);
    return promise.get_future();
}

std::future<QueueManager::json> QueueManager::add_message(const std::string &queue_name, const json &data) {
    std::promise<json> promise;
    json               js{};
    auto               queue = find_queue(queue_name);

    if (queue == queues_.end()) {
        js["is_ok"] = false;
        js["error"] = "the queue doesn't exist";
    } else {
        if (!queue->push(data)) {
            js["is_ok"] = false;
            js["error"] = "can't add message";
        } else {
            js["is_ok"] = true;
        }
    }

    promise.set_value(js);
    return promise.get_future();
}

std::future<QueueManager::json> QueueManager::get_message(const std::string &queue_name) {
    std::promise<json> promise;
    json               js{};
    auto               queue = find_queue(queue_name);

    if (queue == queues_.end()) {
        js["is_ok"] = false;
        js["error"] = "the queue doesn't exist";
    } else {
        json result{};
        if (!queue->peek(result)) {
            js["is_ok"] = false;
            js["error"] = "can't find message";
        } else {
            js["is_ok"] = true;
            js["message"]  = result;
        }
    }

    promise.set_value(js);
    return promise.get_future();
}

std::future<QueueManager::json> QueueManager::confirm_message(const std::string &queue_name, size_t id) {
    std::promise<json> promise;
    json               js{};
    auto               queue = find_queue(queue_name);

    if (queue == queues_.end()) {
        js["is_ok"] = false;
        js["error"] = "the queue doesn't exist";
    } else {
        json result{};
        if (!queue->pop(id)) {
            js["is_ok"] = false;
            js["error"] = "can't find message";
        } else {
            js["is_ok"] = true;
        }
    }

    promise.set_value(js);
    return promise.get_future();
}


