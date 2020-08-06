// Marking tests
// Sample solution by Dr. Brian Fraser (Copyright 2020)


#include "list.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define LIST_SUCCESS 0
#define LIST_FAIL -1




// Macro for custom testing:
// exit(1) on failure.
#define CHECK(condition) do{ \
    if (!(condition)) { \
        printf("ERROR: %s (@%d): failed condition \"%s\"\n", __func__, __LINE__, #condition); \
        exit(1);\
    }\
} while(0)



// *************************************************************************
//    Support:  Make Lists
// *************************************************************************
int itemOne = 1;
int itemTwo = 2;
int itemThree = 3;
int itemFour = 4;

List* makeEmptyList() {
    return List_create();
}
List* makeListOfOne() {
    List* pList =  List_create();
    CHECK(List_add(pList, &itemOne) == LIST_SUCCESS);
    return pList;
}
List* makeListOfTwo() {
    List* pList =  List_create();
    CHECK(List_add(pList, &itemOne) == LIST_SUCCESS);
    CHECK(List_add(pList, &itemTwo) == LIST_SUCCESS);
    return pList;
}
List* makeListOfThree() {
    List* pList =  List_create();
    CHECK(List_add(pList, &itemOne) == LIST_SUCCESS);
    CHECK(List_add(pList, &itemTwo) == LIST_SUCCESS);
    CHECK(List_add(pList, &itemThree) == LIST_SUCCESS);
    return pList;
}
List* makeListOfFour() {
    List* pList =  List_create();
    CHECK(List_add(pList, &itemOne) == LIST_SUCCESS);
    CHECK(List_add(pList, &itemTwo) == LIST_SUCCESS);
    CHECK(List_add(pList, &itemThree) == LIST_SUCCESS);
    CHECK(List_add(pList, &itemFour) == LIST_SUCCESS);
    return pList;
}

// *************************************************************************
//    Support:  Verify List
// *************************************************************************

static bool listEquals(List* pList, void*expectedItemsNullTerm[])
{
    // Ensure each exected item is there
    List_first(pList);
    for (int i = 0; expectedItemsNullTerm[i] != NULL; i++) {
        if (List_curr(pList) != expectedItemsNullTerm[i]) {
            return false;
        }
        List_next(pList);
    }

    // Ensure no extra items there
    if (List_curr(pList) != NULL) {
        return false;
    }

    // Made it!
    return true;
}
// *************************************************************************
//   Support:   Count free lists / nodes
// *************************************************************************
// static void doNothingFreeFn(void* pItem) 
// {
//     // do nothing
// }
// static int getNumFreeLists()
// {
//     // Allocate as many as we can!
//     int numFreeLists = 0;
//     List* lists[LIST_MAX_NUM_HEADS + 1];
//     // .. go past end, just to see!
//     for (int i = 0; i < LIST_MAX_NUM_HEADS + 1; i++) {
//         lists[i] = List_create();
//         if (lists[i] == NULL) {
//             numFreeLists = i;
//             break;
//         }
//     }

//     // Free
//     for (int i = 0; i < numFreeLists; i++) {
//         List_free(lists[i], doNothingFreeFn);
//     }

//     return numFreeLists;
// }
// static int getNumFreeNodes()
// {
//     // Allocate as many as we can!
//     int numFreeNodes = 0;
//     List* pList = makeEmptyList();
//     for (int i = 0; i < LIST_MAX_NUM_NODES + 1; i++) {
//         int status = List_add(pList, NULL);
//         if (status == -1) {
//             numFreeNodes = i;
//             break;
//         }
//     }

//     // Free
//     List_free(pList, doNothingFreeFn);

//     return numFreeNodes;
// }

static int consumeAllFreeLists()
{
    // Allocate as many nodes as we can!
    int numFreeLists = 0;
    for (int i = 0; i < LIST_MAX_NUM_HEADS + 1; i++) {
        List* pLists = List_create();
        if (pLists == NULL) {
            numFreeLists = i;
            break;
        }
    }

    CHECK(numFreeLists <= LIST_MAX_NUM_NODES);
    // Intentionally leak the resources
    return numFreeLists;
}
static int consumeAllFreeNodes(List* pList)
{
    int someGarbageItemInStack = 42;
    // Allocate as many as we can!
    // Do one more than allowed just to check!
    int numFreeNodes = 0;
    for (int i = 0; i < LIST_MAX_NUM_NODES + 1; i++) {
        int status = List_add(pList, &someGarbageItemInStack);
        if (status == -1) {
            numFreeNodes = i;
            break;
        }
    }

    CHECK(numFreeNodes <= LIST_MAX_NUM_NODES);
    // Intentionally leak the resources
    return numFreeNodes;
}

// *************************************************************************
// SUPPORT: Functions to pass to module
// *************************************************************************
static int freeCallbackCount = 0;
static void freeCallback(void* pNode) 
{
    freeCallbackCount++;
}



