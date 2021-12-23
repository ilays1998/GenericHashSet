#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include "HashSet.h"
#include "Utils.h"
#include "Hash.h"
#include "Node.h"


#define TESTS_NUMBER 35
#define ELEMENTS 100
#define DONE "Done!\n\n"
#define FAILED "Failed!\n\n"


int test_1 ()
{
  printf("[Test1] Running ...");
  Node *node = node_alloc (double_key_cpy, double_key_cmp, double_key_free);

  if (!node) {
      return 1;
    }

  node_free (&node);
  return 0;
}


int test_2(){
  printf("[Test2] Running ...");
  Node *node = node_alloc (double_key_cpy, double_key_cmp, double_key_free);

  if (!node) {
      return 1;
    }

  if (node -> data != NULL){
      node_free (&node);
      return 1;
    }

  if (get_hash_count (node) != 0){
      node_free (&node);
      return 1;
    }

  node_free (&node);
  return 0;
}


int test_3()
{
  printf("[Test3] Running ...");
  Node *node = node_alloc (double_key_cpy, double_key_cmp, double_key_free);

  if (!node) {
      return 1;
    }
  double number = 1;
  void *numberptr = (void *) &number;

  if (set_node_data (node, numberptr) != 1){
      node_free (&node);
      return 1;
    }

  if (check_node (node, numberptr) != 1){
      node_free (&node);
      return 1;
    }

  node_free (&node);
  return 0;
}

int test_4()
{
  printf("[Test4] Running ...");
  Node *node = node_alloc (double_key_cpy, double_key_cmp, double_key_free);

  if (!node) {
      return 1;
    }

  double number = 1;
  void *numberptr = (void *) &number;

  if (set_node_data (node, numberptr) != 1){
      node_free (&node);
      return 1;
    }

  if (check_node (node, numberptr) != 1){
      node_free (&node);
      return 1;
    }

  clear_node (node);

  if (node->data != NULL){
      node_free (&node);
      return 1;
    }

  node_free (&node);
  return 0;
}


int test_5()
{
  printf("[Test5] Running ...");
  Node *node = NULL;

  node_free (&node);

  node_free (NULL);

  clear_node (NULL);

  clear_node (node);

  if ((get_hash_count (node) != -1) || (get_hash_count (NULL) != -1)){
      return 1;
    }

  if (set_node_data (node, NULL) != 0 || set_node_data (NULL, NULL)){
      return 1;
    }
  return 0;
}


int test_6(){
  printf("[Test6] Running ...");
  Node* node = node_alloc (double_key_cpy, double_key_cmp, double_key_free);
  if (!node) {
      return 1;
    }

  double number = -1.5;
  void *numberptr = (void *) &number;

  if (set_node_data (node, numberptr) != 1){
      node_free (&node);
      return 1;
    }

  if (check_node (node, numberptr) != 1){
      node_free (&node);
      return 1;
    }

  clear_node (node);

  if (node->data != NULL){
      node_free (&node);
      return 1;
    }

  node_free (&node);
  return 0;
}

int test_7(){
  printf("[Test7] Running ...");
  Node* node = node_alloc (char_key_cpy, char_key_cmp, char_key_free);
  if (!node) {
      node_free (&node);
      return 1;
    }

  char character = 'a';
  void *characterptr = (void *) &character;

  if (set_node_data (node, characterptr) != 1){
      node_free (&node);
      return 1;
    }

  if (check_node (node, characterptr) != 1){
      node_free (&node);
      return 1;
    }

  clear_node (node);

  if (node->data != NULL){
      node_free (&node);
      return 1;
    }

  node_free (&node);
  return 0;
}


