#include "MyStack.h"
#include "Coordinate.h"
#include <iostream>

MyStack::MyStack(int size)
{
    m_iSize = size;
    m_pBuffer = new Coordinate[m_iSize]; 
    m_iTop = 0;
}

MyStack::~MyStack()
{
    delete []m_pBuffer;
    m_pBuffer = NULL;
}

bool MyStack::stackEmpty()
{
    if (0 == m_iTop) {
        return true;
    } else {
        return false;
    }
}

bool MyStack::stackFull()
{
    if (m_iTop == m_iSize) {
        return true;
    } else {
        return false;
    }
}

int MyStack::stackLength()
{
    return m_iTop;
}

bool MyStack::push(Coordinate elem)
{
    if (!stackFull()) {
        m_pBuffer[m_iTop] = elem;
        m_iTop++;
        return true;
    } else {
        return false;
    }
}

bool MyStack::pop(Coordinate &elem)
{
    if (!stackEmpty()) {
        m_iTop--;
        elem = m_pBuffer[m_iTop];
        return true;
    } else {
        return false;
    }
}

void MyStack::stackTraverse()
{
    for (int i = 0; i < m_iTop; i++) {
        cout << m_pBuffer[i];
    }
}

