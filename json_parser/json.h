#ifndef JSON_H
#define JSON_H

#include <unordered_map>
#include <string>
#include <vector>
#include <variant>
#include <iostream>

class json_object{

public:
    bool list_flag;
    std::vector<std::variant<long long, double, std::string, json_object*>> list_arr;
    std::unordered_map<std::string, std::variant<long long, double, std::string, json_object*>> key_value_map;

    long long parse(const std::string& raw_json);
    std::string json_string();

    std::variant<long long, double, std::string, json_object*>& operator[](const std::string& key){
        return key_value_map[key];
    }
    std::variant<long long, double, std::string, json_object*>& operator[](int key){
        return list_arr[key];
    }

    ~json_object();
private:

    long long parse_array(const std::string& raw_json);
    long long parse_map(const std::string& raw_json);
};


#endif //JSON_H
