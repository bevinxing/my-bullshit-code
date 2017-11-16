#include "Coordinate.h"
#include <iostream>
using namespace std;

Coordinate::Coordinate(int x, int y)
{
    m_iX = x;
    m_iY = y;
}

void Coordinate::printInfo()
{
    cout << "(" << m_iX << ", " << m_iY << ")" << endl;
}