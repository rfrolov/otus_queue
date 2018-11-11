#include "Queue.h"

Queue::Queue(std::string name) :
        name_{std::move(name)}
        , messages_{} {

}

std::string Queue::get_name() {
    return name_;
}

bool Queue::push(const json &data) {
    if(data.size() >= messages_limit) {
        return false;
    }

    messages_.emplace(data);
    return true;
}

size_t Queue::peek(json &data) {
    if(messages_.empty()) {
        return 0;
    }

    data = messages_.front();

    // TODO: Выдать id.
    return 1;
}

bool Queue::pop(size_t id) {
    //TODO: Проверить id.
    messages_.pop();
    return true;
}

