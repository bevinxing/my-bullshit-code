#include <iostream>
#include "Coordinate.h"
using namespace std;

int main(void)
{
    Coordinate coord(1, 2);
    cout << coord.getX() << ", " << coord.getY() << endl;
    ++coord;
    cout << coord.getX() << ", " << coord.getY() << endl;
    Coordinate temp = coord++;
    cout << temp.getX() << ", " << temp.getY() << endl;
    cout << coord.getX() << ", " << coord.getY() << endl;

    Coordinate coord2(3, 4);
    Coordinate coord3;
    coord3 = coord + coord2;

    cout << coord3.getX() << ", " << coord3.getY() << endl;
    cout << coord3;

    return 0;
}
