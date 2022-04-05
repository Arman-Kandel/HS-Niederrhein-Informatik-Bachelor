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
    Node<T> *root = NULL;
    void addValue(Node<T>* n, T* val){
        if (n->value == NULL) {
            n->value = *val;
            return;
        }
        if(n->value < *val){
            if(n->left) addValue(n->left, val);
            else {
                Node<T> child;
                child.value = *val;
                child.parent = n;
                n->left = &child;
            }
        }
        else{
            if(n->right) addValue(n->right, val);
            else {
                Node<T> child;
                child.value = *val;
                child.parent = n;
                n->right = &child;
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
        if(root == NULL) {
            Node<T> newRoot;
            newRoot.value = val;
            root = &newRoot;
            std::cout << "SASDDSA\n";
            std::cout << root->value << "\n";
        }
        else addValue(root, &val);
    }

    void print(){
        printNode(root);
    }
};


#endif //U3_SEARCHTREE_SEARCHTREE_H
