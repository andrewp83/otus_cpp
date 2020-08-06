//
//  database.cpp
//  join_server
//
//  Created by a.polyakov on 06.08.2020.
//

#include <algorithm>
#include <sstream>

#include "database.h"
#include "database_sql.h"

namespace my_db {


const std::vector<std::string>& QueryResult::get_rows() const {
    return rows;
}
    
bool QueryResult::is_ok() const {
    return error_message.empty();
}
    
const std::string& QueryResult::get_error_message() const {
    return error_message;
}

// Table

bool Table::insert(const Record &record) {
    auto it = std::lower_bound(records.begin(), records.end(), record, [](const Record& left, const Record& right) {
        return left.get_id() < right.get_id();
    });
    if (it != records.end() && it->get_id() == record.get_id()) {
        return false;
    }
    records.insert(it, record);
    return true;
}

void Table::truncate() {
    records.clear();
}
    
// Database

Database::Database() {
    tables.emplace("A", Table("A"));
    tables.emplace("B", Table("B"));
}

Table* Database::get_table(const std::string& name) {
    auto it = tables.find(name);
    if (it == tables.end()) {
        return nullptr;
    }
    return &(it->second);
}

QueryResult Database::query(const std::string& query_str) {
    
    QueryResult result;
    
    std::vector<std::string> parsed = split_string(query_str);
    
    if (parsed.empty()) {
        result.error_message = "Empty command";
        return result;
    }
    
    std::string command_name = parsed[0];
    
    std::vector<std::string> args(parsed.begin() + 1, parsed.end());
    
    CommandPtr command = CommandFactory::create_command(this, command_name, args, result.error_message);
    
    if (!command) {
        return result;
    }
    
    result = command->run();
    
    return result;
}

std::vector<std::string> split_string(const std::string& text) {
    std::istringstream iss(text);
    std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                     std::istream_iterator<std::string>());
    return results;
}


}
