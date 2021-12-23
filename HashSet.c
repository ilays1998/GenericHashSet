//
// Created by ilays on 21/11/2021.
//
#include "HashSet.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ALLOCATION_FAIL "ERROR: the allocation fail\n"
#define EMPTY_SIZE 0
#define FAIL 0
#define ILLEGAL_HASH "ERROR: Illegal hash\n"
#define VALUE_EXIST "The value is already exist\n"
#define SUCCESSFUL 1
#define QUADRATIC_PROBING ((i + (i  * i)) / 2)) & (hash_set->capacity - 1)
#define QUADRATIC_PROBING_2 ((j + (j  * j)) / 2)) & (hash_set->capacity - 1)
#define VALUE_NOT_EXIST 0
#define NULL_HASH -1

size_t to_hash (HashSet *hash_set, Value value);
static double load_factor(size_t capacity, size_t size);
static void arr_free(HashSet* hash_set, Value *arr, size_t cap);
void insert_after_enlarge (HashSet *hash_set, Value *arr, size_t first_node,
                        size_t index, size_t j);
void insert_new_node (HashSet *hash_set, size_t first_node, size_t index, Value value);
void insert_new_node_after_reduce (HashSet *hash_set, Value *arr, size_t first_node,
                                   size_t index, size_t k);

/**
* Allocates dynamically new hash set.
* @param hash_func a function which "hashes" keys.
* @param value_cpy a function which copies Values.
* @param value_cmp a function which compares Values.
* @param value_free a function which frees Values.
* @return pointer to dynamically allocated HashSet.
* @if_fail return NULL or if one of the function pointers is null.
*/
HashSet *hash_set_alloc(
    HashFunc hash_func, HashSetValueCpy value_cpy,
    HashSetValueCmp value_cmp, HashSetValueFree value_free)
{
    if (hash_func == NULL || value_cpy == NULL || value_cmp == NULL ||
        value_free == NULL) //check if the parameters are legals
    {
        return NULL;
    }
    HashSet* new_hash = malloc (sizeof (HashSet));
    if (new_hash == NULL) //check allocation
    {
        fprintf (stderr, ALLOCATION_FAIL);
        return NULL;
    }
    new_hash->NodesList = calloc (HASH_SET_INITIAL_CAP, sizeof(Node*));
    if (new_hash->NodesList == NULL) //check allocation
    {
        fprintf (stderr, ALLOCATION_FAIL);
        return NULL;
    }
    new_hash->size = EMPTY_SIZE;
    new_hash->capacity = HASH_SET_INITIAL_CAP;
    new_hash->hash_func = hash_func;
    new_hash->value_cpy = value_cpy;
    new_hash->value_cmp = value_cmp;
    new_hash->value_free = value_free;
    return new_hash;
}

/**
 * Frees the hash set and all its elements.
 * @param p_hash_set pointer to pointer to a hash_set.
 */
void hash_set_free(HashSet **p_hash_set)
{
    if (p_hash_set == NULL || *p_hash_set == NULL)
    {
        return;
    }
    if ((*p_hash_set)->NodesList != NULL)
    {
        for (size_t i = 0; i < (*p_hash_set)->capacity; i++)
        {
          if ((*p_hash_set)->NodesList[i] != NULL)
            node_free(&((*p_hash_set)->NodesList[i]));
        }
        free((*p_hash_set)->NodesList);
        (*p_hash_set)->NodesList = NULL;
    }
    free((*p_hash_set));
    (*p_hash_set) = NULL;

}


/**
 * Inserts a new Value to the hash set.
 * The function inserts *new*, *copied*, *dynamically allocated* Value,
 * @param hash_set the hash set where we want to insert the new element
 * @param value a Value we would like to add to the hashset
 * @return returns 1 for successful insertion, 0 otherwise.
 */
