/*******************************************************************************
 * Name        : inversioncounter.cpp
 * Author      : Anna Hauk
 * Version     : 1.0
 * Date        : Nov 1, 2023
 * Description : Counts the number of inversions in an array.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cctype>
#include <cstring>

using namespace std;


/*
    INVERSION WHEN A[i] > A[j]
*/

// Function prototype.
static long mergesort(int array[], int scratch[], int low, int high);
static long merge(int array[], int scratch[], int low, int mid, int high);

/**
 * Counts the number of inversions in an array in Theta(n^2) time using two nested loops.
 */

long count_inversions_slow(int array[], int length) {
    if (length<2)
    {
        return 0;
    }
    long numberOfInversions = 0;
    for (int i=0; i<length-1; ++i)
    {
        for (int j=i+1;j<length; ++j)
        {
            if (array[i]>array[j])
            {
                numberOfInversions++;
            }
        }
    }
    return numberOfInversions;
}

/**
 * Counts the number of inversions in an array in Theta(n lg n) time.
 */
long count_inversions_fast(int array[], int length) {
    vector<int> temp;
    temp.reserve(length);
    int low = 0;
    int high = length-1;
    return mergesort(array,&temp[0],low,high);
}

static long merge(int array[], int scratch[], int low, int mid, int high)
{
    long numberOfInversions=0;
    int i1 = low; //is i1
    int i2 = mid + 1; //is i2
    int i = low; //is i
    while ((i1 <= mid) && (i2 <= high))
    {
        if (array[i1]<=array[i2]) //first element of second half is greater than A[i1] the first half
        {
            scratch[i++]=array[i1++]; //solves 0 inversions
            //copies the first element of the second half into the first element of the copy array
        }
        else
        {
            scratch[i++]=array[i2++];
            numberOfInversions += (mid - i1 + 1);
        }
    }   
    while(i1<=mid)
    {
        scratch[i++]=array[i1++];
    }

    while(i2 <= high)
    {
        scratch[i++]=array[i2++];
    }
    // copy the sorted array fragment
    // from the scratch buffer to the original array
    for(int c=low; c<=high; c++)
    {
        array[c]=scratch[c];
    }

    return numberOfInversions;
}


static long mergesort(int array[], int scratch[], int low, int high) {
    long numberOfInversions = 0;
    int mid;
    if (low < high)
    {
        mid = (low + high) / 2;
        
        numberOfInversions += mergesort(array, scratch, low, mid);
        numberOfInversions += mergesort(array, scratch, mid+1, high);

        numberOfInversions += merge(array, scratch, low, mid, high);
    }
    return numberOfInversions;
}


int main(int argc, char *argv[]) {
    bool isSlow = false;
    if (argc>2)
    {
        cout << "Usage: ./inversioncounter [slow]" << endl;
        return 1;
    }
    if (argc==2)
    {
        string argument(argv[1]);
        if (argument != "slow")
        {
            cout << "Error: Unrecognized option '" << argument << "'." << endl;
            return 1;
        }
        isSlow = true;
    }
    cout << "Enter sequence of integers, each followed by a space: " << flush;
    string line;
    getline(cin, line);
    istringstream iss(line);
    int value;
    vector<int> values;
    string token;
    int index=0;
    while (iss)
    {
        iss >> token;
        if (!iss)
        {
            break;
        }
        istringstream nss(token);
        nss >> value;
        if (nss.fail())
        {
            cout << "Error: Non-integer value '" << token << "' received at index " << index << "." << endl;
            return 0;
        }
        values.push_back(value);
        index++;
    }
    if (values.size() == 0)
    {
        cout << "Error: Sequence of integers not received." << endl;
        return 1;
    }

    if (isSlow)
    {
        cout << "Number of inversions (slow): " << count_inversions_slow(&values[0], values.size()) << endl;
    }
    else
    {
        cout << "Number of inversions (fast): " << count_inversions_fast(&values[0], values.size()) << endl;
    }

    return 0;
}
