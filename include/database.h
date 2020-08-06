//
//  database.hpp
//  join_server
//
//  Created by a.polyakov on 06.08.2020.
//

#pragma once

#include <map>
#include <string>
#include <vector>


namespace my_db {

class Database;


class Record {
public:
    Record() {}
    Record(int _id, const std::string& name) : _id(_id), name(name) {}
    
    size_t get_id() const { return _id; }
    const std::string& get_name() const { return name; }
    
private:
    size_t _id {0};
    std::string name;
};


class Table {
public:
    Table(const std::string& name) : name(name) {}
    
    const std::vector<Record>& get_records() const { return records; }
    
    bool insert(const Record& record);
    
    void truncate();
    
private:
    std::string name;
    std::vector<Record> records;
};


class QueryResult {
    friend class Database;
    
public:
    const std::vector<std::string>& get_rows() const;
    
    void add_row(const std::string& row) { rows.push_back(row); }
    
    bool is_ok() const;
    
    const std::string& get_error_message() const;
    
    void set_error_message(const std::string& msg) { error_message = msg; }
    
private:
    std::string error_message;
    std::vector<std::string> rows;
};


class Database {
public:
    Database();
    
    QueryResult query(const std::string& query_str);
    
    Table* get_table(const std::string& name);
    
private:
    std::map<std::string, Table> tables;
};

std::vector<std::string> split_string(const std::string& str);

}
