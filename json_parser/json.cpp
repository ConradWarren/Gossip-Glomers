#include "json.h"

#include <iostream>

json_object::json_object(){
    list_flag = false;
}
json_object::json_object(bool is_list){
    list_flag = is_list;
}

json_object::~json_object() {

    if (list_flag) {
        for(const auto  &item : list_arr){
            if(auto *value = std::get_if<json_object*>(&item)){
               delete ((json_object*)*value);
            }
        }
    } else {
        for (const auto &item: key_value_map) {
            if (auto *value = std::get_if<json_object *>(&item.second)) {
                delete ((json_object*)*value);
            }
        }
    }
}

long long json_object::parse(const std::string& raw_json){

    long long idx = 0;
    while (idx < raw_json.size() && std::isspace(raw_json[idx])){
        idx++;
    }

    if(idx >= raw_json.size() || (raw_json[idx] != '[' && raw_json[idx] != '{')){
        std::cerr<<"Error: Invalid json formatting"<<std::endl;
        return -1;
    }

    return idx + 1 + ((raw_json[idx] == '[') ? parse_array(raw_json.c_str() + idx + 1) : parse_map(raw_json.c_str() + idx + 1));
}

long long json_object::parse_array(const std::string &raw_json) {

    list_flag = true;
    long long idx = 0;
    bool string_flag = false;
    bool floating_point_flag = false;
    std::string current;

    while (idx < raw_json.size() && raw_json[idx] != ']'){

        if(std::isspace(raw_json[idx])){
            idx++;
            continue;
        }

        if(raw_json[idx] == '"'){
            string_flag = true;
            idx++;
            while (idx < raw_json.size() && raw_json[idx] != '"'){
                current += raw_json[idx];
                idx++;
            }
            if(raw_json[idx] != '"'){
                std::cerr<<"Error: Invalid json formatting at string case"<<std::endl;
                return -1;
            }
        }else if(raw_json[idx] == ','){

            if(current.empty() && !string_flag){
                idx++;
                continue;
            }

            if(string_flag){
                list_arr.emplace_back(current);
            }else if(floating_point_flag){
                list_arr.emplace_back(std::stod(current));
            }else if(current == "null"){
                list_arr.emplace_back(nullptr);
            }else{
                list_arr.emplace_back(std::stoll(current));
            }

            floating_point_flag = false;
            string_flag = false;
            current.clear();
        }else if(raw_json[idx] == '.'){
            floating_point_flag = true;
        }else if(raw_json[idx] == '[' || raw_json[idx] == '{'){
            json_object* next_object = new json_object();
            long long bytes_parsed = (raw_json[idx] == '[') ? next_object->parse_array(raw_json.c_str() + idx + 1) : next_object->parse_map(raw_json.c_str() + idx + 1);
            if(bytes_parsed == -1){
                return -1;
            }
            list_arr.emplace_back(next_object);
            idx += bytes_parsed;
        }else{
            current += raw_json[idx];
        }
        idx++;
    }

    if(idx >= raw_json.size()){
        std::cerr<<"Error: Invalid json formatting"<<std::endl;
        return -1;
    }

    if(!current.empty()){
        if(string_flag){
            list_arr.emplace_back(current);
        }else if(floating_point_flag){
            list_arr.emplace_back(std::stod(current));
        }else if(current == "null"){
            list_arr.emplace_back(nullptr);
        }else{
            list_arr.emplace_back(std::stoll(current));
        }
    }

    return idx + 1;
}

