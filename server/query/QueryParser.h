#pragma once

#include <string>
#include <future>
#include "queue/QueueManager.h"
#include "json/json.hpp"
#include "commands/ICommand.h"

struct QueryParser {
    using json = nlohmann::json;

    QueryParser() = default;

    json operator()(const std::string &raw_data, std::unique_ptr<ICommand>& command);


    QueryParser(const QueryParser &) = delete;
    void operator=(const QueryParser &) = delete;

};
