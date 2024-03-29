#include "utility_functions.h"
#include <random>

long long Generate_Unique_ID(){
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<long long> distribution(1LL, (1LL<<62));

    return distribution(gen);
}

void Send_Init_Reply(json_object& incoming_message){

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

void Send_Broadcast_Reply(json_object& incoming_message, json_object& value_store){

    json_object broadcast_reply;
    broadcast_reply["src"] = incoming_message["dest"];
    broadcast_reply["dest"] = incoming_message["src"];
    broadcast_reply["body"] = new json_object();
    std::get<json_object*>(broadcast_reply["body"])->operator[]("id") = Generate_Unique_ID();
    std::get<json_object*>(broadcast_reply["body"])->operator[]("type") = "broadcast_ok";
    std::get<json_object*>(broadcast_reply["body"])->operator[]("in_reply_to") = std::get<long long>(std::get<json_object*>(incoming_message["body"])->operator[]("msg_id"));

    value_store.list_arr.push_back(std::get<json_object*>(incoming_message["body"])->operator[]("message"));

    std::cout<<broadcast_reply.json_string()<<std::endl;
}

void Send_Read_Reply(json_object& incoming_message, json_object& value_store){

    json_object read_reply;
    read_reply["src"] = incoming_message["dest"];
    read_reply["dest"] = incoming_message["src"];
    read_reply["body"] = new json_object();
    std::get<json_object*>(read_reply["body"])->operator[]("type") = "read_ok";
    std::get<json_object*>(read_reply["id"])->operator[]("id") = Generate_Unique_ID();
    std::get<json_object*>(read_reply["body"])->operator[]("in_reply_to") = std::get<long long>(std::get<json_object*>(incoming_message["body"])->operator[]("msg_id"));
    std::get<json_object*>(read_reply["body"])->operator[]("messages") = &value_store;

    std::cout<<read_reply.json_string()<<std::endl;
    std::get<json_object*>(read_reply["body"])->operator[]("messages") = nullptr;
}