long long json_object::parse_map(const std::string& raw_json){

    list_flag = false;
    long long idx = 0;
    bool string_flag = false;
    bool floating_point_flag = false;
    bool found_key_flag = false;
    std::string current;
    std::string key;

    while (idx < raw_json.size() && raw_json[idx] != '}'){

        if(std::isspace(raw_json[idx])){
            idx++;
            continue;
        }

        if(raw_json[idx] == '"'){
            idx++;
            while (idx < raw_json.size() && raw_json[idx] != '"'){
                current += raw_json[idx];
                idx++;
            }
            if(idx >= raw_json.size()){
                std::cerr<<"Error: Invalid json formatting, missed quotation marks"<<std::endl;
                return -1;
            }
            string_flag = true;
        }else if(raw_json[idx] == ':'){

            if(!string_flag || found_key_flag){
                std::cerr<<"Error: Invalid json formatting at :"<<std::endl;
                return -1;
            }
            key = current;
            current.clear();
            found_key_flag = true;
            string_flag = false;
        }else if(raw_json[idx] == '.'){
            current += raw_json[idx];
            floating_point_flag = true;
        }else if(raw_json[idx] == ','){
            if(!found_key_flag && current.empty()){
                idx++;
                continue;
            }

            if(!found_key_flag){
                std::cerr<<"Error: Invalid json formatting, at ,"<<std::endl;
                return -1;
            }

            if(key_value_map.find(key) != key_value_map.end()){
                std::cerr<<"Error: Invalid json formatting, duplicate keys"<<std::endl;
                return -1;
            }
            if(string_flag){
                key_value_map[key] = current;
            }else if(floating_point_flag){
                key_value_map[key] = std::stod(current);
            }else if(current == "null"){
                key_value_map[key] = nullptr;
            }else{
                key_value_map[key] = std::stoll(current);
            }
            found_key_flag = false;
            string_flag = false;
            floating_point_flag = false;
            current.clear();
        }else if(raw_json[idx] == '{' || raw_json[idx] == '['){

            if(!found_key_flag){
                std::cerr<<"Error: Invalid json formatting at [ or {, idx = "<<idx<<std::endl;
                return -1;
            }

            json_object* next_object = new json_object();
            long long bytes_parsed = (raw_json[idx] == '[') ? next_object->parse_array(raw_json.c_str()+idx+1) : next_object->parse_map(raw_json.c_str() + idx + 1);
            if(bytes_parsed == -1){
                return -1;
            }
            key_value_map[key] = next_object;
            idx += bytes_parsed;
            found_key_flag = false;
        }else{
            current += raw_json[idx];
        }
        idx++;
    }

    if(idx >= raw_json.size()){
        std::cerr<<"Error: Invalid json formatting at end"<<std::endl;
        return -1;
    }

    if(!current.empty() && !found_key_flag){
        std::cerr<<"Error: Invalid json formatting at end"<<std::endl;
        return -1;
    }
    if(!current.empty()){
        if(string_flag){
            key_value_map[key] = current;
        }else if(floating_point_flag){
            key_value_map[key] = std::stod(current);
        }else if(current == "null"){
            key_value_map[key] = nullptr;
        }else{
            key_value_map[key] = std::stoll(current);
        }
    }

    return idx + 1;
}


std::string json_object::json_string(){

    std::string result;

    if(list_flag){
        result += '[';
        for(const auto& item : list_arr){
            if(const std::string* value = std::get_if<std::string>(&item)){
                result += "\"" + *value + "\",";
            }else if(json_object* const* value = std::get_if<json_object*>(&item)){
                result += (*value == nullptr) ? "null," : ((json_object*)*value)->json_string() + ',';
            }else if(const double* value = std::get_if<double>(&item)){
                result += std::to_string(*value);
            }else{
                result += std::to_string(std::get<long long>(item)) + ',';
            }
        }
        result.back() = ']';
        return result;
    }else{

        result += '{';
        for(const auto& item : key_value_map){
            result += '"' + item.first + "\":";
            if(const std::string* value = std::get_if<std::string>(&item.second)){
                result += '"' + *value + "\",";
            }else if(json_object* const* value = std::get_if<json_object*>(&item.second)){
                result += (*value == nullptr) ? "null," : ((json_object*)*value)->json_string() + ',';
            }else if(const double* value = std::get_if<double>(&item.second)){
                result += std::to_string(*value) + ',';
            }else{
                result += std::to_string(std::get<long long>(item.second)) + ',';
            }
        }

    }
    result.back() = '}';
    return result;
}

