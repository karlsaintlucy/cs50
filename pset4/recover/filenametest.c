#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>

// #include "jpeg.h"

// initialize global pointer for index number for output filenames
int jpg_index = 0;

int main()
{
    printf("Give me an index number: ");
    jpg_index = get_int();
    
    // create char* to handle the jpeg index as 3-digit number & reformat
    char *number = (char *) malloc(strlen("###") + 1);
    sprintf(number, "%03d", jpg_index);
    
    // create char* to handle the extension
    char *extension = (char *) malloc(strlen(".jpg") + 1);
    strcpy(extension, ".jpg");
    
    // create char* for the final filename
    char *filename = (char *) malloc(strlen(number) + strlen(extension) + 1);
    
    // copy the 3-digit jpeg index into filename and concatenate the extension
    strcpy(filename, number);
    strcat(filename, extension);
    
    printf("%s\n", filename);
    
    free(number);
    free(extension);
    free(filename);
    
    // add one to jpeg_index
    
    // where to free filename??
    
    // return filename;
}