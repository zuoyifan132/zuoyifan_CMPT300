/**
 * personal test routine for executing each function for all possible cases.
 * Copyright Yifan Zuo, 2020.
 */

#include "list.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

// copy from sample test
// Macro for custom testing; does exit(1) on failure.
#define CHECK(condition) do{ \
    if (!(condition)) { \
        printf("ERROR: %s (@%d): failed condition \"%s\"\n", __func__, __LINE__, #condition); \
        exit(1);\
    }\
} while(0)


// copy from sample test
// For checking the "free" function called
static int complexTestFreeCounter = 0;
static void complexTestFreeFn(void* pItem) 
{
    CHECK(pItem != NULL);
    complexTestFreeCounter++;
}

// copy from sample test
// For searching
static bool itemEquals(void* pItem, void* pArg) 
{
    return (pItem == pArg);
}

//print list
static void printList(List* pList){
	Node* head = pList->head;
	printf("%s", "list:");
	while(head != NULL){
		printf(" %d", *((int*) head->item));
		head = head->next;
	}
	printf("%s\n","");
}

static void mytest()
{
    // Empty list
    List* pList = List_create();
    CHECK(pList != NULL);
    CHECK(List_count(pList) == 0);
    // check when reach max amount of list
    List* pList1 = List_create();
    List* pList2 = List_create();
    List* pList3 = List_create();
    List* pList4 = List_create();
    CHECK(pList1 != NULL);
    CHECK(pList2 != NULL);
    CHECK(pList3 != NULL);
    // pList4 is an empty list in order to test some empty list condition
    CHECK(pList4 != NULL);

    // Add
    // empty list add
    int added1 = 1;
    CHECK(List_add(pList, &added1) == 0);
    CHECK(List_count(pList) == 1);
    CHECK(List_curr(pList) == &added1);
    // added to not nonempty list
    int added2 = 2;
    CHECK(List_add(pList, &added2) == 0);
    CHECK(List_count(pList) == 2);
    CHECK(List_curr(pList) == &added2);
    // added to beyond tail
    CHECK(List_next(pList) == NULL);
    int added3 = 3;
    CHECK(List_add(pList, &added3) == 0);
    CHECK(List_count(pList) == 3);
    CHECK(List_curr(pList) == &added3);
    // added to before the start
    CHECK(List_first(pList) == &added1);
    CHECK(List_prev(pList) == NULL);
    int added4 = 4;
    CHECK(List_add(pList, &added4) == 0);
    CHECK(List_count(pList) == 4);
    CHECK(List_curr(pList) == &added4);

    // Insert
    // empty list insert
    int inserted1 = 5;
    CHECK(List_insert(pList1, &inserted1) == 0);
    CHECK(List_count(pList1) == 1);
    CHECK(List_curr(pList1) == &inserted1);
    // current pointer is in head case
    int inserted2 = 6;
    CHECK(List_insert(pList1, &inserted2) == 0);
    CHECK(List_count(pList1) == 2);
    CHECK(List_curr(pList1) == &inserted2);
    // current pointer is before the start
    int inserted3 = 7;
    CHECK(List_first(pList1) == &inserted2);
    CHECK(List_prev(pList1) == NULL);
    CHECK(List_insert(pList1, &inserted3) == 0);
    CHECK(List_count(pList1) == 3);
    CHECK(List_curr(pList1) == &inserted3);
    // current pointer is beyond the end
    int inserted4 = 8;
    CHECK(List_last(pList1) == &inserted1);
    CHECK(List_next(pList1) == NULL);
    CHECK(List_insert(pList1, &inserted4) == 0);
    CHECK(List_count(pList1) == 4);
    CHECK(List_curr(pList1) == &inserted4);

    // Prepend
    // empty list
    int prepended1 = 9;
    CHECK(List_prepend(pList2, &prepended1) == 0);
	CHECK(List_count(pList2) == 1);
    CHECK(List_curr(pList2) == &prepended1);
    // normal case
    int prepended2 = 10;
    CHECK(List_prepend(pList2, &prepended2) == 0);
	CHECK(List_count(pList2) == 2);
    CHECK(List_curr(pList2) == &prepended2);

    // Append
	// empty list 
    int appended1 = 11;
    CHECK(List_append(pList3, &appended1) == 0);
	CHECK(List_count(pList3) == 1);
    CHECK(List_curr(pList3) == &appended1);
    // normal case
    int appended2 = 12;
    CHECK(List_append(pList3, &appended2) == 0);
	CHECK(List_count(pList3) == 2);
	CHECK(List_curr(pList3) == &appended2);

    // empty list test for List_first(), List_last(), List_prev(), List_next(), List_curr()
    CHECK(List_first(pList4) == NULL);
    CHECK(List_last(pList4) == NULL);
    CHECK(List_prev(pList4) == NULL);
	CHECK(List_next(pList4) == NULL);
	CHECK(List_curr(pList4) == NULL);

    // Next through it all (from before list)
	CHECK(List_first(pList) == &added4);
	CHECK(List_prev(pList) == NULL);
	CHECK(List_next(pList) == &added4);
	CHECK(List_next(pList) == &added1);
	CHECK(List_next(pList) == &added2);
	CHECK(List_next(pList) == &added3);
	CHECK(List_next(pList) == NULL);
	CHECK(List_next(pList) == NULL);

	// Prev through it all
	CHECK(List_last(pList) == &added3);
	CHECK(List_next(pList) == NULL);
	CHECK(List_prev(pList) == &added3);
	CHECK(List_prev(pList) == &added2);
	CHECK(List_prev(pList) == &added1);
	CHECK(List_prev(pList) == &added4);
	CHECK(List_prev(pList) == NULL);
	CHECK(List_prev(pList) == NULL);

	// Remove first
	// empty list
	CHECK(List_remove(pList4) == NULL);
	// current pointer is before start
	CHECK(List_first(pList) == &added4);
	CHECK(List_prev(pList) == NULL);
	CHECK(List_remove(pList) == NULL);
	// current pointer is beyond end
	CHECK(List_last(pList) == &added3);
	CHECK(List_next(pList) == NULL);
	CHECK(List_remove(pList) == NULL);
	// current pointer in head
	CHECK(List_first(pList3) == &appended1);
	CHECK(List_remove(pList3) == &appended1);
	CHECK(List_count(pList3) == 1);
	CHECK(List_curr(pList3) == &appended2);
	// list just contain one element and current pointer point to the element
	CHECK(List_remove(pList3) == &appended2);
	CHECK(List_count(pList3) == 0);
	CHECK(List_curr(pList3) == NULL);
	// if current item in tail
	CHECK(List_last(pList2) == &prepended1);
	CHECK(List_remove(pList2) == &prepended1);
	CHECK(List_count(pList2) == 1);
	CHECK(List_prev(pList2) == &prepended2);
   	
   	// Trim last
   	// empty list
	CHECK(List_trim(pList4) == NULL);
	// if just contain one item
	CHECK(List_trim(pList2) == &prepended2);
	CHECK(List_count(pList2) == 0);
	CHECK(List_curr(pList2) == NULL);
	// normal case
	CHECK(List_trim(pList) == &added3);
	CHECK(List_count(pList) == 3);
	CHECK(List_curr(pList) == &added2);

	// Free remaining 3 elements in pList
    complexTestFreeCounter = 0;
    List_free(pList, complexTestFreeFn);
    CHECK(complexTestFreeCounter == 3);
    CHECK(List_count(pList) == 0);
    CHECK(List_curr(pList) == NULL);

    // Concat
    // pList1 is empty
    List_concat(pList, pList1);
    CHECK(List_count(pList) == 4);
    CHECK(List_first(pList) == &inserted3);
    CHECK(List_last(pList) == &inserted4);
    // pList2 is empty
    List_concat(pList, pList2);
    CHECK(List_count(pList) == 4);
    CHECK(List_first(pList) == &inserted3);
    CHECK(List_last(pList) == &inserted4);
    // normal case
    int a = 12;
    int b = 13;
    List_add(pList1, &a);
    List_add(pList1, &b);
    List_concat(pList, pList1);
    CHECK(List_count(pList) == 6);
    CHECK(List_first(pList) == &inserted3);
    CHECK(List_last(pList) == &b);

    // Search
    List_first(pList);
    CHECK(List_search(pList, itemEquals, &inserted3) == &inserted3);
    CHECK(List_search(pList, itemEquals, &a) == &a);
    CHECK(List_search(pList1, itemEquals, &added1) == NULL);

    // print all list after all manipulation
    printList(pList);
    printList(pList1);
    printList(pList2);
    printList(pList3);
    printList(pList4);
}

int main(int argCount, char *args[]) 
{
    mytest();

    // We got here?!? PASSED!
    printf("********************************\n");
    printf("           WOW YES!\n");
    printf("********************************\n");
    return 0;
}