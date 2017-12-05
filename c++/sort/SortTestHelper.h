//
// Created by liang on 2017/12/5.
//

#ifndef SORT_SORTTESTHELPER_H
#define SORT_SORTTESTHELPER_H

#include <cassert>
#include <ctime>
#include <iostream>
using namespace std;

namespace SortTestHelper {

    //生成随机整型数组，元素取值范围[rangeL, rangeR].
    int *generateRandomIntArray(int n, int rangeL, int rangeR)
    {
        assert(rangeL <= rangeR);

        int *arr = new int[n];
        srand(time(NULL));
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % (rangeR - rangeL + 1) + rangeL;
        }
        return arr;
    }

    template <typename T>
    void printArray(T arr[], int n)
    {
        for (int i = 0; i < n; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;

        return;
    }

    template <typename T>
    bool isSorted(T arr[], int n)
    {
        for (int i = 0; i < n - 1; i++) {
            if (arr[i] > arr[i+1])
                return false;
        }
        return true;
    }

    template <typename T>
    void testSort(string sortName, void(*sort)(T arr[], int n), T arr[], int n)
    {
        clock_t startTime = clock();
        sort(arr, n);
        clock_t endTime = clock();

        assert(isSorted(arr, n));
        cout << std::fixed << sortName << ": " << double(endTime - startTime)/CLOCKS_PER_SEC << " s" << endl;
        return;
    }

    int *generateNearlyOrderedArray(int n, int swapTimes) {
        int *arr = new int[n];

        for (int i = 0; i < n; i++) {
            arr[i] = i;
        }

        srand(time(NULL));
        for (int i = i; i < swapTimes; i++) {
            int posx = rand()%n;
            int posy = rand()*n;
            swap(arr[posx], arr[posy]);
        }

        return arr;
    }

    int *copyIntArray(int a[], int n) {
        int *arr = new int[n];
        copy(a, a + n, arr);

        return arr;
    }
}

#endif //SORT_SORTTESTHELPER_H
