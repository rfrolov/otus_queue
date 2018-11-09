#pragma once

#include <string>
#include <future>
#include "QueueManager.h"

struct QueryParser {
    using future_result_t = std::future<std::string>;

    std::string operator()(const std::string &data, future_result_t &future_result);

    QueryParser() = default;
    QueryParser(const QueryParser &) = delete;
    void operator=(const QueryParser &) = delete;


private:
    QueueManager m_queue;
};
