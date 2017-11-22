//
// Created by liang on 17/11/21.
//

#include <iostream>
#include "Node.h"
using namespace std;

Node::Node() {
    index = 0;
    data = 0;
    pParent = NULL;
    pLChild = NULL;
    pRChild = NULL;
}

Node *Node::SearchNode(int nodeIndex) {
    if (this->index == nodeIndex) {
        cout << "Search found: " << this->index << endl;
        return this;
    }

    Node *temp = NULL;
    if (this->pLChild != NULL) {
        temp = this->pLChild->SearchNode(nodeIndex);
        if (NULL != temp)
            return temp;
    }

    if (this->pRChild != NULL) {
        temp = this->pRChild->SearchNode(nodeIndex);
        if (NULL != temp)
            return temp;
    }

    return NULL;
}

void Node::DeleteNode() {
    if (this->pLChild != NULL) {
        this->pLChild->DeleteNode();
    }
    if (this->pRChild != NULL) {
        this->pRChild->DeleteNode();
    }

    if (this->pParent != NULL) {
        if (this->pParent->pLChild == this) {
            this->pParent->pLChild = NULL;
        }
        if (this->pParent->pRChild == this) {
            this->pParent->pRChild = NULL;
        }
    }
    delete this;
}

void Node::PreorderTraversal() {
    cout << this->index << "  --  " << this->data << endl;

    if (this->pLChild != NULL) {
        this->pLChild->PreorderTraversal();
    }
    if (this->pRChild != NULL) {
        this->pRChild->PreorderTraversal();
    }
}

void Node::InorderTraversal() {
    if (this->pLChild != NULL) {
        this->pLChild->InorderTraversal();
    }

    cout << this->index << "  --  " << this->data << endl;

    if (this->pRChild != NULL) {
        this->pRChild->InorderTraversal();
    }
}

void Node::PostorderTraversal() {
    if (this->pLChild != NULL) {
        this->pLChild->PostorderTraversal();
    }
    if (this->pRChild != NULL) {
        this->pRChild->PostorderTraversal();
    }

    cout << this->index << "  --  " << this->data << endl;
}