// *************************************************************************
//      Tests
// *************************************************************************
void testDefinesLargeEnough() 
{
    CHECK(LIST_MAX_NUM_HEADS >= 10);
    CHECK(LIST_MAX_NUM_NODES >= 100);
}
//
// SIZE
//
void testSizeEmptyList() 
{
    List* pList = List_create();
    CHECK(List_count(pList) == 0);
}
void testSizeOneAdd() 
{
    List* pList = List_create();
    int res = List_add(pList, NULL);
    CHECK(res == LIST_SUCCESS);
    CHECK(List_count(pList) == 1);
}
void testSizeOneInsert() 
{
    List* pList = List_create();
    int res = List_insert(pList, NULL);
    CHECK(res == LIST_SUCCESS);
    CHECK(List_count(pList) == 1);
}
void testSizeOneAppend() 
{
    List* pList = List_create();
    int res = List_append(pList, NULL);
    CHECK(res == LIST_SUCCESS);
    CHECK(List_count(pList) == 1);
}
void testSizeOnePrepend() 
{
    List* pList = List_create();
    int res = List_prepend(pList, NULL);
    CHECK(res == LIST_SUCCESS);
    CHECK(List_count(pList) == 1);
}
void testSizeFour() 
{
    List* pList = makeListOfFour();
    CHECK(List_count(pList) == 4);
}

//
// NEXT
//
void testNextOnEmpty() 
{
    List* pList = makeEmptyList();
    void* item = List_next(pList);
    CHECK(item == NULL);
}
void testNextOnOnePastStart() 
{
    List* pList = makeListOfOne();
    List_first(pList);
    List_prev(pList);
    void* item = List_next(pList);
    CHECK(item == &itemOne);
}
void testNextOnOnePastEnd() 
{
    List* pList = makeListOfOne();
    List_last(pList);
    List_next(pList);
    void* item = List_next(pList);
    CHECK(item == NULL);
}
void testNextOnOneAtFirst() 
{
    List* pList = makeListOfOne();
    List_first(pList);
    void* item = List_next(pList);
    CHECK(item == NULL);
}
void testNextOnTwoAtFirst() 
{
    List* pList = makeListOfTwo();
    List_first(pList);
    void* item = List_next(pList);
    CHECK(item == &itemTwo);
}

// 
// Previous
//
void testPrevOnEmpty() 
{   
    List* pList = makeEmptyList();
    void* item = List_prev(pList);
    CHECK(item == NULL);
}
void testPrevOnOnePastStart() 
{
    List* pList = makeListOfOne();
    List_first(pList);
    List_prev(pList);
    void* item = List_prev(pList);
    CHECK(item == NULL);
}
void testPrevOnOnePastEnd() 
{
    List* pList = makeListOfOne();
    List_last(pList);
    List_next(pList);
    void* item = List_prev(pList);
    CHECK(item == &itemOne);
}
void testPrevOnOneAtFirst() 
{
    List* pList = makeListOfOne();
    List_first(pList);
    void* item = List_prev(pList);
    CHECK(item == NULL);
}
void testPrevOnTwoAtTwo() 
{
    List* pList = makeListOfTwo();
    List_last(pList);
    void* item = List_prev(pList);
    CHECK(item == &itemOne);
}





void testCurOnEmpty() 
{
    List* pList = makeEmptyList();
    CHECK(List_curr(pList) == NULL);
}
void testCurOnOneBeforeStart() 
{
    List* pList = makeListOfOne();
    List_first(pList);
    List_prev(pList);
    CHECK(List_curr(pList) == NULL);
}
void testCurOnOneAfterEnd() 
{
    List* pList = makeListOfOne();
    List_last(pList);
    List_next(pList);
    CHECK(List_curr(pList) == NULL);
}
void testCurOnOneAtFirst() 
{
    List* pList = makeListOfOne();
    List_first(pList);
    CHECK(List_curr(pList) == &itemOne);
}
void testCurOnThreeAtSecond() 
{
    List* pList = makeListOfThree();
    List_first(pList);
    List_next(pList);
    CHECK(List_curr(pList) == &itemTwo);
}

void testFirstOnEmpty() 
{
    List* pList = makeEmptyList();
    List_first(pList);
    CHECK(List_curr(pList) == NULL);
}
void testFirstOnTwo() 
{
    List* pList = makeListOfTwo();
    List_first(pList);
    CHECK(List_curr(pList) == &itemOne);
}

void testLastOnEmpty() 
{
    List* pList = makeEmptyList();
    List_last(pList);
    CHECK(List_curr(pList) == NULL);
}
void testLastOnTwo() 
{
    List* pList = makeListOfTwo();
    List_last(pList);
    CHECK(List_curr(pList) == &itemTwo);
}

