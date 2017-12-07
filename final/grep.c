
#include "ucode.c"
#include "util.c"

int main(int argc, char* argv[])
{
    // print out a newline to make things prettier with KC's shell
    printf("\n");

    // check for terminal cases
    if (argc < 2 || argc > 3)
    {
        // cannot have less than two arguments or more than 3 arguments to grep
        printf("Usage: grep pattern [filename]");
        exit(0);
    }

    if (argc == 3)
    {
        // open the file to check
        int fd = open(argv[2], O_RDONLY);
        
        if (fd < 0)
        {
            printf("Error: %s does not exist\n", argv[1]);
            exit(0);
        }
        dup2(fd, 0);
    }


    // read in line by line
    char line[64];
    int gotChars = 1;
    int pattern_result = 0;

    gotChars = getline(line);
    
    while(gotChars != 0)
    {
       // check for the pattern in the line
       pattern_result = checkPattern(argv[1], line);
       
       // if the pattern exists print it out 
       if (pattern_result == 1)
       {
            printf("%s", line);
       }

       gotChars = getline(line);
    }

    printf("\n");
}

