#include "utility_functions.h"

void Send_Init_Reply(json_object& incoming_message, int message_id){

    json_object init_reply;
    init_reply.list_flag = false;
    init_reply["src"] = incoming_message["dest"];
    init_reply["dest"] = incoming_message["src"];
    init_reply["body"] = new json_object();
    std::get<json_object*>(init_reply["body"])->list_flag = false;
    std::get<json_object*>(init_reply["body"])->operator[]("in_reply_to") = std::get<long long>(std::get<json_object*>(incoming_message["body"])->operator[]("msg_id"));
    std::get<json_object*>(init_reply["body"])->operator[]("msg_id") = message_id;
    std::get<json_object*>(init_reply["body"])->operator[]("type") = "init_ok";

    std::cout<<init_reply.json_string()<<std::endl;
}

void Send_Echo_Reply(json_object& incoming_message, int message_id){

    json_object echo_reply;
    echo_reply.list_flag = false;
    echo_reply["src"] = incoming_message["dest"];
    echo_reply["dest"] = incoming_message["src"];
    echo_reply["body"] = new json_object();
    std::get<json_object*>(echo_reply["body"])->list_flag = false;
    std::get<json_object*>(echo_reply["body"])->operator[]("msg_id") = message_id;
    std::get<json_object*>(echo_reply["body"])->operator[]("in_reply_to") = std::get<long long>(std::get<json_object*>(incoming_message["body"])->operator[]("msg_id"));
    std::get<json_object*>(echo_reply["body"])->operator[]("type") = "echo_ok";
    std::get<json_object*>(echo_reply["body"])->operator[]("echo") = std::get<std::string>(std::get<json_object*>(incoming_message["body"])->operator[]("echo"));

    std::cout<<echo_reply.json_string()<<std::endl;
}