void testRemoveOnEmpty() 
{
    List* pList = makeEmptyList();
    CHECK(List_remove(pList) == NULL);
}
void testRemoneOnTwoBeforeStart() 
{
    List* pList = makeListOfTwo();
    List_first(pList);
    List_prev(pList);
    CHECK(List_remove(pList) == NULL);
    CHECK(List_count(pList) == 2);
}
void testRemoveOnTwoAfterEnd() 
{
    List* pList = makeListOfTwo();
    List_last(pList);
    List_next(pList);
    CHECK(List_remove(pList) == NULL);
    CHECK(List_count(pList) == 2);
    CHECK(listEquals(pList, (void*[]){&itemOne, &itemTwo, NULL}));
}
void testRemoveOnOneAtStart() 
{
    List* pList = makeListOfOne();
    List_first(pList);
    CHECK(List_remove(pList) == &itemOne);
    CHECK(List_count(pList) == 0);
}
void testRemoveOnTwoAtStart() 
{
    List* pList = makeListOfTwo();
    List_first(pList);
    CHECK(List_remove(pList) == &itemOne);
    CHECK(List_count(pList) == 1);
    CHECK(List_curr(pList) == &itemTwo);
    CHECK(listEquals(pList, (void*[]){&itemTwo, NULL}));
}
void testRemoveOnTwoAtEnd() 
{
    List* pList = makeListOfTwo();
    List_last(pList);
    CHECK(List_remove(pList) == &itemTwo);
    CHECK(List_count(pList) == 1);
    CHECK(listEquals(pList, (void*[]){&itemOne, NULL}));
}
void testRemoveOnThreeAtSecond() 
{
    List* pList = makeListOfThree();
    List_first(pList);
    List_next(pList);
    CHECK(List_remove(pList) == &itemTwo);
    CHECK(List_count(pList) == 2);
    CHECK(List_remove(pList) == &itemThree);
    CHECK(listEquals(pList, (void*[]){&itemOne, NULL}));
}

//
// Out Of Resources
//
void testCreateListOutOfLists() 
{
    consumeAllFreeLists();
    CHECK(List_create() == NULL);
}
void testCreateListAfterFree() 
{
    List* pList = List_create();
    consumeAllFreeLists();
    List_free(pList, freeCallback);
    CHECK(List_create() != NULL);
}
void testAddOutOfNodes() 
{
    List* pList = List_create();
    consumeAllFreeNodes(pList);
    CHECK(List_add(pList, NULL) == -1);
}
void testInsertOutOfNodes() 
{
    List* pList = List_create();
    consumeAllFreeNodes(pList);
    CHECK(List_insert(pList, NULL) == -1);
}
void testAppendOutOfNodes() 
{
    List* pList = List_create();
    consumeAllFreeNodes(pList);
    CHECK(List_append(pList, NULL) == -1);
}
void testPrependOutOfNodes() 
{
    List* pList = List_create();
    consumeAllFreeNodes(pList);
    CHECK(List_prepend(pList, NULL) == -1);
}
void testAddAfterRemove() 
{
    int something = 10;
    List* pList = List_create();
    consumeAllFreeNodes(pList);
    CHECK(List_remove(pList) != NULL);
    CHECK(List_add(pList, &something) == 0);
}
void testInsertAfterRemove() 
{
    int something = 10;
    List* pList = List_create();
    consumeAllFreeNodes(pList);
    CHECK(List_remove(pList) != NULL);
    CHECK(List_insert(pList, &something) == 0);
}
void testAppendAfterRemove() 
{
    int something = 10;
    List* pList = List_create();
    consumeAllFreeNodes(pList);
    CHECK(List_remove(pList) != NULL);
    CHECK(List_append(pList, &something) == 0);
}
void testAddAfterFree() 
{
    int something = 10;
    List* pList = List_create();
    consumeAllFreeNodes(pList);
    List_free(pList, freeCallback);

    List* pListNew = List_create();
    CHECK(List_add(pListNew, &something) == 0);
}
void testAddAfterTrim() 
{
    int something = 10;
    List* pList = List_create();
    consumeAllFreeNodes(pList);

    CHECK(List_trim(pList) != NULL);
    CHECK(List_add(pList, &something) == 0);
}

