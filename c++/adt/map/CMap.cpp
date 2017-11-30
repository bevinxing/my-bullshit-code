//
// Created by liang on 17/11/23.
//

#include "CMap.h"
#include <string.h>
#include <iostream>
#include <vector>
using namespace std;

CMap::CMap(int capacity) {
    m_iCapacity = capacity;
    m_pNodeArray = new Node[m_iCapacity];
    m_iNodeCount = 0;
    m_pMatrix = new int[m_iCapacity * m_iCapacity];
    memset(m_pMatrix, 0, m_iCapacity * m_iCapacity * sizeof(int));
}

CMap::~CMap() {
    delete []m_pNodeArray;
    delete []m_pMatrix;
}

bool CMap::addNode(Node *pNode) {
    if (m_iNodeCount >= m_iCapacity)
        return false;
    m_pNodeArray[m_iNodeCount].m_cData = pNode->m_cData;
    m_iNodeCount++;
    return true;
}

void CMap::resetNode() {
    for(int i = 0; i < m_iNodeCount; i++) {
        m_pNodeArray[i].m_bIsVisited = false;
    }
}

bool CMap::setValueToMatrixForDirectedGraph(int row, int col, int val) {
    if (row >= m_iNodeCount || col >= m_iNodeCount) {
        return false;
    }
    m_pMatrix[row * m_iCapacity + col] = val;
    return true;
}

bool CMap::setValueToMatrixForUndirectedGraph(int row, int col, int val) {
    if (row >= m_iNodeCount || col >= m_iNodeCount) {
        return false;
    }
    m_pMatrix[row * m_iCapacity + col] = val;
    m_pMatrix[col * m_iCapacity + row] = val;
    return true;
}

bool CMap::getValueFromMatrix(int row, int col, int &value) {
    if (row >= m_iCapacity || col >= m_iCapacity)
        return false;
    value = m_pMatrix[row * m_iCapacity + col];
    return true;
}

void CMap::printMatrix() {
    for (int i = 0; i < m_iCapacity; i++) {
        for (int j = 0; j < m_iCapacity; j++) {
            cout << m_pMatrix[i * m_iCapacity + j] << " ";
        }
        cout << endl;
    }
}

void CMap::depthFirstTraverse(int nodeIndex) {
    cout << m_pNodeArray[nodeIndex].m_cData << " ";
    m_pNodeArray[nodeIndex].m_bIsVisited = true;

    for (int i = 0; i < m_iCapacity; i++) {
        int value = 0;
        if (!m_pNodeArray[i].m_bIsVisited)
            getValueFromMatrix(nodeIndex, i, value);
        if (value) {
            depthFirstTraverse(i);
        }
    }
}

void CMap::breadthFirstTraverse(int nodeIndex) {
    cout << m_pNodeArray[nodeIndex].m_cData << " ";
    m_pNodeArray[nodeIndex].m_bIsVisited = true;
    
    vector<int> curVec;
    curVec.push_back(nodeIndex);
    breadthFirstTraverseImpl(curVec);
}

void CMap::breadthFirstTraverseImpl(vector<int> preVec) {
    vector<int> curVec;

    for (int i = 0; i < preVec.size(); i++) {
        for (int j = 0; j < m_iCapacity; j++) {
            int value = 0;
            getValueFromMatrix(preVec[i], j, value);
            if (1 == value) {
                if (m_pNodeArray[j].m_bIsVisited)
                    continue;
                else {
                    cout << m_pNodeArray[j].m_cData << " ";
                    m_pNodeArray[j].m_bIsVisited = true;
                    curVec.push_back(j);
                }
            } else
                continue;
        }
    }

    if (0 != curVec.size())
        breadthFirstTraverseImpl(curVec);
}


