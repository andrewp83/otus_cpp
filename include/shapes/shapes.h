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
	static ShapePtr createShape(ShapeType type, Point pt);
};