// 
// Concat
void testConcatOnTwoEmpty() 
{
    // Test with cursor before first
    List* pList1 = makeEmptyList();
    List* pList2 = makeEmptyList();
    
    List_concat(pList1, pList2);
    CHECK(List_count(pList1) == 0);
    CHECK(List_curr(pList1) == NULL);
    CHECK(List_first(pList1) == NULL);
    CHECK(List_last(pList1) == NULL);

    CHECK(listEquals(pList1, (void*[]){NULL}));
}
void testConcatOnEmptyAndTwo() 
{
    List* pList1 = makeEmptyList();

    List* pList2 = List_create();
    int alpha = 1;
    int beta = 2;
    List_add(pList2, &alpha);
    List_add(pList2, &beta);
    List_first(pList2);
    
    List_concat(pList1, pList2);
    CHECK(List_count(pList1) == 2);
    // CHECK(List_curr(pList1) == NULL);    // <-- Not fully specifide what happens with list 1 being empty
    CHECK(List_first(pList1) == &alpha);
    CHECK(List_last(pList1) == &beta);
    CHECK(listEquals(pList1, (void*[]){&alpha, &beta, NULL}));
}
void testConcatOnTwoAndEmpty() 
{
    List* pList1 = makeListOfTwo();
    List_last(pList1);

    List* pList2 = List_create();
    
    List_concat(pList1, pList2);
    CHECK(List_count(pList1) == 2);
    CHECK(List_curr(pList1) == &itemTwo);
    CHECK(List_first(pList1) == &itemOne);
    CHECK(List_last(pList1) == &itemTwo);

    CHECK(listEquals(pList1, (void*[]){&itemOne, &itemTwo, NULL}));
}
void testConcatOnOneAndOne() 
{
    // Test with cursor before first
    List* pList1 = makeListOfOne();
    List_first(pList1);
    List_prev(pList1);

    List* pList2 = List_create();
    int alpha = 1;
    List_add(pList2, &alpha);
    List_last(pList2);
    
    List_concat(pList1, pList2);
    CHECK(List_count(pList1) == 2);
    CHECK(List_curr(pList1) == NULL);
    CHECK(List_first(pList1) == &itemOne);
    CHECK(List_last(pList1) == &alpha);

    CHECK(listEquals(pList1, (void*[]){&itemOne, &alpha, NULL}));
}
void testConcatOnThreeAndThree() 
{
    List* pList1 = makeListOfThree();
    List_last(pList1);

    List* pList2 = List_create();
    int alpha = 1;
    int beta = 2;
    int gamma = 3;
    List_add(pList2, &alpha);
    List_add(pList2, &beta);
    List_add(pList2, &gamma);
    List_last(pList2);
    
    List_concat(pList1, pList2);
    CHECK(List_count(pList1) == 6);
    CHECK(List_curr(pList1) == &itemThree);
    CHECK(List_first(pList1) == &itemOne);
    CHECK(List_last(pList1) == &gamma);

    CHECK(listEquals(pList1, (void*[]){&itemOne, &itemTwo, &itemThree, &alpha, &beta, &gamma, NULL}));
}

void testConcatOutOfNodesAndLists() 
{
    // Make two Lists
    List* pList1 = makeListOfFour();
    List* pList2 = makeEmptyList();

    // Fill up all other lists
    consumeAllFreeNodes(pList2);
    consumeAllFreeLists();

    // Sies
    int size1 = List_count(pList1);
    CHECK(size1 == 4);
    int size2 = List_count(pList2);
    CHECK(size2 > 0);

    // Concat (succeeds)
    List_concat(pList1, pList2);
    int sizeConcat = List_count(pList1);
    CHECK(sizeConcat == size1 + size2);
}

// 
// Free
//
void testFreeOnEmpty() 
{
    List* pList = makeEmptyList();
    freeCallbackCount = 0;
    List_free(pList, freeCallback);
    CHECK(freeCallbackCount == 0);
}
void testFreeOnOne() 
{
    List* pList = makeListOfOne();
    freeCallbackCount = 0;
    List_free(pList, freeCallback);
    CHECK(freeCallbackCount == 1);
}
void testFreeOnThree() 
{
    List* pList = makeListOfThree();
    freeCallbackCount = 0;
    List_free(pList, freeCallback);
    CHECK(freeCallbackCount == 3);
}
void testFreeOnAll() 
{
    // Allocate as many as we can!
    int numFreeLists = 0;
    List* lists[LIST_MAX_NUM_HEADS];
    for (int i = 0; i < LIST_MAX_NUM_HEADS; i++) {
        lists[i] = List_create();
        CHECK(lists[i] != NULL);
    }
    CHECK(List_create() == NULL);

    // Free
    freeCallbackCount = 0;
    for (int i = 0; i < numFreeLists; i++) {
        List_free(lists[i], freeCallback);
    }
    CHECK(freeCallbackCount == 0);
}

// 
// Trim
//
void testTrimOnEmpty() 
{
    List* pList = makeEmptyList();
    CHECK(List_count(pList) == 0);
    CHECK(List_trim(pList) == NULL);
    CHECK(List_count(pList) == 0);

    CHECK(listEquals(pList, (void*[]){NULL}));
}
void testTrimOnOne() 
{
    List* pList = makeListOfOne();
    CHECK(List_count(pList) == 1);
    CHECK(List_trim(pList) == &itemOne);
    CHECK(List_count(pList) == 0);
    
    CHECK(listEquals(pList, (void*[]){NULL}));
}
void testTrimOnTwo() 
{
    List* pList = makeListOfTwo();
    CHECK(List_count(pList) == 2);
    CHECK(List_trim(pList) == &itemTwo);
    CHECK(List_count(pList) == 1);
    CHECK(List_trim(pList) == &itemOne);
    CHECK(List_count(pList) == 0);
}

    