int test_8() {
  printf("[Test8] Running ...");
  Node* node = node_alloc (char_key_cpy, char_key_cmp, char_key_free);
  if (!node) {
      node_free (&node);
      return 1;
    }

  char character = 'a';
  void *characterptr = (void *) &character;

  if (set_node_data (node, characterptr) != 1){
      node_free (&node);
      return 1;
    }

  if (check_node (node, characterptr) != 1){
      node_free (&node);
      return 1;
    }

  clear_node (node);

  if (node->data != NULL){
      node_free (&node);
      return 1;
    }

  node_free (&node);
  return 0;
}

int test_9() {
  printf("[Test9] Running ...");
  Node* node = node_alloc (double_key_cpy, double_key_cmp, double_key_free);
  if (!node) {
      return 1;
    }

  free(node->data);

  node->data = NULL;

  clear_node (node);

  node_free (&node);

  return 0;
}


int test_10() {
  printf("[Test10] Running ...");
  Node* node = node_alloc (double_key_cpy, double_key_cmp, double_key_free);
  if (!node) {
      return 1;
    }

  free(node->data);

  node->data = NULL;

  double number = -1.5;
  void *numberptr = (void *) &number;

  if (set_node_data (node, numberptr) != 1){
      node_free (&node);
      return 1;
    }

  clear_node (node);

  node_free (&node);

  return 0;
}

int test_11() {
  printf("[Test11] Running ...");
  // HashMap alloc.
  HashSet *hash_set = hash_set_alloc(hash_double, double_key_cpy, double_key_cmp, double_key_free);

  // HashMap free.
  hash_set_free (&hash_set);

  return 0;
}

int test_12() {
  printf("[Test12] Running ...");

  // HashMap alloc.
  HashSet *hash_set = hash_set_alloc (hash_double, double_key_cpy, double_key_cmp, double_key_free);

  for (double k_i = 0; k_i < ELEMENTS; ++k_i) {
      if (hash_set_insert (hash_set, &k_i) != 1){
          hash_set_free (&hash_set);
          return 1;
        }
    }


  double value_exists = 0;
  double value_not_exist = 101;
  int contains_val_1 = hash_set_contains_value (hash_set, &value_exists);
  int contains_val_2 = hash_set_contains_value (hash_set, &value_not_exist);

  if (!(contains_val_1 == 1 && contains_val_2 == 0)) {
      hash_set_free (&hash_set);
      return 1;
    }

  // HashMap free.
  hash_set_free (&hash_set);
  return 0;
}

int test_13() {
  printf("[Test13] Running ...");

  // HashMap alloc.
  HashSet *hash_set = hash_set_alloc (hash_double, double_key_cpy, double_key_cmp, double_key_free);

  for (int k_i = 0; k_i < 6; ++k_i) {
      hash_set_insert (hash_set, &k_i);
    }

  if (hash_set->size != 6 && hash_set->capacity != 16) {
      hash_set_free (&hash_set);
      return 1;
    }

  // HashSet free.
  hash_set_free (&hash_set);

  return 0;
}


int test_14() {
  printf("[Test14] Running ...");

  // HashMap alloc.
  HashSet *hash_set = hash_set_alloc (hash_double, double_key_cpy, double_key_cmp, double_key_free);

  for (double k_i = 0; k_i < ELEMENTS; k_i++) {
      hash_set_insert (hash_set, &k_i);
    }

  double good = 99;
  int bad = 198;
  double *at_true = hash_set_at (hash_set, hash_double (&good));
  double *at_false = hash_set_at (hash_set, bad);

  if ((*at_true != 99) || (at_false != NULL)) {
      hash_set_free (&hash_set);
      return 1;
    }

  // HashMap free.
  hash_set_free (&hash_set);

  return 0;
}


