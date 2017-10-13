
#ifndef LIST_C
#define LIST_C

#include "type.h"

extern Proc* sleepingList;

int len(Proc *list)
{
    int len = 0;

    while (list != NULL)
    {
        len++;
        list = list->next;
    }
}

Proc *getProc(Proc **list) // return a proc from a list
{
    if (*list == NULL)
    {
        return NULL;
    }

    Proc * returning = *list;
    *list = returning->next;
    returning->next = NULL;

    return returning;
}

void putProc(Proc **list, Proc *p) // place a proc into a list
{
    p->next = *list;
    *list = p;
}

void insertAt(Proc* elementBefore, Proc* p)
{
    p->next = elementBefore->next;
    elementBefore->next = p;
}

void removeProc(Proc **list, Proc *p)
{
    // check if the proc to remove is the first one in the sleeping list
    if (*list == p)
    {
        *list = p->next;
        p->next = NULL;

        return;
    }

    Proc * current = *list;

    while (current->next != p)
    {
        current = current->next;
    }

    // next is now the proc to remove
    current->next = p->next;
    p->next = NULL;

}

int printList(Proc *list) // print a list or queue of procs
{
    Proc * current = list;
    int i = 0;

    while (current != NULL)
    {
        printf("%d -> ", current->pid);
        if (i == 10)
        {
            printf("houston we have a problem!\n");
            return -1;
        }
        current = current->next;

        i++;
    }

    printf("NULL\n");
}


#endif
