#ifndef LIST_H
#define LIST_H

#include "Node.h"

class List
{
public:
    List();
    ~List();
    void ClearList();
    bool ListEmpty();
    int ListLength();
    void ListTraverse();
    bool ListInsertHead(Node *pNode);
    bool ListInsertTail(Node *pNode);
private:
    Node *m_pList;
    int m_iLength;
};

#endif
