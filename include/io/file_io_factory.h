#pragma once

#include "common_types.h"
#include "file_io.h"

class FileIOFactory {
public:
	static std::unique_ptr<FileIO> create(SaveDocumentFormat format);
};
