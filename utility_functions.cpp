#include "utility_functions.h"
#include <random>

long long Generate_Unique_ID(){
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<long long> distribution(1LL, (1LL<<62));

    return distribution(gen);
}

void Send_Init_Reply(json_object& incoming_message, node_info& node){

    json_object init_reply;
    init_reply["src"] = incoming_message["dest"];
    init_reply["dest"] = incoming_message["src"];
    init_reply["body"] = new json_object();
    std::get<json_object*>(init_reply["body"])->operator[]("in_reply_to") = std::get<long long>(std::get<json_object*>(incoming_message["body"])->operator[]("msg_id"));
    std::get<json_object*>(init_reply["body"])->operator[]("msg_id") = Generate_Unique_ID();
    std::get<json_object*>(init_reply["body"])->operator[]("type") = "init_ok";
    std::cout<<init_reply.json_string()<<std::endl;
}

void Send_Echo_Reply(json_object& incoming_message){

    json_object echo_reply;
    echo_reply["src"] = incoming_message["dest"];
    echo_reply["dest"] = incoming_message["src"];
    echo_reply["body"] = new json_object();
    std::get<json_object*>(echo_reply["body"])->operator[]("msg_id") = Generate_Unique_ID();
    std::get<json_object*>(echo_reply["body"])->operator[]("in_reply_to") = std::get<long long>(std::get<json_object*>(incoming_message["body"])->operator[]("msg_id"));
    std::get<json_object*>(echo_reply["body"])->operator[]("type") = "echo_ok";
    std::get<json_object*>(echo_reply["body"])->operator[]("echo") = std::get<std::string>(std::get<json_object*>(incoming_message["body"])->operator[]("echo"));

    std::cout<<echo_reply.json_string()<<std::endl;
}

void Send_Generate_Reply(json_object& incoming_message){

    json_object generate_reply;
    generate_reply["src"] = incoming_message["dest"];
    generate_reply["dest"] = incoming_message["src"];
    generate_reply["body"] = new json_object();
    std::get<json_object*>(generate_reply["body"])->operator[]("id") = Generate_Unique_ID();
    std::get<json_object*>(generate_reply["body"])->operator[]("type") = "generate_ok";
    std::get<json_object*>(generate_reply["body"])->operator[]("in_reply_to") = std::get<long long>(std::get<json_object*>(incoming_message["body"])->operator[]("msg_id"));

    std::cout<<generate_reply.json_string()<<std::endl;
}

void Send_Broadcast_Reply(json_object& incoming_message, node_info& node){

    if(node.messages_received.find(std::get<long long>(std::get<json_object*>(incoming_message["body"])->operator[]("msg_id"))) != node.messages_received.end()){
        std::cerr<<"Hit messages_received condition "<<incoming_message.json_string()<<std::endl;
        return;
    }

    json_object broadcast_ok_reply;
    broadcast_ok_reply["src"] = incoming_message["dest"];
    broadcast_ok_reply["dest"] = incoming_message["src"];
    broadcast_ok_reply["body"] = new json_object();
    std::get<json_object*>(broadcast_ok_reply["body"])->operator[]("msg_id") = Generate_Unique_ID();
    std::get<json_object*>(broadcast_ok_reply["body"])->operator[]("type") = "broadcast_ok";
    std::get<json_object*>(broadcast_ok_reply["body"])->operator[]("in_reply_to") = std::get<long long>(std::get<json_object*>(incoming_message["body"])->operator[]("msg_id"));

    node.messages_received.insert(std::get<long long>(std::get<json_object*>(incoming_message["body"])->operator[]("msg_id")));
    node.value_store.list_arr.push_back(std::get<json_object*>(incoming_message["body"])->operator[]("message"));

    if(std::get<std::string>(incoming_message["src"])[0] == 'n'){
        std::cout<<broadcast_ok_reply.json_string()<<std::endl;
        return;
    }

    json_object broadcast_message;
    broadcast_message["src"] = broadcast_ok_reply["src"];
    broadcast_message["body"] = new json_object();
    std::get<json_object*>(broadcast_message["body"])->operator[]("type") = "broadcast";
    std::get<json_object*>(broadcast_message["body"])->operator[]("message") = std::get<json_object*>(incoming_message["body"])->operator[]("message");

    for(int i = 0; i<node.neighboring_nodes.size(); i++){

        if(node.neighboring_nodes[i] == std::get<std::string>(incoming_message["src"])){
            continue;
        }

        broadcast_message["dest"] = node.neighboring_nodes[i];
        std::get<json_object*>(broadcast_message["body"])->operator[]("msg_id") = Generate_Unique_ID();
        node.message_ids_broadcasts_sent_map[std::get<long long>(std::get<json_object*>(broadcast_message["body"])->operator[]("msg_id"))] = broadcast_message.json_string();
        std::cout<<node.message_ids_broadcasts_sent_map[std::get<long long>(std::get<json_object*>(broadcast_message["body"])->operator[]("msg_id"))]<<std::endl;
    }

    std::cout << broadcast_ok_reply.json_string() << std::endl;
}

void Send_Read_Reply(json_object& incoming_message, node_info& node){

    json_object read_reply;
    read_reply["src"] = incoming_message["dest"];
    read_reply["dest"] = incoming_message["src"];
    read_reply["body"] = new json_object();
    std::get<json_object*>(read_reply["body"])->operator[]("type") = "read_ok";
    std::get<json_object*>(read_reply["body"])->operator[]("msg_id") = Generate_Unique_ID();
    std::get<json_object*>(read_reply["body"])->operator[]("in_reply_to") = std::get<long long>(std::get<json_object*>(incoming_message["body"])->operator[]("msg_id"));
    std::get<json_object*>(read_reply["body"])->operator[]("messages") = &node.value_store;

    std::cout<<read_reply.json_string()<<std::endl;
    std::get<json_object*>(read_reply["body"])->operator[]("messages") = nullptr;
}

void Send_Topology_Reply(json_object& incoming_message, node_info& node){

    json_object topology_reply;
    topology_reply["src"] = incoming_message["dest"];
    topology_reply["dest"] = incoming_message["src"];
    topology_reply["body"] = new json_object();
    std::get<json_object*>(topology_reply["body"])->operator[]("type") = "topology_ok";
    std::get<json_object*>(topology_reply["body"])->operator[]("msg_id") = Generate_Unique_ID();
    std::get<json_object*>(topology_reply["body"])->operator[]("in_reply_to") = std::get<long long>(std::get<json_object*>(incoming_message["body"])->operator[]("msg_id"));

    for(auto& key_value_pairs : std::get<json_object*>(std::get<json_object*>(incoming_message["body"])->operator[]("topology"))->key_value_map){
        if(key_value_pairs.first != std::get<std::string>(topology_reply["src"])) node.neighboring_nodes.push_back(key_value_pairs.first);
    }

    std::cout<<topology_reply.json_string()<<std::endl;
}

void Receive_Broadcast_Response(json_object& incoming_message, node_info& node){

    long long response_id = std::get<long long>(std::get<json_object*>(incoming_message["body"])->operator[]("in_reply_to"));

    if(node.message_ids_broadcasts_sent_map.find(response_id) == node.message_ids_broadcasts_sent_map.end()){
        std::cerr<<"Error: getting a message in response to an ID that was never sent"<<std::endl;
        return;
    }

    node.message_ids_broadcasts_sent_map.erase(response_id);
}