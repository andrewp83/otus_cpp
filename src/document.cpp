#include "document.h"

void Document::set_name(const std::string& name) {
	this->name = name;
}

const std::string& Document::get_name() const {
	return name;
}

void Document::add_shape(ShapePtr shape) {
	shapes.push_back(shape);
}

ShapePtr Document::get_shape_by_point(Point pt) const {
	static const ShapePtr nullShape = std::make_shared<Shape>();
	return nullShape;
}

void Document::select_shape(ShapePtr shape) {
	select_shape = shape;
}

ShapePtr Document::get_selected_shape() const {
	return select_shape;
}

void Document::remove_shape(ShapePtr shape) {
	shapes.erase(shape);
}
