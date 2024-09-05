#ifndef POINT_H
#define POINT_H

#include "DataType.h"
#include <iostream>

class Point3D {
private:
    NType _x, _y, _z;

public:
    Point3D() : _x(0), _y(0), _z(0) {}
    Point3D(NType x, NType y, NType z) : _x(x), _y(y), _z(z) {}

    ~Point3D() = default;

    NType getX() const { return _x; }
    NType getY() const { return _y; }
    NType getZ() const { return _z; }

    void setX(NType x) { this->_x = x; }
    void setY(NType y) { this->_y = y; }
    void setZ(NType z) { this->_z = z; }

    NType distance(const Point3D &p) const {
        return sqrt((_x - p._x) * (_x - p._x) + (_y - p._y) * (_y - p._y) + (_z - p._z) * (_z - p._z));
    }

    // Operators
    bool operator==(const Point3D &p) const {
        return _x == p._x && _y == p._y && _z == p._z;
    }
    bool operator!=(const Point3D &p) const {
        return !(*this == p);
    }
    Point3D operator-(const Point3D &p) const {
        return Point3D(_x - p._x, _y - p._y, _z - p._z);
    }
    Point3D operator+(const Point3D &p) const {
        return Point3D(_x + p._x, _y + p._y, _z + p._z);
    }

    // Print
    friend std::ostream &operator<<(std::ostream &os, const Point3D &p) {
        os << "(" << p._x.getValue() << "," << p._y.getValue() << "," << p._z.getValue() << ")";
        return os;
    }
};

#endif // POINT_H
