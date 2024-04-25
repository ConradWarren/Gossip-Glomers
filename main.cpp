#include <iostream>
#include <string>
#include <variant>

#include "json_parser/json.h"
#include "utility_functions.h"
#include "node_info.h"

//TODO: Build a separate message_handling method in the node_info class for dropped/partitioned messages
//TODO: Build a buffer for data, so that we can send multiple peaces of data at once from a single control node.
//TODO: Build a voting system or something similar for the control node, so if the control nodes gets partitioned we can still send messages in an efficient manner.

int main(){

    std::string input;
    node_info node;
    while (std::getline(std::cin, input)){

        json_object incoming_message;
        std::string type;
        incoming_message.parse(input);

        if(incoming_message.key_value_map.find("body") == incoming_message.key_value_map.end()){
            std::cerr<<"Error: Could not find body in incoming message\n"<<incoming_message.json_string()<<std::endl;
            exit(EXIT_FAILURE);
        }
        if(std::string* str_ptr = std::get_if<std::string>(&(std::get<json_object*>(incoming_message["body"]))->operator[]("type"))){
            type = *str_ptr;
        }else{
            std::cerr<<"Error: Could not find valid type in incoming_message\n"<<incoming_message.json_string()<<std::endl;
            exit(EXIT_FAILURE);
        }

        if(type == "init"){
            Send_Init_Reply(incoming_message, node);
        }else if(type == "echo"){
            Send_Echo_Reply(incoming_message);
        }else if(type == "generate"){
            Send_Generate_Reply(incoming_message);
        }else if(type == "topology"){
            Send_Topology_Reply(incoming_message, node);
        }else if(type == "broadcast"){
            Send_Broadcast_Reply(incoming_message, node);
        }else if(type == "read"){
            Send_Read_Reply(incoming_message, node);
        }else if(type == "broadcast_ok"){
            Receive_Broadcast_Response(incoming_message, node);
        }

        node.handle_sent_messages();
    }

	return 0;
}
