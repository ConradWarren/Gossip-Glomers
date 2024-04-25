#ifndef GG_BIN_NODE_INFO_H
#define GG_BIN_NODE_INFO_H

#include "json_parser/json.h"
#include <queue>
#include <unordered_set>
#include <chrono>

class node_info{
public:
    std::unordered_map<long long, std::string> message_ids_broadcasts_sent_map;
    std::unordered_set<long long> messages_received;
    std::unordered_set<long long> value_store_set;
    std::queue<std::pair<std::chrono::high_resolution_clock::time_point, long long>> messages_to_control_node_queue;
    std::queue<std::pair<std::chrono::high_resolution_clock::time_point, long long>> messages_sent_as_control_node;
    std::unordered_map<long long, json_object*> message_id_json_object_map;

    std::chrono::high_resolution_clock::time_point message_buffer_time_stamp;
    std::vector<std::string> neighboring_nodes;
    json_object value_store;
    json_object broadcast_value_buffer;

    int node_count;
    int node_id;
    int current_control_node_id;

    node_info(){
        value_store.list_flag = true;
        broadcast_value_buffer.list_flag = true;
    }

    void handle_sent_messages();
};



#endif //GG_BIN_NODE_INFO_H
