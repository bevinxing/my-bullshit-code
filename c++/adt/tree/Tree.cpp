//
// Created by liang on 17/11/21.
//

#include "Node.h"
#include "Tree.h"
#include <iostream>

using namespace std;

Tree::Tree() {
    // Do not use the root node to save data.
    m_pRoot = new Node();
}

Tree::~Tree() {

}

Node *Tree::SearchNode(int nodeIndex) {
    return m_pRoot->SearchNode(nodeIndex);
}

bool Tree::AddNode(int nodeIndex, int direction, Node *pNode) {
    Node *temp = m_pRoot->SearchNode(nodeIndex);
    if (NULL == temp)
        return false;

    Node *newNode = new Node();
    if (NULL == newNode)
        return false;
    newNode->index = pNode->index;
    newNode->data = pNode->data;
    newNode->pParent = temp;

    if (0 == direction && NULL == temp->pLChild) {
        temp->pLChild = newNode;
        return true;
    }

    if (1 == direction && NULL == temp->pRChild) {
        temp->pRChild = newNode;
        return true;
    }

    delete newNode;
    return false;
}

