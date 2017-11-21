//
// Created by liang on 17/11/21.
//

#ifndef TREE_NODE_H
#define TREE_NODE_H

class Node
{
public:
    Node();
    Node *SearchNode(int nodeIndex);
    int index;
    int data;
    Node *pParent;
    Node *pLChild;
    Node *pRChild;

};

#endif //TREE_NODE_H
