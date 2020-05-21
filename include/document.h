#pragma once

#include "shapes.h"

#include <string>
#include <vector>


class Document {
public:
	void set_name(const std::string& name);
	const std::string& get_name() const;

	void add_shape(ShapePtr shape);
	ShapePtr get_shape_by_point(const Point& pt) const;

	void select_shape(ShapePtr shape);
	void remove_shape(ShapePtr shape);

	ShapePtr get_selected_shape() const;

private:
	std::vector<ShapePtr> shapes;
	std::string name;
	ShapePtr selected_shape;
};

using DocumentPtr = std::shared_ptr<Document>;
