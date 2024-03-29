#ifndef ECHO_BIN_UTILITY_FUNCTIONS_H
#define ECHO_BIN_UTILITY_FUNCTIONS_H

#include "json_parser/json.h"

void Send_Init_Reply(json_object& incoming_message);

void Send_Echo_Reply(json_object& incoming_message);

void Send_Generate_Reply(json_object& incoming_message);

void Send_Broadcast_Reply(json_object& incoming_message, json_object& value_store);

void Send_Read_Reply(json_object& incoming_message, json_object& value_store);

#endif //ECHO_BIN_UTILITY_FUNCTIONS_H
