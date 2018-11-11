#include <utility>

#pragma once

#include "ICommand.h"

namespace commands {

    struct GetQueueList : public ICommand {
        GetQueueList() :
                ICommand() {};

        result_t execute() override {
            return queue_manager_.get_list();
        }
    };

    struct CreateQueue : public ICommand {
        explicit CreateQueue(std::string queue_name) :
                ICommand()
                , queue_name_{std::move(queue_name)} {};

        result_t execute() override {
            return queue_manager_.create_queue(queue_name_);
        }

    private:
        std::string queue_name_;
    };

    struct DestroyQueue : public ICommand {
        explicit DestroyQueue(std::string queue_name) :
                ICommand()
                , queue_name_{std::move(queue_name)} {};

        result_t execute() override {
            return queue_manager_.destroy_queue(queue_name_);
        }

    private:
        std::string queue_name_;
    };

    struct AddMessage : public ICommand {
        explicit AddMessage(std::string queue_name, json js) :
                ICommand()
                , queue_name_{std::move(queue_name)}
                , js_{std::move(js)} {};

        result_t execute() override {
            return queue_manager_.add_message(queue_name_, js_);
        }

    private:
        std::string queue_name_;
        json        js_;
    };

    struct GetMessage : public ICommand {
        explicit GetMessage(std::string queue_name) :
                ICommand()
                , queue_name_{std::move(queue_name)} {};

        result_t execute() override {
            return queue_manager_.get_message(queue_name_);
        }

    private:
        std::string queue_name_;
    };

    struct ConfirmMessage : public ICommand {
        explicit ConfirmMessage(std::string queue_name, size_t id) :
                ICommand()
                , queue_name_{std::move(queue_name)}
                , id_{id} {};

        result_t execute() override {
            return queue_manager_.confirm_message(queue_name_, id_);
        }

    private:
        std::string queue_name_;
        size_t      id_;
    };
}