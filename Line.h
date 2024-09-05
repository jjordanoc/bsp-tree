#ifndef LINE_H
#define LINE_H

#include "DataType.h"
#include "Point.h"

class Line;
class Vector3D;

class LineSegment {
private:
    Point3D p1;
    Point3D p2;

public:
    LineSegment() : p1(), p2() {}
    LineSegment(Point3D p1, Point3D p2) : p1(p1), p2(p2) {}
    ~LineSegment() = default;

    Point3D getP1() const { return p1; }
    Point3D getP2() const { return p2; }

    void setP1(Point3D p1) { this->p1 = p1; }
    void setP2(Point3D p2) { this->p2 = p2; }

    NType length() const {
        return p1.distance(p2);
    }
    bool operator==(const LineSegment &l) const {
        return p1 == l.p1 && p2 == l.p2;
    }
    bool operator!=(const LineSegment &l) const {
        return !(*this == l);
    }

    // Linea que contiene al segmento
    Line getLine() const;

    // Imprimir
    friend std::ostream &operator<<(std::ostream &os, const LineSegment &l) {
        os << "[" << l.p1 << " to " << l.p2 << "]";
        return os;
    }
};

class Vector3D : public Point3D {
public:
    Vector3D() : Point3D(0, 0, 0) {}
    Vector3D(NType x, NType y, NType z) : Point3D(x, y, z) {}
    Vector3D(const Point3D &p) : Point3D(p) {}
    ~Vector3D() = default;

    // Producto Punto
    NType dotProduct(const Vector3D &v) const {
        return getX() * v.getX() + getY() * v.getY() + getZ() * v.getZ();
    }
    NType dotProduct(const Point3D &p) const {
        return getX() * p.getX() + getY() * p.getY() + getZ() * p.getZ();
    }

    // Producto Cruz
    Vector3D crossProduct(const Vector3D &v) const {
        return Vector3D(
                getY() * v.getZ() - getZ() * v.getY(),
                getZ() * v.getX() - getX() * v.getZ(),
                getX() * v.getY() - getY() * v.getX()
        );
    }

    // Operadores aritméticos
    Vector3D operator-() const {
        return Vector3D(-getX(), -getY(), -getZ());
    }
    Vector3D operator*(const NType k) const {
        return Vector3D(getX() * k, getY() * k, getZ() * k);
    }
    Vector3D operator/(const NType k) const {
        return Vector3D(getX() / k, getY() / k, getZ() / k);
    }
    Vector3D operator+(const Vector3D &v) const {
        return Vector3D(getX() + v.getX(), getY() + v.getY(), getZ() + v.getZ());
    }
    Vector3D operator-(const Vector3D &v) const {
        return Vector3D(getX() - v.getX(), getY() - v.getY(), getZ() - v.getZ());
    }
    Vector3D &operator+=(const Vector3D &v) {
        setX(getX() + v.getX());
        setY(getY() + v.getY());
        setZ(getZ() + v.getZ());
        return *this;
    }
    Vector3D &operator-=(const Vector3D &v) {
        setX(getX() - v.getX());
        setY(getY() - v.getY());
        setZ(getZ() - v.getZ());
        return *this;
    }
    Vector3D &operator*=(NType k) {
        setX(getX() * k);
        setY(getY() * k);
        setZ(getZ() * k);
        return *this;
    }
    Vector3D &operator/=(NType k) {
        setX(getX() / k);
        setY(getY() / k);
        setZ(getZ() / k);
        return *this;
    }

    // Magnitud y dirección
    NType mag() const {
        return sqrt(getX() * getX() + getY() * getY() + getZ() * getZ());
    }
    Vector3D unit() const {
        return *this / mag();
    }

    // Normalizar el vector
    void normalize() {
        *this /= mag();
    }

    // Imprimir
    friend std::ostream &operator<<(std::ostream &os, const Vector3D &v) {
        os << "{" << v.getX().getValue() << "," << v.getY().getValue() << "," << v.getZ().getValue() << "}";
        return os;
    }
};


class Line {
private:
    Point3D  p;   // Punto de la recta
    Vector3D v;   // Vector unitario (dirección) de la recta
public:
    Line() : p(), v() {}
    Line(Point3D p, Vector3D v) : p(p), v(v.unit()) {}
    Line(Point3D p1, Point3D p2) : p(p1), v(Vector3D(p2 - p1).unit()) {}
    Line(const LineSegment &l) : p(l.getP1()), v(Vector3D(l.getP2() - l.getP1()).unit()) {}
    ~Line() = default;

    Point3D  getPoint() const { return p; }
    Vector3D getUnit () const { return v; }

    void setPoint (Point3D p) { this->p = p; }
    void setVector(Vector3D v) { this->v = v.unit(); }

    // Paralinealidad
    bool isParallel(const Line        &l) const;
    bool isParallel(const Vector3D    &v) const;
    bool isParallel(const LineSegment &l) const;

    // Ortogonalidad
    bool isOrthogonal(const Line        &l) const;
    bool isOrthogonal(const Vector3D    &v) const;
    bool isOrthogonal(const LineSegment &l) const;

    // Imprimir
    friend std::ostream &operator<<(std::ostream &os, const Line &l) {
        os << "P:" << l.p << " V:" << l.v;
        return os;
    }

};

#endif // LINE_H