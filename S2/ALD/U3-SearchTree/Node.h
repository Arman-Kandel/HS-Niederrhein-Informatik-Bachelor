//
// Created by arman on 04.04.2022.
//

#ifndef U3_SEARCHTREE_NODE_H
#define U3_SEARCHTREE_NODE_H


template<typename T>
class Node {
public:
    T value;
    Node* left;
    Node* right;
    Node* parent; // optional
};


#endif //U3_SEARCHTREE_NODE_H
