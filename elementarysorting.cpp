#include <iostream>
#include <chrono>
#include <fstream>
using namespace std;
using namespace std::chrono;

static void swap(int array[], const int a, const int b) {
    int temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}

// Function to print the array
void printArray(int array[], const int length) {
    for (int i = 0; i < length; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
}

void bubble_sort_opt(int array[], const int length) {
    int unsorted = length;
    int count = 0;
    while (unsorted > 1) {
        int s = 0;
        for (int j = 1; j < unsorted; j++) {
            if (array[j] < array[j - 1]) {
                swap(array, j - 1, j);
                s = j;
                cout << "Bubble Sort - Iteration " << count << ": ";
                count++;
                printArray(array, length);
                cout << endl;
            }
        }
        unsorted = s;
    }
}

void selection_sort(int array[], const int length) {
    int count =0;
    for (int i = 0; i < length - 1; i++) {
        int min_j = i;
        for (int j = i + 1; j < length; j++) {
            if (array[j] < array[min_j]) {
                min_j = j;//min j = 3, j = 5
            }
        }
        if (min_j != i) {
            swap(array, i, min_j);
        }
        cout << "Selection Sort - Iteration " << count << ": ";
        count++;
        printArray(array, length);
        cout << endl;
    }
}

void insertion_sort(int array[], const int length) {
    int count = 0;
    for (int i = 1; i < length; i++) {
        int j, current = array[i];
        for (j = i - 1; j >= 0 && array[j] > current; j--) {
            array[j + 1] = array[j];
        }
        array[j + 1] = current;
        cout << "Insertion Sort - Iteration " << count << ": ";
        count++;
        printArray(array, length);
        cout << endl;
    }
}

int main() {
    int prob[] = {78, 15, 23, 2, 97, 85};
    int a[] = {78, 15, 23, 2, 97, 85};
    int b[] = {78, 15, 23, 2, 97, 85};
    int len = sizeof(prob) / sizeof(prob[0]);
    cout << "len is: " << len << endl;
    
    cout << "Initial array: ";
    printArray(prob, len);
    
    bubble_sort_opt(prob, len);
    selection_sort(a, len);
    insertion_sort(b, len);
}
