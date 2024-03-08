#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

#include "Sector.h"

class SpaceSectorBST {
  
public:
    Sector *root;
    SpaceSectorBST();
    ~SpaceSectorBST();
    void readSectorsFromFile(const std::string& filename); 
    void insertSectorByCoordinates(int x, int y, int z);
    void recurInsert(Sector*& treeNode, Sector* newSector);
    void deleteSector(const std::string& sector_code);
    bool recurDelete(Sector*& treeNode, const std::string &sector_code);
    void deleteNode(Sector*& treeNode);
    Sector*& getInOrderSucc(Sector*& treeNode);
    void displaySectorsInOrder();
    void inorder(Sector* treeNode, void (*callBack)(Sector* node));
    void displaySectorsPreOrder();
    void preorder(Sector* treeNode, void (*callBack)(Sector* node));
    void displaySectorsPostOrder();
    void postorder(Sector* treeNode, void (*callBack)(Sector* node));
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    bool recurPath(Sector *treeNode, Sector*& nodeToFind, const std::string &sector_code);
    void printStellarPath(const std::vector<Sector*>& path);
    void deleteTree(Sector* node);
};

#endif // SPACESECTORBST_H
