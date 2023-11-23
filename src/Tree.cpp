//
// Created by ramos on 11/21/2023.
//

#include "Tree.h"


Tree * make(int value, Tree * lChild, Tree * rChild){
    return new Tree{value, lChild, rChild};
}

Tree * read(std::istream &stream){
    Tree * returnTree;
    int value;
    char c;

    stream >> c;
    stream >> c;
    if (c == ')')
        return nullptr;

    value = c - '0';

    returnTree = new Tree{value, read(stream), read(stream)};
    stream >> c;

    return returnTree;
}

void print(Tree * root, std::ostream &stream){
    if (root == nullptr){
        stream << '(' << ')';
        return;
    }

    stream << '(' << root->value;

    print(root->lChild, stream);
    print(root->rChild, stream);

    stream << ')';
}