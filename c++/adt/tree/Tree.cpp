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
    m_pRoot->DeleteNode();
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
        cout << "Add LChild node index: " << nodeIndex << "  " << pNode->index << endl;
        return true;
    }

    if (1 == direction && NULL == temp->pRChild) {
        temp->pRChild = newNode;
        cout << "Add RChild node index: " << nodeIndex << "  " << pNode->index << endl;
        return true;
    }

    delete newNode;
    return false;
}

bool Tree::DeleteNode(int nodeIndex, Node *pNode) {
    Node *temp = m_pRoot->SearchNode(nodeIndex);
    if (NULL == temp)
        return false;

    if (NULL != pNode) {
        pNode->data = temp->data;
    }

    temp->DeleteNode();
    return true;
}

void Tree::PreorderTraversal() {
    m_pRoot->PreorderTraversal();
}

void Tree::InorderTraversal() {
    m_pRoot->InorderTraversal();
}

void Tree::PostorderTraversal() {
    m_pRoot->PostorderTraversal();
}

