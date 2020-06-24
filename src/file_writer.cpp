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

std::string FileWriter::create_filename() const {
    return "bulk" + std::to_string(time(nullptr)) + ".log";
}
