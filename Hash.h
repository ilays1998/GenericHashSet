#include <stdlib.h>

#ifndef _HASH_H_
#define _HASH_H_

/**
 * Integers simple hash func.
 */
size_t hash_int(void *elem){
    size_t hash = (*((int *) elem));
    return hash;
}

/**
 * Chars simple hash func.
 */
size_t hash_char(void *elem){
    size_t hash = (*((char *) elem));
    return hash;
}

/**
 * Doubles simple hash func.
 */
size_t hash_double(void *elem){
    size_t hash = (*((double *) elem));
    return hash;
}


#endif //_HASH_H_
