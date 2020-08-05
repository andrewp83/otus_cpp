//
//  database.cpp
//  join_server
//
//  Created by a.polyakov on 06.08.2020.
//

#include "database.h"

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
    
// Database

QueryResult Database::query(const std::string& query_str) {
    
    std::string error_msg;
    
    CommandPtr command = create_command(query_str, error_msg);
    
    if (!command) {
        QueryResult result;
        result.error_message = error_msg;
        return result;
    }
    
    QueryResult result = command->run();
    
    return result;
}


}
