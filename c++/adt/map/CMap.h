//
// Created by liang on 17/11/23.
//

#ifndef MAP_CMAP_H
#define MAP_CMAP_H

#include "Node.h"
#include <vector>
using namespace std;

class CMap
{
public:
    CMap(int capacity);
    ~CMap();
    bool addNode(Node *pNode);
    void resetNode();
    bool setValueToMatrixForDirectedGraph(int row, int col, int val=1);
    bool setValueToMatrixForUndirectedGraph(int row, int col, int val=1);

    void printMatrix();

    void depthFirstTraverse(int nodeIndex);
    void breadthFirstTraverse(int nodeIndex);
private:
    Node *m_pNodeArray;
    int *m_pMatrix;
    int m_iCapacity;
    int m_iNodeCount;
    bool getValueFromMatrix(int row, int col, int &value);
    void breadthFirstTraverseImpl(vector<int> preVec);

};

#endif //MAP_CMAP_H
