#pragma once

#include "common_types.h"
#include "file_io.h"

class FileIOFactory {
public:
	static FileIOPtr create(SaveDocumentFormat format);
};