int test_15() {
  printf("[Test15] Running ...");

  // HashSet alloc.
  HashSet *hash_set = hash_set_alloc (hash_double, double_key_cpy, double_key_cmp, double_key_free);

  // insert elements.
  for (double kI = 0; kI < ELEMENTS; ++kI) {
      hash_set_insert (hash_set, &kI);
    }
  if (!(hash_set->size == 100 && hash_set->capacity == 256)) {
      hash_set_free (&hash_set);
      return 1;
    }


  for (double kI = 0; kI < ELEMENTS / 2; ++kI) {
      int conf_erase = hash_set_erase (hash_set, &kI);
      if (conf_erase != 1) {
          hash_set_free (&hash_set);
          return 1;
        }
    }
  if (!(hash_set->size == 50 && hash_set->capacity == 128)) {
      hash_set_free (&hash_set);
      return 1;
    }

  for (double kI = ELEMENTS / 2; kI < ELEMENTS; ++kI) {
      int conf_erase = hash_set_erase (hash_set, &kI);
      if (conf_erase != 1) {
          hash_set_free (&hash_set);
          return 1;
        }
    }

  if (!(hash_set->size == 0 && hash_set->capacity == 1)) {
      hash_set_free (&hash_set);
      return 1;
    }

  // HashSet free.
  hash_set_free (&hash_set);

  return 0;
}


int test_16() {
  printf("[Test16] Running ...");
  // HashMap alloc.
  HashSet *hash_set = hash_set_alloc (hash_int, int_key_cpy, int_key_cmp, int_key_free);

  // HashMap free.
  hash_set_free (&hash_set);

  return 0;
}


int test_17() {
  printf("[Test17] Running ...");

  // HashMap alloc.
  HashSet *hash_set = hash_set_alloc (hash_int, int_key_cpy, int_key_cmp, int_key_free);

  for (int k_i = 0; k_i < ELEMENTS; ++k_i) {
      if (hash_set_insert (hash_set, &k_i) != 1){
          hash_set_free (&hash_set);
          return 1;
        }
    }


  int value_exists = 0;
  int value_not_exist = 101;
  int contains_val_1 = hash_set_contains_value (hash_set, &value_exists);
  int contains_val_2 = hash_set_contains_value (hash_set, &value_not_exist);

  if (!(contains_val_1 == 1 && contains_val_2 == 0)) {
      hash_set_free (&hash_set);
      return 1;
    }

  // HashMap free.
  hash_set_free (&hash_set);
  return 0;
}


int test_18() {
  printf("[Test18] Running ...");
  // HashMap alloc.
  HashSet *hash_set = hash_set_alloc (hash_int, int_key_cpy, int_key_cmp, int_key_free);

  for (int k_i = 0; k_i < 6; ++k_i) {
      hash_set_insert (hash_set, &k_i);
    }

  if (hash_set->size != 6 && hash_set->capacity != 16) {
      hash_set_free (&hash_set);
      return 1;
    }

  // HashSet free.
  hash_set_free (&hash_set);

  return 0;
}


int test_19() {
  printf("[Test19] Running ...");

  // HashMap alloc.
  HashSet *hash_set = hash_set_alloc (hash_int, int_key_cpy, int_key_cmp, int_key_free);

  for (int k_i = 0; k_i < ELEMENTS; k_i++) {
      hash_set_insert (hash_set, &k_i);
    }

  int good = 99;
  int bad = 198;
  int *at_true = hash_set_at (hash_set, hash_int (&good));
  int *at_false = hash_set_at (hash_set, bad);

  if ((*at_true != 99) || (at_false != NULL)) {
      hash_set_free (&hash_set);
      return 1;
    }

  // HashMap free.
  hash_set_free (&hash_set);

  return 0;
}