// TODO: SEARCH


// Complex:
static int complexTestFreeCounter = 0;
static void complexTestFreeFn(void* pItem) 
{
    CHECK(pItem != NULL);
    complexTestFreeCounter++;
}
bool itemEquals(void* pItem, void* pArg) 
{
    return (pItem == pArg);
}
void testComplex()
{
    // Empty list; 
    List* pList = List_create();
    CHECK(pList != NULL);
    CHECK(List_count(pList) == 0);

    // Add 
    int added = 41;
    CHECK(List_add(pList, &added) == 0);
    CHECK(List_count(pList) == 1);
    CHECK(List_curr(pList) == &added);

    // Insert
    int inserted = 42;
    CHECK(List_insert(pList, &inserted) == 0);
    CHECK(List_count(pList) == 2);
    CHECK(List_curr(pList) == &inserted);
    
    // Prepend
    int prepended = 43;
    CHECK(List_prepend(pList, &prepended) == 0);
    CHECK(List_count(pList) == 3);
    CHECK(List_curr(pList) == &prepended);
    
    // Append
    int appended = 44;
    CHECK(List_append(pList, &appended) == 0);
    CHECK(List_count(pList) == 4);
    CHECK(List_curr(pList) == &appended);
    
    // Next through it all (from before list)
    CHECK(List_first(pList) == &prepended);
    CHECK(List_prev(pList) == NULL);
    CHECK(List_next(pList) == &prepended);
    CHECK(List_next(pList) == &inserted);
    CHECK(List_next(pList) == &added);
    CHECK(List_next(pList) == &appended);
    CHECK(List_next(pList) == NULL);
    CHECK(List_next(pList) == NULL);

    // Prev through it all
    //   starting from past end
    CHECK(List_last(pList) == &appended);
    CHECK(List_next(pList) == NULL);
    CHECK(List_prev(pList) == &appended);
    CHECK(List_prev(pList) == &added);
    CHECK(List_prev(pList) == &inserted);
    CHECK(List_prev(pList) == &prepended);
    CHECK(List_prev(pList) == NULL);
    CHECK(List_prev(pList) == NULL);

    // Remove first
    CHECK(List_first(pList) == &prepended);
    CHECK(List_remove(pList) == &prepended);
    CHECK(List_curr(pList) == & inserted);

    // Trim last
    CHECK(List_trim(pList) == &appended);
    CHECK(List_curr(pList) == &added);

    // Free remaining 2 elements
    complexTestFreeCounter = 0;
    List_free(pList, complexTestFreeFn);
    CHECK(complexTestFreeCounter == 2);


    // Concat
    int one = 1;
    int two = 2;
    int three = 3;
    int four = 4;
    List* pList1 = List_create();
    List_add(pList1, &one);
    List_add(pList1, &two);
    List* pList2 = List_create();
    List_add(pList2, &three);
    List_add(pList2, &four);
    
    List_concat(pList1, pList2);
    CHECK(List_count(pList1) == 4);
    CHECK(List_first(pList1) == &one);
    CHECK(List_last(pList1) == &four);


    // Search
    List_first(pList1);
    CHECK(List_search(pList1, itemEquals, &two) == &two);
    CHECK(List_search(pList1, itemEquals, &two) == &two);
    CHECK(List_search(pList1, itemEquals, &one) == NULL);
}


/**
 * 
 * TODO 
 * - Add tests:
 *      * 2 lists: A get's all even nodes, B gets odds; Free a; reuse nodes
 *      * 2 lists: A gets first half, B 2nd; Free A, reuse in B.
 *      * 2 lists: use all; free all; allocate all again
 *      * Repeat above tests for List pool
 *      * 2 non-empty lists: one before, one after. Test independent management of before/after
 * 
 * 
 */

