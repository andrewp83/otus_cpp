//
//  point.h
//  kkmeans

#pragma once

struct Point {
    Point(double x, double y) : x(x), y(y) {}
    
    double x {0.0};
    double y {0.0};
};
