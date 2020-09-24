#pragma once

#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>
#include <unordered_map>

#include "task_mgr.h"


using namespace task_mgr;

using namespace std::chrono_literals;

static std::mutex console_m;

class TestDataStorage {
public:
    const std::string& read_data(const std::string& key) {
        std::lock_guard<std::mutex> lock(data_m);
        
        static const std::string empty;
        if (data.count(key) > 0) {
            return data.at(key);
        }
        return empty;
    }
    
    void save_data(const std::string& key, const std::string& value) {
        std::lock_guard<std::mutex> lock(data_m);
        
        data[key] = value;
    }
    
private:
    std::mutex data_m;
    std::unordered_map<std::string, std::string> data;
};

static TestDataStorage g_storage;

class TestProcessingTask : public Task {
public:
    TestProcessingTask(const std::vector<std::string>& source_files, const std::string& target_file)
    : source_files(source_files)
    , target_file(target_file) {}
    
    virtual ~TestProcessingTask() {}
    
    virtual void run(IJob*) override {
        for (const auto& file : source_files) {
            open_file(file);
        }
        std::this_thread::sleep_for(0.5s);
        save_file(target_file);
    }
    
    virtual void callback(IJob*) override {
    }
    
private:
    void open_file(const std::string& file_name) {
        const std::string& data = g_storage.read_data(file_name);
        if (data.empty()) {
            throw std::logic_error(file_name + " file path not exists");
        }
    }
    
    void save_file(const std::string& file_name) {
        g_storage.save_data(file_name, "hello, world!");
    }
    
    std::vector<std::string> source_files;
    std::string target_file;
};

