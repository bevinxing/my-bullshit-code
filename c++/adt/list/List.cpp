#include <iostream>
#include "List.h"
using namespace std;

List::List()
{
    m_pList = new Node;
    m_pList->data = 0;
    m_pList->pNext = NULL;

    m_iLength = 0;
}

List::~List()
{
    cout << "~List()" << endl;

    ClearList();
    delete m_pList;
    m_pList = NULL;
}

void List::ClearList()
{
    Node *currentNode = m_pList->pNext;
    while(NULL != currentNode) {
        Node *temp = currentNode->pNext;
        delete currentNode;
        currentNode = temp;
    }

    m_pList->pNext = NULL;
    m_iLength = 0;
}

int List::ListLength()
{
    return m_iLength;
}

bool List::ListInsertHead(Node *pNode)
{
    Node *temp = new Node;
    if (NULL == temp) {
        return false;
    }
    temp->data = pNode->data;
    temp->pNext = m_pList->pNext;
    m_pList->pNext = temp;
    m_iLength++;

    return true;
}

bool List::ListInsertTail(Node *pNode)
{
    Node *temp = new Node;
    if (NULL == temp) {
        return false;
    }
    temp->data = pNode->data;
    temp->pNext = NULL;
    
    Node *currentNode = m_pList;
    while(NULL != currentNode->pNext) {
        currentNode = currentNode->pNext;
    }
    currentNode->pNext = temp;
    m_iLength++;

    return true;
}

void List::ListTraverse()
{
    Node *currentNode = m_pList->pNext;
    while(NULL != currentNode) {
        currentNode->printNode();
        currentNode = currentNode->pNext;
    }
}


