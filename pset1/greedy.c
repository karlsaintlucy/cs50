#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    // initial prompt
    printf("O hai! How much change is owed?\n");
    double change = get_float();
    
    // continuously ask user for change if input is negative
    while (change < 0.0) {
        printf("How much change is owed?\n");
        change = get_float();
    }
    
    change = round(change * 100);
    int coins = 0;
    
    while (change >= 25.0)
    {
        change -= 25.0;
        coins++;
    }
    
    while (change >= 10.0)
    {
        change -= 10.0;
        coins++;
    }

    while (change >= 5.0)
    {
        change -= 5.0;
        coins++;
    }

    while (change >= 1.0)
    {
        change -= 1;
        coins++;
    }
    
    // print the number of coins
    printf("%i\n", coins);
}