#include "file_io.h"

#include <iostream>

void FileIOPng::write_document(DocumentPtr /*document*/, const std::string&/* name*/) {
	std::cout << "FileIOPng::write_document" << std::endl;
}

DocumentPtr FileIOPng::read_document(const std::string& /*name*/) {
	std::cout << "FileIOPng::read_document" << std::endl;
	return std::make_shared<Document>();
}

void FileIOJpeg::write_document(DocumentPtr /*document*/, const std::string& /*name*/) {
	std::cout << "FileIOJpeg::write_document" << std::endl;
}

DocumentPtr FileIOJpeg::read_document(const std::string& /*name*/) {
	std::cout << "FileIOJpeg::read_document" << std::endl;
	return std::make_shared<Document>();
}
