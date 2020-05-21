#pragma once

#include "document.h"

class FileIO {
public:
    virtual ~FileIO() {}
    
	virtual void write_document(DocumentPtr document, const std::string& name) = 0;
	virtual DocumentPtr read_document(const std::string& name) = 0;
};

using FileIOPtr = std::shared_ptr<FileIO>;

class FileIOPng : public FileIO {
public:
	virtual void write_document(DocumentPtr document, const std::string& name) override;
	virtual DocumentPtr read_document(const std::string& name) override;
};

class FileIOJpeg : public FileIO {
public:
	virtual void write_document(DocumentPtr document, const std::string& name) override;
	virtual DocumentPtr read_document(const std::string& name) override;
};
