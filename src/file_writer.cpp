#include "file_writer.h"

#include <iostream>
#include <fstream>

void FileWriter::bulk_executed(const BulkResult& result) {
	// save to file
    const std::string& filename = create_filename();
    std::fstream fs(filename, fs.out);
    if (!fs.is_open()) {
        std::cerr << "failed to open " << filename << std::endl;
        return;
    }
    fs << result.to_string();
}

std::string FileWriter::create_filename() {
    std::lock_guard<std::mutex> lock(log_mutex);

    std::time_t current_time = time(nullptr);

    if (current_time != last_time) {
        last_time = current_time;
        add_suffix = 0;
    } else {
        add_suffix++;
    }

    std::string filename = "bulk_" + std::to_string(current_time);
    filename += "_";
    filename += std::to_string(add_suffix);
    filename += ".log";

    return filename;
}
