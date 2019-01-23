#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    // get the string from the user
    string name = get_string();
    int length = strlen(name);
    
    // loop through all characters in the string
    for (int i=0; i<length; i++) {
        
        // make initial capital if it's lowercase
        // and if it's a space, make it an empty character
        if (isalpha(name[i]))
        {
            if (islower(name[i]))
            {
                name[i] -= 32;
            }
            
            printf("%c", name[i]);
            
            // advance to the next space
            while (name[i] != 32)
            {
                // check to see if the next character is the termination of the string
                if (name[i] == 0)
                {
                    break;
                }
                
                // advance i
                i++;
            }
            
            // if the next character is a space, continue the for loop
            continue;
        }
        
        // if the next character is a space, advance to the next iteration of the for loop
        else if (name[i] == 32)
        {
            continue;
        }
        
        // if the next character isn't an alphabetic character or a space, break the for loop
        else
        {
            break;
        }
    }
    
    // print a new line at the end
    printf("%c", '\n');
}