void testSearchOnEmpty() 
{
    List* pList = makeEmptyList();
    List_first(pList);
    CHECK(List_search(pList, itemEquals, &itemOne) == NULL);
}
void testSearchOnOneFound() 
{
    List* pList = makeListOfOne();
    List_first(pList);
    CHECK(List_search(pList, itemEquals, &itemOne) == &itemOne);
}
void testSearchOnOneNotFound() 
{
    List* pList = makeListOfOne();
    List_first(pList);
    CHECK(List_search(pList, itemEquals, &itemTwo) == NULL);
    CHECK(List_curr(pList) == NULL);
    List_prev(pList);
    CHECK(List_curr(pList) == &itemOne);

}
void testSearchOnFourFoundTwice() 
{
    List* pList = makeListOfFour();
    List_first(pList);
    CHECK(List_search(pList, itemEquals, &itemOne) == &itemOne);
    CHECK(List_search(pList, itemEquals, &itemOne) == &itemOne);
    List_next(pList);
    CHECK(List_search(pList, itemEquals, &itemFour) == &itemFour);
    CHECK(List_search(pList, itemEquals, &itemFour) == &itemFour);
}
void testSearchOnFourFromBeforeList() 
{
    List* pList = makeListOfFour();
    List_first(pList);
    List_prev(pList);
    CHECK(List_search(pList, itemEquals, &itemOne) == &itemOne);
}
void testSearchOnFourFromBeforeListNotFound()
{
    int garbageValue = 42;
    List* pList = makeListOfFour();
    List_first(pList);
    List_prev(pList);
    CHECK(List_search(pList, itemEquals, &garbageValue) == NULL);

    // Check at end
    List_prev(pList);
    CHECK(List_curr(pList) == &itemFour);
}


void testOnTwoTakeAllNodesFreeOddsReUseAll() 
{
    List* pLists[2];
    pLists[0] = List_create();
    pLists[1] = List_create();

    // Put all nodes into the lists
    //int junkData = 42;

    int data[200];
    for (int i = 0; i<200; i++){
        data[i] = i;
    }

    for (int i = 0; true; i++) {
        int retCode = List_append(pLists[i % 2], &data[i]);
        if (retCode == -1) {
            break;
        }
    }

    

    // Delete one list
    int numNodesFreeing = List_count(pLists[1]);
    freeCallbackCount = 0;
    List_free(pLists[1], freeCallback);
    CHECK(freeCallbackCount == numNodesFreeing);


    /*// Reuse all
    for (int i = 0; i < numNodesFreeing; i++) {
        CHECK(List_append(pLists[0], &junkData) == 0);
    }
    // .. should be no more!
    CHECK(List_append(pLists[0], &junkData) == -1);*/
}
void testOnTwoTakeAllNodesFreeEvensReUseAll()
{
    List* pLists[2];
    pLists[0] = List_create();
    pLists[1] = List_create();

    // Put all nodes into the lists
    int junkData = 42;
    for (int i = 0; true; i++) {
        int retCode = List_append(pLists[i % 2], &junkData);
        if (retCode == -1) {
            break;
        }
    }

    // Delete one list
    int numNodesFreeing = List_count(pLists[0]);
    freeCallbackCount = 0;
    List_free(pLists[0], freeCallback);
    CHECK(freeCallbackCount == numNodesFreeing);

    // Reuse all
    for (int i = 0; i < numNodesFreeing; i++) {
        CHECK(List_append(pLists[1], &junkData) == 0);
    }
    // .. should be no more!
    CHECK(List_append(pLists[1], &junkData) == -1);
}

void testOnTwoTakeAllNodesFreeFirstHalfReUseAll()
{
    List* pLists[2];
    pLists[0] = List_create();
    pLists[1] = List_create();

    // Put all nodes into the lists
    int junkData = 42;
    for (int i = 0; true; i++) {
        int targetListIdx = (i < LIST_MAX_NUM_NODES / 2);
        int retCode = List_append(pLists[targetListIdx], &junkData);
        if (retCode == -1) {
            break;
        }
    }

    // Delete one list
    int numNodesFreeing = List_count(pLists[0]);
    freeCallbackCount = 0;
    List_free(pLists[0], freeCallback);
    CHECK(freeCallbackCount == numNodesFreeing);

    // Reuse all
    for (int i = 0; i < numNodesFreeing; i++) {
        CHECK(List_append(pLists[1], &junkData) == 0);
    }
    // .. should be no more!
    CHECK(List_append(pLists[1], &junkData) == -1);
}

void testOnTwoTakeAllNodesFreeSecondHalfReUseAll()
{
    List* pLists[2];
    pLists[0] = List_create();
    pLists[1] = List_create();

    // Put all nodes into the lists
    int junkData = 42;

    for (int i = 0; true; i++) {
        int targetListIdx = (i < LIST_MAX_NUM_NODES / 2);
        int retCode = List_append(pLists[targetListIdx], &junkData);
        if (retCode == -1) {
            break;
        }
    }

    // Delete one list
    int numNodesFreeing = List_count(pLists[1]);
    freeCallbackCount = 0;
    List_free(pLists[1], freeCallback);
    CHECK(freeCallbackCount == numNodesFreeing);

    // Reuse all
    for (int i = 0; i < numNodesFreeing; i++) {
        CHECK(List_append(pLists[0], &junkData) == 0);
    }
    // .. should be no more!
    CHECK(List_append(pLists[0], &junkData) == -1);
}

