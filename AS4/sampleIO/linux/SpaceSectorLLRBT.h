#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

class SpaceSectorLLRBT {
public:
    Sector* root;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    Sector* recurInsert(Sector* treeNode, Sector* newSector);
    Sector* rotateLeft(Sector* treeNode);
    Sector* rotateRight(Sector* treeNode);
    void inorder(Sector* treeNode, void (*callBack)(Sector* node));
    void preorder(Sector* treeNode, void (*callBack)(Sector* node));
    void postorder(Sector* treeNode, void (*callBack)(Sector* node));
    void flipColors(Sector* treeNode);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    bool recurPath(Sector *treeNode, Sector*& nodeToFind, const std::string &sector_code);
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    std::vector<Sector*> getNodePath(Sector* treeNode);
    void printStellarPath(const std::vector<Sector*>& path);
};

#endif // SPACESECTORLLRBT_H
