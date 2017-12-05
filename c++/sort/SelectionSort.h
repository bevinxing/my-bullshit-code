//
// Created by liang on 2017/12/5.
//

#ifndef SORT_SELECTIONSORT_H
#define SORT_SELECTIONSORT_H
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

#endif //SORT_SELECTIONSORT_H
