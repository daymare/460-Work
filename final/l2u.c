
#include "ucode.c"
#include "util.c"


int main(int argc, char* argv[])
{
    // setup terminal input
    if (argc > 1)
    {
        // open the given input file and use it
        int fd = open(argv[1], O_RDONLY);

        if (fd < 0)
        {
            printf("Error: %s does not exist\n", argv[1]);
            exit(0);
        }
        dup2(fd, 0);
    }
    if (argc > 2)
    {
        // open the given output file and use it
        int fd = open(argv[2], O_WRONLY | O_CREAT);

        dup2(fd, 1);
    }

    // read in character by character and convert to uppercase
    int c = getc();

    while (c != EOF)
    {
        // convert to uppercase
        if (c != '\r' && c != '\n')
        {
            c = toUpper(c);
            printf("%c", c);
        }
        else
        {
            printf("\n");
        }

        // get next character
        c = getc();
    }
}