int test_20() {
  printf("[Test20] Running ...");

  // HashMap alloc.
  HashSet *hash_set = hash_set_alloc (hash_int, int_key_cpy, int_key_cmp, int_key_free);

  // insert elements.
  for (int kI = 0; kI < ELEMENTS; ++kI) {
      hash_set_insert (hash_set, &kI);
    }
  if (!(hash_set->size == 100 && hash_set->capacity == 256)) {
      hash_set_free (&hash_set);
      return 1;
    }


  for (int kI = 0; kI < ELEMENTS / 2; ++kI) {
      int conf_erase = hash_set_erase (hash_set, &kI);
      if (conf_erase != 1) {
          return 1;
        }
    }
  if (!(hash_set->size == 50 && hash_set->capacity == 128)) {
      return 1;
    }

  for (int kI = ELEMENTS / 2; kI < ELEMENTS; ++kI) {
      int conf_erase = hash_set_erase (hash_set, &kI);
      if (conf_erase != 1) {
          return 1;
        }
    }

  if (!(hash_set->size == 0 && hash_set->capacity == 1)) {
      return 1;
    }

  // HashSet free.
  hash_set_free (&hash_set);

  return 0;
}

int test_21() {
  printf("[Test21] Running ...");
  // HashMap alloc.
  HashSet *hash_set = hash_set_alloc (hash_char, char_key_cpy, char_key_cmp, char_key_free);

  // HashMap free.
  hash_set_free (&hash_set);

  return 0;
}

int test_22() {
  printf("[Test22] Running ...");

  // HashMap alloc.
  HashSet *hash_set = hash_set_alloc (hash_char, char_key_cpy, char_key_cmp, char_key_free);

  for (char k_i = '!'; k_i != '{'; ++k_i) {
      if (hash_set_insert (hash_set, &k_i) != 1){
          hash_set_free (&hash_set);
          return 1;
        }
    }


  char value_exists = '@';
  char value_not_exist = '~';
  int contains_val_1 = hash_set_contains_value (hash_set, &value_exists);
  int contains_val_2 = hash_set_contains_value (hash_set, &value_not_exist);

  if (!(contains_val_1 == 1 && contains_val_2 == 0)) {
      hash_set_free (&hash_set);
      return 1;
    }

  // HashMap free.
  hash_set_free (&hash_set);
  return 0;
}


int test_23() {
  printf("[Test23] Running ...");
  // HashMap alloc.
  HashSet *hash_set = hash_set_alloc (hash_char, char_key_cpy, char_key_cmp, char_key_free);

  for (char k_i = '0'; k_i != '6'; ++k_i) {
      hash_set_insert (hash_set, &k_i);
    }

  if (hash_set->size != 6 && hash_set->capacity != 16) {
      hash_set_free (&hash_set);
      return 1;
    }

  // HashSet free.
  hash_set_free (&hash_set);

  return 0;
}


int test_24() {
  printf("[Test24] Running ...");
  // HashMap alloc.
  HashSet *hash_set = hash_set_alloc (hash_char, char_key_cpy, char_key_cmp, char_key_free);
  for (char k_i = '!'; k_i != '{'; k_i++) {

    if (k_i == '@'){
        printf ("l");
    }
      hash_set_insert (hash_set, &k_i);
    }

  char good = '@';
  char bad = '~';
  char *at_true = hash_set_at (hash_set, hash_char (&good));
  char *at_false = hash_set_at (hash_set, hash_char (&bad));

  if ((*at_true != '@') || (at_false != NULL)) {
      hash_set_free (&hash_set);
      return 1;
    }

  // HashMap free.
  hash_set_free (&hash_set);

  return 0;
}


int test_25() {
  printf("[Test25] Running ...");
  // HashMap alloc.
  HashSet *hash_set = hash_set_alloc (hash_char, char_key_cpy, char_key_cmp, char_key_free);

  // insert elements.
  for (char kI = '!'; kI != '{'; ++kI) {
      hash_set_insert (hash_set, &kI);
    }
  if (!(hash_set->size == 90 && hash_set->capacity == 128)) {
      hash_set_free (&hash_set);
      return 1;
    }


  for (int kI = '!'; kI != 'I'; ++kI) {
      int conf_erase = hash_set_erase (hash_set, &kI);
      if (conf_erase != 1) {
          hash_set_free (&hash_set);
          return 1;
        }
    }
  if (!(hash_set->size == 50 && hash_set->capacity == 128)) {
      hash_set_free (&hash_set);
      return 1;
    }

  for (int kI = 'I'; kI != '{'; ++kI) {
      int conf_erase = hash_set_erase (hash_set, &kI);
      if (conf_erase != 1) {
          hash_set_free (&hash_set);
          return 1;
        }
    }

  if (!(hash_set->size == 0 && hash_set->capacity == 1)) {
      hash_set_free (&hash_set);
      return 1;
    }

  // HashSet free.
  hash_set_free (&hash_set);

  return 0;
}



