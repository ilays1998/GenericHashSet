#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_runner.h"
#include "Node.h"
#include "HashSet.h"
#include "Utils.h"
#include "Hash.h"

#define T_ERROR_CODE (-1)
#define T_CMP_EQUAL (1)
#define T_CMP_NOT_EQUAL (0)
#define T_SUCCESS_RES (1)
#define T_FAILURE_RES (0)

size_t data_freed_count = 0;
size_t is_compared = 0;
size_t copy_count = 0;




//  for (double kI = 0; kI < ELEMENTS / 2; ++kI) {
//      int conf_erase = hash_set_erase (hash_set, &kI);
//      if (conf_erase != 1) {
//          hash_set_free (&hash_set);
//          return 1;
//        }
//    }
//  if (!(hash_set->size == 50 && hash_set->capacity == 128)) {
//      hash_set_free (&hash_set);
//      return 1;
//    }
//
//  for (double kI = ELEMENTS / 2; kI < ELEMENTS; ++kI) {
//      int conf_erase = hash_set_erase (hash_set, &kI);
//      if (conf_erase != 1) {
//          hash_set_free (&hash_set);
//          return 1;
//        }
//    }
//
//  if (!(hash_set->size == 0 && hash_set->capacity == 1)) {
//      hash_set_free (&hash_set);
//      return 1;
//    }
//
//  // HashSet free.
//  hash_set_free (&hash_set);
//
//  return 0;
//}
char *create_char_pointer(char value) {
    char *char_p = malloc(sizeof(char));
    *char_p = value;
    return char_p;
}

int *create_int_pointer(int value) {
    int *int_p = malloc(sizeof(int));
    *int_p = value;
    return int_p;
}

void *test_char_key_cpy(const void *key) {
    copy_count += 1;
    return char_key_cpy(key);
}

void test_char_key_free(void **val) {
    data_freed_count += 1;
    char_key_free(val);
}

int test_char_key_cmp(const void *val_1, const void *val_2) {
    is_compared = 1;
    return char_key_cmp(val_1, val_2);
}

size_t test_hash_char(void *elem) {
    return (size_t) (*(char *) (elem));
}

Node *testing_node_alloc() {
    return node_alloc(test_char_key_cpy, test_char_key_cmp,
                      test_char_key_free);
}

HashSet *testing_hash_set_alloc() {
    return hash_set_alloc(test_hash_char,
                          test_char_key_cpy,
                          test_char_key_cmp,
                          test_char_key_free);
}

int test_node_alloc_valid() {
    Node *node = testing_node_alloc();
    int result = node->data == NULL &&
                 node->hashCount == 0 &&
                 node->elem_copy_func == test_char_key_cpy &&
                 node->elem_cmp_func == test_char_key_cmp &&
                 node->elem_free_func == test_char_key_free;

    node_free(&node);
    return result;
}


typedef struct ComplexStruct {
    int a;
    double b;
    char c;
    char *d;
} ComplexStruct;

char *copy_str(char *val) {
    char *d = malloc(strlen(val) + 1);
    strcpy(d, val);

    return d;
}

void *complex_key_cpy(const void *key) {
    ComplexStruct c = *(ComplexStruct *) key;
    ComplexStruct *c_copy = malloc(sizeof(ComplexStruct));

    c_copy->a = c.a;
    c_copy->b = c.b;
    c_copy->c = c.c;
    c_copy->d = copy_str(c.d);

    return c_copy;
}

void complex_key_free(void **val) {
    ComplexStruct *c = *(ComplexStruct **) val;
    free(c->d);
    free(c);

    *val = NULL;
}

int complex_key_cmp(const void *val_1, const void *val_2) {
    ComplexStruct c1 = *(ComplexStruct *) val_1;
    ComplexStruct c2 = *(ComplexStruct *) val_2;

    return c1.a == c2.a && c1.b == c2.b && c1.c == c2.c &&
           (strcmp(c1.d, c2.d) == 0);
}

size_t complex_hash(void *elem) {
    ComplexStruct c = *(ComplexStruct *) elem;
    size_t hash_val = ((size_t) c.a) * ((size_t) c.b) * ((size_t) c.c);

    for (int i = 0; i < strlen(c.d); i++) {
        hash_val += ((size_t) (c.d)[i]);
    }
//    printf("hash: %lu, %lu\n", hash_val, hash_val % 32);

    return hash_val;
}


int test_node_alloc_elem_copy_func_is_null() {
    Node *node = node_alloc(NULL, char_key_cmp, char_key_free);
    return node == NULL;
}

int test_node_alloc_elem_cmp_func_is_null() {
    Node *node = node_alloc(char_key_cpy, NULL, char_key_free);
    return node == NULL;
}

int test_node_alloc_elem_free_func_is_null() {
    Node *node = node_alloc(char_key_cpy, char_key_cmp, NULL);
    return node == NULL;
}

int test_node_free_null_assignment() {
    Node *node = testing_node_alloc();
    node_free(&node);

    return node == NULL;
}

int test_node_free_null_node_pointer() {
    // if this test crashes - it's because node_free didn't check if node pointer is null
    node_free(NULL);
    return 1;
}

int test_node_free_null_node() {
    // if this test crashes - it's because node_free didn't check if node is null
    Node *node = NULL;
    node_free(&node);

    return 1;
}

int test_node_free_null_data() {
    Node *node = testing_node_alloc();
    data_freed_count = 0;
    node_free(&node);

    return data_freed_count == 0;
}

int test_node_free_free_data_using_free_func() {
    Node *node = testing_node_alloc();
    node->data = malloc(sizeof(char));
    data_freed_count = 0;
    node_free(&node);

    return data_freed_count == 1;
}

int test_node_free_null_elem_free_func() {
    Node *node = testing_node_alloc();
    node->elem_free_func = NULL;
    node->data = malloc(sizeof(char));
    data_freed_count = 0;
    node_free(&node);

    // I checked against the school's solution, node should not be freed
    int result = node != NULL && data_freed_count == 0;

    char_key_free(&(node->data));
    free(node);
    return result;
}

int test_check_node_node_is_null() {
    char *ptr = malloc(sizeof(char));
    int result = check_node(NULL, ptr);
    free(ptr);

    return result == T_ERROR_CODE;
}

int test_check_node_value_is_null() {
    Node *node = testing_node_alloc();

    int result = check_node(node, NULL);
    node_free(&node);

    return result == T_ERROR_CODE;
}

int test_check_node_cmp_func_is_null() {
    char *ptr = malloc(sizeof(char));
    Node *node = testing_node_alloc();

    node->elem_cmp_func = NULL;
    int result = check_node(node, ptr);

    free(ptr);
    node_free(&node);

    return result == T_ERROR_CODE;
}

int test_check_node_compare_using_cmp_func_equal() {
    Node *node = testing_node_alloc();
    char *data_ptr = malloc(sizeof(char));
    char *node_data = malloc(sizeof(char));

    char data_char = 'A';
    *data_ptr = data_char;
    *node_data = data_char;
    node->data = node_data;

    is_compared = 0;
    int cmp_res = check_node(node, data_ptr);
    int result = is_compared == 1 && cmp_res == T_CMP_EQUAL;

    node_free(&node);
    free(data_ptr);

    return result;
}

