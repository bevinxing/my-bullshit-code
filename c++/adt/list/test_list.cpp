#include <iostream>
#include "List.h"
using namespace std;

int main(void)
{
    List *pList = new List();
    
    Node n1, n2, n3, n4;
    n1.data = 1;
    n2.data = 2;
    n3.data = 3;
    n4.data = 4;

    pList->ListInsertHead(&n1);
    pList->ListInsertHead(&n2);
    pList->ListInsertHead(&n3);
    pList->ListInsert(2, &n4);
    pList->ListInsert(4, &n4);

    Node temp;
    pList->ListDelete(1, &temp);
    pList->ListDelete(0, &temp);
    pList->ListDelete(5, &temp);

    pList->ListTraverse();
    //pList->ClearList();
    cout << "List length: " << pList->ListLength() << endl;

    delete pList;
    pList = NULL;

    return 0;
}
