/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

/* VERSION 2 */

#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // return false immediately if non-positive n entered
    if (n <= 0) {
        return false;
    }
    
    // set variables for midpoint, lower and upper search bounds
    int midpoint, lower = 0, upper = n - 1;
    
    while (n > 0) {
        // halve the length of the list, so n is the index of the midpoint
        midpoint = lower + (upper - lower) / 2;
        
        // return true right away if the midpoint is the value
        if (values[midpoint] == value) {
            return true;
        }
        
        // search left if midpoint is greater than the search value
        else if (values[midpoint] > value) {
            upper = midpoint - 1;
            n /= 2;
        }
        
        // search right if the midpoint is lesser than the search value
        else if (values[midpoint] < value) {
            lower = midpoint + 1;
            n /= 2;
        }
    }
    return false;

}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    
    // COUNTING SORT
    // Having issues with declaring a constant for 65536 at the top:
    // 1. declaring MAX again in helpers.c throws an error in compiler (duplicate declaration)
    // 2. setting MAXVAL and initializing counting[MAXVAL]; causes segmentation fault when running
    // 3. setting MAXVAL and initializing counting[MAXVAL] = {0}; throws an error in compiler:
    // (can't initialize an array with variable number of elements;
    // can't figure out why that works in find.c but not here)
    int counting[65536] = {0};
    
    // Iterate over indices i in values[];
    // each value of values[] will correspond
    // to an index in counting[]. Add one
    // to the index in counting each time
    // a corresponding value arises
    for (int i = 0; i < n; i++) {
        counting[values[i]]++;
    }
    
    // Iterate over indices i in counting[];
    
    for (int i = 0, vindex = 0, quantity; i < 65536; i++) {
        // only proceed if the count is greater than 0
        if (counting[i] > 0) {
            // save the value of counting at i as quantity
            quantity = counting[i];
            // loop over quantity with j, storing i in
            // values[] at position vindex; vindex increments
            // after each iteration of the for loop on j
            for (int j = 0; j < quantity; j++) {
                values[vindex] = i;
                vindex++;
            }
        }
    }
    
    /*
    // BUBBLE SORT
    
    // I cheated and looked this up, so I redid the sort
    // function using counting sort, above
    
    // OUTER LOOP: Iterate over indices i,
    // subtracting one from n because indices
    // are 0-indexed but count of contents is not
    for (int i = 0, temp; i < n - 1; i++) {
        
        // INNER LOOP: Iterate over indices j,
        // stopping short of the final index,
        // which will hold the highest value
        for (int j = 0; j < n - i - 1; j++) {
            if (values[j] > values[j+1]) {
                temp = values[j+1];
                values[j+1] = values[j];
                values[j] = temp;
            }
        }
    }
    return;
    */
}
