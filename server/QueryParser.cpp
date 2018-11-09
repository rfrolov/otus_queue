#include "QueryParser.h"
#include "json/json.hpp"

std::string QueryParser::operator()(const std::string &data, future_result_t &future_result) {
    using json = nlohmann::json;

    json data_js;
    try {
        data_js = json::parse(data);
    } catch (const json::exception &e) {
        json js;
        js["error"] = e.what();
        return js.dump();
    }

    if (data_js["cmd"].is_null()) {
        json js;
        js["error"] = "unknown command";
        return js.dump();
    }

    std::string cmd;
    try {
        cmd = data_js["cmd"].get<std::string>();
    } catch (const json::exception &e) {
        json js;
        js["error"] = e.what();
        return js.dump();
    }

    if (cmd == "get_queue_list") {
        json js;
        const auto list = m_queue.get_list();

        if(list.empty()) {
            js["queue_list"] = json{};
        } else {
            for (const auto &name:list) {
                js["queue_list"].emplace_back(name);
            }
        }
        return js.dump();
    } else if (cmd == "create_queue") {
//        if (data_js["data"].is_null()) {
//            json js;
//            js["error"] = R"(command "create_queue" must have parametr "data")";
//            return js.dump();
//        }
//        m_queue.create_queue();
        return "OK";
    } else if (cmd == "set_message") {
        return "OK";
    } else if (cmd == "get_message") {
        return "OK";
    } else if (cmd == "confirm_message") {
        return "OK";
    } else {
        json js;
        js["error"] = "unknown command";
        return js.dump();
    }
}
