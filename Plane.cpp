//
// Created by Joaquin on 5/09/24.
//
#include "Plane.h"

Point3D Polygon::getNormal() const {
    return nullptr;
}

RelationType Polygon::relationWithPlane(const Plane &plane) const {
    long posCnt = 0, negCnt = 0, zCnt = 0;
    for (const auto &pt : vertices) {
        auto normalProduct = Vector3D(plane.getNormal()).dotProduct(Vector3D(getNormal()));
        if (normalProduct > 0) {
            posCnt++;
        }
        else if (normalProduct < 0) {
            negCnt++;
        }
        else {
            zCnt++;
        }
    }
    if (zCnt == vertices.size()) {
        return COINCIDENT;
    }
    else if (posCnt == vertices.size()) {
        return IN_FRONT;
    }
    else if (negCnt == vertices.size()) {
        return BEHIND;
    }
    else {
        return SPLIT;
    }
}