int test_check_node_compare_using_cmp_func_not_equal() {
    Node *node = testing_node_alloc();
    char *data_ptr = malloc(sizeof(char));
    char *node_data = malloc(sizeof(char));

    *data_ptr = 'A';
    *node_data = 'B';
    node->data = node_data;

    is_compared = 0;

    int cmp_res = check_node(node, data_ptr);
    int result = is_compared == 1 && cmp_res == T_CMP_NOT_EQUAL;

    node_free(&node);
    free(data_ptr);

    return result;
}

int test_set_node_data_node_is_null() {
    char *ptr = malloc(sizeof(char));
    int set_res = set_node_data(NULL, ptr);

    free(ptr);
    return set_res == T_FAILURE_RES;
}

int test_set_node_data_data_is_null() {
    Node *node = testing_node_alloc();
    int set_res = set_node_data(node, NULL);

    node_free(&node);
    return set_res == T_FAILURE_RES;
}

int test_set_node_data_elem_copy_func_is_null() {
    Node *node = testing_node_alloc();
    char *ptr = malloc(sizeof(char));

    node->elem_copy_func = NULL;
    int set_res = set_node_data(node, ptr);

    int result = set_res == T_FAILURE_RES && node->data == NULL;

    free(ptr);
    node_free(&node);
    return result;
}

int test_set_node_data_elem_free_func_is_null() {
    Node *node = testing_node_alloc();
    char *ptr = malloc(sizeof(char));

    NodeElemFree free_tmp = node->elem_free_func;
    node->elem_free_func = NULL;
    int set_res = set_node_data(node, ptr);

    int result = set_res == T_FAILURE_RES && node->data == NULL;

    free(ptr);
    node->elem_free_func = free_tmp;
    node_free(&node);
    return result;
}

int test_set_node_data_valid() {
    Node *node = testing_node_alloc();
    char *ptr = malloc(sizeof(char));
    *ptr = 'A';

    copy_count = 0;
    int set_res = set_node_data(node, ptr);

    int result = set_res == T_SUCCESS_RES &&
                 char_key_cmp(node->data, ptr) &&
                 copy_count == 1;

    free(ptr);
    node_free(&node);
    return result;
}

int test_set_node_data_update_data() {
    Node *node = testing_node_alloc();
    char *ptr1 = malloc(sizeof(char));
    *ptr1 = 'A';
    char *ptr2 = malloc(sizeof(char));
    *ptr2 = 'B';

    copy_count = 0;
    int set_res1 = set_node_data(node, ptr1);
    int set_res2 = set_node_data(node, ptr2);

    int result = set_res1 == T_SUCCESS_RES &&
                 set_res2 == T_SUCCESS_RES &&
                 char_key_cmp(node->data, ptr2) &&
                 copy_count == 2;

    free(ptr1);
    free(ptr2);
    node_free(&node);
    return result;
}

int test_clear_node_node_is_null() {
    // should not crash
    clear_node(NULL);
    return 1;
}

int test_clear_node_data_is_null() {
    // should not crash
    Node *node = testing_node_alloc();
    clear_node(node);

    node_free(&node);

    return 1;
}

int test_clear_node_free_func_is_null() {
    // should not crash
    char *ptr = malloc(sizeof(char));
    Node *node = testing_node_alloc();
    set_node_data(node, ptr);

    node->elem_free_func = NULL;
    clear_node(node);

    char_key_free(&node->data);
    free(node);
    free(ptr);

    return 1;
}

int test_clear_node_valid() {
    char *ptr = malloc(sizeof(char));
    Node *node = testing_node_alloc();
    set_node_data(node, ptr);

    clear_node(node);

    int result = node->data == NULL;

    // clear_node should free data itself, so there shouldn't be any leaks here
    free(node);
    free(ptr);

    return result;
}

int test_get_hash_count_node_is_null() {
    return get_hash_count(NULL) == T_ERROR_CODE;
}

int test_get_hash_count_valid() {
    int hash_count = 7;
    Node *node = testing_node_alloc();
    node->hashCount = hash_count;

    int result = get_hash_count(node) == hash_count;

    node_free(&node);

    return result;
}

// ----------------------------

int test_hash_set_alloc_null_hash_func() {
    HashSet *hash_set = hash_set_alloc(NULL,
                                       test_char_key_cpy,
                                       test_char_key_cmp,
                                       test_char_key_free);
    return hash_set == NULL;
}

int test_hash_set_alloc_null_value_cpy() {
    HashSet *hash_set = hash_set_alloc(test_hash_char,
                                       NULL,
                                       test_char_key_cmp,
                                       test_char_key_free);
    return hash_set == NULL;
}


int test_hash_set_alloc_null_value_cmp() {
    HashSet *hash_set = hash_set_alloc(test_hash_char,
                                       test_char_key_cpy,
                                       NULL,
                                       test_char_key_free);
    return hash_set == NULL;
}

int test_hash_set_alloc_null_value_free() {
    HashSet *hash_set = hash_set_alloc(test_hash_char,
                                       test_char_key_cpy,
                                       test_char_key_cmp,
                                       NULL);
    return hash_set == NULL;
}

int test_hash_set_alloc_null_value_valid() {
    HashSet *hash_set = testing_hash_set_alloc();

    int result = hash_set->size == 0 &&
                 hash_set->capacity == HASH_SET_INITIAL_CAP &&
                 hash_set->hash_func == test_hash_char &&
                 hash_set->value_cpy == test_char_key_cpy &&
                 hash_set->value_cmp == test_char_key_cmp &&
                 hash_set->value_free == test_char_key_free;

    hash_set_free(&hash_set);
    return result;
}

int test_hash_set_free_null_pointer_to_hash_set() {
    // this test should not crash
    hash_set_free(NULL);

    return 1;
}

int test_hash_set_free_pointer_to_null_hash_set() {
    // this test should not crash
    HashSet *hash_set = NULL;
    hash_set_free(&hash_set);

    return 1;
}

int test_hash_set_free_valid_data_not_null() {
    HashSet *hash_set = testing_hash_set_alloc();
    size_t size = 3;
    char *ptr1 = create_char_pointer('A');
    char *ptr2 = create_char_pointer('B');
    char *ptr3 = create_char_pointer('A' + (HASH_SET_INITIAL_CAP - 2));

    hash_set_insert(hash_set, ptr1);
    hash_set_insert(hash_set, ptr2);
    hash_set_insert(hash_set, ptr3);

    data_freed_count = 0;
    hash_set_free(&hash_set);

    free(ptr1);
    free(ptr2);
    free(ptr3);

    return data_freed_count == size;
}

int test_hash_set_free_valid_some_null_nodes() {
    HashSet *hash_set = testing_hash_set_alloc();
    size_t amount_to_free = 3;
    char *ptr1 = create_char_pointer('A');
    char *ptr2 = create_char_pointer('B');
    char *ptr3 = create_char_pointer('C');
    char *ptr4 = create_char_pointer('A' + (HASH_SET_INITIAL_CAP - 2));

    hash_set_insert(hash_set, ptr1);
    hash_set_insert(hash_set, ptr2);
    hash_set_insert(hash_set, ptr3);
    hash_set_insert(hash_set, ptr4);
    hash_set_erase(hash_set, ptr4);

    data_freed_count = 0;
    hash_set_free(&hash_set);

    free(ptr1);
    free(ptr2);
    free(ptr3);
    free(ptr4);

    return data_freed_count == amount_to_free;
}


