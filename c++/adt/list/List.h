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
    bool ListInsert(int i, Node *pNode);
    bool ListDelete(int i, Node *pNode);
    bool ListInsertHead(Node *pNode);
    bool ListInsertTail(Node *pNode);
private:
    Node *m_pList;
    int m_iLength;
};

#endif
