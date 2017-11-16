#ifndef MYSTACK_H
#define MYSTACK_H

#include "Coordinate.h"
#include <iostream>
using namespace std;

template <typename T>
class MyStack
{
public:
    MyStack(int size);
    ~MyStack();
    bool stackEmpty();
    bool stackFull();
    int stackLength();
    bool push(T elem);
    bool pop(T &elem);
    void stackTraverse();
private:
    T *m_pBuffer;
    int m_iSize;
    int m_iTop;
};

template <typename T>
MyStack<T>::MyStack(int size)
{
    m_iSize = size;
    m_pBuffer = new T[m_iSize]; 
    m_iTop = 0;
}

template <typename T>
MyStack<T>::~MyStack()
{
    delete []m_pBuffer;
    m_pBuffer = NULL;
}

template <typename T>
bool MyStack<T>::stackEmpty()
{
    if (0 == m_iTop) {
        return true;
    } else {
        return false;
    }
}

template <typename T>
bool MyStack<T>::stackFull()
{
    if (m_iTop == m_iSize) {
        return true;
    } else {
        return false;
    }
}

template <typename T>
int MyStack<T>::stackLength()
{
    return m_iTop;
}

template <typename T>
bool MyStack<T>::push(T elem)
{
    if (!stackFull()) {
        m_pBuffer[m_iTop] = elem;
        m_iTop++;
        return true;
    } else {
        return false;
    }
}

template <typename T>
bool MyStack<T>::pop(T &elem)
{
    if (!stackEmpty()) {
        m_iTop--;
        elem = m_pBuffer[m_iTop];
        return true;
    } else {
        return false;
    }
}

template <typename T>
void MyStack<T>::stackTraverse()
{
    for (int i = 0; i < m_iTop; i++) {
        cout << m_pBuffer[i];
    }
}

#endif
