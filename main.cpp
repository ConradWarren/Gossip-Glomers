#include <iostream>
#include <string>

#include "json_parser/json.h"
#include "utility_functions.h"


int main(){

    std::string input;
    json_object value_store(true);


    while (std::getline(std::cin, input)){

        json_object incoming_message;
        incoming_message.parse(input);

        if(incoming_message.key_value_map.find("body") == incoming_message.key_value_map.end()){
            std::cerr<<"Error: Could not find body in incoming message\n"<<incoming_message.json_string()<<std::endl;
            exit(EXIT_FAILURE);
        }

        if(std::get<std::string>((std::get<json_object*>(incoming_message["body"]))->operator[]("type")) == "init"){
            Send_Init_Reply(incoming_message);
        }else if(std::get<std::string>((std::get<json_object*>(incoming_message["body"]))->operator[]("type")) == "echo"){
            Send_Echo_Reply(incoming_message);
        }else if(std::get<std::string>(std::get<json_object*>(incoming_message["body"])->operator[]("type")) == "generate"){
            Send_Generate_Reply(incoming_message);
        }else if(std::get<std::string>(std::get<json_object*>(incoming_message["body"])->operator[]("type")) == "topology"){

        }
        else if(std::get<std::string>(std::get<json_object*>(incoming_message["body"])->operator[]("type")) == "broadcast"){
            Send_Broadcast_Reply(incoming_message, value_store);
        }else if(std::get<std::string>(std::get<json_object*>(incoming_message["body"])->operator[]("type")) == "read"){
            Send_Read_Reply(incoming_message, value_store);
        }
    }

	return 0;
}
