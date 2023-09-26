/*******************************************************************************
* Filename: sqrt.cpp
* Author : Anna Hauk
* Version : 1.0
* Date : 9-11-2023
* Description: compute the square root of a double using Newton's method.
* Pledge : I pledge my honor that I have abided by the Stevens Honor System.
******************************************************************************/

/*
compute the square root of a double using Newton's method.

The main function should take in either one or two command line arguments, both of which are doubles.

The first argument is the number of which to take the square root.  It is, of course, a required argument.
The second argument is optional.  It is the value of epsilon, the amount by which the abs(last_guess - next_guess) is allowed to differ.  If no argument is supplied, 
espilon should be set to 1e-7, which is 10-7.

You MUST implement in your code a function that has the following header:

double sqrt(double num, double epsilon)

The algorithm breaks down as follows:

Return numeric_limits<double>::quiet_NaN(), if the num < 0.  This constant is found in the limits header.
Return num, if num is 0 or 1.
Repeat next_guess = (last_guess + num/last_guess) / 2 until abs(last_guess - next_guess) <= epsilon.
Ultimately, return the last value of next_guess.
The output should be displayed with fixed notation and 8 digits of precision after the decimal point.  You will need to include the
 iomanip header to modify the output format.

*/

#include <iostream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <iomanip>
using namespace std;

double sqrt(double num, double epsilon){
    istringstream iss;
    //Return numeric_limits<double>::quiet_NaN(), if the num < 0.  This constant is found in the limits header.
    if(num < 0.0){
        return numeric_limits<double>::quiet_NaN();
    }

    //Return num, if num is 0 or 1.
    if(num == 0.0 || num == 1.0){
        return num;
    }

    //Repeat next_guess = (last_guess + num/last_guess) / 2 until 
    //abs(last_guess - next_guess) <= epsilon.
    double last_guess = num;
    double next_guess = (last_guess + num/last_guess) / 2.0; 

    while(abs(last_guess - next_guess) > epsilon){
        last_guess = next_guess;
        next_guess = (last_guess + num/last_guess) / 2.0;
    }
    return next_guess;
    
}

//int argc = number of arguments
int main(int argc, char* argv[]) {
    istringstream iss; //read data from a stream

    //Checks if one or two numbers were passed in-- the number or the num and epsilon
    if(argc != 3 && argc != 2){//name of the program is one param
        cerr << "Usage: ./sqrt <value> [epsilon]" << endl;
        return 1;
    }
    //read in num val
    double num;
    iss.str(argv[1]);
    if(!(iss >> num)){//if conversion fails, will be false
        cerr << "Error: Value argument must be a double." << endl;
        return 1;
    }
    iss.clear();

    //read in epsilon
    double epsilon = 1e-7;
    if(argc == 3){
        iss.str(argv[2]);
        if(!(iss >> epsilon)){//if conversion fails, will be false
        cerr << "Error: Epsilon argument must be a positive double." << endl;
        return 1;
        }
        
        if(epsilon <= 0.0){
            cout << "Error: Epsilon argument must be a positive double." << endl;
            return 1;
        }
    }

    //cout << "The Square root of " << num << " is: " << fixed << setprecision(8) << sqrt(num,epsilon) << endl;
    cout << fixed << setprecision(8) << sqrt(num,epsilon) << endl;

    return 0;
}

