#include <stdlib.h>
#include <string.h>
#ifndef EX4_UTILS_H
#define EX4_UTILS_H

/**
 * Copies the double value.
 */
void *double_key_cpy(const void *key) {
    double *new_d = malloc(sizeof(double));
    *new_d = *((double *) key);
    return new_d;
}

/**
 * Copies the int value.
 */
void *int_key_cpy(const void *value) {
    int *new_int = malloc(sizeof(int));
    *new_int = *((int *) value);
    return new_int;
}

/**
 * Copies the char key.
 */
void *char_key_cpy(const void *key) {
    char *new_char = malloc(sizeof(char));
    *new_char = *((char *) key);
    return new_char;
}

/**
 * Compares the double value.
 */
int double_key_cmp(const void *val_1, const void *val_2) {
    return *(double *) val_1 == *(double *) val_2;
}

/**
 * Compares the int value.
 */
int int_key_cmp(const void *val_1, const void *val_2) {
    return *(int *) val_1 == *(int *) val_2;
}

/**
 * Compares the char key.
 */
int char_key_cmp(const void *val_1, const void *val_2) {
    return *(char *) val_1 == *(char *) val_2;
}

/**
 * Frees the double value.
 */
void double_key_free(void **val) {
    if (val && *val) {
        free(*val);
        *val = NULL;
    }
}

/**
 * Frees the int value.
 */
void int_key_free(void **val) {
    if (val && *val) {
        free(*val);
        *val = NULL;
    }
}

/**
 * Frees the char key.
 */
void char_key_free(void **val) {
    if (val && *val) {
        free(*val);
        *val = NULL;
    }
}

typedef struct SomeObject {
    int id;
} SomeObject;

/**
 * allocates an object with an id
 */
SomeObject* object_alloc(int id){
    SomeObject *temp = malloc(sizeof(SomeObject));
    temp->id = id;
    return temp;
}

/**
 * Copies the object value.
 */
void *object_key_cpy(const void *key) {
    SomeObject *temp = (SomeObject *) key;
    return (void *) object_alloc (temp->id);
}

/**
 * Compares the object value.
 */
int object_key_cmp(const void *val_1, const void *val_2) {
    SomeObject *temp1 = (SomeObject *) val_1;
    SomeObject *temp2 = (SomeObject *) val_2;
    return ((temp1->id) == (temp2->id));
}


/**
 * Frees the object value.
 */
void object_key_free(void **val) {
    if (val && (*val)) {
        SomeObject **temp = (SomeObject **) val;
        free(*temp);
        *temp = NULL;
    }
}

/**
 * Object simple hash func.
 */
size_t hash_object(void *elem){
    size_t hash = (size_t) (((SomeObject *) elem)->id);
    return hash;
}


#endif //EX4_UTILS_H
