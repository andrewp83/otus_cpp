#pragma once

#include "common_types.h"

#include <memory>

enum class ShapeType {
	UNKNOWN,
	TRIANGLE,
	ROUND,
};

class Shape {
public:
	Shape(const Point& pt);
    virtual ~Shape() {}

	virtual void draw() = 0;
    
    virtual bool is_point_inside(const Point&) const { return true; }

protected:
	Point pt;
};

using ShapePtr = std::shared_ptr<Shape>;

class TriangleShape : public Shape {
public:
	TriangleShape(const Point& pt);

	virtual void draw() override;
};

class RoundShape : public Shape {
public:
	RoundShape(const Point& pt);

	virtual void draw() override;
};

class ShapeFactory {
public:
	static std::unique_ptr<Shape> createShape(ShapeType type, Point pt);
};
