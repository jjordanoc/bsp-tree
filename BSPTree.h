#ifndef BSP_H
#define BSP_H

#include "DataType.h"
#include "Point.h"
#include "Line.h"
#include "Plane.h"
#include <vector>

class BSPNode {
private:
    BSPNode *front;
    BSPNode * back;
    Plane partition;
    std::vector<Polygon> polygons;

public:
    BSPNode(const Plane &partition) : partition(partition), front(nullptr), back(nullptr) {}
    ~BSPNode() {
        delete front;
        delete back;
    }

    // Insert a polygon into the subtree (node)
    void insert(const Polygon &polygon);

    // Getters
    BSPNode *getFront() const { return front; }
    BSPNode *getBack() const { return back; }
    Plane getPartition() const { return partition; }
    std::vector<Polygon> getPolygons() const { return polygons; }

    // Setters
    void setFront(BSPNode *front) { this->front = front; }
    void setBack(BSPNode *back) { this->back = back; }
    void setPartition(Plane partition) { this->partition = partition; }
    void setPolygons(std::vector<Polygon> polygons) { this->polygons = polygons; }

    // Detect collision with a line
    const Polygon* detectCollision(const LineSegment& traceLine) const;

    // Get number of polygons in the subtree
    size_t getPolygonsCount() const {
        size_t count = polygons.size();
        if (front) {
            count += front->getPolygonsCount();
        }
        if (back) {
            count += back->getPolygonsCount();
        }
        return count;
    }
};


class BSPTree {
private:
    BSPNode *root;

public:
    BSPTree() : root(nullptr) {}
    ~BSPTree() {
        delete root;
    }

    // Getters
    BSPNode *getRoot() const { return root; }
    size_t   getRootPolygonsCount() const { return root ? root->polygons.size() : 0; }

    // Setters
    void setRoot(BSPNode *root) { this->root = root; }

    // Insert a polygon into the tree
    void insert(const Polygon &polygon);

    // Detect collision with a line
    const Polygon* detectCollision(const LineSegment& traceLine) const{
        return root ? root->detectCollision(traceLine) : nullptr;
    }

    // Get number of polygons in the tree
    size_t getRootPolygonsCount() const { return root ? root->polygons.size() : 0; }

    // Check if the tree is empty
    bool isEmpty() const { return root == nullptr; }
};

#endif // BSP_H