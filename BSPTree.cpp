//
// Created by Joaquin on 5/09/24.
//
#include "BSPTree.h"
#include <stack>

void BSPNode::insert(const Polygon &polygon) {
    // determine on which side of the plane the current polygon is
    auto relation = polygon.relationWithPlane(partition);
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
            front->setParent(this);
            front->insert(polygon);
            break;
        case BEHIND:
            if (back == nullptr) {
                back = new BSPNode(polygon.getPlane());
            }
            back->setParent(this);
            back->insert(polygon);
            break;
        case SPLIT:
            auto [frontPart, backPart] = polygon.split(partition);
            if (front == nullptr) {
                front = new BSPNode(frontPart.getPlane());
            }
            front->setParent(this);
            front->insert(frontPart);
            if (back == nullptr) {
                back = new BSPNode(backPart.getPlane());
            }
            back->setParent(this);
            back->insert(backPart);
            break;
    }
}

const Polygon *BSPNode::detectCollision(const LineSegment &traceLine) const {
    auto p0 = traceLine.getP1();
    auto pf = traceLine.getP2();


    // if same node, nullptr

    // else, find common ancestor

//   if (parent->getPartition().inPositiveSide(p0) && parent->getPartition().inPositiveSide(pf)) {
//       // node is common ancestor
//       if ()
//   }
//   else {
//       return parent->detectCollision(traceLine);
//   }
}

BSPNode *BSPNode::visibilityOrder(const Point3D &point) {
    if (front == nullptr && back == nullptr) {
        return this;
    }
    if (partition.inPositiveSide(point)) {
        return front->visibilityOrder(point);
    }
    else {
        return back->visibilityOrder(point);
    }
}

BSPNode *BSPNode::getFirstCommonAncestor(BSPNode *node1, BSPNode *node2) {
    std::stack<BSPNode *> st1, st2;
    while (node1->getParent() != nullptr) {
        st1.push(node1);
        node1 = node1->getParent();
    }
    while (node2->getParent() != nullptr) {
        st2.push(node2);
        node2 = node2->getParent();
    }
    BSPNode *firstCommonAncestor = st1.top();
    while (!st1.empty() && !st2.empty() && st1.top() == st2.top()) {
        st1.pop();
        st2.pop();
    }
    return firstCommonAncestor;
}


void BSPTree::insert(const Polygon &polygon) {
    if (root == nullptr) {
        root = new BSPNode(polygon.getPlane());
    }
    root->insert(polygon);
}

const Polygon *BSPTree::detectCollision(const LineSegment &traceLine) const {
    if (root == nullptr) return nullptr;
    // get nodo for p1 and p2
    auto node0 = root->visibilityOrder(traceLine.getP1());
    auto nodef = root->visibilityOrder(traceLine.getP2());
    // if its the same node, null
    if (node0 == nodef) return nullptr;
    // else, find common ancestor
    auto ancestor = BSPNode::getFirstCommonAncestor(node0, nodef);

    return root ? root->detectCollision(traceLine) : nullptr;
}
