#ifndef PLANE_H
#define PLANE_H

#include "DataType.h"
#include "Point.h"
#include "Line.h"
#include <vector>

std::ostream &operator<<(std::ostream &os, const RelationType &type);

class Plane {
private:
    Point3D  _p;
    Vector3D _n;

public:
    Plane(const Point3D &point, const Vector3D &normal) : _p(point), _n(normal) {}

    NType    distance(const Point3D &p) const;
    Point3D intersect(const Line    &l) const;

    // Contain
    bool contains(const Point3D &p) const;
    bool contains(const Line    &l) const;

    // Getters
    Point3D  getPoint() const { return _p; }
    Vector3D getNormal() const { return _n; }

    // Setters
    void setPoint (Point3D   point) { _p = point; }
    void setNormal(Vector3D normal) { _n = normal; }

    // Operators
    bool operator==(const Plane &other) const;
    bool operator!=(const Plane &other) const;

    // Print
    friend std::ostream &operator<<(std::ostream &os, const Plane &p) {
        os << "Punto: " << p._p << ", Normal: " << p._n;
        return os;
    }
};

class Polygon {
private:
    std::vector<Point3D> vertices;

public:
    Polygon(const std::vector<Point3D> &vertices) : vertices(vertices) {}

    // Getters
    std::vector<Point3D> getVertices() const { return vertices; }
    Point3D getVertex(size_t index) const { return vertices[index]; }
    Plane   getPlane   () const;    // Get the plane of the polygon
    Point3D getNormal  () const;    // Get the normal of the polygon
    Point3D getCentroid() const;    // Get the centroid of the polygon

    // Setters
    void setVertices(std::vector<Point3D> vertices) { this->vertices = vertices; }

    // Check if a point is inside the polygon
    bool contains(const Point3D &p) const;

    // Get the relation of the polygon with a plane
    RelationType relationWithPlane(const Plane &plane) const;

    // Split the polygon by a plane
    std::pair<Polygon, Polygon> split(const Plane &plane) const;

    // Compute the area of the polygon
    NType area() const;

    // Operators
    bool operator==(const Polygon &other) const;
    bool operator!=(const Polygon &other) const;

    // Print
    friend std::ostream &operator<<(std::ostream &os, const Polygon &p) {
        os << "Vertices: ";
        for (const auto &vertex: p.vertices) {
            os << vertex << " ";
        }
        return os;
    }
};


#endif // PLANE_H