int test_hash_set_insert_simple_valid() {
    HashSet *hash_set = testing_hash_set_alloc();
    char *char_p1 = create_char_pointer('A');
    char *char_p2 = create_char_pointer('B');

    int expected_char_p1_index = 1; // hash('A') % 16 = hash(65) % 16 = 65 % 16 = 1
    int expected_char_p2_index = 2; // hash('B') % 16 = hash(66) % 16 = 65 % 16 = 2

    int insertion_res1 = hash_set_insert(hash_set, char_p1);

    Node *expected_node1 = hash_set->NodesList[expected_char_p1_index];
    int result = insertion_res1 == T_SUCCESS_RES &&
                 hash_set->capacity == HASH_SET_INITIAL_CAP &&
                 hash_set->size == 1 &&
                 expected_node1->data != NULL &&
                 test_char_key_cmp(expected_node1->data, char_p1) &&
                 expected_node1->hashCount == 1;

    int insertion_res2 = hash_set_insert(hash_set, char_p2);

    Node *expected_node2 = hash_set->NodesList[expected_char_p2_index];
    result = result &&
             insertion_res2 == T_SUCCESS_RES &&
             hash_set->capacity == HASH_SET_INITIAL_CAP &&
             hash_set->size == 2 &&
             expected_node1->data != NULL &&
             test_char_key_cmp(expected_node1->data, char_p1) &&
             expected_node1->hashCount == 1 &&
             expected_node2->data != NULL &&
             test_char_key_cmp(expected_node2->data, char_p2) &&
             expected_node2->hashCount == 1;


    hash_set_free(&hash_set);
    free(char_p1);
    free(char_p2);

    return result;
}

int test_hash_set_insert_collision_valid() {
    HashSet *hash_set = testing_hash_set_alloc();
    char *char_p1 = create_char_pointer('A');
    char *char_p2 = create_char_pointer('A' + HASH_SET_INITIAL_CAP);

    int expected_char_p1_index = 1; // hash(65) % 16 = 65 % 16 = 1
    int expected_char_p2_index = 2;

    int insertion_res1 = hash_set_insert(hash_set, char_p1);

    Node *expected_node1 = hash_set->NodesList[expected_char_p1_index];
    int result = insertion_res1 == T_SUCCESS_RES &&
                 hash_set->capacity == HASH_SET_INITIAL_CAP &&
                 hash_set->size == 1 &&
                 expected_node1->data != NULL &&
                 test_char_key_cmp(expected_node1->data, char_p1) &&
                 expected_node1->hashCount == 1;

    int insertion_res_2 = hash_set_insert(hash_set, char_p2);

    Node *expected_node2 = hash_set->NodesList[expected_char_p2_index];
    result = result &&
             insertion_res_2 == T_SUCCESS_RES &&
             hash_set->capacity == HASH_SET_INITIAL_CAP &&
             hash_set->size == 2 &&
             expected_node1->data != NULL &&
             test_char_key_cmp(expected_node1->data, char_p1) &&
             expected_node1->hashCount == 2 &&
             expected_node2->data != NULL &&
             test_char_key_cmp(expected_node2->data, char_p2) &&
             expected_node2->hashCount == 0;


    hash_set_free(&hash_set);
    free(char_p1);
    free(char_p2);

    return result;
}

int test_hash_set_insert_same_value() {
    HashSet *hash_set = testing_hash_set_alloc();
    char *char_p1 = create_char_pointer('A');

    int expected_char_p1_index = 1; // hash(65) % 16 = 65 % 16 = 1

    int insertion_res1 = hash_set_insert(hash_set, char_p1);

    Node *expected_node1 = hash_set->NodesList[expected_char_p1_index];
    int result = insertion_res1 == T_SUCCESS_RES &&
                 hash_set->capacity == HASH_SET_INITIAL_CAP &&
                 hash_set->size == 1 &&
                 expected_node1->data != NULL &&
                 test_char_key_cmp(expected_node1->data, char_p1) &&
                 expected_node1->hashCount == 1;

    int insertion_res2 = hash_set_insert(hash_set, char_p1);

    result = result &&
             insertion_res2 == T_FAILURE_RES &&
             hash_set->capacity == HASH_SET_INITIAL_CAP &&
             hash_set->size == 1 &&
             expected_node1->data != NULL &&
             test_char_key_cmp(expected_node1->data, char_p1) &&
             expected_node1->hashCount == 1;


    hash_set_free(&hash_set);
    free(char_p1);

    return result;
}


int test_hash_set_insert_increase_capacity() {
    HashSet *hash_set = hash_set_alloc(hash_int, int_key_cpy, int_key_cmp,
                                       int_key_free);
    int result = 1;

    unsigned int capacity = HASH_SET_INITIAL_CAP;

    unsigned int size = 0;
    for (unsigned int i = 0; i < 7; i++) {
        unsigned int iterations;
        if (i == 0) {
            iterations = capacity * HASH_SET_MAX_LOAD_FACTOR - 1;
        } else {
            iterations = (capacity * HASH_SET_MAX_LOAD_FACTOR *
                          (1 - (1.0 / HASH_SET_GROWTH_FACTOR)));
        }

        for (unsigned int j = 0; j < iterations; j++) {
            int *int_p = malloc(sizeof(int));
            *int_p = (int) size;
            hash_set_insert(hash_set, int_p);
            free(int_p);
            size++;
            if (hash_set->capacity != capacity || hash_set->size != size) {
                result = 0;
                break;
            }
        }

        unsigned int hash_count_sum = 0;
        for (int j = 0; j < hash_set->capacity; j++) {
            int hash_count = get_hash_count(hash_set->NodesList[j]);
            hash_count_sum += hash_count != T_ERROR_CODE ? hash_count : 0;
        }

        if (hash_count_sum != size) {
            result = 0;
        }

        if (result == 0) break;
        capacity *= HASH_SET_GROWTH_FACTOR;
    }

    hash_set_free(&hash_set);

    return result;
}

int test_hash_set_insert_null_hash_set() {
    char *char_p1 = create_char_pointer('A');
    int insertion_res = hash_set_insert(NULL, char_p1);

    free(char_p1);
    return insertion_res == T_FAILURE_RES;
}

int test_hash_set_insert_null_pointer() {
    HashSet *hash_set = testing_hash_set_alloc();
    int insertion_res = hash_set_insert(hash_set, NULL);

    hash_set_free(&hash_set);
    return insertion_res == T_FAILURE_RES;
}

int test_hash_set_insert_null_hash_func() {
    HashSet *hash_set = testing_hash_set_alloc();
    char *char_p1 = create_char_pointer('A');

    hash_set->hash_func = NULL;
    int insertion_res = hash_set_insert(hash_set, char_p1);

    hash_set_free(&hash_set);
    free(char_p1);
    return insertion_res == T_FAILURE_RES;
}

int test_hash_set_insert_null_value_cpy() {
    HashSet *hash_set = testing_hash_set_alloc();
    char *char_p1 = create_char_pointer('A');

    hash_set->value_cpy = NULL;
    int insertion_res = hash_set_insert(hash_set, char_p1);

    hash_set_free(&hash_set);
    free(char_p1);
    return insertion_res == T_FAILURE_RES;
}

