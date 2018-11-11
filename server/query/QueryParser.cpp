#include <memory>

#include <commands/commands.hpp>
#include "QueryParser.h"
#include "json/json.hpp"

QueryParser::json QueryParser::operator()(const std::string &raw_data, std::unique_ptr<ICommand> &command) {
    using json = nlohmann::json;

    json data_js;
    try {
        data_js = json::parse(raw_data);
    } catch (const json::exception &e) {
        json js;
        js["error"] = e.what();
        return js;
    }

    if (data_js["cmd"].is_null()) {
        json js;
        js["error"] = "unknown command";
        return js;
    }

    std::string cmd;
    try {
        cmd = data_js["cmd"].get<std::string>();
    } catch (const json::exception &e) {
        json js;
        js["error"] = e.what();
        return js;
    }

    if (cmd == "get_queue_list") {
        command = std::make_unique<commands::GetQueueList>();
        return {};
    } else if (cmd == "create_queue") {
        if (data_js["name"].is_null() || !data_js["name"].is_string()) {
            json js;
            js["error"] = R"(command "create_queue" must have string parameter "name")";
            return js;
        }
        command = std::make_unique<commands::CreateQueue>(data_js["name"].get<std::string>());
        return {};

    } else if (cmd == "destroy_queue") {
        if (data_js["name"].is_null() || !data_js["name"].is_string()) {
            json js;
            js["error"] = R"(command "destroy_queue" must have string parameter "name")";
            return js;
        }
        command = std::make_unique<commands::DestroyQueue>(data_js["name"].get<std::string>());
        return {};

    } else if (cmd == "add_message") {
        if (data_js["name"].is_null() || !data_js["name"].is_string()) {
            json js;
            js["error"] = R"(command "add_message" must have string parameter "name")";
            return js;
        }
        if (data_js["message"].is_null()) {
            json js;
            js["error"] = R"(command "add_message" must have json parameter "message")";
            return js;
        }
        command = std::make_unique<commands::AddMessage>(data_js["name"].get<std::string>(), data_js["message"]);
        return {};

    } else if (cmd == "get_message") {
        if (data_js["name"].is_null() || !data_js["name"].is_string()) {
            json js;
            js["error"] = R"(command "get_message" must have string parameter "name")";
            return js;
        }
        command = std::make_unique<commands::GetMessage>(data_js["name"].get<std::string>());
        return {};

    } else if (cmd == "confirm_message") {
        if (data_js["name"].is_null() || !data_js["name"].is_string()) {
            json js;
            js["error"] = R"(command "confirm_message" must have string parameter "name")";
            return js;
        }
        if (data_js["id"].is_null() || !data_js["id"].is_number()) {
            json js;
            js["error"] = R"(command "confirm_message" must have numeric parameter "id")";
            return js;
        }
        command = std::make_unique<commands::ConfirmMessage>(data_js["name"].get<std::string>(),
                                                         data_js["id"].get<size_t>());
        return {};

    } else {
        json js;
        js["error"] = "unknown command";
        return js;
    }
}

//{"cmd":"create_queue","name":"queue1"}
//{"cmd":"add_message","name":"queue1", "message":{"data":"123"}}
//{"cmd":"get_message","name":"queue1"}
//{"cmd":"confirm_message","name":"queue1","id":"1"}