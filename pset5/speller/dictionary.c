/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#include "dictionary.h"

#define MAXWORDLEN 45
#define ALPHABETLEN 26

unsigned int count = 0;

typedef struct node
{
    struct node* children[ALPHABETLEN + 1];
    bool is_word;
} node;

node* root = NULL;

/**
 * Process characters into their relevant indices
 */
int indexify(int character)
{
    if (isupper(character))
    {
        character += 32;
    }
        
    if (character >= 'a')
    {
        character -= 97;
    }
    
    else
    {
        character -= 13;
    }
    
    return character;
}

/**
 * A recursive function to free all the memory used by the program
 */
bool free_nodes(node* tptr)
{
    for (int i = 0; i < (ALPHABETLEN + 1); i++)
    {
        if (tptr == NULL)
        {
            free(tptr);
        }
        else
        {
            tptr = tptr->children[i];
            free_nodes(tptr);
        }
    }
    return true;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    int word_array[MAXWORDLEN] = {0};
    int wordlen = 0;
    
    for (int i = 0; i < strlen(word); i++)
    {
        word_array[i] = word[i];
        wordlen++;
    }
    
    for (int i = 0; i < wordlen; i++)
    {
        word_array[i] = indexify(word_array[i]);
    }
    
    node* trav = root;
    
    if (trav == NULL)
    {
        return false;
    }
    
    for (int i = 0; i < wordlen; i++)
    {
        int c = word_array[i];
        if (trav->children[c])
            trav = trav->children[c];
        
        else
            return false;
    }
    
    if (trav->is_word)
        return true;
        
    else
        return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // open the dictionary, and return false if it can't be opened
    FILE* dptr = fopen(dictionary, "r");
    if (dptr == NULL)
    {
        return false;
    }
    
    root = malloc(sizeof(node));
    
    for (int i = 0; i < (ALPHABETLEN + 1); i++)
    {
        root->children[i] = NULL;
    }
    
    if (root == NULL)
    {
        return false;
    }
    
    // do...while for whole dictionary
    while (!feof(dptr))
    {
        // reset entry
        int entry[MAXWORDLEN] = {0};
        int entrylen = 0;
        bool the_end = false;
        
        // save dictionary entry in entry[]
        for (int i = 0; i < sizeof(entry); i++)
        {
            int c = fgetc(dptr);
            if (c != EOF)
            {
                entry[i] = c;
                if (entry[i] == '\n')
                {
                    entry[i] = '\0';
                    break;
                }
            entrylen++;
            }
            
            // if the fgetc() grabbed the EOF
            else
                the_end = true;
        }
        
        // break out of the while loop if we're at the EOF
        if (the_end)
            break;
        
        // initialize traversal node pointer
        node *trav = root;
        
        for (int i = 0; i < entrylen; i++)
        {
            int c = indexify(entry[i]);
            
            // if the child node at c exists, go to it
            if (trav->children[c])
            {
                trav = trav->children[c];
            }
            
            // if the child node at c does not exist, make it
            else
            {
                trav->children[c] = malloc(sizeof(node));
                
                trav = trav->children[c];
                for (int i = 0; i < (ALPHABETLEN + 1); i++)
                {
                    trav->children[i] = NULL;
                }
            }
        }
        
        trav->is_word = true;
        count++;
        
    }
    
    fclose(dptr);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return count;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    node* trav = root;
    
    if(!free_nodes(trav))
        return false;
    
    return true;
}