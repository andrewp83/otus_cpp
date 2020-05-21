#include <iostream>

#include "application.h"

namespace ui {
	void on_create() {
		Application::instance()->get_controller().create_new_document();
	}

	void on_load_from_file(const std::string& name) {
		Application::instance()->get_controller().open_document(name);
	}

	void on_save_to_file(const std::string& name, SaveDocumentFormat format) {
		Application::instance()->get_controller().save_document(name, format);
	}

	void on_shape_create(ShapeType type, Point pos) {
		Application::instance()->get_controller().create_shape(type, pos);
	}

	void on_shape_remove() {
		Application::instance()->get_controller().remove_shape();
	}

	void select_document(size_t index) {
		Application::instance()->get_controller().select_document(index);
	}
}

int main (int, char **) {
    
	Application::instance()->run();
    
    return 0;
}