int test_26() {
  printf("[Test26] Running ...");
  // HashMap alloc.
  HashSet *hash_set = hash_set_alloc (hash_object, object_key_cpy, object_key_cmp, object_key_free);

  // HashMap free.
  hash_set_free (&hash_set);

  return 0;
}

int test_27() {
  printf("[Test27] Running ...");

  // HashMap alloc.
  HashSet *hash_set = hash_set_alloc (hash_object, object_key_cpy, object_key_cmp, object_key_free);

  SomeObject* objectList[ELEMENTS];
  for (int k_i = 0; k_i < ELEMENTS; ++k_i) {
      objectList[k_i] = object_alloc (k_i);
    }

  for (int k_i = 0; k_i < ELEMENTS; ++k_i) {
      if (hash_set_insert (hash_set, objectList[k_i]) != 1){
          for (int k_i = 0; k_i < ELEMENTS; ++k_i) {
              object_key_free ((void **) &objectList[k_i]);
            }
          hash_set_free (&hash_set);
          return 1;
        }
    }

  SomeObject* value_not_exist = object_alloc (ELEMENTS);
  int contains_val_1 = hash_set_contains_value (hash_set, objectList[0]);
  int contains_val_2 = hash_set_contains_value (hash_set, value_not_exist);

  if (!(contains_val_1 == 1 && contains_val_2 == 0)) {
      for (int k_i = 0; k_i < ELEMENTS; ++k_i) {
          object_key_free ((void **) &objectList[k_i]);
        }
      hash_set_free (&hash_set);
      object_key_free((void**) &value_not_exist);
      return 1;
    }

  for (int k_i = 0; k_i < ELEMENTS; ++k_i) {
      object_key_free ((void **) &objectList[k_i]);
    }

  // HashMap free.
  hash_set_free (&hash_set);
  object_key_free((void**) &value_not_exist);
  return 0;
}


int test_28() {
  printf("[Test28] Running ...");

  // HashMap alloc.
  HashSet *hash_set = hash_set_alloc (hash_object, object_key_cpy, object_key_cmp, object_key_free);

  SomeObject* objectList[6];
  for (int k_i = 0; k_i < 6; ++k_i) {
      objectList[k_i] = object_alloc (k_i);
    }

  for (int k_i = 0; k_i < 6; ++k_i) {
      hash_set_insert (hash_set, objectList[k_i]);
    }

  if (hash_set->size != 6 && hash_set->capacity != 16) {
      for (int k_i = 0; k_i < 6; ++k_i) {
          object_key_free ((void **) &objectList[k_i]);
        }
      hash_set_free (&hash_set);
      return 1;
    }

  // HashSet free.
  for (int k_i = 0; k_i < 6; ++k_i) {
      object_key_free ((void **) &objectList[k_i]);
    }
  hash_set_free (&hash_set);

  return 0;
}



