//
// Created by ramos on 11/21/2023.
//

#ifndef GAME_PROJECT_TREE_H
#define GAME_PROJECT_TREE_H

#include <iostream>
#include <string>

struct Tree{
    int value;

    Tree * lChild;
    Tree * rChild;
};

Tree * make (int value, Tree * lChild = nullptr, Tree * rChild = nullptr);
Tree * read(std::istream &stream);
void print(Tree * root, std::ostream &stream);

#endif //GAME_PROJECT_TREE_H
