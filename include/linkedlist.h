#if !defined(_LIST_H)
#define _LIST_H

typedef struct _listnode {
    char *name;
	void *data;
	unsigned long sz;

	struct _listnode *next;
} LISTNODE;

typedef struct _list {
	LISTNODE *head;
	LISTNODE *tail;
} LINKEDLIST;

LISTNODE *AddNode(LINKEDLIST *, char *, void *, unsigned long);
void DeleteNode(LINKEDLIST *, LISTNODE *);
LISTNODE *FindNodeByRef(LINKEDLIST *, void *);
LISTNODE *FindNodeByValue(LINKEDLIST *, void *, unsigned long);
LISTNODE *FindNodeByName(LINKEDLIST *, char *);
void FreeNodes(LINKEDLIST *, int);
void FreeList(LINKEDLIST *);
void PrintList(LINKEDLIST *);

#endif
