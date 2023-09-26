/*******************************************************************************
* Filename: sieve.cpp
* Author : Anna Hauk
* Version : 1.0
* Date : 9-19-2023
* Description: use the sieve algorithm to find and display prime numbers.
* Pledge : I pledge my honor that I have abided by the Stevens Honor System.
******************************************************************************/

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

class PrimesSieve {
public:
    PrimesSieve(int limit);

    ~PrimesSieve() { //destructor to ensure no memory leak
    //object and array are seperate so make sure you delete both
        delete [] is_prime_;
    }

    void display_primes() const;

private:
    // Instance variables
    bool * const is_prime_;
    const int limit_;
    int num_primes_, max_prime_;

    // Method declarations
    void sieve();
    static int num_digits(int num);
};


//Constructor
PrimesSieve::PrimesSieve(int limit) :
        is_prime_{new bool[limit + 1]}, limit_{limit} {
            for(int i = 0; i <= limit; i++){//need to intialize all 0-n to true or memory leak happens
                is_prime_[i] = true;
            }
            num_primes_ = 0;
            // for(int a = 0; a <= limit; a++){
            //     cout << a << " : " << is_prime_[a] << endl;
            // }
    sieve();
}

void PrimesSieve::display_primes() const {
    // write code to display the primes in the format specified in the
    // requirements document.
    cout << endl;
    
    cout << "Number of primes found: " << num_primes_ <<endl;
    cout << "Primes up to " << limit_ << ":" << endl;

    const int max_prime_width = num_digits(max_prime_),           
            primes_per_row = 80 / (max_prime_width + 1);

    //cout << max_prime_ << " " << max_prime_width << " " << primes_per_row << endl;
    if (num_primes_ <= primes_per_row)
    {
        int primes_count = 0;
        for(int a = 2; a <= limit_; a++)
        {
            if(is_prime_[a])
            {
                primes_count++;
                if (primes_count == num_primes_)
                {
                    cout << a;
                }
                else
                {
                    cout << a << " "; //for spacing
                }
            }
        }
    }
    else
    {
        int print_count = 0;
        int primes_count = 0;
        for(int a = 2; a <= limit_; a++)
        {
            if(is_prime_[a])
            {
                primes_count++;
                print_count++;
                if(print_count == primes_per_row)
                {
                    cout << setw(max_prime_width) << a << endl;
                    print_count = 0;
                }
                else
                {
                    if (primes_count == num_primes_)
                    {
                        cout << setw(max_prime_width) << a;
                    }
                    else
                    {
                        cout << setw(max_prime_width) << a << " ";
                    }
                }
            }
        }
    }
    cout << endl;
}

void PrimesSieve::sieve() {
    // sieve algorithm
    // All instance variables must be initialized by the end of this method.
   int n = limit_;
   for (int i = 2; i <= sqrt(n); i++){
        if (is_prime_[i] == true){
            for(int j = i*i; j <= n; j+= i){
                is_prime_[j] = false; //marks the numbers with the factor of a prime number not prime
            }
        }
   }
   // starting from the top of the is_prime_
   // array, figure out the maximum prime value
   // by jumping out of the loop at the first one
   // detected as prime.  we're guaranteed for it
   // to always be at least 2 if limit is >= 2
   for(int b = limit_; b >= 2; b--){
        if(is_prime_[b]){
            max_prime_ = b;
            break;
        }
   }
   // excluding 0 and 1, count the number
   // of primes and store in num_primes_
   for(int c = limit_; c >= 2; c--){
        if(is_prime_[c]){
            num_primes_++;
        }
    }
}

int PrimesSieve::num_digits(int number) {
    //determine how many digits are in an integer
   int count = 0;
   while(number != 0) {
      number = number / 10;
      count++;
   }
   return count;
}

int main() {
    cout << "**************************** " <<  "Sieve of Eratosthenes" <<
            " ****************************" << endl;
    cout << "Search for primes up to: ";
    string limit_str;
    cin >> limit_str;
    int limit;

    // Use stringstream for conversion. Don't forget to #include <sstream>
    istringstream iss(limit_str);

    // Check for error.
    if ( !(iss >> limit) ) {
        cerr << "Error: Input is not an integer." << endl;
        return 1;
    }
    if (limit < 2) {
        cerr << "Error: Input must be an integer >= 2." << endl;
        return 1;
    }
    PrimesSieve pr(limit);
    pr.display_primes();
    
    return 0;
}