void testUseAllNodesFreeAllReuseAll() 
{
    List* pList = List_create();

    // Put all nodes into the lists
    int junkData = 42;
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++) {
        CHECK(List_insert(pList, &junkData) == 0);
    }
    CHECK(List_count(pList) == LIST_MAX_NUM_NODES);

    // Free all
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++) {
        CHECK(List_trim(pList) == &junkData);
    }
    CHECK(List_count(pList) == 0);

    // Reuse all
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++) {
        CHECK(List_insert(pList, &junkData) == 0);
    }
    CHECK(List_count(pList) == LIST_MAX_NUM_NODES);
    // .. should be no more!
    CHECK(List_append(pList, &junkData) == -1);
}


void testUseAllListsFreeFirstReuse()
{
    // Use all lists
    List* pLists[LIST_MAX_NUM_HEADS];
    for (int i = 0; i < LIST_MAX_NUM_HEADS; i++) {
        pLists[i] = List_create();
        CHECK(pLists[i] != NULL);
    }
    CHECK(List_create() == NULL);

    // Free first
    List_free(pLists[0], freeCallback);

    // Reuse
    CHECK(List_create() != NULL);
    CHECK(List_create() == NULL);
}
void testUseAllListsFreeSomeReuse()
{
    // Use all lists
    List* pLists[LIST_MAX_NUM_HEADS];
    for (int i = 0; i < LIST_MAX_NUM_HEADS; i++) {
        pLists[i] = List_create();
        CHECK(pLists[i] != NULL);
    }
    CHECK(List_create() == NULL);

    // Free some
    int countFreed = 0;
    for (int i = 0; i < LIST_MAX_NUM_HEADS; i+=3) {
        List_free(pLists[i], freeCallback);
        countFreed++;
    }

    // Reuse
    for (int i = 0; i < countFreed; i++) {
        CHECK(List_create() != NULL);
    }
    CHECK(List_create() == NULL);
}

void testTwoListsTestIndependentBeforeAfter() 
{
    List* pList1 = makeListOfThree();
    List* pList2 = makeListOfFour();
    
    // Set list 1 to before
    CHECK(List_first(pList1) == &itemOne);
    CHECK(List_prev(pList1) == NULL);

    // Set list 2 to after
    CHECK(List_last(pList2) == &itemFour);
    CHECK(List_next(pList2) == NULL);
    

    // Ensure the before/after are independent
    CHECK(List_prev(pList2) == &itemFour);
    CHECK(List_next(pList2) == NULL);

    CHECK(List_next(pList1) == &itemOne);
    CHECK(List_prev(pList1) == NULL);

    CHECK(List_prev(pList2) == &itemFour);
    CHECK(List_next(pList2) == NULL);
}


void testInsertInMiddle()
{
    double newValue = 25;

    List* pList = makeListOfFour();
    List_first(pList);
    CHECK(List_next(pList) == &itemTwo);
    CHECK(List_insert(pList, &newValue) == 0);
    CHECK(listEquals(pList, (void*[]){&itemOne, &newValue, &itemTwo, &itemThree, &itemFour, NULL}));
}



// Array of function pointers
struct TestFunction_s {
    char* name;
    void (*fn)();
};
#define MAKE_TEST_FN(fn) { #fn, fn}

struct TestFunction_s tests[] = {
    MAKE_TEST_FN(testDefinesLargeEnough),
    MAKE_TEST_FN(testSizeEmptyList),
    MAKE_TEST_FN(testSizeOneAdd),
    MAKE_TEST_FN(testSizeOneInsert),
    MAKE_TEST_FN(testSizeOneAppend),
    MAKE_TEST_FN(testSizeOnePrepend),

    MAKE_TEST_FN(testNextOnEmpty),
    MAKE_TEST_FN(testNextOnOnePastStart),
    MAKE_TEST_FN(testNextOnOnePastEnd),
    MAKE_TEST_FN(testNextOnOneAtFirst),
    MAKE_TEST_FN(testNextOnTwoAtFirst),

    MAKE_TEST_FN(testPrevOnEmpty),
    MAKE_TEST_FN(testPrevOnOnePastStart),
    MAKE_TEST_FN(testPrevOnOnePastEnd),
    MAKE_TEST_FN(testPrevOnOneAtFirst),
    MAKE_TEST_FN(testPrevOnTwoAtTwo),

    MAKE_TEST_FN(testCurOnEmpty),
    MAKE_TEST_FN(testCurOnOneBeforeStart),
    MAKE_TEST_FN(testCurOnOneAfterEnd),
    MAKE_TEST_FN(testCurOnOneAtFirst),
    MAKE_TEST_FN(testCurOnThreeAtSecond),

    MAKE_TEST_FN(testFirstOnEmpty),
    MAKE_TEST_FN(testFirstOnTwo),

    MAKE_TEST_FN(testLastOnEmpty),
    MAKE_TEST_FN(testLastOnTwo),

