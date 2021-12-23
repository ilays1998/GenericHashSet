#ifndef HASHSET_H_
#define HASHSET_H_

#include <stdlib.h>
#include "Node.h"

/**
 * @def HASH_SET_INITIAL_CAP
 * The initial capacity of the hash set.
 * It means, the initial number of <b> Nodes </b> the hash set has.
 */
#define HASH_SET_INITIAL_CAP 16

/**
 * @def HASH_SET_GROWTH_FACTOR
 * The growth factor of the hash set.
 * Example: hash_set(size=16),
 *          hash_set.add([16 elements]) ->
 *          hash_set(size=32)
 */
#define HASH_SET_GROWTH_FACTOR 2

/**
 * @def hash_set_MIN_LOAD_FACTOR
 * The minimal load factor the hash set can be in.
 * Example: if the hash_set capacity is 16,
 * and it has 4 elements in it (size is 4),
 * if an element is erased, the load factor drops below 0.25,
 * so the hash set should be minimized (to 8 Nodes).
 */
#define HASH_SET_MIN_LOAD_FACTOR 0.25

/**
 * @def hash_set_MAX_LOAD_FACTOR
 * The maximal load factor the hash set can be in.
 * Example: if the hash_set capacity is 16,
 * and it has 12 elements in it (size is 12),
 * if another element is added, the load factor goes above 0.75,
 * so the hash set should be extended (to 32 Nodes).
 */
#define HASH_SET_MAX_LOAD_FACTOR 0.75

/**
 * @typedef HashFunc
 * This type of function receives a Value and returns
 * a representational number of it.
 * Example: lets say we have a Value ('Joe') that we want to store in the hash set,
 * the value is 'Joe' so it determines the index in the Node List in the hash set,
 * his index would be:  size_t ind = HashFunc('Joe') & (capacity - 1);
 */
typedef size_t (*HashFunc)(Value);

/**
 * @typedef HashSetValueCpy
 * A copy function for the Values stored in the Nodes (which are stored in the hash set).
 */
typedef void *(*HashSetValueCpy)(const void *);

/**
 * @typedef HashSetValueCmp
 * A compare function for the Values stored in the Nodes (which are stored in the hash set).
 */
typedef int (*HashSetValueCmp)(const void *, const void *);

/**
 * @typedef HashSetValueFree
 * A free function for the Values stored in Nodes (which are stored in the hash set).
 */
typedef void (*HashSetValueFree)(void **);

/**
 * @struct HashSet
 * @param NodeList dynamic array of Nodes which stores the values.
 * @param size the number of elements (Values) stored in the hash set.
 * @param capacity the number of Nodes in the hash set.
 * @param hash_func a function which "hashes" keys.
 * @param Value_cpy a function which copies Values.
 * @param Value_cmp a function which compares Values.
 * @param Value_free a function which frees Values.
 */
typedef struct HashSet {
  Node **NodesList;
  unsigned int size;
  unsigned int capacity; // num of nodes.
  HashFunc hash_func;
  HashSetValueCpy value_cpy;
  HashSetValueCmp value_cmp;
  HashSetValueFree value_free;
} HashSet;

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
    HashSetValueCmp value_cmp, HashSetValueFree value_free);

/**
 * Frees the hash set and all its elements.
 * @param p_hash_set pointer to pointer to a hash_set.
 */
void hash_set_free(HashSet **p_hash_set);

/**
 * Inserts a new Value to the hash set.
 * The function inserts *new*, *copied*, *dynamically allocated* Value,
 * @param hash_set the hash set where we want to insert the new element
 * @param value a Value we would like to add to the hashset
 * @return returns 1 for successful insertion, 0 otherwise.
 */
int hash_set_insert(HashSet *hash_set, Value value);

/**
 * The function checks if the given value exists in the hash set.
 * @param hash_set a hash set.
 * @param value the value to be checked.
 * @return 1 if the value is in the hash set, 0 otherwise.
 */
int hash_set_contains_value(HashSet *hash_set, Value value);

/**
 * The function erases a Value.
 * @param hash_set a hash set.
 * @param value the value to be erased from the hashtable
 * @return 1 if the erasing was done successfully, 0 otherwise.
 */
int hash_set_erase(HashSet *hash_set, Value value);

/**
 * This function returns the load factor of the hash set.
 * @param hash_set a hash set.
 * @return the hash set's load factor, -1 if the function failed.
 */
double hash_set_get_load_factor(HashSet *hash_set);

/**
 * This function deletes all the values in the hash set.
 * @param hash_set a hash set to be cleared.
 */
void hash_set_clear(HashSet *hash_set);

/**
 * This function gets the element in the hashset hashed to the indexed value
 * @param hash_set the hashset
 * @param value the index of the node in the hashtable that we would like to have its value
 * @return the element in the hashset hashed to the indexed value
 * NULL in case of an error or an empty node or an unexpected value
 */
Value hash_set_at(HashSet *hash_set, int value);

#endif //HASHSET_H_
