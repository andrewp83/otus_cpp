#include "document.h"

#include <algorithm>

void Document::set_name(const std::string& name) {
	this->name = name;
}

const std::string& Document::get_name() const {
	return name;
}

void Document::add_shape(ShapePtr shape) {
	shapes.push_back(shape);
}

ShapePtr Document::get_shape_by_point(const Point&) const {
	static const ShapePtr nullShape = std::make_shared<TriangleShape>(Point());
	return nullShape;
}

void Document::select_shape(ShapePtr shape) {
	selected_shape = shape;
}

ShapePtr Document::get_selected_shape() const {
	return selected_shape;
}

void Document::remove_shape(ShapePtr shape) {
    auto it = std::find(shapes.begin(), shapes.end(), shape);
    if (it != shapes.end()) {
        shapes.erase(it);
    }
}
