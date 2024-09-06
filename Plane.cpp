//
// Created by Joaquin on 5/09/24.
//
#include "Plane.h"

Point3D Polygon::getNormal() const {
    return nullptr;
}

RelationType Polygon::relationWithPlane(const Plane &plane) const {
    long posCnt = 0, negCnt = 0, zCnt = 0;
    for (const auto &pt: vertices) {
        auto normalProduct = Vector3D(plane.getNormal()).dotProduct(Vector3D(getNormal()));
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
    } else if (posCnt == vertices.size()) {
        return IN_FRONT;
    } else if (negCnt == vertices.size()) {
        return BEHIND;
    } else {
        return SPLIT;
    }
}

std::pair<Polygon, Polygon> Polygon::split(const Plane &plane) const {
    std::vector<Point3D> intersectionPoints;
    std::vector<NType> products;
    std::vector<bool> createsPoint;
    std::vector<Point3D> polyPtsPos, polyPtsNeg;
    for (const auto &pt: vertices) {
        auto normalProduct = Vector3D(plane.getNormal()).dotProduct(Vector3D(getNormal()));
        products.push_back(normalProduct);
    }

    for (size_t i = 0; i < vertices.size() - 1; ++i) {
        // different signs mean plane intersection
        if (products[i] * products[i + 1] < 0) {
            auto intersectionPoint = plane.intersect(Line(vertices[i], vertices[i + 1]));
            intersectionPoints.push_back(intersectionPoint);
            createsPoint[i] = true;
            createsPoint[i+1] = true;
        }
    }
    size_t j = 0;
    size_t k = intersectionPoints.size() - 1;
    // create polygon with positives and other with negatives
    for (size_t i = 0; i < vertices.size(); ++i) {
        // iterate over positive and alternate
        if (products[i] > 0) {
            polyPtsPos.push_back(vertices[i]);
            if (createsPoint[i]) {
                polyPtsPos.push_back(intersectionPoints[j++]);
            }
        }
        else {
            polyPtsNeg.push_back(vertices[i]);
            if (createsPoint[i]) {
                polyPtsPos.push_back(intersectionPoints[k--]);
            }
        }
    }
    return {Polygon(polyPtsPos), Polygon(polyPtsNeg)};
}

Point3D Plane::intersect(const Line &l) const {
    return Point3D();
}


