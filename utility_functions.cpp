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
    node.node_id = std::stoi(std::get<std::string>(incoming_message["dest"]).substr(1));
    node.current_control_node_id = 0;
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

    json_object broadcast_reply;
    broadcast_reply["src"] = incoming_message["dest"];
    broadcast_reply["dest"] = incoming_message["src"];
    broadcast_reply["body"] = new json_object();
    std::get<json_object*>(broadcast_reply["body"])->operator[]("msg_id") = Generate_Unique_ID();
    std::get<json_object*>(broadcast_reply["body"])->operator[]("type") = "broadcast_ok";
    std::get<json_object*>(broadcast_reply["body"])->operator[]("in_reply_to") = std::get<long long>(std::get<json_object*>(incoming_message["body"])->operator[]("msg_id"));

    if(node.messages_received.find(std::get<long long>(std::get<json_object*>(incoming_message["body"])->operator[]("msg_id"))) != node.messages_received.end()){
        std::cout<<broadcast_reply.json_string()<<std::endl;
        return;
    }
    node.messages_received.insert(std::get<long long>(std::get<json_object*>(incoming_message["body"])->operator[]("msg_id")));

    //if message is a json object we send the broadcast_reply and put the info in out json_object value store.
    if(std::get_if<json_object*>(&(std::get<json_object*>(incoming_message["body"])->operator[]("message")))){

        for(auto &value : std::get<json_object*>(std::get<json_object*>(incoming_message["body"])->operator[]("message"))->list_arr){
            if(node.value_store_set.find(std::get<long long>(value)) != node.value_store_set.end()){
                continue;
            }
            node.value_store_set.insert(std::get<long long>(value));
            node.value_store.list_arr.push_back(value);
        }
        std::cout<<broadcast_reply.json_string()<<std::endl;
        return;
    }

    if(node.value_store_set.find(std::get<long long>(std::get<json_object*>(incoming_message["body"])->operator[]("message"))) == node.value_store_set.end()){
        node.value_store_set.insert(std::get<long long>(std::get<json_object*>(incoming_message["body"])->operator[]("message")));
        node.value_store.list_arr.push_back(std::get<json_object*>(incoming_message["body"])->operator[]("message"));
    }


    if(std::get<std::string>(incoming_message["src"])[0] == 'n' || node.current_control_node_id == node.node_id){

        if(node.broadcast_value_buffer.list_arr.empty()){
            node.message_buffer_time_stamp = std::chrono::high_resolution_clock::now();
        }
        node.broadcast_value_buffer.list_arr.push_back(std::get<json_object*>(incoming_message["body"])->operator[]("message"));
        std::cout<<broadcast_reply.json_string()<<std::endl;
        return;
    }

    //if it's a piece of data from a client we give it to the control node and put the value in our object store
    //send reply
    if(std::get<std::string>(incoming_message["src"])[0] == 'c'){

        long long unique_id = Generate_Unique_ID();
        json_object* broadcast_to_control_node = new json_object();
        broadcast_to_control_node->operator[]("src") = incoming_message["dest"];
        broadcast_to_control_node->operator[]("dest") = "n" + std::to_string(node.current_control_node_id);
        broadcast_to_control_node->operator[]("body") = new json_object();
        std::get<json_object*>(broadcast_to_control_node->operator[]("body"))->operator[]("type") = "broadcast";
        std::get<json_object*>(broadcast_to_control_node->operator[]("body"))->operator[]("message") = std::get<json_object*>(incoming_message["body"])->operator[]("message");
        std::get<json_object*>(broadcast_to_control_node->operator[]("body"))->operator[]("msg_id") = unique_id;

        std::cout<<broadcast_to_control_node->json_string()<<std::endl;
        std::cout<<broadcast_reply.json_string()<<std::endl;
        delete broadcast_to_control_node;
    }
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

    for(auto& connected_nodes : std::get<json_object*>(std::get<json_object*>(incoming_message["body"])->operator[]("topology"))->key_value_map){
        if(connected_nodes.first != std::get<std::string>(incoming_message["dest"])){
            node.neighboring_nodes.push_back(connected_nodes.first);
        }
        node.node_count++;
    }

    std::cout<<topology_reply.json_string()<<std::endl;
}

void Receive_Broadcast_Response(json_object& incoming_message, node_info& node){


}