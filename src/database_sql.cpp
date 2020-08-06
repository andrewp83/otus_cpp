#include "database_sql.h"

namespace my_db {

CommandPtr CommandFactory::create_command(Database* db, const std::string& name, const std::vector<std::string>& args, std::string& error_msg) {
    
    CommandPtr command;
    
    if (name == "INSERT") {
        command = create_insert(db, args, error_msg);
    } else if (name == "TRUNCATE") {
        command = create_truncate(db, args, error_msg);
    } else if (name == "INTERSECTION") {
        command = create_intersection(db);
    } else if (name == "SYMMETRIC_DIFFERENCE") {
        command = create_symmetric_difference(db);
    } else if (name == "PRINT") {
        command = create_print(db, args, error_msg);
    } else {
        error_msg = "Unknown command name";
    }
        
    return command;
}

CommandPtr CommandFactory::create_insert(Database* db, const std::vector<std::string>& args, std::string& error_msg) {
    
    // table name
    if (args.size() < 1) {
        error_msg = "Table name is not specified";
        return nullptr;
    }
    
    std::string table = args[0];
    
    // id
    
    if (args.size() < 2) {
        error_msg = "id is not specified";
        return nullptr;
    }
    
    std::size_t _id = 0;
    try {
        _id = std::stoi(args[1]);
    } catch (std::exception&) {
        error_msg = "id has wrong format";
        return nullptr;
    }
    
    // name
    
    if (args.size() < 3) {
        error_msg = "name is not specified";
        return nullptr;
    }
    
    std::string name = args[2];
    
    Record record(_id, name);
    
    return std::make_shared<CommandInsert>(db, table, record);
}

CommandPtr CommandFactory::create_truncate(Database* db, const std::vector<std::string>& args, std::string& error_msg) {
    // table name
    if (args.size() < 1) {
        error_msg = "Table name is not specified";
        return nullptr;
    }
    
    std::string table = args[0];
    
    return std::make_shared<CommandTruncate>(db, table);
}

CommandPtr CommandFactory::create_intersection(Database* db) {
    return std::make_shared<CommandIntersection>(db);
}

CommandPtr CommandFactory::create_symmetric_difference(Database* db) {
    return std::make_shared<CommandSymmetricDifference>(db);
}

CommandPtr CommandFactory::create_print(Database* db, const std::vector<std::string>& args, std::string& error_msg) {
    // table name
    if (args.size() < 1) {
        error_msg = "Table name is not specified";
        return nullptr;
    }
    
    std::string table = args[0];
    
    return std::make_shared<CommandPrint>(db, table);
}



// СОБСТВЕННО КОМАНДЫ

QueryResult CommandInsert::run() {
    QueryResult result;
    
    Table* table = db->get_table(table_name);
    if (!table) {
        result.set_error_message("Table " + table_name + " not found");
        return result;
    }
    
    bool is_ok = table->insert(record);
    if (!is_ok) {
        result.set_error_message("duplicate " + std::to_string(record.get_id()));
    }
    
    return result;
}

QueryResult CommandTruncate::run() {
    QueryResult result;
    
    Table* table = db->get_table(table_name);
    if (!table) {
       result.set_error_message("Table " + table_name + " not found");
       return result;
    }
       
    table->truncate();
    
    return result;

}

QueryResult CommandIntersection::run() {
    QueryResult result;
    
    Table* table_A = db->get_table("A");
    Table* table_B = db->get_table("B");
    
    const std::vector<Record>& records_A = table_A->get_records();
    const std::vector<Record>& records_B = table_B->get_records();
    
    auto itA = records_A.begin();
    auto itB = records_B.begin();
    
    std::string row_str;
    
    while (itA != records_A.end() && itB != records_B.end()) {
        if (itA->get_id() == itB->get_id()) {
            
            row_str = std::to_string(itA->get_id());
            row_str += ",";
            row_str += itA->get_name();
            row_str += ",";
            row_str += itB->get_name();
            result.add_row(row_str);
            
            ++itA;
            ++itB;
        } else if (itA->get_id() < itB->get_id()) {
            ++itA;
        } else {
            ++itB;
        }
    }
    
    return result;

}

QueryResult CommandSymmetricDifference::run() {
    QueryResult result;
    
    Table* table_A = db->get_table("A");
    Table* table_B = db->get_table("B");
    
    const std::vector<Record>& records_A = table_A->get_records();
    const std::vector<Record>& records_B = table_B->get_records();
    
    auto itA = records_A.begin();
    auto itB = records_B.begin();
    
    auto add_row = [&](const Record& rec, bool is_left) {
        std::string row_str;
        row_str += std::to_string(rec.get_id());
        row_str += ",";
        if (is_left) {
            row_str += rec.get_name();
            row_str += ",";
        } else {
            row_str += ",";
            row_str += rec.get_name();
        }
        result.add_row(row_str);
    };
    
    while (itA != records_A.end() || itB != records_B.end()) {
        
        if (itA == records_A.end()) {
            for (; itB != records_B.end(); ++itB) {
                add_row(*itB, false);
            }
            break;
        }
        
        if (itB == records_B.end()) {
            for (; itA != records_A.end(); ++itA) {
                add_row(*itA, true);
            }
            break;
        }

        if (itA->get_id() == itB->get_id()) {
            ++itA;
            ++itB;
        } else if (itA->get_id() < itB->get_id()) {
            add_row(*itA, true);
            ++itA;
        } else {
            add_row(*itB, false);
            ++itB;
        }
    }

    return result;
}


QueryResult CommandPrint::run() {
    QueryResult result;
    
    Table* table = db->get_table(table_name);
    if (!table) {
       result.set_error_message("Table " + table_name + " not found");
       return result;
    }
       
    const std::vector<Record>& records = table->get_records();
    std::string row_str;
    for (const auto& record : records) {
        row_str = std::to_string(record.get_id());
        row_str += ",";
        row_str += record.get_name();
        result.add_row(row_str);
    }
    
    return result;

}

}