int test_29() {
  printf("[Test29] Running ...");

  // HashMap alloc.
  HashSet *hash_set = hash_set_alloc (hash_object, object_key_cpy, object_key_cmp, object_key_free);

  SomeObject* objectList[ELEMENTS];
  for (int k_i = 0; k_i < ELEMENTS; ++k_i) {
      objectList[k_i] = object_alloc (k_i);
    }

  for (int k_i = 0; k_i < ELEMENTS; k_i++) {
      hash_set_insert (hash_set, objectList[k_i]);
    }

  int good = 99;
  int bad = 198;
  SomeObject *at_true = hash_set_at (hash_set, good);
  SomeObject *at_false = hash_set_at (hash_set, bad);

  if ((at_true->id != 99) || (at_false != NULL)) {
      for (int k_i = 0; k_i < ELEMENTS; ++k_i) {
          object_key_free ((void **) &objectList[k_i]);
        }
      return 1;
    }

  // HashMap free.
  for (int k_i = 0; k_i < ELEMENTS; ++k_i) {
      object_key_free ((void **) &objectList[k_i]);
    }
  hash_set_free (&hash_set);

  return 0;
}



int test_30() {
  printf("[Test30] Running ...");

  // HashSet alloc.
  HashSet *hash_set = hash_set_alloc (hash_object, object_key_cpy, object_key_cmp, object_key_free);

  SomeObject* objectList[ELEMENTS];
  for (int k_i = 0; k_i < ELEMENTS; ++k_i) {
      objectList[k_i] = object_alloc (k_i);
    }

  // insert elements.
  for (int k_i = 0; k_i < ELEMENTS; k_i++) {
      hash_set_insert (hash_set, objectList[k_i]);
    }
  if (!(hash_set->size == 100 && hash_set->capacity == 256)) {
      for (int k_i = 0; k_i < ELEMENTS; ++k_i) {
          object_key_free ((void **) &objectList[k_i]);
        }
      hash_set_free (&hash_set);
      return 1;
    }


  for (int kI = 0; kI < ELEMENTS / 2; ++kI) {
      int conf_erase = hash_set_erase (hash_set, objectList[kI]);
      if (conf_erase != 1) {
          for (int k_i = 0; k_i < ELEMENTS; ++k_i) {
              object_key_free ((void **) &objectList[k_i]);
            }
          hash_set_free (&hash_set);
          return 1;
        }
    }
  if (!(hash_set->size == 50 && hash_set->capacity == 128)) {
      for (int k_i = 0; k_i < ELEMENTS; ++k_i) {
          object_key_free ((void **) &objectList[k_i]);
        }
      hash_set_free (&hash_set);
      return 1;
    }

  for (int kI = ELEMENTS / 2; kI < ELEMENTS; ++kI) {
      int conf_erase = hash_set_erase (hash_set, objectList[kI]);
      if (conf_erase != 1) {
          for (int k_i = 0; k_i < ELEMENTS; ++k_i) {
              object_key_free ((void **) &objectList[k_i]);
            }
          hash_set_free (&hash_set);
          return 1;
        }
    }

  if (!(hash_set->size == 0 && hash_set->capacity == 1)) {
      for (int k_i = 0; k_i < ELEMENTS; ++k_i) {
          object_key_free ((void **) &objectList[k_i]);
        }
      hash_set_free (&hash_set);
      return 1;
    }

  // HashSet free.
  for (int k_i = 0; k_i < ELEMENTS; ++k_i) {
      object_key_free ((void **) &objectList[k_i]);
    }
  hash_set_free (&hash_set);

  return 0;
}

int test_31() {
  printf("[Test31] Running ...");

  // HashSet alloc.
  HashSet *hash_set = hash_set_alloc (hash_double, double_key_cpy, double_key_cmp, double_key_free);

  // insert elements.
  for (double kI = 0; kI < ELEMENTS; ++kI) {
      hash_set_insert (hash_set, &kI);
    }

  if (hash_set->size != 100) {
      hash_set_free (&hash_set);
      return 1;
    }

  unsigned int old_cap = hash_set->capacity;

  hash_set_clear (hash_set);

  for (size_t k_i = 0; k_i < hash_set->capacity; ++k_i) {
      if ((hash_set->NodesList[k_i]->data != NULL) || (hash_set->NodesList[k_i]->hashCount != 0)) {
          hash_set_free (&hash_set);
          return 1;
        }
    }

  if ((hash_set->capacity != old_cap) || (hash_set->size != 0) || (hash_set_get_load_factor (hash_set) != 0)){
      hash_set_free (&hash_set);
      return 1;
    }

  // HashSet free.
  hash_set_free (&hash_set);
  return 0;
}

