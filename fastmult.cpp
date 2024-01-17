/*******************************************************************************
 * Name        : fastmult.cpp
 * Author      : Anna Hauk
 * Version     : 1.0
 * Date        : Nov 18, 2023
 * Description : Performs Karatsuba Fast Multiplication.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <string>

using namespace std;

size_t pad(string& a,string& b);
string add(string& a, string& b);
string subtract(string& a, string& b);

string add(string& a, string& b)
{
    size_t length = pad(a,b);
    int carry = 0;
    string output("");
    for (int i=length-1;i>=0;--i)
    {
        int aDigit = a[i] - '0';
        int bDigit = b[i] - '0';
        int sum = aDigit+bDigit+carry;
        if (sum>=10)
        {
            //int outputDigit = sum-10;
            output.insert(0,1,sum-10 + '0');
            carry = sum/10;
        }
        else
        {
            output.insert(0,1,sum+'0');
            carry = 0;
        }
    }
    if (carry!=0)
    {
        output.insert(0,1,carry+'0');
    }
    // strip leading zeros
    // count them
    int zeroCount = 0; 
    while (output[zeroCount] == '0')
        zeroCount++; 
    // then strip them
    output.erase(0,zeroCount);
    return output;
}

string subtract(string& a, string& b)
{
    size_t length = pad(a,b);
    bool borrow=false;
    string output("");
    for (int i=length-1;i>=0;--i)
    {
        int aDigit = a[i] - '0';
        if(borrow)
        {
            aDigit-=1;
        }
        int bDigit = b[i] - '0';
        if (aDigit<bDigit)
        {
            borrow=true;
            output.insert(0,1,aDigit+10-bDigit+'0');
        }
        else
        {
            borrow=false;
            output.insert(0,1,aDigit-bDigit+'0');
        }
    }
    // strip leading zeros
    // count them
    int zeroCount = 0; 
    while (output[zeroCount] == '0')
        zeroCount++; 
    // then strip them
    output.erase(0,zeroCount);
    return output;
}

size_t pad(string& a,string& b)
{
    size_t m = max(a.length(),b.length());
    if (m==0)
    {
        return 0;
    }
    size_t powerOfTwo=1;
    while(powerOfTwo<m)
    {
        powerOfTwo <<= 1;
    }

    if (a.length()<powerOfTwo)
    {
        size_t numDigitsRequired = powerOfTwo-a.length();
        for(size_t i=0;i<numDigitsRequired;++i)
        {
            a = "0" + a;
        }
    }
    if (b.length()<powerOfTwo)
    {
        size_t numDigitsRequired = powerOfTwo-b.length();
        for(size_t i=0;i<numDigitsRequired;++i)
        {
            b = "0" + b;
        }
    }
    return powerOfTwo;
}

string multiply(string& a,string& b)
{
    size_t length = pad(a,b);
    if (length == 0)
    {
        return "";
    }
    if (length == 1)
    {
        int a_int = a[0]-'0';
        int b_int = b[0]-'0';
        return to_string(a_int*b_int);
    }
    int splitIndex = length/2;
    string a1 = a.substr(0,splitIndex);
    string a2 = a.substr(splitIndex,length-splitIndex);
    string b1 = b.substr(0,splitIndex);
    string b2 = b.substr(splitIndex,length-splitIndex);

    string product_1 = multiply(a1,b1);  // P1
    string product_2 = multiply(a2,b2); // P2
    string a1_plus_a2 = add(a1,a2);
    string b1_plus_b2 = add(b1,b2);
    string product_3 = multiply(a1_plus_a2,b1_plus_b2);  // P3
    int numberOfTensTimesHundred = 2*(length-splitIndex);
    int numberOfTens = length-splitIndex;

    string product_3_minus_product_1 = subtract(product_3,product_1);
    string  product_3_minus_product_1_minus_product_2 = subtract(product_3_minus_product_1,product_2);
    product_3_minus_product_1_minus_product_2.append(numberOfTens,'0');  // multiply times 10^(length-splitindex)
    string product_3_minus_product_1_minus_product_2_plus_product_2 = add(product_3_minus_product_1_minus_product_2,product_2);
    product_1.append(numberOfTensTimesHundred,'0'); // multiply times 10^(2*(length-splitindex))


    string output = add(product_1,product_3_minus_product_1_minus_product_2_plus_product_2);
    // strip leading zeros
    // count them
    int zeroCount = 0; 
    while (output[zeroCount] == '0')
        zeroCount++; 
    // then strip them
    output.erase(0,zeroCount);
    return output;
}


int main(int argc, char *argv[])
{
    string a(argv[1]);
    string b(argv[2]);
    cout << multiply(a,b) << endl;
    return 0;
}