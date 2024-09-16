#include <cassert>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include "DataType.h"
#include "Line.h"
#include "Plane.h"
#include "BSPTree.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dis(0.0f, 1.0f);

// Funciones auxiliares para generar polígonos aleatorios
NType randomInRange(NType min, NType max) {
    return min + (max - min) * dis(gen);
}
Vector3D randomUnitVector() {
    NType theta = randomInRange(0, 2 * M_PI);
    NType phi = acos(randomInRange(-1.0f, 1.0f));
    NType x = sin(phi) * cos(theta);
    NType y = sin(phi) * sin(theta);
    NType z = cos(phi);
    return Vector3D(x, y, z);
}
Point3D randomPointInBox(NType x_min, NType x_max, NType y_min, NType y_max, NType z_min, NType z_max) {
    NType x = randomInRange(x_min, x_max);
    NType y = randomInRange(y_min, y_max);
    NType z = randomInRange(z_min, z_max);
    return Point3D(x, y, z);
}
std::pair<Vector3D, Vector3D> generateOrthogonalVectors(const Vector3D& v) {
    Vector3D a = (abs(v.getX()) > abs(v.getZ())) ? Vector3D(v.getY(), -v.getX(), 0) : Vector3D(0, -v.getZ(), v.getY());
    Vector3D b = v.crossProduct(a);
    a.normalize();
    b.normalize();
    return {a, b};
}
std::vector<Polygon> generateRandomPolygons(int n, NType x_min, NType x_max, NType y_min, NType y_max, NType z_min, NType z_max) {
    std::vector<Polygon> polygons;

    for (int i = 0; i < n; ++i) {
        Point3D P   = randomPointInBox(x_min, x_max, y_min, y_max, z_min, z_max);
        Vector3D v  = randomUnitVector();
        auto [u, w] = generateOrthogonalVectors(v);

        Plane plane(P, v);
        std::vector<Point3D> vertices;
        vertices.push_back(P);

        int numExtraPoints = 2; // 2 + (rand() % 3);
        for (int j = 0; j < numExtraPoints; ++j) {
            NType scale_u = randomInRange(-2.0, 2.0);
            NType scale_w = randomInRange(-2.0, 2.0);
            Point3D extraPoint = P + u * scale_u + w * scale_w;
            if (std::find(vertices.begin(), vertices.end(), extraPoint) == vertices.end()) {
                vertices.push_back(extraPoint);
            }
        }

        Polygon polygon(vertices);
        polygons.push_back(polygon);
    }
    return polygons;
}

// Función para verificar que los polígonos estén correctamente ubicados en el BSP-Tree
bool verifyPolygonsInNode(BSPNode* node, const std::vector<Polygon>& allPolygons) {
    if (node == nullptr) {
        return true;    // Creo que esto era un error antes :'c
    }

    // Verificar que los polígonos almacenados en el nodo son coplanares con el plano de partición
    for (const Polygon& polygon : node->getPolygons()) {
        RelationType relation = polygon.relationWithPlane(node->getPartition());  // Usar el getter
        if (relation != COINCIDENT) {
            std::cerr << "Error: El polígono almacenado en el nodo no es coplanar al plano de partición." << std::endl;
            return false;
        }
    }

    // Verificar que los polígonos que cruzan el plano están en los subárboles correctos
    for (const Polygon& polygon : allPolygons) {
        RelationType relation = polygon.relationWithPlane(node->getPartition());
        if (relation == SPLIT) {
            std::pair<Polygon, Polygon> splitPolys = polygon.split(node->getPartition());
            bool frontContains = node->getFront() && std::find(node->getFront()->getPolygons().begin(), node->getFront()->getPolygons().end(), splitPolys.first) != node->getFront()->getPolygons().end();
            bool backContains  = node->getBack() && std::find(node->getBack()->getPolygons().begin(), node->getBack()->getPolygons().end(), splitPolys.second) != node->getBack()->getPolygons().end();

            if (!frontContains || !backContains) {
                std::cerr << "Error: El polígono que debería estar dividido no está en los subárboles correspondientes." << std::endl;
                return verifyPolygonsInNode(node->getFront(), allPolygons) &&
                       verifyPolygonsInNode(node->getBack(), allPolygons);;
            }
            else {
                return true;
            }
        } else if (relation == IN_FRONT && node->getFront() && std::find(node->getFront()->getPolygons().begin(), node->getFront()->getPolygons().end(), polygon) == node->getFront()->getPolygons().end()) {
            std::cerr << "Error: Un polígono está en el lado positivo incorrecto del plano de partición." << std::endl;
            return verifyPolygonsInNode(node->getFront(), allPolygons);
        } else if (relation == BEHIND && node->getBack() && std::find(node->getBack()->getPolygons().begin(), node->getBack()->getPolygons().end(), polygon) == node->getBack()->getPolygons().end()) {
            std::cerr << "Error: Un polígono está en el lado negativo incorrecto del plano de partición." << std::endl;
            return verifyPolygonsInNode(node->getBack(), allPolygons);
        }
        else {
            return true;
        }
    }

    return verifyPolygonsInNode(node->getFront(), allPolygons) &&
           verifyPolygonsInNode(node->getBack(), allPolygons);
}


