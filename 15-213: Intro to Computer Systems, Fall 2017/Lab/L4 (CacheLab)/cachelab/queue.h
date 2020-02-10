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
#include <stdbool.h>
 
/************** Data structure declarations ****************/
 
/* Linked list element (You shouldn't need to change this) */
typedef struct cacheline{
	int index;
	int validBit;
	long tag;
	struct cacheline * next;
} cacheLine;



/* Queue structure */
typedef struct queue_t{
    cacheLine *head;  /* Linked list of elements */
    cacheLine *tail;
    unsigned size;
} cacheQueue;

/************** Operations on queue ************************/
 
cacheQueue *q_new()
{
    cacheQueue *q = NULL;
    if((q = malloc(sizeof(cacheQueue))))
    {
      q->head = NULL;
      q->tail = NULL;
      q->size = 0;
      return q;
    }
    else
    {
      return NULL;
    }
}
 
/* Free all storage used by queue */
void q_free(cacheQueue *q)
{
    /* How about freeing the list elements? */
    /* Free queue structure */
    if (q)
    {
      cacheLine *i = q->head;
      while(i)
      {
        cacheLine *tmp = i;
        i = i->next;
        free(tmp);
      }
      free(q);
    }
}
 
/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
 */
bool q_insert_head(cacheQueue *q, int v)
{
    cacheLine *newh;
    /* What should you do if the q is NULL? */
    if (q)
    {
      if ((newh = malloc(sizeof(cacheLine))))
      {
        newh->index = v;
        newh->next = q->head;
        q->head = newh;
        if (!q->size)
        {
          q->tail = newh;
        }
        ++q->size;
        return true;
      }
      else
      {
        return false;
      }
    }
    else
    {
      return false;
    }
}
 
 
/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
 */
bool q_insert_tail(cacheQueue *q, int v)
{
    /* You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    cacheLine *newh;
    if (q)
    {
      if ((newh = malloc(sizeof(cacheLine))))
      {
        newh->index = v;
        newh->next = NULL;
        if (q->tail)
        {
          q->tail->next = newh;
          q->tail = newh;
          ++q->size;
        }
        else
        {
          q->head = q->tail = newh;
          ++q->size;
        }
        return true;
      }
      else
      {
        return false;
      }
    }
    else
    {
      return false;
    }
}
 
/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  Any unused storage should be freed
*/
bool q_remove_head(cacheQueue *q)
{
    /* You need to fix up this code. */
    if (!q || !q->size)
    {
      return false;
    }
    else
    {
      q->head = q->head->next;
      if(q -> head == NULL)
        q -> tail = NULL;
      --q->size;
      return true;
    }
}


bool q_remove_target(cacheQueue *q, int target)
{

    if (!q || !q->size)
    {
      return false;
    }

  if(q -> head -> index == target){
    q_remove_head(q);
    return true;
    }
  cacheLine *temp = q->head;
  while(temp -> next){
    if (temp -> next -> index == target)
    {
      temp -> next = temp -> next -> next;
      if(temp -> next == NULL){q -> tail = temp;}
      q -> size--;
      return true;
    } else {
      temp = temp->next;
    }
  }
  return false;
}


int q_size(cacheQueue *q)
{
    /* You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    if (!q || !q->size)
    {
      return 0;
    }
    else
    {
      return q->size;
    }
}
 
/*
  Reverse elements in queue
 */
void q_reverse(cacheQueue *q)
{
    if (q && q->size)
    {
      int cache[q->size];
      cacheLine *tmp = q->head;
      for (int i = q->size - 1; (i >= 0) && (tmp != NULL); --i)
      {
        cache[i] = tmp -> index;
        tmp = tmp->next;
      }
      tmp = q->head;
      for (int i = 0; (i < q->size) && (tmp != NULL); ++i)
      {
        tmp->index = cache[i];
        tmp = tmp->next;
      }
    }
}