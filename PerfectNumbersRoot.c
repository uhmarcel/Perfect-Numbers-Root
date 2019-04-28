
/*
   Source code:  PerfectNumbersRoot.c
        Author:  Marcel Riera

      Language:  C
   Compile/Run:  gcc PerfectNumbersRoot.c -o PerfectNumbersRoot.out -lm
                 ./PerfectNumbersRoot.out

  --------------------------------------------------------------------

  Description:  This program finds and displays perfect numbers in an
                specified range, then it finds their square root by
                applying the Babylonian algorithm for square roots.
                This implementation of the Babylonian method was based on
                https://en.wikipedia.org/wiki/Methods_of_computing_
                square_roots

        Input:  No input

       Output:  The program displays each perfect number, its corresponding
                factors, the expected square root result, and the computed
                value based on the Babylonian method.

      Process:  The program's steps are as follows
                1. Test number to see if its perfect number
                   - If not, increase number an repeat step 1.
                2. Display perfect number and factors.
                3. Display expected square root from sqrt().
                4. Display computed square root and iterations.

*/

#include <stdio.h>   // Required for input and output
#include <math.h>    // Required to compare sqrt() function

#define NO_ERRORS 0             // Exit successful code.
#define PRECISION 15            // Square root decimal precision.
#define BASE_E 10               // Base to compute scientific notation 1e-n
#define RANGE_UPPER_BOUND 10000  // Largest number to test in loop
#define RANGE_LOWER_BOUND 1     // Smallest number to test
#define TRUE 1
#define FALSE 0

// Function Prototypes
int isPerfectNumber(int number);
void displayFactors(int perfectNumber);
void displayExpectedSqrt(double radicand);
double computeSquareRoot(double radicand);
double getInitialGuess(double radicand);


int main(void) {
    // Initialize variables
    int count = 0;
    // Test range, compute and display
    for (count = RANGE_LOWER_BOUND; count <= RANGE_UPPER_BOUND; count++) {
        if (isPerfectNumber(count)) {
            displayFactors(count);
            displayExpectedSqrt(count);
            computeSquareRoot(count);
        } //endif
    }//endfor
    return NO_ERRORS;
} //endmain


/**
 *  Function isPerfectNumber(int integer)
 *  Checks if an input integer is a perfect number. A perfect number is any
 *  number which the addition of its factors (excluding itself) result in the
 *  original number.
 *  (reference from http://mathworld.wolfram.com/PerfectNumber.html)
 *  The algorithm takes advantage of factors being in pair, computing
 *  two values per iteration. Also, the loop navigates up to sqrt of
 *  the input, as it is the last number before checking the upper pairs
 *  of the factors which were already included.
 *  The algorithm also discards odd numbers, as it is unknown if they
 *  exist, and if they do, their expected value would be bigger than 10^1500,
 *  far out of the reach of this algorithm. This measure theoretically cuts
 *  execution time by half.
 *  (odd value reference from https://en.wikipedia.org/wiki/Perfect_number)
 *  @param number  Any integer to be checked.
 *  @return  TRUE(1) if the input is a perfect number, else FALSE (0).
 **/

int isPerfectNumber(int number) {
    if (number % 2 == 1) { // Check if number is odd
        return FALSE;      // if so discard.
    } // endif

    int count = 0;
    int sum = 0;

    for (count = 1; count <= sqrt(number); count++) {
        if (number % count == 0) { // If count divides the input
            int pair = number / count;
            if (count == pair) {  // If the number and its pair are equal
                sum += count;     // just add once.
            } //endif
            else {                // If not, add both.
                sum += count;
                sum += pair;
            } //endelse
        } //endif
    } //endfor

    sum -= number; // Removed input as it's not counted for perfect #s.
    if (sum == number) {
        return TRUE;
    } // endif
    return FALSE;
} // endfunction


