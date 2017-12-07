
#include "ucode.c"

int main()
{
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
        exec("login");
    }

    // parent continues
}

