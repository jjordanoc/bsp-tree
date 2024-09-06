//
// Created by Joaquin on 5/09/24.
//
#include "BSPTree.h"

void BSPNode::insert(const Polygon &polygon) {
    // determine on which side of the plane the current polygon is
    auto relation = polygon.relationWithPlane(partition);
    auto cnt = getPolygonsCount();
    switch (relation) {
        case COINCIDENT:
            // insert in this node
            polygons.push_back(polygon);
            break;
        case IN_FRONT:
            // insert recursively
            if (front == nullptr) {
                front = new BSPNode(polygon.getPlane());
            }
            front->insert(polygon);
            break;
        case BEHIND:
            if (back == nullptr) {
                back = new BSPNode(polygon.getPlane());
            }
            back->insert(polygon);
            break;
        case SPLIT:
            auto [frontPart, backPart] = polygon.split(partition);
            front->insert(frontPart);
            back->insert(backPart);
            break;
    }
}

void BSPTree::insert(const Polygon &polygon) {
    if (root == nullptr) {
        root = new BSPNode(polygon.getPlane());
//        root->setPartition()
    }
    root->insert(polygon);
}
