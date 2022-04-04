//
// Created by arman on 04.04.2022.
//

#ifndef U3_SEARCHTREE_SEARCHTREE_H
#define U3_SEARCHTREE_SEARCHTREE_H


#include <iostream>
#include "Node.h"

template<typename T>
class SearchTree {
private:
    Node<T> *root;
    void addValue(Node<T>* n, T* val){
        int a = 10;
        int* ap = &a;
        a = *ap;
        if(*n->value < *val){
            if(n->left) addValue(n->left, val);
            else {
                Node<T> child(val);
                n->left = &child;
                n->left->parent = n;
            }
        }
        else{
            if(n->right) addValue(n->right, val);
            else {
                Node<T> child(val);
                n->right = &child;
                n->right->parent = n;
            }
        }
    }
    void printNode(Node<T> n){
        std::cout << n->value << " p:" << n->parent->value << " l:" << n->left->value << " r:" << n->right->value << "\n";
        printNode(n.left);
        printNode(n.right);
    }

public:

    void add(T val){
        if(!root) root = new Node<T>(&val);
        else addValue(root, &val);
    }

    void print(){
        printNode(root);
    }
};


#endif //U3_SEARCHTREE_SEARCHTREE_H
