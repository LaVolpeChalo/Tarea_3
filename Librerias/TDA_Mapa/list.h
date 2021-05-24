#include <stdlib.h>
#include <stdio.h>

#ifndef _list_h
#define _list_h

typedef struct List List;

/* list operations */

List* createList();
int is_empty(List* list);
void* front(List* list);
void* first(List* list);
void* next(List* list);
void* last(List* list);
void* prev(List* list);
void popFront(List* list);
void popBack(List* list);
void pushFront(List* list, void* data);
void pushBack(List* list, void* data);
void pushCurrent(List* list, void* data);
void popCurrent(List* list);
int get_size();




#endif /* _list_h */
