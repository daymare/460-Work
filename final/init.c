
#include "ucode.c"

int main()
{
    setupConsole();
    printf("hellllllooo! Proc 1 here!");

    // fork P2, login process
    forkLogin();

    // wait forever for any children to die
    int status;
    while(1)
    {
        wait(&status);
        forkLogin();
    }
}

void forkLogin()
{
    int pid = fork();
    if (pid == 0)
    {
        // child process, exec init process
        exec("init");
    }

    // parent continues
}

