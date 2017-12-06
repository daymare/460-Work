
#include "ucode.c"
#include "util.c"


int main(int argc, char * argv[])
{
    setupConsole();

    if (argc == 2)
    {
        // open the proper file and use it
        close(0); // close stdin
        open(argv[1], O_RDONLY);
    }

    // just keep reading and outputting back
    int c = 'f';
    while(c != EOF && c != '\0')
    {
        c = getc();
        printf("%c", c);
    }
}


