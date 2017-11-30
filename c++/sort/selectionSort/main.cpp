#include <iostream>
#include <algorithm>
using namespace std;

template <typename T>
void selectionSort(T array[], int size)
{
    int minIndex = 0;
    for (int i = 0; i < size; i++) {
        minIndex = i;
        for (int j = i + 1; j < size; j++) {
            if (array[j] < array[minIndex])
                minIndex = j;
        }
        swap(array[i], array[minIndex]);
    }
}

int main() {
    int arrTest[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    selectionSort(arrTest, 10);
    for (int i = 0; i < 10; i++)
        cout << arrTest[i] << " ";
    cout << endl;

    return 0;
}