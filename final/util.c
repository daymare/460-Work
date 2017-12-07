#ifndef UTIL
#define UTIL

#include "ucode.c"


/*
    strtok
    input: string to tokenize, token to split on
    throughput: first delimiter will be replaced with \0
    output: pointer to start of the rest of the string
*/
char* strtok(char* string, char delimiter)
{
    char* current = string;

    while (*current != delimiter && *current != '\0')
    {
        current++;
    }

    *current = '\0';
    return ++current;
}


// removes \n or \r from the end of the string
void normalize(char* string)
{
    replace(string, '\r', '\0');
    replace(string, '\n', '\0');
}

// replace all instances of a character in a string with another character;
void replace(char* string, char character, char replacement)
{
    while (*string != '\0')
    {
        if (*string == character)
        {
            *string = replacement;
        }

        string++;
    }
}


int toUpper(int c)
{
    if (c >= 'a' && c <= 'z')
    {
        return c - 'a' + 'A';
    }
    else
    {
        return c;
    }
}



/*
void strcpy(char* destination, char* source)
{
    while (*source != '\0')
    {
        *destination = *source;

        destination++;
        source++;
    }

    *destination = '\0';
}
*/

int strToInt(char* string)
{
    int num = 0;
    while(*string != '\0')
    {
        num *= 10;
        num += (int)((char)*string - '0');
        string++;
    }

    return num;
}




#endif
