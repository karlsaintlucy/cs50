#include <stdio.h>
#include <stdlib.h>

int collatz(int n);

int main(int argc, char* argv[])
{
    // sanity check
    if (argc != 2) {
        printf("Usage: ./collatz n\n");
        return 1;
    }
    
    // take command-line argument and send to collatz() function
    int number = atoi(argv[1]);
    printf("collatz(n): %i\n", collatz(number));
}

int collatz(int n)
{
    // base case
    if (n == 1)
        return 0;
    
    // recursion case (even)
    else if (n % 2 == 0)
        return 1 + collatz(n / 2);
        
    // recursion case (odd)
    else
        return 1 + collatz(3 * n + 1);
}
