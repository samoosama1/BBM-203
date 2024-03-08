#include <stack>
#include <queue>
#include "SpaceSectorBST.h"

using namespace std;

void print(Sector* treeNode);

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {
    // Free any dynamically allocated memory in this class.
    if(root != nullptr)
        delete root;
}

void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {
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

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    // Instantiate and insert a new sector into the space sector BST map according to the 
    // coordinates-based comparison criteria.
    Sector* sector = new Sector{x, y, z};
    recurInsert(root, sector);
}

void SpaceSectorBST::deleteSector(const std::string& sector_code) {
    recurDelete(root, sector_code);
}

void SpaceSectorBST::displaySectorsInOrder() {
    std::cout << "Space sectors inorder traversal:\n";
    inorder(root, print);
    std::cout << "\n";
}

void SpaceSectorBST::displaySectorsPreOrder() {
    std::cout << "Space sectors preorder traversal:\n";
    preorder(root, print);
    std::cout << "\n";
}

void SpaceSectorBST::displaySectorsPostOrder() {
    std::cout << "Space sectors postorder traversal:\n";
    postorder(root, print);
    std::cout << "\n";
}

std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    stack<Sector*> s;
    Sector* node = nullptr;
    recurPath(root, node, sector_code);
    while(node != nullptr) {
        s.push(node);
        node = node->parent;
    }
    while(!s.empty()) {
        path.push_back(s.top());
        s.pop();
    }
    return path;
}

void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {
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

void SpaceSectorBST::recurInsert(Sector*& treeNode, Sector* newSector) {
    if(treeNode == nullptr)
        treeNode = newSector;
    else if(*newSector < *treeNode) {
        recurInsert(treeNode->left, newSector);
        treeNode->left->parent = treeNode;
    }
    else if (*newSector > *treeNode) {
        recurInsert(treeNode->right, newSector);
        treeNode->right->parent = treeNode;
    }
}

bool SpaceSectorBST::recurDelete(Sector *&treeNode, const std::string &sector_code) {
    if(treeNode == nullptr)
        return false;
    if (treeNode->sector_code == sector_code) {
        deleteNode(treeNode);
        return true;
    }
    if(!recurDelete(treeNode->left, sector_code)) {
        return recurDelete(treeNode->right, sector_code);
    }
    return true;
}

void SpaceSectorBST::deleteNode(Sector *&treeNode) {
    Sector* temp;
    if(treeNode->left == nullptr && treeNode->right == nullptr) {
        delete treeNode;
        treeNode = nullptr;
    } else if(treeNode->left == nullptr) {
        temp = treeNode;
        treeNode = treeNode->right;
        treeNode->parent = temp->parent;
        temp->right = nullptr;
        delete temp;
    } else if(treeNode->right == nullptr) {
        temp = treeNode;
        treeNode = treeNode->left;
        treeNode->parent = temp->parent;
        temp->left = nullptr;
        delete temp;
    } else {
        Sector*& succ = getInOrderSucc(treeNode->right);
        temp = treeNode;
        treeNode = succ;
        treeNode->left = temp->left;
        if(temp->right != succ) {
            treeNode->right = temp->right;
        }
        if(treeNode->right != nullptr) {
            treeNode->right->parent = treeNode;
        }
        if(treeNode->left != nullptr) {
            treeNode->left->parent = nullptr;
        }
        treeNode->parent = temp->parent;
        temp->left = nullptr;
        temp->right = nullptr;
        succ = nullptr;
        delete temp;
    }
}

Sector *&SpaceSectorBST::getInOrderSucc(Sector*& treeNode) {
    if(treeNode->left == nullptr) {
        return treeNode;
    }
    return getInOrderSucc(treeNode->left);
}

void SpaceSectorBST::inorder(Sector *treeNode, void (*callBack)(Sector *)) {
    if(treeNode == nullptr)
        return;
    inorder(treeNode->left, callBack);
    callBack(treeNode);
    inorder(treeNode->right, callBack);
}

void SpaceSectorBST::preorder(Sector *treeNode, void (*callBack)(Sector *)) {
    if(treeNode == nullptr)
        return;
    callBack(treeNode);
    preorder(treeNode->left, callBack);
    preorder(treeNode->right, callBack);
}

void SpaceSectorBST::postorder(Sector *treeNode, void (*callBack)(Sector *)) {
    if(treeNode == nullptr)
        return;
    postorder(treeNode->left, callBack);
    postorder(treeNode->right, callBack);
    callBack(treeNode);
}

bool SpaceSectorBST::recurPath(Sector *treeNode, Sector*& nodeToFind, const string &sector_code) {
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

void print(Sector* treeNode) {
    std::cout << treeNode->sector_code << "\n";
}
