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

ShapePtr Document::get_shape_by_point(const Point& pt) const {
    auto it = std::find_if(shapes.cbegin(), shapes.cend(), [=](const ShapePtr ptr){
        return ptr->is_point_inside(pt);
    });
    if (it == shapes.cend()) {
        return nullptr;
    }
    return *it;
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