/**
 *  Function computeSquareRoot(double radicand)
 *  Calculates the square root of the input value by applying the
 *  Babylonian method.
 *               x[n+1] = (1/2) * (x[n] + S / x[n])
 *  Where S is the input, x[n] is a guess, an x[n+1] is a refined guess.
 *  (retrieved from https://en.wikipedia.org/wiki/Methods_of_computing_
 *  square_roots)
 *  The starting guess is retrieved from getInitialGuess() function.
 *  The formula gets repeated, each step refining the guess, until the
 *  difference between current and previous guesses is less than a limit
 *  specified according to the required precision, which is equal to
 *  10^-PRECISION.
 *  When done, the function displays its output and the amount of
 *  iterations required to achieve the result.
 *  NOTE: I had to use long double as Ocelot machine's compiler could not
 *  handle 15 digits precision with its native double.
 *  @param radicand  Any positive double value
 *  @return  The resulting square root of the input.
 **/

double computeSquareRoot(double radicand) {
    const double limit = pow(BASE_E, PRECISION * -1); // == 1e-PRECISION
    long double guess = getInitialGuess(radicand);
    long double previousGuess = 0.0;
    int iterations = 1; // Set to one to account initial guess computation.

    do {
        previousGuess = guess;
        guess = (1.0/2.0) * (guess + (radicand / guess));
        iterations++;
    } while (fabs(previousGuess - guess) > limit);

    printf("Computed square root of %.0f\t", radicand);
    printf("= %.*Lf;\n", PRECISION, guess);
    printf("\treached in %d iterations.\n\n", iterations);
    return guess;
} // endfunction


/**
 *  Function getInitialGuess(double radicand)
 *  Computes an initial rough estimation of the square root of
 *  the input to be used on the Babylonian algorithm for square
 *  roots.
 *  Calculates the number of digits of the input by finding the log base
 *  10 and rounding up. Then, using the number of digits, a estimation
 *  is generated based on the formula:
 *      For input S = a * x^(2n)
 *      sqrt(S) ≈ 2 x 10^n    (For 1 <= a < 10)
 *  The formula was modified by removing the multiplication by two, as
 *  it improves iterations for the required computations.
 *  (retrieved from https://en.wikipedia.org/wiki/Methods_
 *  of_computing_square_roots#Rough_estimation)
 *  @param radicand Positive number to approximate square root.
 *  @return double  An estimation of the input's square root.
 **/

double getInitialGuess(double radicand) {
    double digits = ceil(log10(radicand)); // Calculates n# of digits
    double result = pow(BASE_E, digits / 2.0);    // Rough estimation
    return result;
} // endfunction


/**
 *  Function displayFactors(int perfectNumber)
 *  Displays the input value along with its proper factors in a format.
 *  The function can also be called with a regular number, as
 *  it would also display their proper factors, but it is not intended.
 *  To preserve the order of the factors, the loop reaches up to
 *  input / 2 instead of the square root as used before.
 *  @param perfectNumber A perfect number integer to display
 *  @return none
 **/

void displayFactors(int perfectNumber) {
    printf("Perfect number: %d = 1", perfectNumber);
    int count = 0;

    // loop started at 2 as 1 is already displayed.
    for (count = 2; count <= (perfectNumber / 2); count++) {
        if (perfectNumber % count == 0) { // if count divides the number
            printf(" + %d", count);       // (is factor)
        } // endif
    } // endfor
    printf(";\n");
} // endfunction


/**
 *  Function displayExpectedSqrt(double radicand)
 *  Displays the expected square root value retrieved from sqrt()
 *  function in math.h library in a formatted way, considering
 *  decimal precision of the output.
 *  NOTE: I had to do a last minute change from sqrt to sqrtl, as
 *  sqrt was not showing proper precision for the specifications
 *  on the FIU's Ocelot machine.
 *  @param radicand The double value to compute its square root.
 *  @return none
 **/

void displayExpectedSqrt(double radicand) {
    printf("Expected sqrt() of %.0f\t\t", radicand);
    printf("= %.*Lf;\n", PRECISION, sqrtl(radicand));
} // endfunction

