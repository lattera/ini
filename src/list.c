#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "linkedlist.h"
#include "misc.h"

LISTNODE *AddNode(LINKEDLIST *list, char *name, void *data, unsigned long sz)
{
    LISTNODE *node;

    if (!(list))
        return NULL;

    if (!(list->head)) {/*uninitialized list! set up head & tail nodes.*/
        list->head = xmalloc(sizeof(LISTNODE));
        if (!(list->head))
            return NULL;
        list->tail = list->head; /*initial degenerate case, head==tail*/
        node = list->tail;
    } else {
        list->tail->next = xmalloc(sizeof(LISTNODE));/*make room for the next one*/
        if (!(list->tail->next))
            return NULL;
        list->tail = list->tail->next; /*tail must always point to the last node*/
        node = list->tail; /*tail is always the last node by definition*/
    }

    /* Yeah, small memory leak here */
    if ((name))
        if (!(node->name = strdup(name)))
            return NULL;
    
    node->data = xmalloc(sz);
    node->sz = sz;
    memcpy(node->data, data, sz);

    return node;
}

void _free_node(LISTNODE *node)
{
    if ((node->name))
        free(node->name);

    if ((node->data))
        free(node->data);

    free(node);
}

void DeleteNode(LINKEDLIST *list, LISTNODE *node)
{
    LISTNODE *del;

    if (!(list))
        return;

    if (node == list->head) {
        list->head = node->next;

        _free_node(node);

        return;
    }

    del = list->head;
    while (del != NULL && del->next != node)
        del = del->next;

    if (!(del))
        return;
    
    if(del->next == list->tail)
        list->tail = del;

    del->next = del->next->next;
    
    _free_node(node);
}

LISTNODE *FindNodeByRef(LINKEDLIST *list, void *data)
{
    LISTNODE *node;

    if (!(list))
        return NULL;

    node = list->head;
    while ((node)) {
        if (node->data == data)
            return node;

        node = node->next;
    }

    return NULL;
}

LISTNODE *FindNodeByValue(LINKEDLIST *list, void *data, unsigned long sz)
{
    LISTNODE *node;

    if (!(list))
        return NULL;

    node = list->head;
    while ((node)) {
        if (!memcmp(node->data, data, (sz > node->sz) ? node->sz : sz))
            return node;

        node = node->next;
    }

    return NULL;
}

LISTNODE *FindNodeByName(LINKEDLIST *list, char *name)
{
    LISTNODE *node;

    if (!(list))
        return NULL;

    node = list->head;
    while ((node)) {
        if ((node->name))
            if (!strcmp(node->name, name))
                return node;

        node = node->next;
    }

    return NULL;
}

/*frees all nodes in a list, including their data*/
void FreeNodes(LINKEDLIST *list, int free_list)
{
    if (!(list))
        return;

    while ((list->head))
        DeleteNode(list, list->head);

    if (free_list)
        free(list);
    
    return;
}

void FreeList(LINKEDLIST *list)
{
    FreeNodes (list, 1);
    return;
}

void PrintList(LINKEDLIST *list)
{
    LISTNODE *node;

    for (node = list->head; node != NULL; node = node->next)
        fprintf(stderr, "%s => %s\n", node->name, node->data);
}
