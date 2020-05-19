#include "controller.h"
#include "file_io_factory.h"

void Controller::create_new_document() {
	model->create_document();
}

void Controller::open_document(const std::string& name) {
	SaveDocumentFormat format = utils::get_file_format(name);
	FileIO reader = FileIOFactory::create(format);
	DocumentPtr doc = reader.read_document(name);
	model->add_document(doc);
}

void Controller::save_document(const std::string& name, SaveDocumentFormat format) {
	DocumentPtr doc = model->get_current_document();
	FileIO writer = FileIOFactory::create(format);
	writer.write_document(doc, name);
	doc.set_name(name);
}

void Controller::create_shape(ShapeType type, const Point& pt) {
	ShapePtr shape = ShapeFactory::createShape(type, pt);
	DocumentPtr doc = model->get_current_document();
	doc.add_shape(shape);
}

void Controller::select_shape(const Point& pt) {
	DocumentPtr doc = model->get_current_document();
	ShapePtr shape = doc->get_shape_by_point(pt);
	if (shape) {
		doc->select_shape(shape);
	}
}

void Controller::remove_shape() {
	DocumentPtr doc = model->get_current_document();
	ShapePtr shape = doc.get_selected_shape();
	if (shape) {
		doc.remove_shape(shape);
	}
}

void Controller::select_document(size_t index) {
	model->select_document(index);
}

