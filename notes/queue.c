
#include "proc.c"

Proc *get_proc(Proc **list); // return a proc from a list
int put_proc(Proc **list, Proc *p); // place a proc into a list
int enqueue(Proc **queue, Proc *p); // enter a proc into the queue by priority
Proc* dequeue(Proc **queue); // return the next proc to run in the queue
int printList(Proc *list); // print a list or queue of procs




