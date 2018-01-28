/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Chih-Hsiang, Wang
 * Email: wangchih@oregonstate.edu
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "dynarray.h"
#include "pq.h"


/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq {
  struct dynarray* heap;
};

struct pq_node {
  void* value;
  int priority_val;
};


/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
  struct pq* priority_queue = malloc(sizeof(struct pq));
  assert(priority_queue);

  priority_queue->heap = dynarray_create();

  return priority_queue;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
  assert(pq);

  dynarray_free(pq->heap);
  free(pq);
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
  assert(pq);

  if (dynarray_size(pq->heap) == 0) {
      return 1;
  } else {
      return 0;
  }
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
  assert(pq);

  // create the inserted node
  struct pq_node* node = malloc(sizeof(struct pq_node));
  node->value = value;
  node->priority_val = priority;

  dynarray_insert(pq->heap, -1, node); // insert at the last of array
  int size = dynarray_size(pq->heap);
  int i = size - 1; //index of last node

  // move the value to follow rule of heap
  struct dynarray* heap = pq->heap; // simplify

  if (size >= 2) {
    struct pq_node* child = dynarray_get(heap, i);
    struct pq_node* parent = dynarray_get(heap, (i-1)/2);

    while (child->priority_val < parent->priority_val) {
      dynarray_set(heap, i, parent);
      dynarray_set(heap, (i-1)/2, child);
      i = (i-1)/2;

      if (i == 0) {
        break;
      }
      child = dynarray_get(heap, i);
      parent = dynarray_get(heap, (i-1)/2);
    }
  }
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
  assert(pq);

  struct pq_node* first_node = dynarray_get(pq->heap, 0);

  return first_node->value;
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
  assert(pq);

  struct pq_node* first_node = dynarray_get(pq->heap, 0);

  return first_node->priority_val;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
  assert(pq);

  // get the first_node's value
  struct dynarray* heap = pq->heap; // simplify
  struct pq_node* first_node = dynarray_get(heap, 0);
  void* first_value = first_node->value;

  // substitue first by last node and delete last node
  int i = dynarray_size(heap);
  if (i > 1) {
    struct pq_node* get_last = dynarray_get(heap, -1);
    dynarray_set(heap, 0, get_last);
  }
  dynarray_remove(heap, -1);
  free(first_node); // remember to free the node
  i = dynarray_size(heap);

  // move the value to follow rule of heap
  int j = 0;

  struct pq_node* parent;
  struct pq_node* left_child;
  struct pq_node* right_child;

  if (i > 0){
    parent = dynarray_get(heap, j);
  }
  if (2*j+1 < i) {
    left_child = dynarray_get(heap, 2*j+1);
  }
  if (2*j+2 < i) {
    right_child = dynarray_get(heap, 2*j+2);
  }

  while (true) {
    if (2*j+1 >= i) { // check if there is no left child
      break;
    }

    if (2*j+2 >= i) { // no right child but with left child
      if (parent->priority_val > left_child->priority_val) {
        dynarray_set(heap, j, left_child);
        dynarray_set(heap, 2*j+1, parent);
        j = 2*j+1;

        parent = dynarray_get(heap, j);
        if (2*j+1 < i) {
          left_child = dynarray_get(heap, 2*j+1);
        }
        if (2*j+2 < i) {
          right_child = dynarray_get(heap, 2*j+2);
        }

      } else { // break if it is in order
        break;
      }
    }

    else { // have left and right child
      if (parent->priority_val <= left_child->priority_val\
          && parent->priority_val <= right_child->priority_val) {
        break;
      }

      else {
        if (right_child->priority_val < left_child->priority_val) {
          dynarray_set(heap, j, right_child);
          dynarray_set(heap, 2*j+2, parent);
          j = 2*j+2;

          parent = dynarray_get(heap, j);
          if (2*j+1 < i) {
            left_child = dynarray_get(heap, 2*j+1);
          }
          if (2*j+2 < i) {
            right_child = dynarray_get(heap, 2*j+2);
          }
        }

        else if (right_child->priority_val >= left_child->priority_val) {
          dynarray_set(heap, j, left_child);
          dynarray_set(heap, 2*j+1, parent);
          j = 2*j+1;

          parent = dynarray_get(heap, j);
          if (2*j+1 < i) {
            left_child = dynarray_get(heap, 2*j+1);
          }
          if (2*j+2 < i) {
            right_child = dynarray_get(heap, 2*j+2);
          }
        }
      }
    }
  }

  return first_value;
}
