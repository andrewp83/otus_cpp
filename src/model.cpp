#include "model.h"

void Model::create_document() {
	DocumentPtr doc = std::make_shared<Document>();
	add_document(doc);
}

void Model::add_document(DocumentPtr doc) {
	documents.push_back(doc);
}

DocumentPtr Model::get_current_document() const {
	static DocumentPtr nullDoc = std::make_shared<Document>();
	if (current_document < 0 || current_document >= documents.size()) {
		return nullDoc;
	}
	return documents[current_document];
}

void Model::select_document(size_t index) {
	if (index >= 0 && index < documents.size()) {
		current_document = index;
	}
}