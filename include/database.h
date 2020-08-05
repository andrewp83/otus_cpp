//
//  database.hpp
//  join_server
//
//  Created by a.polyakov on 06.08.2020.
//

#pragma once

#include <string>
#include <vector>


namespace my_db {

class Database;

class QueryResult {
    friend class Database;
    
public:
    const std::vector<std::string>& get_rows() const;
    
    bool is_ok() const;
    
    const std::string& get_error_message() const;
    
private:
    std::string error_message;
    std::vector<std::string> rows;
};

class Database {
public:
    QueryResult query(const std::string& query_str);
};

}
