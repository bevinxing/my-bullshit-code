#include <iostream>
#include "List.h"
using namespace std;

int main(void)
{
    List *pList = new List();
    
    Node n1, n2, n3, n4, n5;
    n1.data = 1;
    n2.data = 2;
    n3.data = 3;
    n4.data = 4;
    n5.data = 5;

    pList->ListInsertHead(&n1);
    pList->ListInsertHead(&n2);
    pList->ListInsertHead(&n3);
    pList->ListInsert(2, &n4);
    pList->ListInsert(4, &n5);

    Node temp;

    pList->GetElem(2, &temp);
    cout << "Get: " << temp.data << endl;

    cout << "Traverse List: " << endl;
    pList->ListTraverse();

    temp.data = 1;
    cout << "Locate: 1 >>" << pList->LocateElem(&temp) << endl;

    Node preTemp;
    Node nextTemp;

    pList->PriorElem(&temp, &preTemp);
    cout << "Prior of 1: " << preTemp.data << endl;

    pList->NextElem(&temp, &nextTemp);
    cout << "Next of 1: " << nextTemp.data << endl;

    temp.data = 3;
    if (pList->PriorElem(&temp, &preTemp))
        cout << "Prior of 3: true" << endl;
    else
        cout << "Prior of 3: false" << endl;

    temp.data = 5;
    if (pList->NextElem(&temp, &nextTemp))
        cout << "Next of 5: true" << endl;
    else
        cout << "Next of 5: false" << endl;

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
