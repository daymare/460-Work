// system call interface functions.
// user mode utility functions.
// for some reason the user mode program menu print function.

#include "string.c"

int umenu()
{
    uprintf("-------------------------\n");
    uprintf("getpid getppid ps chname \n");
    uprintf("------------------00000--\n");
}

// system calls to kernel
int getpid() { return syscall(0, 0, 0, 0); }
int getppid() { return syscall(1, 0, 0, 0); }
int ps() { return syscall(2, 0, 0, 0); }
int chname(char *s) { return syscall(3, s, 0, 0); }

int uprintf(char *fmt, ...) { return syscall(4, fmt, (int*)&fmt + 1); }

int ugetpid()
{
    int pid = getpid();
    uprintf("pid = %d\n", pid);
}

int ugetppid()
{
    int ppid = getppid();
    uprintf("ppid = %d\n", ppid);
}

int uchname()
{
    char s[32];
    uprintf("input a name string : ");
    ugetline(s); uprintf("\n");
    chname(s);
}

int ups() { ps(); }

// basic user mode io
// syscalls to kernel
int ugetc() { return syscall(90, 0, 0, 0); }
int uputc(char c) { return syscall(91, c, 0, 0); }

