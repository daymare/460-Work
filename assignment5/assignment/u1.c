#include "ucode.c"

int main()
{
    int i;
    int pid;
    int ppid;
    int entryPoint;
    int mode;

    char line[64];
    mode = (get_cpsr() & 0x1F); // get CPSR to determine CPU mode
    printf("CPU mode = %x\n", mode);

    pid = getpid();
    ppid = getppid();
    entryPoint = get_entry();

    while(1)
    {
        printf("This is process %d in Umode at %x: parent = %d\n", pid, entryPoint, ppid);
        umenu();
        uprintf("input a command : ");
        ugetline(line); uprintf("\n");
        if (!strcmp(line, "getpid"))
            ugetpid();
        else if (!strcmp(line, "getppid"))
            ugetppid();
        else if (!strcmp(line, "ps"))
            ups();
        else if (!strcmp(line, "chname"))
            uchname();
    }
}



