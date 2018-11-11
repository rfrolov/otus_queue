#pragma once

#include "json/json.hpp"
#include "queue/QueueManager.h"
#include <memory>

struct ICommand {
    using json     = nlohmann::json;
    using result_t = std::future<json>;

    virtual ~ICommand() = default;
    virtual result_t execute() = 0;

protected:
    explicit ICommand() :
            queue_manager_{QueueManager::get_instance()} {}

    QueueManager &queue_manager_;
};
