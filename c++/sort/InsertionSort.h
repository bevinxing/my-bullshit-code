//
// Created by liang on 2017/12/5.
//

#ifndef SORT_INSERTIONSORT_H
#define SORT_INSERTIONSORT_H

template <typename T>
void insertionSort(T arr[], int n)  {
    T e;
    for (int i = 1; i < n; i++) {
        e = arr[i];
        //寻找arr[i]合适的插入位置
        int j;
        //j保存e应该插入的位置
        for (j = i; j > 0 && e < arr[j-1]; j--) {
            arr[j] = arr[j-1];
        }
        arr[j] = e;
    }
}

#endif //SORT_INSERTIONSORT_H
