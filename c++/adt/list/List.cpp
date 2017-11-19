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

bool List::ListEmpty()
{
    if (0 == m_iLength) {
        return true;
    } else {
        return false;
    }
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

/* i is in the range of 0~length.
   0 - insert after the head.
   length - insert after the tail.
*/
bool List::ListInsert(int i, Node *pNode)
{
    if (i < 0 || i > m_iLength) {
        return false;
    }
    
    Node *currentNode = m_pList;
    while (i-- > 0) {
        currentNode = currentNode->pNext;
    }

    Node *newNode = new Node;
    if (NULL == newNode) {
        return false;
    }

    newNode->data = pNode->data;
    newNode->pNext = currentNode->pNext;
    currentNode->pNext = newNode;
    m_iLength++;

    return true;
}

/* i is in the range of 0~length-1
   0 - delete the first node
   length-1 - delete the last node
*/
bool List:: ListDelete(int i, Node *pNode)
{
    if (i < 0 || i >= m_iLength) {
        return false;
    }

    Node *currentNode = m_pList;
    while (i-- > 0) {
        currentNode = currentNode->pNext;
    }

    Node *temp = currentNode->pNext;
    pNode->data = temp->data;
    currentNode->pNext = temp->pNext;

    delete temp;
    temp = NULL;
    m_iLength--;

    return true;
}

/* i in range of 0~length-1 */
bool List::GetElem(int i, Node *pNode) {
    if (i < 0 || i >= m_iLength) {
        return false;
    }
    Node *currentNode = m_pList->pNext;
    while (i-- > 0) {
        currentNode = currentNode->pNext;
    }
    pNode->data = currentNode->data;
    return true;
}

int List::LocateElem(Node *pNode) {
    Node *currentNode = m_pList;
    int i = 0;
    while (currentNode->pNext) {
        currentNode = currentNode->pNext;
        if (currentNode->data == pNode->data)
            return i;
        i++;
    }
    return -1;
}

bool List::PriorElem(Node *pCurrentElem, Node *pPreElem) {
    Node *currentNode = m_pList;
    Node *preNode = NULL;

    while(currentNode->pNext) {
        preNode = currentNode;
        currentNode = currentNode->pNext;
        if (currentNode->data == pCurrentElem->data) {
            if (m_pList == preNode)
                return false;
            pPreElem->data = preNode->data;
            return true;
        }
    }
    return false;
}

bool List::NextElem(Node *pCurrentElem, Node *pNextElem) {
    Node *currentNode = m_pList;
    while (currentNode->pNext) {
        currentNode = currentNode->pNext;
        if (currentNode->data == pCurrentElem->data) {
            if (!currentNode->pNext) {
                return false;
            }
            pNextElem->data = currentNode->pNext->data;
            return true;
        }
    }
    return true;
}