int test_hash_set_insert_null_value_cmp() {
    HashSet *hash_set = testing_hash_set_alloc();
    char *char_p1 = create_char_pointer('A');

    hash_set->value_cmp = NULL;
    int insertion_res = hash_set_insert(hash_set, char_p1);

    hash_set_free(&hash_set);
    free(char_p1);
    return insertion_res == T_FAILURE_RES;
}

int test_hash_set_insert_null_value_free() {
    HashSet *hash_set = testing_hash_set_alloc();
    char *char_p1 = create_char_pointer('A');

    HashSetValueFree value_free_tmp = hash_set->value_free;
    hash_set->value_free = NULL;
    int insertion_res = hash_set_insert(hash_set, char_p1);

    hash_set->value_free = value_free_tmp;
    hash_set_free(&hash_set);
    free(char_p1);
    return insertion_res == T_FAILURE_RES;
}

int test_hash_set_contains_value_hash_set_is_null() {
    char *char_p = create_char_pointer('A');
    int result = hash_set_contains_value(NULL, char_p) == T_FAILURE_RES;

    free(char_p);

    return result;
}


int test_hash_set_contains_value_value_is_null() {
    HashSet *hash_set = testing_hash_set_alloc();
    int result = hash_set_contains_value(hash_set, NULL) == T_FAILURE_RES;

    hash_set_free(&hash_set);
    return result;
}

int test_hash_set_contains_value_contains() {
    HashSet *hash_set = testing_hash_set_alloc();
    char *char_p1 = create_char_pointer('A');
    char *char_p2 = create_char_pointer('B');
    char *char_p3 = create_char_pointer('C');
    char *char_p4 = create_char_pointer('A' + HASH_SET_INITIAL_CAP);
    char *char_p5 = create_char_pointer('A' + HASH_SET_INITIAL_CAP * 2);

    hash_set_insert(hash_set, char_p1);
    int result = hash_set_contains_value(hash_set, char_p1) == T_SUCCESS_RES;
    hash_set_insert(hash_set, char_p2);
    result = result &&
             hash_set_contains_value(hash_set, char_p1) == T_SUCCESS_RES &&
             hash_set_contains_value(hash_set, char_p2) == T_SUCCESS_RES;
    hash_set_insert(hash_set, char_p3);
    result = result &&
             hash_set_contains_value(hash_set, char_p1) == T_SUCCESS_RES &&
             hash_set_contains_value(hash_set, char_p2) == T_SUCCESS_RES &&
             hash_set_contains_value(hash_set, char_p3) == T_SUCCESS_RES;

    // test probing
    hash_set_insert(hash_set, char_p4);
    hash_set_insert(hash_set, char_p5);

    result = result &&
             hash_set_contains_value(hash_set, char_p1) == T_SUCCESS_RES &&
             hash_set_contains_value(hash_set, char_p2) == T_SUCCESS_RES &&
             hash_set_contains_value(hash_set, char_p3) == T_SUCCESS_RES &&
             hash_set_contains_value(hash_set, char_p4) == T_SUCCESS_RES &&
             hash_set_contains_value(hash_set, char_p5) == T_SUCCESS_RES;

    hash_set_erase(hash_set, char_p1);
    result = result &&
             hash_set_contains_value(hash_set, char_p1) == T_FAILURE_RES &&
             hash_set_contains_value(hash_set, char_p2) == T_SUCCESS_RES &&
             hash_set_contains_value(hash_set, char_p3) == T_SUCCESS_RES &&
             hash_set_contains_value(hash_set, char_p4) == T_SUCCESS_RES &&
             hash_set_contains_value(hash_set, char_p5) == T_SUCCESS_RES;

    free(char_p1);
    free(char_p2);
    free(char_p3);
    free(char_p4);
    free(char_p5);
    hash_set_free(&hash_set);
    return result;
}


int test_hash_set_contains_value_not_contains() {
    HashSet *hash_set = testing_hash_set_alloc();
    char *char_p1 = create_char_pointer('A');
    char *char_p2 = create_char_pointer('B');
    char *char_p_not_contained = create_char_pointer('C');

    int result = hash_set_contains_value(hash_set, char_p_not_contained) ==
                 T_FAILURE_RES;
    hash_set_insert(hash_set, char_p1);
    result = result &&
             hash_set_contains_value(hash_set, char_p_not_contained) ==
             T_FAILURE_RES;
    hash_set_insert(hash_set, char_p2);
    result = result &&
             hash_set_contains_value(hash_set, char_p_not_contained) ==
             T_FAILURE_RES;

    free(char_p1);
    free(char_p2);
    free(char_p_not_contained);
    hash_set_free(&hash_set);
    return result;
}

int test_hash_set_erase_null_hash_set() {
    char *char_p1 = create_char_pointer('A');
    int erase_res = hash_set_erase(NULL, char_p1);

    free(char_p1);
    return erase_res == T_FAILURE_RES;
}

int test_hash_set_erase_null_pointer() {
    HashSet *hash_set = testing_hash_set_alloc();
    int erase_res = hash_set_erase(hash_set, NULL);

    hash_set_free(&hash_set);
    return erase_res == T_FAILURE_RES;
}

int test_hash_set_erase_null_hash_func() {
    HashSet *hash_set = testing_hash_set_alloc();
    char *char_p1 = create_char_pointer('A');

    hash_set->hash_func = NULL;
    int erase_res = hash_set_erase(hash_set, char_p1);

    hash_set_free(&hash_set);
    free(char_p1);
    return erase_res == T_FAILURE_RES;
}

int test_hash_set_erase_null_value_cpy() {
    HashSet *hash_set = testing_hash_set_alloc();
    char *char_p1 = create_char_pointer('A');

    hash_set->value_cpy = NULL;
    int erase_res = hash_set_erase(hash_set, char_p1);

    hash_set_free(&hash_set);
    free(char_p1);
    return erase_res == T_FAILURE_RES;
}

int test_hash_set_erase_null_value_cmp() {
    HashSet *hash_set = testing_hash_set_alloc();
    char *char_p1 = create_char_pointer('A');

    hash_set->value_cmp = NULL;
    int erase_res = hash_set_erase(hash_set, char_p1);

    hash_set_free(&hash_set);
    free(char_p1);
    return erase_res == T_FAILURE_RES;
}

int test_hash_set_erase_null_value_free() {
    HashSet *hash_set = testing_hash_set_alloc();
    char *char_p1 = create_char_pointer('A');

    HashSetValueFree value_free_tmp = hash_set->value_free;
    hash_set->value_free = NULL;
    int erase_res = hash_set_erase(hash_set, char_p1);

    hash_set->value_free = value_free_tmp;
    hash_set_free(&hash_set);
    free(char_p1);
    return erase_res == T_FAILURE_RES;
}

int hash_set_erase_minimal_capacity() {
    HashSet *hash_set = testing_hash_set_alloc();
    char *char_p1 = create_char_pointer('A');
    int result =
            hash_set_insert(hash_set, char_p1) &&
            hash_set_erase(hash_set, char_p1) && // capacity is 8
            hash_set_insert(hash_set, char_p1) &&
            hash_set_erase(hash_set, char_p1) && // capacity is 4
            hash_set_insert(hash_set, char_p1) &&
            hash_set_erase(hash_set, char_p1) && // capacity is 2
            hash_set_insert(hash_set, char_p1) &&
            hash_set_erase(hash_set, char_p1) && // capacity is 1
            hash_set_insert(hash_set, char_p1) &&
            hash_set_erase(hash_set, char_p1) && // capacity should still be 1
            hash_set_insert(hash_set, char_p1) &&
            hash_set_erase(hash_set, char_p1); // capacity should still be 1


    hash_set_free(&hash_set);
    free(char_p1);

    return result;
}

