#pragma once

#include "json/json.hpp"
#include <string>
#include <queue>

struct Queue {
    using json = nlohmann::json;

    explicit Queue(std::string name);

    std::string get_name();
    bool push(const json &data);
    size_t peek(json &data);
    bool pop(size_t id);

private:
    static const size_t messages_limit = 1000;

    std::string      name_;
    std::queue<json> messages_;
};