int hash_set_insert(HashSet *hash_set, Value value)
{
    if (hash_set == NULL || hash_set->NodesList == NULL ||
            value == NULL || hash_set->hash_func == NULL
            || hash_set->value_cmp == NULL
            || hash_set->value_cpy == NULL || hash_set->value_free == NULL)
    {
        fprintf (stderr, ILLEGAL_HASH);
        return FAIL;
    }
    if (hash_set_contains_value (hash_set, value) == SUCCESSFUL)
    {
        fprintf (stderr, VALUE_EXIST);
        return FAIL;
    }
    size_t index = EMPTY_SIZE;
    size_t first_node = to_hash(hash_set, value);

    if ( load_factor(hash_set->capacity, hash_set->size + 1) >= HASH_SET_MAX_LOAD_FACTOR)
    {
        Value *arr = calloc (hash_set->capacity, sizeof (Value));
        for (size_t k = 0; k < hash_set->capacity; k++){
            if (hash_set->NodesList[k] != NULL && hash_set->NodesList[k]->data != NULL){
                arr[k] = hash_set->value_cpy(hash_set->NodesList[k]->data);
              }
          }
        size_t old_cap = hash_set->capacity;
        hash_set->capacity = hash_set->capacity * HASH_SET_GROWTH_FACTOR;
        hash_set->NodesList = realloc (hash_set->NodesList, hash_set->capacity * sizeof (Node*));
        for (size_t i = old_cap; i < hash_set->capacity; i++){
          hash_set->NodesList[i] = NULL;
        }
        for (size_t k = 0; k < old_cap; k++)
          {
            node_free (&hash_set->NodesList[k]);

          }

        for (size_t j = 0; j < old_cap; j++)
        {
            if (arr[j] != NULL){
              first_node = to_hash(hash_set, arr[j]);
              insert_after_enlarge(hash_set, arr, first_node, index, j);
              }
        }
        arr_free (hash_set, arr, old_cap);

    }
    first_node = to_hash(hash_set, value);
    for (size_t i = 0; i < hash_set->capacity; i++)
    {
        index = (to_hash(hash_set, value) + QUADRATIC_PROBING;
        if (hash_set->NodesList[index] == NULL)
        {
            insert_new_node(hash_set, first_node, index, value);
            break;
        }
    }

    return SUCCESSFUL;
}
void insert_new_node (HashSet *hash_set, size_t first_node, size_t index, Value value)
{
    Node* new_node = node_alloc (hash_set->value_cpy, hash_set->value_cmp, hash_set->value_free);
    new_node->data = new_node->elem_copy_func(value);
    hash_set->NodesList[index] = new_node;
    hash_set->size++;
    hash_set->NodesList[first_node]->hashCount++;
}
void insert_after_enlarge (HashSet *hash_set, Value *arr, size_t first_node,
                            size_t index, size_t j)
{
    for (size_t i = 0; i < hash_set->capacity; i++)
    {
      index = (to_hash(hash_set, arr[j]) + QUADRATIC_PROBING;
      if (hash_set->NodesList[index] == NULL)
        {
          hash_set->NodesList[index] = node_alloc (hash_set->value_cpy, hash_set->value_cmp, hash_set->value_free);
          hash_set->NodesList[index]->data = hash_set->value_cpy(arr[j]);
          hash_set->NodesList[first_node]->hashCount++;
          break;
        }
    }
}

size_t to_hash (HashSet *hash_set, Value value)
{
      return (hash_set->hash_func(value) & (hash_set->capacity - 1));
}

static double load_factor(size_t capacity, size_t size)
{
      return  (double) size / (double) capacity ;
}

/**
 * The function checks if the given value exists in the hash set.
 * @param hash_set a hash set.
 * @param value the value to be checked.
 * @return 1 if the value is in the hash set, 0 otherwise.
 */
int hash_set_contains_value(HashSet *hash_set, Value value)
{
    if (hash_set == NULL || hash_set->value_cmp == NULL || value == NULL)
    {
        fprintf (stderr, ILLEGAL_HASH);
        return FAIL;
    }
    for (size_t i = 0; i < hash_set->capacity; i++)
    {
        if (hash_set->NodesList != NULL && hash_set->NodesList[i] != NULL && hash_set->NodesList[i]->data != NULL &&
            hash_set->value_cmp(hash_set->NodesList[i]->data, value) == SUCCESSFUL)
        {
            return SUCCESSFUL;
        }
    }
    return VALUE_NOT_EXIST;
}

/**
 * The function erases a Value.
 * @param hash_set a hash set.
 * @param value the value to be erased from the hashtable
 * @return 1 if the erasing was done successfully, 0 otherwise.
 */
int hash_set_erase(HashSet *hash_set, Value value)
{
    if (hash_set == NULL || hash_set->NodesList == NULL || value == NULL || hash_set->hash_func == NULL)
    {
        fprintf (stderr, ILLEGAL_HASH);
        return FAIL;
    }
    for (size_t i = 0; i < hash_set->capacity; i++)
    {

        if ( hash_set->NodesList[i] != NULL &&  hash_set->NodesList[i]->data != NULL &&
              hash_set->value_cmp( hash_set->NodesList[i]->data, value) == SUCCESSFUL)
        {
            hash_set->value_free(&hash_set->NodesList[i]->data);
            hash_set->size--;
            if(hash_set->NodesList[i] != NULL)
                hash_set->NodesList[i]->hashCount--;
            if (  load_factor(hash_set->capacity, hash_set->size) <= (HASH_SET_MIN_LOAD_FACTOR))
            {
                Value *arr = calloc (hash_set->capacity, sizeof (Value));
                for (size_t k = 0; k < hash_set->capacity; k++){
                  if (hash_set->NodesList[k] != NULL && hash_set->NodesList[k]->data != NULL){
                    arr[k] = hash_set->value_cpy(hash_set->NodesList[k]->data);
                  }
                }
                for (size_t k = 0; k < hash_set->capacity; k++)
                {
                    node_free (&hash_set->NodesList[k]);

                }
                size_t old_cap = hash_set->capacity;
                hash_set->capacity = hash_set->capacity / HASH_SET_GROWTH_FACTOR;
                for (size_t k = 0; k < old_cap; k++)
                {
                  if (arr[k] != NULL){
                      size_t index;
                      size_t first_node = to_hash(hash_set, arr[k]);
                      for (size_t j = 0; j < hash_set->capacity; j++)
                        {
                          index = (to_hash(hash_set, arr[k]) + QUADRATIC_PROBING_2;
                          if (hash_set->NodesList[index] == NULL || hash_set->NodesList[index]->data == NULL)
                            {
                              insert_new_node_after_reduce(hash_set, arr, first_node, index, k);
                              break;
                            }
                        }


                    }
                }
                arr_free (hash_set, arr, old_cap);
                return SUCCESSFUL;
            }
            return SUCCESSFUL;
        }
    }
    return VALUE_NOT_EXIST;
}
void insert_new_node_after_reduce (HashSet *hash_set, Value *arr, size_t first_node,
                                   size_t index, size_t k)
{
    hash_set->NodesList[index] = node_alloc (hash_set->value_cpy, hash_set->value_cmp, hash_set->value_free);
    hash_set->NodesList[index]->data = hash_set->value_cpy(arr[k]);
    hash_set->NodesList[first_node]->hashCount++;
}

static void arr_free(HashSet* hash_set, Value *arr, size_t cap)
{
  for (size_t k = 0; k < cap; k++)
    {
      if (arr[k] != NULL)
      {
          hash_set->value_free(&arr[k]);
      }

    }
  free (arr);
}


/**
 * This function returns the load factor of the hash set.
 * @param hash_set a hash set.
 * @return the hash set's load factor, -1 if the function failed.
 */
double hash_set_get_load_factor(HashSet *hash_set)
{
    if (hash_set == NULL)
    {
        return NULL_HASH;
    }
    if (hash_set->size == EMPTY_SIZE)
    {
        return EMPTY_SIZE;
    }
    return ((double )hash_set->size )/ hash_set->capacity;
}

/**
 * This function deletes all the values in the hash set.
 * @param hash_set a hash set to be cleared.
 */
void hash_set_clear(HashSet *hash_set)
{
    if (hash_set == NULL)
      {
        return;
      }
    Node* tmp;
    for (size_t i = 0; i < hash_set->capacity; i++)
    {
        tmp = hash_set->NodesList[i];
        if (tmp != NULL)
        {
            if (tmp->data != NULL)
              tmp->elem_free_func(&tmp->data);
            tmp->hashCount = 0;
        }
    }
    hash_set->size = 0;
    for (size_t i = 0; i < hash_set->capacity; i++)
    {
      if (hash_set->NodesList[i] == NULL)
      {
          Node* new_node = node_alloc (hash_set->value_cpy, hash_set->value_cmp, hash_set->value_free);
          hash_set->NodesList[i] = new_node;
      }
    }
}

/**
 * This function gets the element in the hashset hashed to the indexed value
 * @param hash_set the hashset
 * @param value the index of the node in the hashtable that we would like to have its value
 * @return the element in the hashset hashed to the indexed value
 * NULL in case of an error or an empty node or an unexpected value
 */
Value hash_set_at(HashSet *hash_set, int value)
{
    if (hash_set == NULL || value < 0 || value >= (int) hash_set->capacity ||
            hash_set->NodesList == NULL || hash_set->NodesList[value] == NULL)
    {
        return NULL;
    }
    return hash_set->NodesList[value]->data;
}
