//
// Created by Joaquin on 5/09/24.
//
#include "Plane.h"

Vector3D Polygon::getNormal() const {
    // use third vertex as reference point
    auto p0 = Vector3D(vertices[0] - vertices[2]);
    auto p1 = Vector3D(vertices[1] - vertices[2]);
    return p0.crossProduct(p1);
}

RelationType Polygon::relationWithPlane(const Plane &plane) const {
    long posCnt = 0, negCnt = 0, zCnt = 0;

    for (size_t i = 0; i < vertices.size(); ++i) {
        std::cout << plane.getNormal() << " " << getNormal() << std::endl;
        // TODO: usar otro punto para el plano, o cambiar de punto
        auto normalProduct = plane.getNormal().dotProduct(getVertex(i) - plane.getPoint());
        std::cout << normalProduct << std::endl;
        if (normalProduct > 0) {
            posCnt++;
        } else if (normalProduct < 0) {
            negCnt++;
        } else {
            zCnt++;
        }
    }
    if (zCnt == vertices.size()) {
        return COINCIDENT;
    } else if (posCnt + zCnt == vertices.size()) {
        return IN_FRONT;
    } else if (negCnt + zCnt == vertices.size()) {
        return BEHIND;
    } else {
        return SPLIT;
    }
}

std::pair<Polygon, Polygon> Polygon::split(const Plane &plane) const {
    size_t numVertices = vertices.size();
    std::map<size_t, Point3D> intersectionPoints;
    std::vector<NType> products;
    std::vector<Point3D> polyPtsPos, polyPtsNeg;
    for (size_t i = 0; i < numVertices; ++i) {
        auto normalProduct = plane.getNormal().dotProduct(getVertex(i) - plane.getPoint());
        products.push_back(normalProduct);
    }
    for (size_t i = 0; i < numVertices; ++i) {
        if (products[i] > 0) {
            polyPtsPos.push_back(getVertex(i));
        } else {
            polyPtsNeg.push_back(getVertex(i));
        }
        // different signs mean plane intersection
        if (products[i] * products[nextVertexIndex(i)] < 0) {
            auto intersectionPoint = plane.intersect(Line(getVertex(i), getVertex(nextVertexIndex(i))));
            polyPtsPos.push_back(intersectionPoint);
            polyPtsNeg.push_back(intersectionPoint);
        }
    }
    return {Polygon(polyPtsPos), Polygon(polyPtsNeg)};
}

Plane Polygon::getPlane() const {
    return Plane(vertices[2], getNormal());
}

bool Polygon::operator==(const Polygon &other) const {
    for (const auto &pt: vertices) {
        if (pt != vertices[0]) {
            return false;
        }
    }
    return true;
}

Point3D Plane::intersect(const Line &l) const {
    auto v = l.getUnit();
    auto p0 = Vector3D(l.getPoint());
    auto r0 = Vector3D(getPoint());
    auto n = getNormal();
    NType t = n.dotProduct(r0 - p0) / n.dotProduct(v);
    return Point3D(p0 + (v * t));
}


