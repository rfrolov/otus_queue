#pragma once

#include <string>
#include <vector>

struct QueueManager {
    QueueManager() = default;

    std::vector<std::string> get_list();
    std::string create_queue(std::string queue_name);
//    void destroy_queue(std::string queue_name);
//    void add_event(std::string queue_name, std::string event);
//    void get_event(std::string queue_name);
//    void confirm_event(std::string queue_name, size_t event_id);

};


