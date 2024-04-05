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
    std::queue<std::pair<std::chrono::high_resolution_clock::time_point, long long>> message_queue;
    std::vector<std::string> neighboring_nodes;
    json_object value_store;

    node_info(){
        value_store.list_flag = true;
    }
};



#endif //GG_BIN_NODE_INFO_H
