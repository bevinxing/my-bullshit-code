#ifndef COORDINATE_H
#define COORDINATE_H

#include <iostream>
using namespace std;

class Coordinate
{
friend ostream &operator<<(ostream &out, Coordinate &c);
public:
    Coordinate(int x = 0, int y = 0);
    void printInfo();
private:
    int m_iX;
    int m_iY;
};

#endif
