

#include "ucode.c"
#include "util.c"

int main()
{
    while(1)
    {
        // prompt for a command
        char cwd[64];
        getcwd(cwd);
        printf("%s : ", cwd);

        char command[50];
        gets(command);
        normalize(command);

        // check for logout
        if (strcmp(command, "logout") == 0)
        {
            // kill this process
            exit(0);
        }

        // one command case
        forkSingleCommand(command);
    }
}


void forkSingleCommand(char* command)
{
    int status = 0;
    int pid = fork();

    if (pid == 0)
    {
        exec(command);
    }
    else
    {
        pid = wait(&status);
    }
}


