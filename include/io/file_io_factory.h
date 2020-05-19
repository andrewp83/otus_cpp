#pragma once

#include "common_types.h"

class FileIOFactory {
public:
	static FileIO create(SaveDocumentFormat format);
};