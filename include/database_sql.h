#pragma once

#include <memory>

#include "database.h"

namespace my_db {
	
class Command;

using CommandPtr = std::shared_ptr<Command>;

class CommandFactory {
public:
    static CommandPtr create_command(Database* db, const std::string& name, const std::vector<std::string>& args, std::string& error_msg);
    
private:
    static CommandPtr create_insert(Database* db, const std::vector<std::string>& args, std::string& error_msg);
    static CommandPtr create_truncate(Database* db, const std::vector<std::string>& args, std::string& error_msg);
    static CommandPtr create_intersection(Database* db);
    static CommandPtr create_symmetric_difference(Database* db);
    static CommandPtr create_print(Database* db, const std::vector<std::string>& args, std::string& error_msg);
    
};

class Command {
public:
    Command(Database* db) : db(db) {}
    
    virtual QueryResult run() = 0;
    
    virtual ~Command() {}
    
protected:
    Database* db {nullptr};
};


class CommandInsert : public Command {
public:
    CommandInsert(Database* db, const std::string& table_name, const Record& record) : Command(db), table_name(table_name), record(record) {}
    
    QueryResult run() override;
    
protected:
    std::string table_name;
    Record record;
};


class CommandTruncate : public Command {
public:
    CommandTruncate(Database* db, const std::string& table_name) : Command(db), table_name(table_name) {}
    
    QueryResult run() override;
    
private:
    std::string table_name;
};


class CommandIntersection : public Command {
public:
    CommandIntersection(Database* db) : Command(db) {}
    
    QueryResult run() override;
};


class CommandSymmetricDifference : public Command {
public:
    CommandSymmetricDifference(Database* db) : Command(db) {}
    
    QueryResult run() override;
};


class CommandPrint : public Command {
public:
    CommandPrint(Database* db, const std::string& table_name) : Command(db), table_name(table_name) {}
    
    QueryResult run() override;
    
private:
    std::string table_name;
};


}
