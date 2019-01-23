#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SALTLEN 2
#define PASSLEN 4
#define HASHLEN 13
#define ALPHABETLEN 26

char hash[HASHLEN + 1] = {0};

int check_pass(char pass[PASSLEN + 1], char encrypted[HASHLEN + 1]);

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }
    
    char alphabet[(ALPHABETLEN * 2) + 1] = {0};
    char pass[PASSLEN + 1] = {0};
    char salt[SALTLEN + 1] = {0};
    char encrypted[HASHLEN + 1] = {0};
    
    // save salt
    for (int i = 0; i < SALTLEN; i++)
    {
        salt[i] = argv[1][i];
    }
    
    // save hash
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        hash[i] = argv[1][i];
    }
    
    // save the alphabet in indices 0-51, starting at 'A'
    for (int i = 0, c = 'A'; i < (ALPHABETLEN * 2); i++)
    {
        // save them to the alphabet[] array as long as they're alphabetic
        if (c <= 'Z' || c >= 'a')
        {
            alphabet[i] = c;
            c++;
        }
        
        // skip the nonalphabetic characters between 'Z' and 'a'
        else
        {
            i--; // reset the i index
            c++; // advance the character
        }
    }
    
    // test for one character
    for (int i = 0; i < (ALPHABETLEN * 2); i++)
    {
        pass[0] = alphabet[i];
        
        strcpy(encrypted, crypt(pass, salt));
        if (check_pass(pass, encrypted) == 0)
            return 0;
    }
    
    // test for two characters
    for (int i = 0; i < (ALPHABETLEN * 2); i++)
    {
        pass[0] = alphabet[i];
        
        for (int j = 0; j < (ALPHABETLEN * 2); j++)
        {
            pass[1] = alphabet[j];
            
            strcpy(encrypted, crypt(pass, salt));
            if (check_pass(pass, encrypted) == 0)
                return 0;
        }
    }
         
    // test for three characters   
    for (int i =0; i < (ALPHABETLEN * 2); i++)
    {
        pass[0] = alphabet[i];
        
        for (int j = 0; j < (ALPHABETLEN * 2); j++)
        {    
            pass[1] = alphabet[j];
            
            for (int k = 0; k < (ALPHABETLEN * 2); k++)
            {
                pass[2] = alphabet[k];
                
                strcpy(encrypted, crypt(pass, salt));
                if (check_pass(pass, encrypted) == 0)
                    return 0;
            }
        }
    }
    
    // test for four characters
    for (int i =0; i < (ALPHABETLEN * 2); i++)
    {
        pass[0] = alphabet[i];
        
        for (int j = 0; j < (ALPHABETLEN * 2); j++)
        {   
            pass[1] = alphabet[j];
            
            for (int k = 0; k < (ALPHABETLEN * 2); k++)
            {
                pass[2] = alphabet[k];
                
                for (int l = 0; l < (ALPHABETLEN * 2); l++)
                {
                    pass[3] = alphabet[l];
                    
                    strcpy(encrypted, crypt(pass, salt));
                    if (check_pass(pass, encrypted) == 0)
                        return 0;
                }
            }
        }
    }
    
    printf("Password not found :(\n");
    return 1;
}

int check_pass(char pass[PASSLEN + 1], char encrypted[HASHLEN + 1])
{
    if (strcmp(hash, encrypted) == 0)
    {
        printf("%s\n", pass);
        return 0; // success!
    }
    
    else
        return 1; // didn't pass
}