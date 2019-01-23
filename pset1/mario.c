#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;
    
    // make sure height is between 0 and 23
    do {
        printf("Height: ");
        height = get_int();
    } while (height < 0 || height > 23);
    
    //set the quantity of spaces and hashes for the first row
    int spaces = height - 1;
    int hashes = 1;
    
    // for loop for each row
    for (int i=0; i<height; i++) {
        
        //print the spaces
        for (int j=0; j<spaces; j++) {
            printf(" ");
        }
        
        // print the hashes
        for (int k=0; k<hashes; k++) {
            printf("#");
        }
        
        // print the spaces between the left and right sides of the pyramid
        printf("  ");
        
        // print the hashes again
        for (int k=0; k<hashes; k++) {
            printf("#");
        }
        
        // create new line at end of row
        printf("%c",'\n');
        
        // update the number of spaces and hashes for the next row
        spaces--;
        hashes++;
    }
}