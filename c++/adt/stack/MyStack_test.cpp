#include "MyStack.h"
#include "Coordinate.h"
#include <iostream>
using namespace std;

int main(void)
{
    MyStack *pStack = new MyStack(5);

    pStack->stackTraverse();

    Coordinate c1(1, 2);
    Coordinate c2(3, 4);

    pStack->push(c1);
    pStack->push(c2);

    //pStack->stackTraverse();

    Coordinate c3(5, 6);
    pStack->push(c3);

    pStack->stackTraverse();

    cout << pStack->stackLength() << endl;

    Coordinate c;
    pStack->pop(c);

    cout << pStack->stackLength() << endl;
    cout << c;
    c.printInfo();

    pStack->stackTraverse();

    delete pStack;
    pStack = NULL;

    return 0;
}
