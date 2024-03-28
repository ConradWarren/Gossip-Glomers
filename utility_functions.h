#ifndef ECHO_BIN_UTILITY_FUNCTIONS_H
#define ECHO_BIN_UTILITY_FUNCTIONS_H

#include "json_parser/json.h"

void Send_Init_Reply(json_object& incoming_message, int message_id);

void Send_Echo_Reply(json_object& incoming_message, int message_id);


#endif //ECHO_BIN_UTILITY_FUNCTIONS_H
