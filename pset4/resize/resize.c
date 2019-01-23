/**
 * Resize - LESS COMFORTABLE
 */
       
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }
    
    int n = atoi(argv[1]);
    
    if (n < 1 || n > 100) {
        fprintf(stderr, "For scale, use a positive integer between 1 and 100 (inclusive).\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // create RGBTRIPLE array that will hold input image's RBG data
    RGBTRIPLE bitmap[abs(bi.biHeight)][bi.biWidth];
    
    // capture input RGB data into the bitmap array
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        for (int j = 0, biWidth = bi.biWidth; j < biWidth; j++)
        {
            RGBTRIPLE triple;
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            bitmap[i][j] = triple;
        }
        
        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);
    }
    
    // close the inptr, since we don't need it anymore
    fclose(inptr);
    
    // modify header data for output file
    bi.biWidth *= n;
    bi.biHeight *= n;
    
    // re-calculate padding for output write
    padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    bi.biSizeImage = (bi.biWidth * sizeof(RGBTRIPLE) + padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    int row = 0;
    
    // reset width and height values to original
    bi.biWidth /= n;
    bi.biHeight /= n;
    
    // while the row index is less than bi.biHeight:
    while (row < abs(bi.biHeight))
    {
        // for each row:
        for (int i = 0; i < n; i++)
        {
            // reset the pixel index at the beginning of each row
            int pixel = 0;
            
            // while the pixel index is less than the total width:
            while (pixel < bi.biWidth)
            {
                for (int j = 0; j < n; j++)
                {
                    fwrite(&bitmap[row][pixel], sizeof(RGBTRIPLE), 1, outptr);
                }
                
                // increment the pixel index after each pixel is copied n times
                pixel++;
            }
            
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }
        }
        
        // increment the row index after each row is complete
        row++;
    }

    // close outfile
    fclose(outptr);

    // success
    return 0;
}