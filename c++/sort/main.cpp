#include <iostream>
#include "SortTestHelper.h"
#include "SelectionSort.h"
#include "InsertionSort.h"

int main() {
    int n = 10000;

    int *arrTest = SortTestHelper::generateRandomIntArray(n, 0, n);
    int *arrTest2 = SortTestHelper::generateNearlyOrderedArray(n, 100);
    int *arrTest3 = SortTestHelper::copyIntArray(arrTest, n);
    SortTestHelper::testSort("insertionTest", insertionSort, arrTest, n);
    SortTestHelper::testSort("insertionTest", insertionSort, arrTest2, n);
    SortTestHelper::testSort("selectionTest", selectionSort, arrTest3, n);

    SortTestHelper::printArray(arrTest, n);
    SortTestHelper::printArray(arrTest2, n);
    SortTestHelper::printArray(arrTest3, n);

    delete[] arrTest;
    delete[] arrTest2;
    delete[] arrTest3;

    return 0;
}