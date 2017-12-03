
#include "type.h"

#include "list.c"

void enqueue(Proc **queue, Proc *p) // enter a proc into the queue by priority
{
    /*
     *printf("list before enqueue: ");
     *printList(*queue);
     */

    // find the intersection of where the proc should go
    // proc should go where last >= proc > next
    Proc* current = *queue;

    if (current == NULL)
    {
        *queue = p;
        p->next = NULL;
        return;
    }

    // check if proc belongs at the front
    if (current->priority < p->priority)
    {
        p->next = *queue;
        *queue = p;
        return;
    }

    // find where the element belongs
    while (current->next != NULL && current->next->priority >= p->priority)
    {
        current = current->next;
    }

    insertAt(current, p);

    /*
     *printf("list after enqueue: ");
     *printList(*queue);
     */
}

// return the next proc to run in the queue
Proc* dequeue(Proc **queue) 
{
    /*
     *printf("list before dequeue: ");
     *printList(*queue);
     */

    Proc* returning = *queue;
    *queue = returning->next;
    returning->next = NULL;

    /*
     *printf("list after dequeue: ");
     *printList(*queue);
     */

    return returning;
}