int test_hash_set_erase_valid() {
    HashSet *hash_set = testing_hash_set_alloc();
    char *char_p1 = create_char_pointer('A');
    char *char_p2 = create_char_pointer('B');
    char *char_p3 = create_char_pointer(
            'A' + 16); // should be hashed to the same index as char_p1
    char *char_p4 = create_char_pointer('C');

    int expected_char_p1_index = 1; // hash(65) % 16 = 65 % 16 = 1
    int expected_char_p2_index = 2; // hash('B') % 16 = hash(66) % 16 = 65 % 16 = 2
    int expected_char_p3_after_probing_index = 4; // first -> 'A'; second -> 'B'; third -> 4

    hash_set_insert(hash_set, char_p1);
    int erase_res1 = hash_set_erase(hash_set, char_p1);

    Node *expected_node_1 = hash_set->NodesList[expected_char_p1_index];
    int result = erase_res1 == T_SUCCESS_RES &&
                 hash_set->size == 0 &&
                 hash_set->capacity == 8 && //should shrink
                 (!expected_node_1 || (
                         expected_node_1->data == NULL &&
                         hash_set->NodesList[expected_char_p1_index]->hashCount ==
                         0));

    hash_set_insert(hash_set, char_p1);
    hash_set_insert(hash_set, char_p2);
    hash_set_insert(hash_set, char_p3);
    hash_set_insert(hash_set, char_p4);
    int erase_res2 = hash_set_erase(hash_set, char_p1);

    expected_node_1 = hash_set->NodesList[expected_char_p1_index];
    Node *expected_node_2 = hash_set->NodesList[expected_char_p2_index];
    Node *expected_node_3 = hash_set->NodesList[expected_char_p3_after_probing_index];
    result = result &&
             erase_res2 == T_SUCCESS_RES &&
             hash_set->size == 3 &&
             hash_set->capacity == 8 && //should not shrink
             expected_node_1->data == NULL &&
             // char_p3 was hashed to this index
             expected_node_1->hashCount == 1 &&
             check_node(expected_node_2, char_p2) &&
             hash_set->NodesList[expected_char_p1_index]->hashCount == 1 &&
             check_node(expected_node_3, char_p3) &&
             expected_node_3->hashCount == 0;

    free(char_p1);
    free(char_p2);
    free(char_p3);
    free(char_p4);
    hash_set_free(&hash_set);

    return result;
}

int test_hash_set_erase_not_in_table() {
    HashSet *hash_set = testing_hash_set_alloc();
    char *char_p1 = create_char_pointer('A');
    char *char_p2 = create_char_pointer('B');

    int expected_char_p1_index = 1; // hash(65) % 16 = 65 % 16 = 1

    hash_set_insert(hash_set, char_p1);
    int erase_res1 = hash_set_erase(hash_set, char_p2);

    Node *expected_node1 = hash_set->NodesList[expected_char_p1_index];
    int result = erase_res1 == T_FAILURE_RES &&
                 hash_set->size == 1 &&
                 hash_set->capacity == 16 && //should not shrink on failure
                 check_node(expected_node1, char_p1) &&
                 hash_set->NodesList[expected_char_p1_index]->hashCount == 1;

    free(char_p1);
    free(char_p2);
    hash_set_free(&hash_set);

    return result;
}

int test_hash_set_get_load_factor_null_hash_set() {
    return hash_set_get_load_factor(NULL) == T_ERROR_CODE;
}

int test_hash_set_get_load_factor_valid() {
    HashSet *hash_set = testing_hash_set_alloc();
    char *char_p1 = create_char_pointer('B');
    char *char_p2 = create_char_pointer('C');
    char *char_p3 = create_char_pointer('A');

    int result = hash_set_get_load_factor(hash_set) == 0;
    hash_set_insert(hash_set, char_p1);
    result = result &&
             hash_set_get_load_factor(hash_set) == 1.0 / HASH_SET_INITIAL_CAP;
    hash_set_insert(hash_set, char_p2);
    result = result &&
             hash_set_get_load_factor(hash_set) == 2.0 / HASH_SET_INITIAL_CAP;

    int loop_iterations = HASH_SET_INITIAL_CAP * HASH_SET_MAX_LOAD_FACTOR - 3;
    for (int i = 0; i < loop_iterations; i++) {
        char *char_p = create_char_pointer('D' + i);
        hash_set_insert(hash_set, char_p);
        free(char_p);
    }

    result = result &&
             hash_set_get_load_factor(hash_set) ==
             (double) (loop_iterations + 2) / HASH_SET_INITIAL_CAP;

    hash_set_insert(hash_set, char_p3);
    result = result &&
             hash_set_get_load_factor(hash_set) ==
             ((HASH_SET_INITIAL_CAP * HASH_SET_MAX_LOAD_FACTOR)) /
             (HASH_SET_INITIAL_CAP * HASH_SET_GROWTH_FACTOR);

    hash_set_erase(hash_set, char_p1);
    result = result &&
             hash_set_get_load_factor(hash_set) ==
             ((HASH_SET_INITIAL_CAP * HASH_SET_MAX_LOAD_FACTOR) - 1) /
             (HASH_SET_INITIAL_CAP * HASH_SET_GROWTH_FACTOR);

    free(char_p1);
    free(char_p2);
    free(char_p3);
    hash_set_free(&hash_set);

    return result;
}

int test_hash_set_clear_hash_set_is_null() {
    // this test should not crash
    hash_set_clear(NULL);
    return 1;
}

int test_hash_set_clear_valid() {
    HashSet *hash_set = testing_hash_set_alloc();
    char *char_p1 = create_char_pointer('A');
    char *char_p2 = create_char_pointer('B');

    hash_set_insert(hash_set, char_p1);
    hash_set_insert(hash_set, char_p2);

    hash_set_clear(hash_set);

    int result = hash_set->capacity == HASH_SET_INITIAL_CAP;
    for (unsigned int i = 0; i < hash_set->capacity; i++) {
        Node *node = hash_set->NodesList[i];
        if (node && (node->data != NULL || node->hashCount != 0)) {
            result = 0;
            break;
        }
    }

    free(char_p1);
    free(char_p2);
    hash_set_free(&hash_set);

    return result;
}

int test_hash_set_clear_higher_capacity() {
    HashSet *hash_set = testing_hash_set_alloc();

    for (unsigned int i = 0; i < HASH_SET_INITIAL_CAP; i++) {
        char *char_p = create_char_pointer('A' + i);
        hash_set_insert(hash_set, char_p);
        free(char_p);
    }

    hash_set_clear(hash_set);

    int result = hash_set->capacity ==
                 HASH_SET_INITIAL_CAP * HASH_SET_GROWTH_FACTOR;
    for (unsigned int i = 0; i < hash_set->capacity; i++) {
        Node *node = hash_set->NodesList[i];
        if (node && (node->data != NULL || get_hash_count(node) != 0)) {
            result = 0;
            break;
        }
    }

    hash_set_free(&hash_set);

    return result;
}

