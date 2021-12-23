//
// Created by ilays on 21/11/2021.
//

#include "Node.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define ERROR_FAIL -1
#define EQUAL 1
#define UNEQUAL 0
#define NODE_ILLEGAL 0
#define SUCCESS 1

/**
 * Allocates dynamically new Node element.
 * @param elem_copy_func func which copies the element stored in the Node (returns
 * dynamically allocated copy).
 * @param elem_cmp_func func which is used to compare elements stored in the Node.
 * @param elem_free_func func which frees elements stored in the Node.
 * @return pointer to dynamically allocated Node.
 * @if_fail return NULL (it returns null also if one of the function pointers is null).
 * You can assume memory allocation success
 */
Node *node_alloc(NodeElemCpy elem_copy_func, NodeElemCmp elem_cmp_func, NodeElemFree elem_free_func)
{
    Node *new_node = malloc(sizeof (Node));
    if (new_node == NULL || elem_cmp_func == NULL || elem_copy_func == NULL ||
          elem_free_func == NULL)
    {
        return NULL;
    }
    new_node->hashCount = 0;
    new_node->data = NULL;
    new_node->elem_cmp_func = elem_cmp_func;
    new_node->elem_copy_func = elem_copy_func;
    new_node->elem_free_func = elem_free_func;
    return new_node;
}

/**
 * Frees a Node and the elements the Node itself allocated.
 * @param p_Node pointer to dynamically allocated pointer to Node.
 */
void node_free(Node **p_Node)
{
    if (p_Node == NULL)
    {
        return;
    }
    if ((*p_Node) == NULL || (*p_Node)->elem_free_func == NULL)
    {
        return;
    }
    if ((*p_Node)->data != NULL)
    {
        (*p_Node)->elem_free_func(&((*p_Node)->data));
    }
    free(*p_Node);
    *p_Node = NULL;
}

/**
 * Gets a value and checks if the value is in the Node.
 * @param Node a pointer to Node.
 * @param value the value to look for.
 * @return 1 if the value is in the Node, 0 if no such value in the Node.
 * Returns -1  if something went wrong during the check
 * You cannot assume anything about the input.
 */
int check_node(Node *Node, void *value)
{
    if (Node == NULL || value == NULL || Node->elem_cmp_func == NULL)
    {
        return ERROR_FAIL;
    }
    int result = Node->elem_cmp_func(value, Node->data);
    if (result == EQUAL)
    {
        return EQUAL;
    }
    return UNEQUAL;
}

/**
 * Adds a new value to the data of the Node.
 * @param Node a pointer to Node.
 * @param value the value to be added to the Node.
 * @return 1 if the adding has been done successfully, 0 otherwise.
 */
int set_node_data(Node *Node, void *value)
{
    if (Node == NULL || value == NULL || Node->elem_copy_func == NULL
           || Node->elem_free_func == NULL)
    {
        return NODE_ILLEGAL;
    }
    if (Node->data == NULL)
    {
        Node->data = Node->elem_copy_func(value);
    }
    else
    {
        (Node)->elem_free_func(&(Node)->data);
        Node->data = Node->elem_copy_func(value);
    }
    return SUCCESS;
}

/**
 * Deletes the data in the Node.
 * @param Node Node a pointer to Node.
 */
void clear_node(Node *Node)
{
    if (Node == NULL || Node->elem_free_func == NULL)
    {
      return;
    }
    (Node)->elem_free_func(&(Node)->data);
}

/**
 * Gets the number of data values in the hashtable that would have been hashed
 * to this node
 * @param node The current node
 * @return the number of data in the hashtable that would have been hashed
 * to this node or -1 in case of illegal input;
 * You cannot assume anything about the input.
 */
int get_hash_count(Node *node)
{
    if (node == NULL)
    {
        return ERROR_FAIL;
    }
    return node->hashCount;
}

