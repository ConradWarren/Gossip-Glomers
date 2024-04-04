#ifndef ECHO_BIN_UTILITY_FUNCTIONS_H
#define ECHO_BIN_UTILITY_FUNCTIONS_H

#include "json_parser/json.h"
#include "node_info.h"

void Send_Init_Reply(json_object& incoming_message, node_info& node);

void Send_Echo_Reply(json_object& incoming_message);

void Send_Generate_Reply(json_object& incoming_message);

void Send_Broadcast_Reply(json_object& incoming_message, node_info& info);

void Send_Read_Reply(json_object& incoming_message, node_info& node);

void Send_Topology_Reply(json_object& incoming_message, node_info& node);

void Receive_Broadcast_Response(json_object& incoming_message, node_info& node);

#endif //ECHO_BIN_UTILITY_FUNCTIONS_H
