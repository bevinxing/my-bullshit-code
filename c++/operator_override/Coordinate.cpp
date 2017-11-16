#include <iostream>
#include "Coordinate.h"
using namespace std;

Coordinate::Coordinate()
{
    
}

Coordinate::Coordinate(int x, int y)
{
    m_iX = x;
    m_iY = y;
}

Coordinate::~Coordinate()
{
    cout << "~Coordinate" << endl;
}

int Coordinate::getX()
{
    return m_iX;
}

int Coordinate::getY()
{
    return m_iY;
}

Coordinate &Coordinate::operator++()
{
    this->m_iX = ++m_iX;
    this->m_iY = ++m_iY;
    return *this;
}

Coordinate Coordinate::operator++(int)
{
    Coordinate old(*this);
    this->m_iX++;
    this->m_iY++;
    return old;
}

Coordinate Coordinate::operator+(Coordinate &c)
{
    Coordinate temp;
    temp.m_iX = this->m_iX + c.m_iX;
    temp.m_iY = this->m_iY + c.m_iY;
    return temp;
}

ostream &operator<<(ostream &out, Coordinate &c)
{
    out << c.m_iX << ", " << c.m_iY << endl;
    return out;
}
