#include "QueryParser.h"
#include "json/json.hpp"

std::string QueryParser::operator()(const std::string &data, future_result_t &future_result) {
    using json = nlohmann::json;

    json js;

    std::string result{};
    try {
        js = json::parse(data);
    } catch (const json::exception &e) {
        return e.what();
    }

    if (js["cmd"].is_null()) {
        return "Error: unknown command";
    }

    std::string cmd;
    try {
        cmd = js["cmd"].get<std::string>();
    } catch (const json::exception &e) {
        return e.what();
    }

    if (cmd == "get_queue_list") {
        return "OK";
    } else if (cmd == "create_queue") {
        return "OK";
    } else if (cmd == "set_message") {
        return "OK";
    } else if (cmd == "get_message") {
        return "OK";
    } else if (cmd == "confirm_message") {
        return "OK";
    } else {
        return "Error: unknown command";
    }
}