int test_hash_set_clear_lower_capacity() {
    HashSet *hash_set = testing_hash_set_alloc();
    char *char_p1 = create_char_pointer('A');
    char *char_p2 = create_char_pointer('B');

    hash_set_insert(hash_set, char_p1);
    hash_set_insert(hash_set, char_p2);
    hash_set_erase(hash_set, char_p2);

    hash_set_clear(hash_set);

    int result = hash_set->capacity ==
                 HASH_SET_INITIAL_CAP * (1.0 / HASH_SET_GROWTH_FACTOR);
    for (unsigned int i = 0; i < hash_set->capacity; i++) {
        Node *node = hash_set->NodesList[i];
        if (node && (node->data != NULL || get_hash_count(node) != 0)) {
            result = 0;
            break;
        }
    }

    free(char_p1);
    free(char_p2);
    hash_set_free(&hash_set);

    return result;
}


int test_hash_set_at_null_hash_set() {
    return hash_set_at(NULL, 0) == NULL;
}

int test_hash_set_at_null_nodes_list() {
    HashSet *hash_set = testing_hash_set_alloc();
    Node **nodes_list_tmp = hash_set->NodesList;

    hash_set->NodesList = NULL;
    int result = hash_set_at(hash_set, 0) == NULL;

    hash_set->NodesList = nodes_list_tmp;
    hash_set_free(&hash_set);

    return result;
}

int test_hash_set_at_null_node() {
    HashSet *hash_set = testing_hash_set_alloc();
    int index = 1;
    Node *node_tmp = hash_set->NodesList[index];

    hash_set->NodesList[index] = NULL;

    int result = hash_set_at(hash_set, index) == NULL;

    hash_set->NodesList[index] = node_tmp;
    hash_set_free(&hash_set);

    return result;
}

int test_hash_set_at_invalid_value() {
    HashSet *hash_set = testing_hash_set_alloc();

    int result = hash_set_at(hash_set, -1) == NULL &&
                 hash_set_at(hash_set, HASH_SET_INITIAL_CAP) == NULL;
    hash_set_free(&hash_set);

    return result;
}

int test_hash_set_at_valid() {
    HashSet *hash_set = testing_hash_set_alloc();
    char *char_p1 = create_char_pointer('A'); // hashed to 1
    char *char_p2 = create_char_pointer('C'); // hashed to 3
    hash_set_insert(hash_set, char_p1);
    hash_set_insert(hash_set, char_p2);

    int result = hash_set_at(hash_set, 0) == NULL &&
                 char_key_cmp(hash_set_at(hash_set, 1), char_p1) &&
                 hash_set_at(hash_set, 2) == NULL &&
                 char_key_cmp(hash_set_at(hash_set, 3), char_p2);

    hash_set_erase(hash_set, char_p1);
    result = result && hash_set_at(hash_set, 1) == NULL;

    free(char_p1);
    free(char_p2);
    hash_set_free(&hash_set);

    return result;
}

int full_test_1() {
    HashSet *hash_set = testing_hash_set_alloc();
    char *char_p1 = create_char_pointer('A'); // hashed to 1
    char *char_p2 = create_char_pointer('B'); // hashed to 2
    char *char_p3 = create_char_pointer(
            'A' + HASH_SET_INITIAL_CAP); // hashed to 1

    size_t capacity_after_first_decrease =
            HASH_SET_INITIAL_CAP * (1.0 / HASH_SET_GROWTH_FACTOR);
    size_t capacity_after_second_decrease =
            capacity_after_first_decrease * (1.0 / HASH_SET_GROWTH_FACTOR);
    size_t capacity_after_third_decrease =
            capacity_after_second_decrease * (1.0 / HASH_SET_GROWTH_FACTOR);

    int result = \
            hash_set_insert(hash_set, char_p1) && // char_p1
            hash_set->size == 1 &&
            hash_set->capacity == HASH_SET_INITIAL_CAP &&
            hash_set_contains_value(hash_set, char_p1) &&
            !hash_set_contains_value(hash_set, char_p2) &&
            get_hash_count(hash_set->NodesList[1]) == 1 &&

            hash_set_insert(hash_set, char_p2) && // char_p1, char_p2
            hash_set->size == 2 &&
            hash_set->capacity == HASH_SET_INITIAL_CAP &&
            hash_set_contains_value(hash_set, char_p1) &&
            hash_set_contains_value(hash_set, char_p2) &&
            get_hash_count(hash_set->NodesList[1]) == 1 &&
            get_hash_count(hash_set->NodesList[2]) == 1 &&

            !hash_set_insert(hash_set, char_p1) && // char_p1, char_p2
            hash_set->capacity == HASH_SET_INITIAL_CAP &&
            hash_set->size == 2 &&
            hash_set_contains_value(hash_set, char_p1) &&
            hash_set_contains_value(hash_set, char_p2) &&
            get_hash_count(hash_set->NodesList[1]) == 1 &&
            get_hash_count(hash_set->NodesList[2]) == 1 &&

            hash_set_insert(hash_set, char_p3) && // char_p1, char_p2, char_p3
            hash_set->capacity == HASH_SET_INITIAL_CAP &&
            hash_set->size == 3 &&
            hash_set_contains_value(hash_set, char_p1) &&
            hash_set_contains_value(hash_set, char_p2) &&
            hash_set_contains_value(hash_set, char_p3) &&
            get_hash_count(hash_set->NodesList[1]) == 2 &&
            get_hash_count(hash_set->NodesList[2]) == 1 &&

            hash_set_erase(hash_set, char_p2) && // char_p1, char_p3
            hash_set->capacity == capacity_after_first_decrease &&
            hash_set->size == 2 &&
            hash_set_contains_value(hash_set, char_p1) &&
            !hash_set_contains_value(hash_set, char_p2) &&
            hash_set_contains_value(hash_set, char_p3) &&
            get_hash_count(hash_set->NodesList[1]) == 2 &&
            get_hash_count(hash_set->NodesList[2]) == 0 &&

            !hash_set_erase(hash_set, char_p2) && // char_p1, char_p3
            hash_set->capacity == capacity_after_first_decrease &&
            hash_set->size == 2 &&
            hash_set_contains_value(hash_set, char_p1) &&
            !hash_set_contains_value(hash_set, char_p2) &&
            hash_set_contains_value(hash_set, char_p3) &&
            get_hash_count(hash_set->NodesList[1]) == 2 &&
            get_hash_count(hash_set->NodesList[2]) == 0 &&

            hash_set_insert(hash_set, char_p2) && // char_p1, char_p2, char_p3
            hash_set->capacity == capacity_after_first_decrease &&
            hash_set->size == 3 &&
            hash_set_contains_value(hash_set, char_p1) &&
            hash_set_contains_value(hash_set, char_p2) &&
            hash_set_contains_value(hash_set, char_p3) &&
            get_hash_count(hash_set->NodesList[1]) == 2 &&
            get_hash_count(hash_set->NodesList[2]) == 1 &&

            hash_set_erase(hash_set, char_p2) && // char_p1, char_p3
            hash_set->capacity == capacity_after_second_decrease &&
            hash_set->size == 2 &&
            hash_set_contains_value(hash_set, char_p1) &&
            !hash_set_contains_value(hash_set, char_p2) &&
            hash_set_contains_value(hash_set, char_p3) &&
            get_hash_count(hash_set->NodesList[1]) == 2 &&
            get_hash_count(hash_set->NodesList[2]) == 0 &&

            hash_set_erase(hash_set, char_p1) && // char_p3
            hash_set->capacity == capacity_after_third_decrease &&
            hash_set->size == 1 &&
            !hash_set_contains_value(hash_set, char_p1) &&
            !hash_set_contains_value(hash_set, char_p2) &&
            hash_set_contains_value(hash_set, char_p3) &&
            get_hash_count(hash_set->NodesList[1]) == 1;

    free(char_p1);
    free(char_p2);
    free(char_p3);
    hash_set_free(&hash_set);

    return result;
}