int test_32() {
  printf("[Test32] Running ...");

  // HashMap alloc.
  HashSet *hash_set = hash_set_alloc (hash_int, int_key_cpy, int_key_cmp, int_key_free);

  // insert elements.
  for (int kI = 0; kI < ELEMENTS; ++kI) {
      hash_set_insert (hash_set, &kI);
    }

  if (hash_set->size != 100) {
      hash_set_free (&hash_set);
      return 1;
    }

  unsigned int old_cap = hash_set->capacity;

  hash_set_clear (hash_set);

  for (size_t k_i = 0; k_i < hash_set->capacity; ++k_i) {
      if ((hash_set->NodesList[k_i]->data != NULL) || (hash_set->NodesList[k_i]->hashCount != 0)) {
          hash_set_free (&hash_set);
          return 1;
        }
    }

  if ((hash_set->capacity != old_cap) || (hash_set->size != 0) || (hash_set_get_load_factor (hash_set) != 0)){
      hash_set_free (&hash_set);
      return 1;
    }

  // HashSet free.
  hash_set_free (&hash_set);
  return 0;
}

int test_33() {
  printf("[Test33] Running ...");
  // HashMap alloc.
  HashSet *hash_set = hash_set_alloc (hash_char, char_key_cpy, char_key_cmp, char_key_free);

  // insert elements.
  for (char kI = '!'; kI != '{'; ++kI) {
      hash_set_insert (hash_set, &kI);
    }

  if (hash_set->size != 90) {
      hash_set_free (&hash_set);
      return 1;
    }

  unsigned int old_cap = hash_set->capacity;

  hash_set_clear (hash_set);

  for (size_t k_i = 0; k_i < hash_set->capacity; ++k_i) {
      if ((hash_set->NodesList[k_i]->data != NULL) || (hash_set->NodesList[k_i]->hashCount != 0)) {
          hash_set_free (&hash_set);
          return 1;
        }
    }

  if ((hash_set->capacity != old_cap) || (hash_set->size != 0) || (hash_set_get_load_factor (hash_set) != 0)){
      hash_set_free (&hash_set);
      return 1;
    }

  // HashSet free.
  hash_set_free (&hash_set);
  return 0;
}

int test_34() {
  printf("[Test34] Running ...");

  // HashSet alloc.
  HashSet *hash_set = hash_set_alloc (hash_object, object_key_cpy, object_key_cmp, object_key_free);

  SomeObject* objectList[ELEMENTS];
  for (int k_i = 0; k_i < ELEMENTS; ++k_i) {
      objectList[k_i] = object_alloc (k_i);
    }

  // insert elements.
  for (int kI = 0; kI < ELEMENTS; ++kI) {
      hash_set_insert (hash_set, objectList[kI]);
    }

  if (hash_set->size != ELEMENTS) {
      for (int k_i = 0; k_i < ELEMENTS; ++k_i) {
          object_key_free ((void **) &objectList[k_i]);
        }
      hash_set_free (&hash_set);
      return 1;
    }

  unsigned int old_cap = hash_set->capacity;

  hash_set_clear (hash_set);

  for (size_t k_i = 0; k_i < hash_set->capacity; ++k_i) {
      if ((hash_set->NodesList[k_i]->data != NULL) || (hash_set->NodesList[k_i]->hashCount != 0)) {
          for (int kI = 0; kI < ELEMENTS; ++kI) {
              object_key_free ((void **) &objectList[kI]);
            }
          hash_set_free (&hash_set);
          return 1;
        }
    }

  if ((hash_set->capacity != old_cap) || (hash_set->size != 0) || (hash_set_get_load_factor (hash_set) != 0)){
      for (int k_i = 0; k_i < ELEMENTS; ++k_i) {
          object_key_free ((void **) &objectList[k_i]);
        }
      hash_set_free (&hash_set);
      return 1;
    }

  // HashSet free.
  for (int k_i = 0; k_i < ELEMENTS; ++k_i) {
      object_key_free ((void **) &objectList[k_i]);
    }
  hash_set_free (&hash_set);
  return 0;
}



