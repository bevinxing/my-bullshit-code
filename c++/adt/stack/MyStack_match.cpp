#include "MyStack.h"
#include <iostream>
#include <cstring>

using namespace std;

int main(void)
{
    MyStack<char> *pStack = new MyStack<char>(20);
    MyStack<char> *pNeedStack = new MyStack<char>(20);

    char str[] = "[[]>]]";
    char isNeeded = 0;

    cout << strlen(str) << endl;
    for (int i = 0; i < strlen(str); i++)
    {
        cout << str[i] << endl;
        if (str[i] != isNeeded)
        {
            pStack->push(str[i]);
            cout << "push stack: " << str[i] << endl;
            switch(str[i]) {
            case '[':
                if (0 != isNeeded) {
                    pNeedStack->push(isNeeded);
                    cout << "push isNeeded: " << isNeeded << endl;
                }
                isNeeded = ']';
                break;
            case '(':
                if (0 != isNeeded) {
                    pNeedStack->push(isNeeded);
                    cout << "push isNeeded: " << isNeeded << endl;
                }
                isNeeded = ')';
                break;
            default:
                cout << "string is not matched." << endl;
                return 0;
                break;
            }
        } else {
            char temp = 0;
            pStack->pop(temp);
            cout << "pop stack: " << temp << endl;
            if (!pNeedStack->pop(isNeeded)) {
                isNeeded = 0;
            }
            cout << "pop isNeeded: " << isNeeded << endl;
        }
    }

    if (pStack->stackEmpty()) {
        cout << "string is matched" << endl;
    } else {
        cout << "string is not matched" << endl;
    }

    delete pStack;
    pStack = NULL;
    delete pNeedStack;
    pNeedStack = NULL;

    return 0;
}
