#ifndef COORDINATE_H
#define COORDINATE_H
#include <ostream>
using namespace std;

class Coordinate
{
friend ostream &operator<<(ostream &out, Coordinate &c);
public:
    Coordinate();
    Coordinate(int x, int y);
    ~Coordinate();
    Coordinate &operator++();
    Coordinate operator++(int);
    Coordinate operator+(Coordinate &c);
    int getX();
    int getY();

private:
    int m_iX;
    int m_iY;

};

#endif
