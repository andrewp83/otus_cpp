#include <iostream>

#include "includes/application.h"

namespace ui {
	void on_create() {
		Application::getInstance()->getController().create_new_document();
	}

	void on_load_from_file(const std::string& name) {
		Application::getInstance()->getController().open_document(name);	
	}

	void on_save_to_file(const std::string& name, SaveDocumentFormat format) {
		Application::getInstance()->getController().save_document(name, format);
	}

	void on_shape_create(Point pos) {
		Application::getInstance()->getController().create_shape(pos);
	}

	void on_shape_remove() {
		Application::getInstance()->getController().remove_shape();
	}

	void select_document(size_t index) {
		Application::getInstance()->getController().select_document(index);
	}
};

int main (int, char **) {
	Application::getInstance()->run();
	
    return 0;
}
