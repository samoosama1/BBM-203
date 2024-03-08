#include <queue>
#include <stack>
#include "SpaceSectorLLRBT.h"

using namespace std;

void printSector(Sector* treeNode);

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    ifstream ifs{filename};
    string line;
    getline(ifs, line);
    while(getline(ifs, line)) {
        if(line == "")
            continue;
        queue<int> q;
        string coord;
        stringstream ss{line};
        while(getline(ss, coord, ',')) {
            q.push(stoi(coord));
        }
        int x = q.front(); q.pop();
        int y = q.front(); q.pop();
        int z = q.front(); q.pop();
        insertSectorByCoordinates(x, y, z);
    }
}

// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    // TODO: Free any dynamically allocated memory in this class.
    delete root;
}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    // TODO: Instantiate and insert a new sector into the space sector LLRBT map 
    // according to the coordinates-based comparison criteria.
    Sector* sector = new Sector{x, y, z};
    root = recurInsert(root, sector);
    root->color = BLACK;
}

void SpaceSectorLLRBT::displaySectorsInOrder() {
    std::cout << "Space sectors inorder traversal:\n";
    inorder(root, printSector);
    std::cout << "\n";
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    std::cout << "Space sectors preorder traversal:\n";
    preorder(root, printSector);
    std::cout << "\n";
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    std::cout << "Space sectors postorder traversal:\n";
    postorder(root, printSector);
    std::cout << "\n";
}

std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    Sector* earth = nullptr;
    Sector* target = nullptr;
    recurPath(root, earth, "0SSS");
    recurPath(root, target, sector_code);
    std::vector<Sector*> earthPath = getNodePath(earth);
    std::vector<Sector*> targetPath = getNodePath(target);
    std::vector<Sector*> common;
    std::vector<Sector*> path;
    if(targetPath.empty())
        return path;
    for (int i = 0; i < earthPath.size() && i < targetPath.size() && earthPath.at(i) == targetPath.at(i); ++i) {
        common.push_back(targetPath.at(i));
    }
    Sector* intersection = common.back();
    for (int i = 0; i < common.size(); ++i) {
        earthPath.erase(earthPath.begin());
        targetPath.erase(targetPath.begin());
    }
    for (auto rit = earthPath.rbegin(); rit != earthPath.rend() ; ++rit) {
        path.push_back(*rit);
    }

    path.push_back(intersection);
    for (Sector* sector : targetPath) {
        path.push_back(sector);
    }
    return path;
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
    if(path.empty()) {
        cout << "A path to Dr. Elara could not be found.\n";
        return;
    }
    cout << "The stellar path to Dr. Elara: ";
    auto it = path.begin();
    while(it != path.end()) {
        cout << (*it)->sector_code;
        it++;
        if(it != path.end()) {
            cout << "->";
        }
    }
    cout << "\n";
}

Sector* SpaceSectorLLRBT::recurInsert(Sector *treeNode, Sector *newSector) {
    if(treeNode == nullptr)
        return newSector;
    else if(*newSector < *treeNode) {
        treeNode->left = recurInsert(treeNode->left, newSector);
        treeNode->left->parent = treeNode;
    }
    else if (*newSector > *treeNode) {
        treeNode->right = recurInsert(treeNode->right, newSector);
        treeNode->right->parent = treeNode;
    }
    if(treeNode->left != nullptr && treeNode->left->left != nullptr && treeNode->left->color && treeNode->left->left->color) {
        treeNode = rotateRight(treeNode);
    }
    if(treeNode->left != nullptr && treeNode->right != nullptr && treeNode->right->color && treeNode->left->color) {
        flipColors(treeNode);
    }
    if(treeNode->right != nullptr && treeNode->right->color) {
        treeNode = rotateLeft(treeNode);
    }
    return treeNode;
}

Sector *SpaceSectorLLRBT::rotateLeft(Sector *treeNode) {
    Sector* temp = treeNode->right;
    treeNode->right = temp->left;
    if(treeNode->right != nullptr)
        treeNode->right->parent = treeNode;
    temp->left = treeNode;
    temp->parent = treeNode->parent;
    if(temp->left != nullptr)
        temp->left->parent = temp;
    temp->color = treeNode->color;
    treeNode->color = RED;
    return temp;
}

Sector *SpaceSectorLLRBT::rotateRight(Sector *treeNode) {
    Sector* temp = treeNode->left;
    treeNode->left = temp->right;
    if(treeNode->left != nullptr)
        treeNode->left->parent = treeNode;
    temp->right = treeNode;
    temp->parent = treeNode->parent;
    if(temp->right != nullptr)
        temp->right->parent = temp;
    temp->color = treeNode->color;
    treeNode->color = RED;
    return temp;
}

void SpaceSectorLLRBT::flipColors(Sector *treeNode) {
    treeNode->color = RED;
    treeNode->right->color = BLACK;
    treeNode->left->color = BLACK;
}

void SpaceSectorLLRBT::inorder(Sector *treeNode, void (*callBack)(Sector *)) {
    if(treeNode == nullptr)
        return;
    inorder(treeNode->left, callBack);
    callBack(treeNode);
    inorder(treeNode->right, callBack);
}

void SpaceSectorLLRBT::preorder(Sector *treeNode, void (*callBack)(Sector *)) {
    if(treeNode == nullptr)
        return;
    callBack(treeNode);
    preorder(treeNode->left, callBack);
    preorder(treeNode->right, callBack);
}

void SpaceSectorLLRBT::postorder(Sector *treeNode, void (*callBack)(Sector *)) {
    if(treeNode == nullptr)
        return;
    postorder(treeNode->left, callBack);
    postorder(treeNode->right, callBack);
    callBack(treeNode);
}

bool SpaceSectorLLRBT::recurPath(Sector *treeNode, Sector *&nodeToFind, const string &sector_code) {
    if(treeNode == nullptr)
        return false;
    if (treeNode->sector_code == sector_code) {
        nodeToFind = treeNode;
        return true;
    }
    if(!recurPath(treeNode->left, nodeToFind, sector_code)) {
        return recurPath(treeNode->right, nodeToFind, sector_code);
    }
    return true;
}

std::vector<Sector *> SpaceSectorLLRBT::getNodePath(Sector *treeNode) {
    std::vector<Sector *> path;
    std::stack<Sector *> s;
    while(treeNode != nullptr) {
        s.push(treeNode);
        treeNode = treeNode->parent;
    }
    while(!s.empty()) {
        path.push_back(s.top());
        s.pop();
    }
    return path;
}

void printSector(Sector* treeNode) {
    std::cout << (treeNode->color ? "RED" : "BLACK") << " sector: " << treeNode->sector_code << "\n";
}

