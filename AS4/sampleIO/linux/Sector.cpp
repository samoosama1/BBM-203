#include <cmath>
#include <string>
#include "Sector.h"

// Constructor implementation
Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
    // Earths coordinates (0, 0, 0)
    distance_from_earth = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    long roundDist = (long) (distance_from_earth);
    sector_code.append(std::to_string(roundDist));
    sector_code += x == 0 ? 'S' : x > 0 ? 'R' : 'L';
    sector_code += y == 0 ? 'S' : y > 0 ? 'U' : 'D';
    sector_code += z == 0 ? 'S' : z > 0 ? 'F' : 'B';
}

Sector::~Sector() {
    if(left != nullptr)
        delete left;
    if(right != nullptr)
        delete right;
}

Sector& Sector::operator=(const Sector& other) {
    if(this != &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        distance_from_earth = other.distance_from_earth;
        sector_code = other.sector_code;
        left = other.left;
        right = other.right;
        parent = other.parent;
        color = other.color;
    }
    return *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator<(const Sector &other) const {
    if((x < other.x) || (x == other.x && y < other.y) || (y == other.y && z < other.z))
        return true;
    return false;
}

bool Sector::operator>(const Sector &other) const {
    return *this != other && !(*this < other);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}