#pragma once

#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>

#include "task_mgr.h"


using namespace task_mgr;

using namespace std::chrono_literals;

static std::mutex console_m;

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
        if (!std::filesystem::exists("test/data/" + file_name)) {
            throw std::logic_error(file_name + " file path not exists");
        }
    }
    
    void save_file(const std::string& file_name) {
        std::fstream fs;
        fs.open("test/data/" + file_name, std::fstream::out);
        fs << "hello, world!";
        fs.close();
    }
    
    std::vector<std::string> source_files;
    std::string target_file;
};