void *ElemCpy(const void *elem) {
  int *a = malloc(sizeof(int));
  *a = *((int *) elem);
  return a;
}

int ElemCmp(const void *elem_1, const void *elem_2) {
  return *((const int *) elem_1) == *((const int *) elem_2);
}

void ElemFree(void **elem) {
  free(*elem);
}


int test_35() {
  printf("[Pre-Submission] 3 ...\n");
  printf("[Pre-Submission] 2 ...\n");
  printf("[Pre-Submission] 1 ...\n");
  printf("[Pre-Submission] Ignition Test35 ...\n");

  printf("[Pre-Submission] Creating Node of Ints ...\n");
  Node *node = node_alloc(ElemCpy, ElemCmp, ElemFree);

  printf("[Pre-Submission] Free Node ...\n");
  node_free(&node);


  printf("[Pre-Submission] Creating HashSet of {Char : Int} ...\n");
  HashSet *hash_set = hash_set_alloc(hash_double, double_key_cpy, double_key_cmp, double_key_free);

  printf("[Pre-Submission] Inserting Pairs to HashSet ...\n");
  for (double kI = 0; kI < 8; ++kI) {
      hash_set_insert(hash_set, &kI);
    }

  printf("[Pre-Submission] Check hash-map size with assert ...\n");
  assert(hash_set->size == 8 && "Incorrect size of hash-map.");
  assert(hash_set->capacity == 16 && "Incorrect capacity of hash-map.");


  printf("[Pre-Submission] Free hash map ...\n");
  hash_set_free(&hash_set);
  return 0;
}



int main()
{
  int failed_tests = TESTS_NUMBER;
  FILE *output = fopen("output.txt", "w");
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 1**********/
  if (test_1 () != 0)
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 2**********/
  if (test_2 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);


  /**********Test 3**********/
  if (test_3 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 4**********/
  if (test_4 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);


  /**********Test 5**********/
  if (test_5 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);


  /**********Test 6**********/
  if (test_6 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 7**********/
  if (test_7 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 8**********/
  if (test_8 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 9**********/
  if (test_9 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 10**********/
  if (test_10 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 11**********/
  if (test_11 () != 0)
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 12**********/
  if (test_12 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);


  /**********Test 13**********/
  if (test_13 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 14**********/
  if (test_14 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);


  /**********Test 15**********/
  if (test_15 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);


  /**********Test 16**********/
  if (test_16 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 17**********/
  if (test_17 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 18**********/
  if (test_18 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 19**********/
  if (test_19 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 20**********/
  if (test_20 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 21**********/
  if (test_21 () != 0)
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 22**********/
  if (test_22 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);


  /**********Test 23**********/
  if (test_23 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 24**********/
  if (test_24 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);


  /**********Test 25**********/
  if (test_25 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);


  /**********Test 26**********/
  if (test_26 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 27**********/
  if (test_27 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 28**********/
  if (test_28 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 29**********/
  if (test_29 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 30**********/
  if (test_30 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 31**********/
  if (test_31 () != 0)
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 32**********/
  if (test_32 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);


  /**********Test 33**********/
  if (test_33 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);

  /**********Test 34**********/
  if (test_34 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);


  /**********Test 35**********/
  if (test_35 ())
    {
      printf(FAILED);
      fclose(output);
      return failed_tests;
    }
  printf(DONE);
  failed_tests--;
  fseek(output, 0, SEEK_SET);
  fprintf(output, "%d", failed_tests);
  fflush(output);
  fclose(output);
  return failed_tests;
}
