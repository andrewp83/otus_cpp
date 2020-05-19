#include "shapes.h"


Shape::Shape(const Point& pt) : pt(pt) {

}

TriangleShape(const Point& pt) : Shape(pt) {}

void TriangleShape::draw() {
	std::cout << "TriangleShape::draw()" << std::endl;
}

RoundShape(const Point& pt) : Shape(pt) {}

void RoundShape::draw() {
	std::cout << "RoundShape::draw()" << std::endl;
}

ShapePtr ShapeFactory::createShape(ShapeType type, Point pt) {
	switch (type) {
		case ShapeType::TRIANGLE:
			return std::make_shared<TriangleShape>(pt);
		case ShapeType::ROUND:
			return std::make_shared<RoundShape>(pt);
	};
	return nullptr;
}