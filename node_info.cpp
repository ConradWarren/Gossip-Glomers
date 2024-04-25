#include "node_info.h"
#include "utility_functions.h"

void node_info::handle_sent_messages(){

    /*
    int queue_size = (int)messages_sent_as_control_node.size();
    for(int i = 0; i<queue_size; i++){

        if(message_ids_broadcasts_sent_map.find(messages_sent_as_control_node.front().second) == message_ids_broadcasts_sent_map.end()){
            messages_sent_as_control_node.pop();
            continue;
        }

        std::chrono::high_resolution_clock::time_point current = std::chrono::high_resolution_clock::now();
        std::chrono::duration duration = std::chrono::duration_cast<std::chrono::milliseconds>(current - messages_sent_as_control_node.front().first);

        if(duration.count() <= 300){
            break;
        }

        std::cout<<message_ids_broadcasts_sent_map[messages_sent_as_control_node.front().second]<<std::endl;
        messages_sent_as_control_node.push(messages_sent_as_control_node.front());
        messages_sent_as_control_node.pop();
    }
    */

    std::chrono::high_resolution_clock::time_point current_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - message_buffer_time_stamp);
    if(broadcast_value_buffer.list_arr.size() >= 3 || (!broadcast_value_buffer.list_arr.empty() && duration.count() >= 300)){

        json_object broadcast_message;
        broadcast_message["src"] = "n" + std::to_string(node_id);
        broadcast_message["body"] = new json_object();
        std::get<json_object*>(broadcast_message["body"])->operator[]("type") = "broadcast";
        std::get<json_object*>(broadcast_message["body"])->operator[]("message") = &broadcast_value_buffer;

        for(int i = 0; i<node_count; i++){

            if(i == node_id){
                continue;
            }

            broadcast_message["dest"] = "n" + std::to_string(i);
            std::get<json_object*>(broadcast_message["body"])->operator[]("msg_id") = Generate_Unique_ID();
            std::cout<<broadcast_message.json_string()<<std::endl;
            //std::cerr<<broadcast_message.json_string()<<std::endl;
        }

        std::get<json_object*>(broadcast_message["body"])->operator[]("message") = nullptr;
        broadcast_value_buffer.list_arr.clear();
    }

}