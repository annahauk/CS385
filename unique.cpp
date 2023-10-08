/*******************************************************************************
 * Name        : unique.cpp
 * Author      : Anna Hauk
 * Date        : 10/2/2023
 * Description : Determining uniqueness of chars with int as bit vector.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System
 ******************************************************************************/
#include <iostream>
#include <cctype>
#include <sstream>

using namespace std;

bool is_all_lowercase(const string &s) {
    // TODO: returns true if all characters in string are lowercase
    // letters in the English alphabet; false otherwise.
    const char *str = s.c_str(); //creates a pointer that won't be changed
    //c_str returns a pointer to an array made from s
    std::string::size_type length = s.length(); // Store the length to avoid recalculating it
    for (std::string::size_type i = 0; i < length; i++)
    {
        char c = str[i];
        if (!islower(c))
        {   // if any character is not lowercase
            // just return false here
            // no need to check other characters
            return false;
        }
    }
    return true;
}

bool all_unique_letters(const string &s) {
    // TODO: returns true if all letters in string are unique, that is
    // no duplicates are found; false otherwise.

    
    // You MUST use only single unsigned integers and work with bitwise
    // and bitshifting operators only.  Using any other kind of solution
    // will automatically result in a grade of ZERO for the whole assignment.
    // there are just 26 lowercase characters
    // the unsigned int is 32 bits.  so, the lower 26 bits can each
    // be used as a flag if the character has already been seen in
    // the input string
    // to determine that we assign the rightmost    bit position to the letter "a"
    //                      assign the next         bit position to the letter "b"
    //                      assign third from right bit position to the letter "c"
    //                      and so on and so on
    // pictorially:
    //    32        25                                      3   2   1   0
    //   +---------+---+----------------------------------+---+---+---+---+
    //   |         | z | y   ......     etc  ..........   | d | c | b | a |
    //   +---------+---+----------------------------------+---+---+---+---+


    unsigned int seen_bit_vector = 0;
    const char *str = s.c_str();
    std::string::size_type length = s.length(); // Store the length to avoid recalculating it
    for (std::string::size_type i = 0; i < length; i++) //this ensures the types 
    //match for the loop counter and the result of s.length
    {
        char c = str[i];
        // we take an integer with the least signifigant
        // bit position set and then we shift it over
        // by (c-'a') bits.  so, for the letter 'a',
        // 'a'-'a' = 0, so the bit will not be shifted
        // at all, and then 'b'-'a' = 1 to the bit
        // for 'b' would get shifted to the left one position
        // bit_position is a 32 bit integer that will only
        // have a single bit set that represents the position
        // of which character is being seen as shown in the diagram above
        unsigned int bit_position = 1u << (c - 'a');
        // now we just bitwise "and", also called masking,
        // the seen_bit_vector with the bit shifted into the
        // slot at which that letter is represented as being seen
        if (seen_bit_vector & bit_position)
        {
            // if that vector position in seen_bit_vector
            // has been set already, the masking operation will
            // yield a number which is non-zero which means we have
            // seen this letter in the input already, so we can
            // just return false in this routine, because the first
            // character we see replicated means we don't have to check more
            return false;
        }
        else
        {
            // if masking the bit_position with seen_bit_vector yields zero
            // it means that the particular character has not been seen yet
            // so we set that particular bit representing that character in the
            // seen_bit_vector to represent the fact that we have now seen that
            // particular character in the input string for the first time
            seen_bit_vector |= bit_position;
        }
    }
    return true;
}

int main(int argc, char * const argv[]) {
    // TODO: reads and parses command line arguments.
    // Calls other functions to produce correct output.
    if(argc != 2){
        cout << "Usage: ./unique <string>" << endl;
        return 0;
    }
    string input_string(argv[1]); //creates a string object called input_string
    //can use this instead of iss.str method because it doesn't create an unnecessary intermediate
    if (!is_all_lowercase(input_string))
    {
        cout << "Error: String must contain only lowercase letters." << endl;
        return 1;
    }
    if (all_unique_letters(input_string))
    {
        cout << "All letters are unique." << endl;
    }
    else
    {
        cout << "Duplicate letters found." << endl;
    }
    return 0;
}
