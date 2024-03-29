#include <iostream>
#include <string>

#include "json_parser/json.h"
#include "utility_functions.h"


int main(){

    std::string input;
    int message_id = 1;
    while (std::getline(std::cin, input)){

        json_object incoming_message;
        incoming_message.parse(input);

        if(incoming_message.key_value_map.find("body") == incoming_message.key_value_map.end()){
            std::cerr<<"Error: Could not find body in incoming message\n"<<incoming_message.json_string()<<std::endl;
            exit(EXIT_FAILURE);
        }

        if(std::get<std::string>((std::get<json_object*>(incoming_message["body"]))->operator[]("type")) == "init"){
            Send_Init_Reply(incoming_message, message_id);
        }else if(std::get<std::string>((std::get<json_object*>(incoming_message["body"]))->operator[]("type")) == "echo"){
            Send_Echo_Reply(incoming_message, message_id);
        }else if(std::get<std::string>(std::get<json_object*>(incoming_message["body"])->operator[]("type")) == "generate"){
            Send_Generate_Reply(incoming_message, message_id);
        }

        message_id++;
    }

	return 0;
}