    MAKE_TEST_FN(testRemoveOnEmpty),
    MAKE_TEST_FN(testRemoneOnTwoBeforeStart),
    MAKE_TEST_FN(testRemoveOnTwoAfterEnd),
    MAKE_TEST_FN(testRemoveOnOneAtStart),
    MAKE_TEST_FN(testRemoveOnTwoAtStart),
    MAKE_TEST_FN(testRemoveOnTwoAtEnd),
    MAKE_TEST_FN(testRemoveOnThreeAtSecond),


    MAKE_TEST_FN(testCreateListOutOfLists),
    MAKE_TEST_FN(testCreateListAfterFree),
    MAKE_TEST_FN(testAddOutOfNodes),
    MAKE_TEST_FN(testInsertOutOfNodes),
    MAKE_TEST_FN(testAppendOutOfNodes),
    MAKE_TEST_FN(testPrependOutOfNodes),
    MAKE_TEST_FN(testAddAfterRemove),
    MAKE_TEST_FN(testInsertAfterRemove),
    MAKE_TEST_FN(testAppendAfterRemove),
    MAKE_TEST_FN(testAddAfterFree),
    MAKE_TEST_FN(testAddAfterTrim),

    MAKE_TEST_FN(testConcatOnTwoEmpty),
    MAKE_TEST_FN(testConcatOnEmptyAndTwo),
    MAKE_TEST_FN(testConcatOnTwoAndEmpty),
    MAKE_TEST_FN(testConcatOnOneAndOne),
    MAKE_TEST_FN(testConcatOnThreeAndThree),
    MAKE_TEST_FN(testConcatOutOfNodesAndLists),

    MAKE_TEST_FN(testFreeOnEmpty),
    MAKE_TEST_FN(testFreeOnOne),
    MAKE_TEST_FN(testFreeOnThree),
    MAKE_TEST_FN(testFreeOnAll),

    MAKE_TEST_FN(testTrimOnEmpty),
    MAKE_TEST_FN(testTrimOnOne),
    MAKE_TEST_FN(testTrimOnTwo),

    MAKE_TEST_FN(testComplex),

    MAKE_TEST_FN(testSearchOnEmpty),
    MAKE_TEST_FN(testSearchOnOneFound),
    MAKE_TEST_FN(testSearchOnOneNotFound),
    MAKE_TEST_FN(testSearchOnFourFoundTwice),
    MAKE_TEST_FN(testSearchOnFourFromBeforeList),
    MAKE_TEST_FN(testSearchOnFourFromBeforeListNotFound),

    MAKE_TEST_FN(testOnTwoTakeAllNodesFreeOddsReUseAll),
    MAKE_TEST_FN(testOnTwoTakeAllNodesFreeEvensReUseAll),
    MAKE_TEST_FN(testOnTwoTakeAllNodesFreeFirstHalfReUseAll),
    MAKE_TEST_FN(testOnTwoTakeAllNodesFreeSecondHalfReUseAll),

    MAKE_TEST_FN(testUseAllNodesFreeAllReuseAll),
    MAKE_TEST_FN(testUseAllListsFreeFirstReuse),
    MAKE_TEST_FN(testUseAllListsFreeSomeReuse),
    MAKE_TEST_FN(testTwoListsTestIndependentBeforeAfter),

    MAKE_TEST_FN(testInsertInMiddle),


    {"", NULL}  // Terminate
};


int main(int argCount, char *args[]) 
{
    int numTests = 0;
    while (tests[numTests].fn != NULL) {
        numTests++;
    }

    // Help
    if (argCount != 2) {
        printf("USAGE:\n");
        printf("  %s get_num_tests  : Print # known tests\n", args[0]);
        printf("  %s N              : Where N is 0 to num_tests - 1: execute test N\n", args[0]);
        printf("  %s -1             : Execute all tests\n", args[0]);
        printf("        Returns 0 for succes; 1 for failure.\n");
        printf("        Prints to stdout test's output.\n");
        return 0;
    }

    // Poll for number of tests
    if (strcmp("get_num_tests", args[1]) == 0) {
        printf("%d\n", numTests);
        return 0;
    }

    // Get test number to execute
    int testNumber = atoi(args[1]);
    if (testNumber >= numTests || testNumber < -1) {
        printf("Inavlid test number; must be [0, %d]\n", numTests-1);
        return 1;
    }

    
    // All tests?
    if (testNumber == -1) {     
        printf("Running ALL tests...\n");   
        for (int i = 0; i < numTests; i++) {
            printf("Running test (%d) %s\n", i, tests[i].name);
            (*tests[i].fn)();
        }
    }

    // Specific test:
    else {
        printf("Test function: %s (#%d)\n", tests[testNumber].name, testNumber);
        // Flush so if app freezes, we still get output
        fflush(stdout);

        (*tests[testNumber].fn)();
    }

    // We got here?!? PASSED!
    // printf("********************************\n");
    // printf("           PASSED\n");
    // printf("********************************\n");
    return 0;
}