int full_test_2() {
    HashSet *hash_set = hash_set_alloc(hash_int,
                                       int_key_cpy,
                                       int_key_cmp,
                                       int_key_free);

    int amount = 12;

    int result = 1;
    for (int i = 0; i < amount; i++) {
        int *int_p = create_int_pointer(i * HASH_SET_INITIAL_CAP);
        hash_set_insert(hash_set, int_p);

        for (int j = 0; j <= i; j++) {
            int *int_p1 = create_int_pointer(j * HASH_SET_INITIAL_CAP);
            result = result && hash_set_contains_value(hash_set, int_p1);
            free(int_p1);
        }
        free(int_p);
    }

    for (int i = 0; i < amount; i++) {
        int *int_p = create_int_pointer(i * HASH_SET_INITIAL_CAP);
        hash_set_erase(hash_set, int_p);

        for (int j = 0; j <= i; j++) {
            int *int_p1 = create_int_pointer(j * HASH_SET_INITIAL_CAP);
            result = result && !hash_set_contains_value(hash_set, int_p1);
            free(int_p1);
        }

        for (int j = i + 1; j < amount; j++) {
            int *int_p1 = create_int_pointer(j * HASH_SET_INITIAL_CAP);
            result = result && hash_set_contains_value(hash_set, int_p1);
            free(int_p1);
        }

        free(int_p);
    }

    hash_set_free(&hash_set);
    return result;
}

ComplexStruct *create_complex_struct(int a, double b, char c, char *d) {
    ComplexStruct *ptr = malloc(sizeof(ComplexStruct));

    ptr->a = a;
    ptr->b = b;
    ptr->c = c;
    ptr->d = d;

    return ptr;
}


int complex_print_nodes(HashSet *hash_set) {
    printf("\n-------------------------\n");
    for (size_t i = 0; i < hash_set->capacity; i++) {
        Node *node = hash_set->NodesList[i];
        ComplexStruct *c = (ComplexStruct *) (node->data);
        printf("(%lu) %d - (%d, %.2f, %c %s)\n", i, node->hashCount,
               (!c) ? 0 : c->a,
               (!c) ? 0 : c->b,
               (!c) ? '0' : c->c,
               (!c) ? "0" : c->d);
    }
    printf("-------------------------\n");

    return 1;
}

int full_test_3() {
    HashSet *hash_set = hash_set_alloc(complex_hash, complex_key_cpy,
                                       complex_key_cmp, complex_key_free);

    ComplexStruct *p1 = create_complex_struct(1, 1.0, 'A', copy_str("Hel1"));
    ComplexStruct *p2 = create_complex_struct(3, 2.0, 'B', copy_str("Hel2"));
    ComplexStruct *p3 = create_complex_struct(5, 3.0, 'C', copy_str("Hel3"));
    ComplexStruct *p4 = create_complex_struct(7, 4.0, 'D', copy_str("Hel4"));
    ComplexStruct *p5 = create_complex_struct(11, 5.0, 'E', copy_str("Hel5"));
    ComplexStruct *p6 = create_complex_struct(13, 6.0, 'F', copy_str("Hel6"));
    ComplexStruct *p7 = create_complex_struct(17, 7.0, 'G', copy_str("Hel7"));
    ComplexStruct *p8 = create_complex_struct(19, 8.0, 'H', copy_str("Hel8"));
    ComplexStruct *p9 = create_complex_struct(23, 9.0, 'I', copy_str("Hel9"));
    ComplexStruct *p10 = create_complex_struct(29, 10.0, 'J',
                                               copy_str("Hel10"));
    ComplexStruct *p11 = create_complex_struct(31, 11.0, 'K',
                                               copy_str("Hel11"));
    ComplexStruct *p12_1 = create_complex_struct(37, 12.0, 'L',
                                                 copy_str("Hel12"));
    ComplexStruct *p12_2 = create_complex_struct(37, 12.0, 'L',
                                                 copy_str("Hel12"));

    int result =
            hash_set_insert(hash_set, p1) &&
            !hash_set_insert(hash_set, p1) &&
            hash_set_insert(hash_set, p2) &&
            hash_set_insert(hash_set, p3) &&
            hash_set_insert(hash_set, p4) &&
            hash_set_insert(hash_set, p5) &&


            hash_set_insert(hash_set, p6) &&
            !hash_set_insert(hash_set, p4) &&
            !hash_set_insert(hash_set, p5) &&
            !hash_set_insert(hash_set, p6) &&
            hash_set_insert(hash_set, p7) &&
            hash_set_insert(hash_set, p8) &&
            hash_set_insert(hash_set, p9) &&
            hash_set_insert(hash_set, p10) &&
            hash_set_insert(hash_set, p11) &&
            hash_set_insert(hash_set, p12_1) &&
            !hash_set_insert(hash_set, p12_2) &&


            hash_set_contains_value(hash_set, p1) &&
            hash_set_contains_value(hash_set, p2) &&
            hash_set_contains_value(hash_set, p3) &&
            hash_set_contains_value(hash_set, p4) &&
            hash_set_contains_value(hash_set, p5) &&
            hash_set_contains_value(hash_set, p6) &&
            hash_set_contains_value(hash_set, p7) &&
            hash_set_contains_value(hash_set, p8) &&
            hash_set_contains_value(hash_set, p9) &&
            hash_set_contains_value(hash_set, p10) &&
            hash_set_contains_value(hash_set, p11) &&
            hash_set_contains_value(hash_set, p12_1) &&
            hash_set_contains_value(hash_set, p12_2) &&

            hash_set_erase(hash_set, p3) &&
            hash_set_erase(hash_set, p5) &&
            hash_set_erase(hash_set, p7) &&
            hash_set_erase(hash_set, p12_2) &&


            hash_set_contains_value(hash_set, p1) &&
            hash_set_contains_value(hash_set, p2) &&
            !hash_set_contains_value(hash_set, p3) &&
            hash_set_contains_value(hash_set, p4) &&
            !hash_set_contains_value(hash_set, p5) &&
            hash_set_contains_value(hash_set, p6) &&
            !hash_set_contains_value(hash_set, p7) &&
            hash_set_contains_value(hash_set, p8) &&
            hash_set_contains_value(hash_set, p9) &&
            hash_set_contains_value(hash_set, p10) &&
            hash_set_contains_value(hash_set, p11) &&
            !hash_set_contains_value(hash_set, p12_1) &&
            !hash_set_contains_value(hash_set, p12_2) &&

            hash_set_insert(hash_set, p3) &&
            !hash_set_insert(hash_set, p4) &&
            hash_set_insert(hash_set, p12_2) &&


            hash_set_contains_value(hash_set, p1) &&
            hash_set_contains_value(hash_set, p2) &&
            hash_set_contains_value(hash_set, p3) &&
            hash_set_contains_value(hash_set, p4) &&
            !hash_set_contains_value(hash_set, p5) &&
            hash_set_contains_value(hash_set, p6) &&
            !hash_set_contains_value(hash_set, p7) &&
            hash_set_contains_value(hash_set, p8) &&
            hash_set_contains_value(hash_set, p9) &&
            hash_set_contains_value(hash_set, p10) &&
            hash_set_contains_value(hash_set, p11) &&
            hash_set_contains_value(hash_set, p12_1) &&
            hash_set_contains_value(hash_set, p12_2) &&

            hash_set_erase(hash_set, p1) &&
            hash_set_erase(hash_set, p2) &&
            hash_set_erase(hash_set, p4) &&
            hash_set_erase(hash_set, p6) &&
            hash_set_erase(hash_set, p8) &&
            hash_set_erase(hash_set, p9) &&
            hash_set_erase(hash_set, p10) &&
            hash_set_erase(hash_set, p11) &&


            !hash_set_contains_value(hash_set, p1) &&
            !hash_set_contains_value(hash_set, p2) &&
            hash_set_contains_value(hash_set, p3) &&
            !hash_set_contains_value(hash_set, p4) &&
            !hash_set_contains_value(hash_set, p5) &&
            !hash_set_contains_value(hash_set, p6) &&
            !hash_set_contains_value(hash_set, p7) &&
            !hash_set_contains_value(hash_set, p8) &&
            !hash_set_contains_value(hash_set, p9) &&
            !hash_set_contains_value(hash_set, p10) &&
            !hash_set_contains_value(hash_set, p11) &&
            hash_set_contains_value(hash_set, p12_1) &&
            hash_set_contains_value(hash_set, p12_2);

    complex_key_free((void **) &p1);
    complex_key_free((void **) &p2);
    complex_key_free((void **) &p3);
    complex_key_free((void **) &p4);
    complex_key_free((void **) &p5);
    complex_key_free((void **) &p6);
    complex_key_free((void **) &p7);
    complex_key_free((void **) &p8);
    complex_key_free((void **) &p9);
    complex_key_free((void **) &p10);
    complex_key_free((void **) &p11);
    complex_key_free((void **) &p12_1);
    complex_key_free((void **) &p12_2);
    hash_set_free(&hash_set);

    return result;
}


