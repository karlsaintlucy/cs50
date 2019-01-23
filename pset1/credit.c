#include <stdio.h>
#include <cs50.h>

int main(void)
{
    long long cc;
    
    do
    {
        printf("Number: ");
        cc = get_long_long();
    } while (cc < 0);
    
    if (cc < 1E12)
    {
        printf("INVALID\n");
        return 0;
    }
    
    int checksum = 0;
    
    for (long long i = 10; i < cc; i *= 100)
    {
        int x = (cc % (i * 10) / i) * 2;
        
        for (int j = 10; j > 0; j /= 10)
        {
            checksum += x / j;
        }
    }
    
    for (long long k = 1; k < cc; k *= 100)
    {
        int y = cc % (k * 10) / k;
        checksum += y;
    }
    
    if ((checksum % 10) == 0)
    {
        int amex_prefix = cc / 1E13;
        int mc_prefix = cc / 1E14;
        int visa_prefix13 = cc / 1E12;
        int visa_prefix16 = cc / 1E15;
        
        if (amex_prefix == 34 || amex_prefix == 37)
        {
            printf("AMEX\n");
        }
        
        else if (mc_prefix >= 51 && mc_prefix <= 55)
        {
            printf("MASTERCARD\n");
        }
        
        else if (visa_prefix13 == 4 || visa_prefix16 == 4)
        {
            printf("VISA\n");
        }
        
        else
        {
            printf("INVALID\n");
        }
    }
    
    else
    {
        printf("INVALID\n");
    }
}