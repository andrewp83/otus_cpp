#pragma once

#include <vector>

#include "document.h"

class Model {
public:
	void create_document();
	void add_document(DocumentPtr doc);
	DocumentPtr get_current_document() const;
	void select_document(size_t index);

private:
	std::vector<DocumentPtr> documents;
	size_t current_document {0};
};

using ModelPtr = std::shared_ptr<Model>;
