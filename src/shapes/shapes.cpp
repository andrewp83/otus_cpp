#include "shapes.h"

#include <iostream>


Shape::Shape(const Point& pt) : pt(pt) {

}

TriangleShape::TriangleShape(const Point& pt) : Shape(pt) {}

void TriangleShape::draw() {
	std::cout << "TriangleShape::draw()" << std::endl;
}

RoundShape::RoundShape(const Point& pt) : Shape(pt) {}

void RoundShape::draw() {
	std::cout << "RoundShape::draw()" << std::endl;
}

std::unique_ptr<Shape> ShapeFactory::createShape(ShapeType type, Point pt) {
	switch (type) {
		case ShapeType::TRIANGLE:
			return std::make_unique<TriangleShape>(pt);
		case ShapeType::ROUND:
			return std::make_unique<RoundShape>(pt);
        case ShapeType::UNKNOWN:
            return nullptr;
	};
	return nullptr;
}
