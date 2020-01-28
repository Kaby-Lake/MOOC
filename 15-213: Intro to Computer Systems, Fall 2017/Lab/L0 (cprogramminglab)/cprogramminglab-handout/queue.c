/* 
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdlib.h>
#include <stdio.h>

#include "harness.h"
#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if(q == NULL)
      return q;
    /* What if malloc returned NULL? */
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if(q == NULL)
      return;
    list_ele_t * p;
    list_ele_t * next;
    for (p = q -> head; p != NULL; p = next)
    {
      next = p->next;
      free(p);
    }
      free(q);
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
 */
bool q_insert_head(queue_t *q, int v)
{
    /* if q is NULL */
    if (q == NULL)
      return false;
    list_ele_t *newh = malloc(sizeof(list_ele_t));
    /* if malloc returned NULL */
    if (newh == NULL)
      return false;

    newh->value = v;
    newh->next = q->head;
    q->head = newh;
    if (q->tail == NULL)
      q->tail = newh;
    q -> size++;
    return true;
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
 */
bool q_insert_tail(queue_t *q, int v)
{
    /* if q is NULL */
    if (q == NULL)
      return false;
    list_ele_t *newh = malloc(sizeof(list_ele_t));
    /* if malloc returned NULL */
    if (newh == NULL)
      return false;

    newh->value = v;
    newh->next = NULL;
    if (q->tail == NULL){
      q->head = newh;
      q->tail = newh;
    }
    else
    {
      q->tail->next = newh;
      q->tail = newh;
    }
    q -> size++;
    return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If vp non-NULL and element removed, store removed value at *vp.
  Any unused storage should be freed
*/
bool q_remove_head(queue_t *q, int *vp)
{
    if (q == NULL || vp == NULL || q->size == 0)
    return false;
    *vp = q->head->value;
    list_ele_t * remove = q->head;
    q->head = q->head->next;
    if (q->head == NULL)
      q -> tail = NULL;
    free(remove);
    q -> size--;
    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q == NULL)
      return 0;
    return q->size;
}

/*
  Reverse elements in queue.

  Your implementation must not allocate or free any elements (e.g., by
  calling q_insert_head or q_remove_head).  Instead, it should modify
  the pointers in the existing data structure.
 */
void q_reverse(queue_t *q)
{
    if (q == NULL || q -> size == 0 || q -> size == 1)
      return;
    list_ele_t * traversal = q->head -> next -> next;
    q -> head -> next -> next = q -> head;
    q -> head = q -> head -> next;
    q -> head -> next -> next = NULL;
    q -> tail = q -> head -> next;
    while(traversal){
        list_ele_t * newh = traversal;
        traversal = traversal -> next;
        newh->next = q->head;
        q->head = newh;
    }
    return;
}

