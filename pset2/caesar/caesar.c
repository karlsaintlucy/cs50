#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // check that we've got the right number of input arguments
    if (argc != 2) {
        printf("Usage: ./caesar k\n");
        return 1;
    }
    
    // save the argument as an integer:
    // (when it wasn't unsigned there was weird math
    // happening when c and k were added),
    // and mod the key so the shift range is within the alphabet
    unsigned int k = atoi(argv[1]) % 26;
    
    // prompt the user for plaintext,
    // store the input and the length
    printf("plaintext: ");
    string plaintext = get_string();
    int length = strlen(plaintext);
    
    // create an integer variable for the character
    int c = 0;
    
    // create a nice visual display before ciphertext
    printf("ciphertext: ");
    
    // loop to cipher each character
    for (int i=0; i<length; i++) {
        
        // store each iterated character as c
        c = (int) plaintext[i];
        
        // logic for alphabetic characters
        if (isalpha(c)) {
            
            // add the key
            c += k;
            
            // wrap around the alphabet if necessary
            if ((c > 'Z' && c < 'a') || (c > 'z')) {
                c -= 26;
                printf("%c", c);
            }
            
            // print letter as-is if no wraparound needed
            else {
                printf("%c", c);
            }
            
        }
        
        // logic for all other characters
        else {
            printf("%c", c);
        }
    }
    
    // print out a new line at the end of the ciphertext
    printf("\n");
}