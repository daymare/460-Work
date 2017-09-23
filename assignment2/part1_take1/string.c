

int strlen(char* s)
{
    int n = 0;
    while (*s != '\0')
    {
        n++;
        s++;
    }

    return n;
}

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





