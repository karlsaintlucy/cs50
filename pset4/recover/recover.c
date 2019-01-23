#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>

#define BLOCK_MAX_SIZE 512

typedef uint8_t  BYTE;

typedef struct
{
   BYTE JPEGData[BLOCK_MAX_SIZE];
 } BLOCK;

// define length of JPEG header as 4 (bytes)
#define HEADERLEN 4

// define length of each BLOCK of data as 512 (bytes)
#define BLOCKLEN 512

// initialize array of first three bytes for JPEG header
BYTE jpegheaderstart[HEADERLEN - 1] = {0xff, 0xd8, 0xff};

// initialize global pointers inptr and outptr
FILE* inptr = NULL;
FILE* outptr = NULL;


// initialize filename as global char*, which will eventually be used to hold the output file names
char* filename = NULL;


// initialize global variable to store the jpeg file index number
int jpg_index = 0;

// initialize global BLOCK type variable to hold the read data
BLOCK bl;

// initialize functions that exist outside of main()
int test_header();
char *update_filename();
int write_jpeg(char *outputfilename, BLOCK photodata);

/*
 *
 * BEGIN MAIN FUNCTION
 *
 */

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[1];

    // open input file 
    inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    
    while (test_header() != -1);
    
    free(filename);
    
    // END OF PROCESS:
    // free(filename);
    fclose(inptr);
    
    // outptr gets closed after every BLOCK written to output file
    
    return 0;
}

/*
 *
 * BEGIN FUNCTIONS OUTSIDE OF MAIN
 *
 */

// TEST THE HEADER TO SEE IF A NEW FILE IS BEING READ
int test_header(void)
{
    // count the number of matching items in the JPEG header for comparison against HEADERLEN
    int matches = 0;
    
    /*
    if (feof(inptr))
    {
        return 0;
    }
    */
    
    // read next block
    fread(&bl, sizeof(BLOCK), 1, inptr);
    
    if (feof(inptr))
    {
        return -1;
    }

    // check the first three bytes against JPEG header standard, stored in jpegheaderstart[]
    for (int i = 0; i < HEADERLEN - 1; i++)
    {
        if (bl.JPEGData[i] == jpegheaderstart[i])
        {
            matches++;
        }
    }
    
    // check final byte against JPEG header standard
    if (bl.JPEGData[3] >= 0xe0 && bl.JPEGData[3] <= 0xef)
    {
        matches++;
    }
    
    // Do the below the first time JPEG header data is found
    if (matches == HEADERLEN && jpg_index == 0)
    {
        write_jpeg(update_filename(), bl);
    }
    
    // Do the below if the BLOCK appears to be interior JPEG data
    else if (matches != HEADERLEN && jpg_index > 0)
    {
        write_jpeg(filename, bl);
    }
    
    // Do the below if the header test appears to indicate a new JPEG image
    else if (matches == HEADERLEN && jpg_index > 0)
    {
        write_jpeg(update_filename(), bl);
    }
    return 0;
}


// UPDATE THE OUTPUT FILE NAME
char *update_filename()
{
    // create char* to handle the jpeg index as 3-digit number & reformat
    char *number = malloc(strlen("###") + 1);
    sprintf(number, "%03d", jpg_index);
    
    // create char* to handle the extension
    char *extension = malloc(strlen(".jpg") + 1);
    strcpy(extension, ".jpg");
    
    if (filename == NULL)
    {
        filename = malloc(strlen(number) + strlen(extension) + 1);
    }
    
    // copy the 3-digit jpeg index into filename and concatenate the extension
    strcpy(filename, number);
    strcat(filename, extension);
    
    free(number);
    free(extension);
    
    // add one to jpg_index for next time the function is run
    jpg_index++;
    
    return filename;
}

// WRITE JPEG DATA WHEN FOUND
int write_jpeg(char* outputfilename, BLOCK photodata)
{
    // open output file in append mode
    outptr = fopen(outputfilename, "a");
    if (outptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", outputfilename);
        return 2;
    }
    
    // write photodata to outfile
    fwrite(&photodata, BLOCKLEN, 1, outptr);
    
    // close the outptr
    fclose(outptr);
    
    return 0;
}