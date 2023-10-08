/*******************************************************************************
 * Name        : stairclimber.cpp
 * Author      : Anna Hauk
 * Date        : 10/6/2023
 * Description : Lists the number of ways to climb n stairs.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System
 ******************************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>


using namespace std;

int num_digits(int number)
{
    // helper func to determine how many digits are in an integer
   int count = 0;
   while(number != 0)
   {
      number = number / 10;
      count++;
   }
   return count;
}

vector< vector<int> > get_ways(int num_stairs) {
    // TODO: Return a vector of vectors of ints representing
    // the different combinations of ways to climb num_stairs
    // stairs, moving up either 1, 2, or 3 stairs at a time.
    if (num_stairs<=0)
    {
        vector<vector<int>> paths = {{}};
        return paths;
    }
    vector<vector<int>> paths = {};
    for (int i=1;i<4;++i)
    {
        if (num_stairs>=i)
        {
            auto result = get_ways(num_stairs-i); //recursive call
            //auto makes it return the function return type
            for (size_t j = 0; j < result.size(); j++) 
            {
                result[j].insert(result[j].begin(),i);
                //inserts a number 1 2 or 3 at begining of list
                //begins returns a pointer to the first elem of a vector
                paths.push_back(result[j]);
                 // append result to the paths variable list 
                 //thats returned in the get_ways func
            }
        }
    }
    return paths;
}

void display_ways(const vector< vector<int> > &ways)
{
    // TODO: Display the ways to climb stairs by iterating over
    // the vector of vectors and printing each combination.
    const int max_label_width = num_digits(ways.size());
    const int max_length = ways[0].size();
    if (ways.size() == 1)
    {
        cout << ways.size() << " way to climb " << max_length << " stair." << endl;
    }
    else
    {
        cout << ways.size() << " ways to climb " << max_length << " stairs." << endl;
    }

    for(size_t i=0;i<ways.size();i++)
    {
        cout << setw(max_label_width) << i+1 << ". [";
        for (size_t j=0;j<ways[i].size();j++)
        {
            if (j==ways[i].size()-1)
            {
                cout << ways[i][j];
            }
            else
            {
                cout << ways[i][j] << ", ";
            }
        }
        cout << "]" << endl;
    }
}

int main(int argc, char * const argv[])
{
    if (argc != 2)
    {
        cout << "Usage: ./stairclimber <number of stairs>" << endl;
        return 1;
    }
    string input_string(argv[1]);
    int numberOfStairs;
    istringstream iss(input_string);

    // Check for error.
    if ( !(iss >> numberOfStairs) || numberOfStairs < 1)
    {
        cout << "Error: Number of stairs must be a positive integer." << endl;
        return 1;
    }
    // if (numberOfStairs < 1)
    // {
    //     cout << "Error: Number of stairs must be a positive integer." << endl;
    //     return 1;
    // }
    vector<vector<int>> ways_vec = get_ways(numberOfStairs);
    display_ways(ways_vec);
    return 0;
}
