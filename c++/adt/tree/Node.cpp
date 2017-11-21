//
// Created by liang on 17/11/21.
//

#include <iostream>
#include "Node.h"

Node::Node() {
    index = 0;
    data = 0;
    pParent = NULL;
    pLChild = NULL;
    pRChild = NULL;
}

Node *Node::SearchNode(int nodeIndex) {
    if (this->index == nodeIndex)
        return this;

    if (this->pLChild != NULL)
        return this->pLChild->SearchNode(nodeIndex);

    if (this->pRChild != NULL)
        return this->pRChild->SearchNode(nodeIndex);

    return NULL;
}