// ----------------------------

int main() {
    TestInfo node_tests_info[] = {
            TEST_CASE(test_node_alloc_valid),
            TEST_CASE(test_node_alloc_elem_copy_func_is_null),
            TEST_CASE(test_node_alloc_elem_cmp_func_is_null),
            TEST_CASE(test_node_alloc_elem_free_func_is_null),
            TEST_CASE(test_node_free_null_assignment),
            TEST_CASE(test_node_free_null_node_pointer),
            TEST_CASE(test_node_free_null_node),
            TEST_CASE(test_node_free_null_data),
            TEST_CASE(test_node_free_free_data_using_free_func),
            TEST_CASE(test_node_free_null_elem_free_func),
            TEST_CASE(test_check_node_node_is_null),
            TEST_CASE(test_check_node_value_is_null),
            TEST_CASE(test_check_node_cmp_func_is_null),
            TEST_CASE(test_check_node_compare_using_cmp_func_equal),
            TEST_CASE(test_check_node_compare_using_cmp_func_not_equal),
            TEST_CASE(test_set_node_data_node_is_null),
            TEST_CASE(test_set_node_data_data_is_null),
            TEST_CASE(test_set_node_data_elem_copy_func_is_null),
            TEST_CASE(test_set_node_data_elem_free_func_is_null),
            TEST_CASE(test_set_node_data_valid),
            TEST_CASE(test_set_node_data_update_data),
            TEST_CASE(test_clear_node_node_is_null),
            TEST_CASE(test_clear_node_data_is_null),
            TEST_CASE(test_clear_node_free_func_is_null),
            TEST_CASE(test_clear_node_valid),
            TEST_CASE(test_get_hash_count_node_is_null),
            TEST_CASE(test_get_hash_count_valid)
    };

    TestInfo hash_set_tests_info[] = {
            TEST_CASE(test_hash_set_alloc_null_hash_func),
            TEST_CASE(test_hash_set_alloc_null_value_cpy),
            TEST_CASE(test_hash_set_alloc_null_value_cmp),
            TEST_CASE(test_hash_set_alloc_null_value_free),
            TEST_CASE(test_hash_set_alloc_null_value_valid),
            TEST_CASE(test_hash_set_free_null_pointer_to_hash_set),
            TEST_CASE(test_hash_set_free_pointer_to_null_hash_set),
            TEST_CASE(test_hash_set_free_valid_data_not_null),
            TEST_CASE(test_hash_set_free_valid_some_null_nodes),
            TEST_CASE(test_hash_set_insert_simple_valid),
            TEST_CASE(test_hash_set_insert_collision_valid),
            TEST_CASE(test_hash_set_insert_increase_capacity),
            TEST_CASE(test_hash_set_insert_same_value),
            TEST_CASE(test_hash_set_insert_null_hash_set),
            TEST_CASE(test_hash_set_insert_null_pointer),
            TEST_CASE(test_hash_set_insert_null_hash_func),
            TEST_CASE(test_hash_set_insert_null_value_cpy),
            TEST_CASE(test_hash_set_insert_null_value_cmp),
            TEST_CASE(test_hash_set_insert_null_value_free),
            TEST_CASE(test_hash_set_contains_value_hash_set_is_null),
            TEST_CASE(test_hash_set_contains_value_value_is_null),
            TEST_CASE(test_hash_set_contains_value_contains),
            TEST_CASE(test_hash_set_contains_value_not_contains),
            TEST_CASE(test_hash_set_erase_null_hash_set),
            TEST_CASE(test_hash_set_erase_null_pointer),
            TEST_CASE(test_hash_set_erase_null_hash_func),
            TEST_CASE(test_hash_set_erase_null_value_cpy),
            TEST_CASE(test_hash_set_erase_null_value_cmp),
            TEST_CASE(test_hash_set_erase_null_value_free),
            TEST_CASE(hash_set_erase_minimal_capacity),
            TEST_CASE(test_hash_set_erase_valid),
            TEST_CASE(test_hash_set_erase_not_in_table),
            TEST_CASE(test_hash_set_get_load_factor_null_hash_set),
            TEST_CASE(test_hash_set_get_load_factor_valid),
            TEST_CASE(test_hash_set_clear_hash_set_is_null),
            TEST_CASE(test_hash_set_clear_valid),
            TEST_CASE(test_hash_set_clear_higher_capacity),
            TEST_CASE(test_hash_set_clear_lower_capacity),
            TEST_CASE(test_hash_set_at_null_hash_set),
            TEST_CASE(test_hash_set_at_invalid_value),
            TEST_CASE(test_hash_set_at_null_nodes_list),
            TEST_CASE(test_hash_set_at_null_node),
            TEST_CASE(test_hash_set_at_valid),
            TEST_CASE(full_test_1),
            TEST_CASE(full_test_2),
            TEST_CASE(full_test_3),
    };

    TestSuite test_suites[] = {
            {"Node", node_tests_info,
                    sizeof(node_tests_info) / sizeof(TestInfo),     NULL, NULL},
            {"HashSet",
                     hash_set_tests_info,
                    sizeof(hash_set_tests_info) / sizeof(TestInfo), NULL, NULL}
    };

    run_test_suites(test_suites, sizeof(test_suites) / sizeof(TestSuite));
}