// Verificación de que no se repitan planos de partición
bool arePlanesEqual(const Plane& plane1, const Plane& plane2) {
    Vector3D normal1 = plane1.getNormal();
    Vector3D normal2 = plane2.getNormal();

    // Verificar si los vectores normales son paralelos
    Vector3D crossProduct = normal1.crossProduct(normal2);
    if (crossProduct.mag() != NType(0)) {
        return false;
    }

    // Si el punto cumple con la ecuación del plano
    Point3D pointInPlane1 = plane1.getPoint();
    Point3D pointInPlane2 = plane2.getPoint();
    Vector3D pointDifference = pointInPlane1 - pointInPlane2;

    NType dotProduct = normal2.dotProduct(pointDifference);
    if (abs(dotProduct) == NType(0)) {
        return true;
    }

    return false;
}

long glob = 0;
bool verifyUniquePartitions(BSPNode* node, std::vector<BSPNode>& usedPartitions) {
    if (!node) return true;
    BSPNode currentPartition = node->getPartition();

    // Verificar si el plano de partición ya ha sido utilizado
    for (long i = 0; i < usedPartitions.size(); ++i) {
        auto existingPartition = usedPartitions[i];

        if (arePlanesEqual(currentPartition, existingPartition)) {
            std::cout << i << " " << glob << std::endl;
            std::cerr << "Error: El plano de partición ya ha sido utilizado en otro nodo." << std::endl;
            return false;
        }
    }

    // Verificar recursivamente en los subárboles
    usedPartitions.push_back(currentPartition);
    glob++;
    return verifyUniquePartitions(node->getFront(), usedPartitions) &&
           verifyUniquePartitions(node->getBack(), usedPartitions);
}

// Verificación de que todos los nodos tengan al menos un polígono
bool verifyNonEmptyNodes(BSPNode* node) {
    if (!node) return true;
    if (node->getPolygons().empty()) {
        std::cerr << "Error: El nodo no tiene ningún polígono almacenado." << std::endl;
        return false;
    }
    return verifyNonEmptyNodes(node->getFront()) &&
           verifyNonEmptyNodes(node->getBack());
}

void testBSPTree() {
    BSPTree bspTree;

    int n_polygons = 200;
    int p_min = 0, p_max = 500;
    std::vector<Polygon> randomPolygons = generateRandomPolygons(n_polygons, p_min, p_max, p_min, p_max, p_min, p_max);
    for (const auto& polygon : randomPolygons) {
        bspTree.insert(polygon);
    }

    bool polygonsCorrectlyPlaced = verifyPolygonsInNode(bspTree.getRoot(), randomPolygons);
    assert(polygonsCorrectlyPlaced && "Error: Los polígonos no están correctamente ubicados en el BSP-Tree.");

    std::vector<Plane> usedPartitions;
    bool uniquePartitions = verifyUniquePartitions(bspTree.getRoot(), usedPartitions);
    assert(uniquePartitions && "Error: Hay planos de partición repetidos en el BSP-Tree.");

    bool nonEmptyNodes = verifyNonEmptyNodes(bspTree.getRoot());
    assert(nonEmptyNodes && "Error: Hay nodos en el BSP-Tree que no tienen ningún polígono.");

    // Happy end :D
    std::cout << "Todos los tests del BSP-Tree pasaron correctamente :D" << std::endl;
}

int main() {
    testBSPTree();
    return 0;
}