#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // check that we've got the right number of input arguments
    if (argc != 2) {
        printf("Usage: ./vigenere k\n");
        return 1;
    }
    
    // set up key variables
    string k = argv[1];
    int klength = strlen(k);
    bool capital;
    
    // c will be the variable we use to test characters
    int c;
    
    // PROCESS KEY
    for (int i=0; i<klength; i++) {
        c = k[i];
        
        capital = isupper(c);
        
        // process the letters into integers 0-25,
        // return if string includes non-alphabetic characters
        if (capital) {
            k[i] = c - 65;
        } else if (isalpha(c) && !capital) {
            k[i] = c - 97;
        } else {
            printf("Usage: ./vigenere k\n");
            return 1;
        }
    }
    
    // prompt the user for plaintext,
    // store the input and the length
    printf("plaintext: ");
    string p = get_string();
    int plength = strlen(p);
    
    // create a nice visual display before ciphertext
    printf("ciphertext: ");
    
    // loop to cipher each character in plaintext string:
    // initialize a secondary counter to cycle through the
    // characters in the key; and a shift variable to 
    // represent the shift itself
    for (int i=0, j=0, shift; i<plength; i++) {
        
        // store each iterated character as c
        c = p[i];
        
        // to-dos for alphabetic characters
        if (isalpha(c)) {
            
            // add the shift
            // increment j
            capital = isupper(c);
            shift = j % klength;
            c += k[shift];
            j++;
            
            // wrap around if necessary
            if (capital && (c > 'Z')) {
                c -= 26;
                printf("%c", c);
            } else if (c > 'z') {
                c-= 26;
                printf("%c", c);
            }
            // print letter as-is if no wraparound necessary
            else {
                printf("%c", c);
            }
        }
        
        // print non-alphabetic characters as-is
        else {
            printf("%c", c);
        }
    }
    
    // print out a new line at the end of the cyphertext
    printf("\n");
}