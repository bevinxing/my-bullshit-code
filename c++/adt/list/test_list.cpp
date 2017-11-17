#include <iostream>
#include "List.h"
using namespace std;

int main(void)
{
    List *pList = new List();
    
    Node n1, n2, n3;
    n1.data = 1;
    n1.pNext = NULL;
    n2.data = 2;
    n2.pNext = NULL;
    n3.data = 3;
    n3.pNext = NULL;

    pList->ListInsertHead(&n1);
    pList->ListInsertHead(&n2);
    pList->ListInsertHead(&n3);
    pList->ListTraverse();
    //pList->ClearList();
    cout << "List length: " << pList->ListLength() << endl;

    delete pList;
    pList = NULL;

    return 0;
}
