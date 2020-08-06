#include "list.h"


static List List_array[LIST_MAX_NUM_HEADS];
static Node Node_array[LIST_MAX_NUM_NODES];
static int List_counter = 0;
static int Node_counter = 0;


// get node_counter
int get_node_counter(){
	return Node_counter;
}

// get list_counter
int get_list_counter(){
	return List_counter;
} 

//print list
void printList(List* pList){
    Node* head = pList->head;
    printf("%s", "list:");
    while(head != NULL){
        printf(" %d", *((int*) head->item));
        head = head->next;
    }
    printf("%s\n","");
}

// create and set up a new Node
static Node set_up_new_Node(void* pItem, Node* p, Node* n){
	Node newNode;
	newNode.item = pItem;
	newNode.index = Node_counter;
	newNode.prev = p;
	newNode.next = n;
	Node_array[Node_counter] = newNode;
	Node_counter++;
	return Node_array[Node_counter-1];
}


// set up pList
static void set_up_pList(List* pList, void* pItem, int i, int count, Node* h, Node* t, int index){
	pList->current_item = pItem;
	pList->item_index = i;
	pList->count = count;
	pList->head = h;
	pList->tail = t;
	pList->index = index;
}


// remove the item from the node
static void free_Node(Node node, int index){
	node.prev = NULL;
	node.next = NULL;

	for (int i = index; i < Node_counter-1; i++){
		int newIndex = Node_array[i].index;
		Node_array[i] = Node_array[i+1];
		Node_array[i].index = newIndex;
	}
	Node_counter--;

	return;
}

// remove list from list_array
static void remove_list(List* pList){
	for (int i = pList->index; i < List_counter-1; i++){
		List_array[i] = List_array[i+1];
		List_array[i].index = i;
	}
	List_counter--;
}


// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create(){
	// When counter exceed maximum number of list return NULL
	if (List_counter+1 > LIST_MAX_NUM_HEADS)		
		return NULL;

	// Create newlist and initialize all attributes
	// index is -1 means point beyond the start
	// index is -2 means point after the end 
	// index is -3 means the list is initially empty
	// Assign newlist to the global static List_array 
	else{			

 		List newlist;
 		newlist.current_item = NULL;
 		newlist.item_index = -3;
 		newlist.count = 0;
 		newlist.head = NULL;
 		newlist.tail = NULL;
 		newlist.index = List_counter;
		List_array[List_counter] = newlist;		
		List_counter ++;
		return &List_array[List_counter-1];
	}
}


// Returns the number of items in pList.
int List_count(List* pList){
	return pList->count;
}


// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList){
	// If pList is empty
	if (pList->head == NULL){
		pList->current_item = NULL;
		pList->item_index = -3;
		return NULL;
	}

	// pList not empty
	else{
		void* item_reference = pList->head->item;
		int head_item_index = pList->head->index;
		pList->current_item = item_reference;
		pList->item_index = head_item_index;
		return item_reference;
	}
}


// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList){
	// If pList is empty
	if (pList->head == NULL){
		pList->current_item = NULL;
		pList->item_index = -3;
		return NULL;
	}

	// pList not empty
	else{
		void* item_reference = pList->tail->item;
		int last_item_index = pList->tail->index;
		pList->current_item = item_reference;
		pList->item_index = last_item_index;
		return item_reference;
	}
}


// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList){
	// list is empty
	if (pList->item_index == -3){
		return NULL;
	}

	// current item after the end of the pList
	if (pList->item_index == -2){
		pList->current_item = NULL;
		pList->item_index = -2;
		return NULL;
	}

	// if next node is NULL
	if (pList->item_index >= 0 && Node_array[pList->item_index].next == NULL){
		pList->current_item = NULL;
		pList->item_index = -2;
		return NULL;
	}

	// if current item is beyond the start of list
	if (pList->item_index == -1){
		void* head_item = pList->head->item;
		pList->current_item = head_item;
		pList->item_index = pList->head->index;
		return head_item;
	}

	// normal case
	else{
		int next_index = Node_array[pList->item_index].next->index;
		void* next_item = Node_array[pList->item_index].next->item;
		pList->current_item = next_item;
		pList->item_index = next_index;
		return next_item;
	}
}


// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList){
	// if list is empty
	if (pList->item_index == -3){
		return NULL;
	}

	// current item beyond the start of the pList
	if (pList->item_index == -1){
		pList->current_item = NULL;
		pList->item_index = -1;
		return NULL;
	}

	// if previous node is NULL
	if (pList->item_index >= 0 && Node_array[pList->item_index].prev == NULL){
		pList->current_item = NULL;
		pList->item_index = -1;
		return NULL;
	}

	// if current node is after the end of list
	if (pList->item_index == -2){
		void* tail_item = pList->tail->item;
		pList->current_item = tail_item;
		pList->item_index = pList->tail->index;
		return tail_item;
	}
	// normal case
	else{
		int prev_index = Node_array[pList->item_index].prev->index;
		void* prev_item = Node_array[pList->item_index].prev->item;
		pList->current_item = prev_item;
		pList->item_index = prev_index;
		return prev_item;
	}
}


// Returns a pointer to the current item in pList.
// Returns NULL if current is before the start of the pList, or after the end of the pList.
void* List_curr(List* pList){
	// current item before start or after end or empty list
	if (pList->item_index == -1 || pList->item_index == -2 || pList->item_index == -3){
		return NULL;
	}
	else{
		return pList->current_item;
	}
}


// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_add(List* pList, void* pItem){
	// reach max nodes leads to add failure 
	if (Node_counter+1 > LIST_MAX_NUM_NODES){		
		return -1;
	}

	// empty list
	if (pList->item_index == -3){
		//set up new Node
		Node newNode = set_up_new_Node(pItem, NULL, NULL);
		// set up pList
		set_up_pList(pList, pItem, newNode.index, pList->count+1, &Node_array[Node_counter-1], &Node_array[Node_counter-1], pList->index);
		return 0;
	}

	// current pointer is before the start of the pList
	if (pList->item_index == -1){
		// set up new Node
		Node newNode = set_up_new_Node(pItem, NULL, pList->head); 
		// head previous point to the new Node
		pList->head->prev = &Node_array[Node_counter-1];
		// set up pList
		set_up_pList(pList, pItem, newNode.index, pList->count+1, &Node_array[Node_counter-1], pList->tail, pList->index);
		return 0;
	}

	// current pointer is beyond the end of the pList or current item is in tail
	if (pList->item_index == -2 || pList->item_index == pList->tail->index){
		// set up new Node 
		Node newNode = set_up_new_Node(pItem, pList->tail, NULL); 
		// tail next point to the new Node
		pList->tail->next = &Node_array[Node_counter-1];
		// set up pList
		set_up_pList(pList, pItem, newNode.index, pList->count+1, pList->head, &Node_array[Node_counter-1], pList->index);
		return 0;
	}

	// normal case
	else{
		// set up new Node
		Node newNode = set_up_new_Node(pItem, &Node_array[pList->item_index], Node_array[pList->item_index].next); 
		// current_item next item previous point to new Node and current_item next point to new Node 
		Node_array[pList->item_index].next->prev = &Node_array[Node_counter-1];
		Node_array[pList->item_index].next = &Node_array[Node_counter-1];
		// set up pList
		set_up_pList(pList, pItem, newNode.index, pList->count+1, pList->head, pList->tail, pList->index);
		return 0;
	}
}


// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert(List* pList, void* pItem){
	// reach max nodes leads to add failure 
	if (Node_counter+1 > LIST_MAX_NUM_NODES){			
		return -1;
	}

	// empty list
	if (pList->item_index == -3){
		//set up new Node
		Node newNode = set_up_new_Node(pItem, NULL, NULL); 
		// set up pList
		set_up_pList(pList, pItem, newNode.index, pList->count+1, &Node_array[Node_counter-1], &Node_array[Node_counter-1], pList->index);
		return 0;
	}

	// current pointer is before the start of the pList or current item is in head
	if (pList->item_index == -1 || pList->item_index == pList->head->index){
		// set up new Node 
		Node newNode = set_up_new_Node(pItem, NULL, pList->head); 
		// pList head previous point to new Node
		pList->head->prev = &Node_array[Node_counter-1];
		// set up pList
		set_up_pList(pList, pItem, newNode.index, pList->count+1, &Node_array[Node_counter-1], pList->tail, pList->index);
		return 0;
	}

	// current pointer is beyond the end of the pList
	if (pList->item_index == -2){
		// set up new Node 
		Node newNode = set_up_new_Node(pItem, pList->tail, NULL); 
		// pList tail next connect to new Node
		pList->tail->next = &Node_array[Node_counter-1];
		// set up pList
		set_up_pList(pList, pItem, newNode.index, pList->count+1, pList->head, &Node_array[Node_counter-1], pList->index);
		return 0;
	}

	// normal case
	else{
		// set up new Node
		Node newNode = set_up_new_Node(pItem, Node_array[pList->item_index].prev, &Node_array[pList->item_index]); 
		// previous item of current_item point to new Node and current_item previous point to new Node
		Node_array[pList->item_index].prev->next = &Node_array[Node_counter-1];
		Node_array[pList->item_index].prev = &Node_array[Node_counter-1];
		// set up pList
		set_up_pList(pList, pItem, newNode.index, pList->count+1, pList->head, pList->tail, pList->index);
		return 0;
	}
}


// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem){
	// reach max nodes leads to add failure
	if(Node_counter+1 > LIST_MAX_NUM_NODES){
		return -1;
	}

	// if the pList is empty
	if (pList->item_index == -3){
		// set up new Node
		Node newNode = set_up_new_Node(pItem, NULL, NULL); 
		// set up pList
		set_up_pList(pList, pItem, newNode.index, pList->count+1, &Node_array[Node_counter-1], &Node_array[Node_counter-1], pList->index);
		return 0;
	}

	// pList not empty: normal case
	else{
		// set up new Node
		Node newNode = set_up_new_Node(pItem, pList->tail, NULL); 
		// pList tail next connect to new Node
		pList->tail->next = &Node_array[Node_counter-1];
		// set up pList
		set_up_pList(pList, pItem, newNode.index, pList->count+1, pList->head, &Node_array[Node_counter-1], pList->index);
		return 0;
	}
}


// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem){
	// reach max nodes leads to add failure
	if(Node_counter+1 > LIST_MAX_NUM_NODES){
		return -1;
	}

	// if the pList is empty
	if (pList->item_index == -3){
		// set up new Node
		Node newNode = set_up_new_Node(pItem, NULL, NULL); 
		// set up pList
		set_up_pList(pList, pItem, newNode.index, pList->count+1, &Node_array[Node_counter-1], &Node_array[Node_counter-1], pList->index);
		return 0;
	}

	// pList not empty: normal case
	else{
		// set up new Node
		Node newNode = set_up_new_Node(pItem, NULL, pList->head); 
		// pList head previous point to new Node
		pList->head->prev = &Node_array[Node_counter-1];
		// set up pList
		set_up_pList(pList, pItem, newNode.index, pList->count+1, &Node_array[Node_counter-1], pList->tail, pList->index);
		return 0;
	}
}


// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList){
	// empty list or current pointer is before the start of the pList, or beyond the end of the pList
	if (pList->item_index == -3 || pList->item_index == -1 || pList->item_index == -2){
		return NULL;
	}

	// if the current item is in head
	if (pList->item_index == pList->head->index){

		/*printf("head_index: %d\n", pList->head->index);
		for (int i = 0; i < Node_counter; i++){
			printf("%d ", *((int*) Node_array[i].item));
		}
		printf("\n");*/


		void* return_item = pList->current_item;
		Node* temp = pList->head;
		// remove the Node which contain the item from the pList
		pList->head = pList->head->next;
		// becomes empty list
		if (pList->head == NULL)
			set_up_pList(pList, NULL, -3, 0, NULL, NULL, pList->index);
		else{
			pList->head->prev = NULL;
			set_up_pList(pList, pList->head->item, pList->head->index, pList->count-1, pList->head, pList->tail, pList->index);
		}

		// remove the item from the node
		free_Node(Node_array[temp->index], temp->index);
		return return_item;
	}

	// if the current item is in tail
	if (pList->item_index == pList->tail->index){
		void* return_item = pList->current_item;
		// remove the Node which contain the item from the pList
		set_up_pList(pList, NULL, pList->item_index, pList->count-1, pList->head, pList->tail->prev, pList->index);
		pList->tail->next = NULL;
		// remove the item from the node
		free_Node(Node_array[pList->item_index], pList->item_index);
		pList->item_index = -2;
		return return_item;
	}

	// normal case
	else{
		void* return_item = pList->current_item;
		// remove the Node which contain the item from the pList
		pList->current_item = Node_array[pList->item_index].next->item;
		int new_index = Node_array[pList->item_index].next->index;
		Node_array[pList->item_index].next->prev = Node_array[pList->item_index].prev;
		Node_array[pList->item_index].prev->next = Node_array[pList->item_index].next;
		// remove the item from the node
		free_Node(Node_array[pList->item_index], pList->item_index);
		pList->count = pList->count - 1;
		pList->item_index = new_index;
		return return_item;
	}
}


// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2){
	// if pList1 is empty
	if (pList1->head == NULL){
		set_up_pList(pList1, NULL, -1, pList2->count, pList2->head, pList2->tail, pList2->index);

		// Remove plist2 from the List_array
		remove_list(pList2);

		set_up_pList(pList2, NULL, -3, 0, NULL, NULL, -1);
		return;
	}

	// if pList2 is empty
	if (pList2->head == NULL)
		return;

	// normal case
	else{
		pList1->tail->next = pList2->head;
		pList2->head->prev = pList1->tail;
		set_up_pList(pList1, pList1->current_item, pList1->item_index, pList1->count+pList2->count, pList1->head, pList2->tail, pList1->index);

		// Remove plist2 from the List_array
		remove_list(pList2);

		set_up_pList(pList2, NULL, -3, 0, NULL, NULL, -1);
		return;
	}
}


// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
// UPDATED: Changed function pointer type, May 19
typedef void (*FREE_FN)(void* pItem);
void List_free(List* pList, FREE_FN pItemFreeFn){
	// empty list
	if (pList->head == NULL){
		// remove plist from List_array 
		remove_list(pList);

		set_up_pList(pList, NULL, -3, 0, NULL, NULL, -1);
		return;
	}

	// not empty
	int number_iter = pList->count;
	// set current item to its head
	pList->current_item = pList->head->item;
	pList->item_index = pList->head->index;
	// call List_remove to remove node
	while (number_iter > 0){
		void* client_item = List_remove(pList);
		pItemFreeFn(client_item);
		number_iter--;
	}

	// Remove plist from the List_array
	remove_list(pList);

	set_up_pList(pList, NULL, -3, 0, NULL, NULL, -1);

	return;
}


// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList){
	// if pList is empty
	if (pList->head == NULL){
		return NULL;
	}

	// if just contain one item
	if (pList->count == 1){
		void* last_item = pList->tail->item;

		// remove the Node which contain the item from the pList
		free_Node(Node_array[pList->tail->index], pList->tail->index);
		set_up_pList(pList, NULL, -3, 0, NULL, NULL, pList->index);
		return last_item;
	}

	// normal case
	else{
		Node* last_node = pList->tail;
		void* last_node_item = last_node->item;
		Node* new_last_node = Node_array[pList->tail->index].prev;
		// remove the Node which contain the item from the pList
		free_Node(Node_array[pList->tail->index], pList->tail->index);
		set_up_pList(pList, new_last_node->item, new_last_node->index, pList->count-1, pList->head, new_last_node, pList->index);
		pList->tail->next = NULL;
		// remove item from the node
		last_node->prev = NULL;
		last_node->next = NULL;
		last_node->item = NULL;
		return last_node_item;
	}
}


// Search pList, starting at the current item, until the end is reached or a match is found. 
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second 
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match, 
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator. 
// 
// If a match is found, the current pointer is left at the matched item and the pointer to 
// that item is returned. If no match is found, the current pointer is left beyond the end of 
// the list and a NULL pointer is returned.
// 
// UPDATED: Added clarification of behaviour May 19
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg);
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){
	// empty list
	if (pList->head == NULL){
		set_up_pList(pList, NULL, -3, 0, NULL, NULL, pList->index);
		return NULL;
	}

	Node* temp;
	// if the current pointer is before the start of the pList
	if (pList->item_index == -1){
		temp = &Node_array[pList->head->index];
	}
	// not empty
	else{
		temp = &Node_array[pList->item_index];
	}

	while (temp != NULL){
		// matched
		if (pComparator(temp->item, pComparisonArg)){
			pList->current_item = temp->item;
			pList->item_index = temp->index;
			return pList->current_item;
		}
		temp = temp->next;
	}

	// not found
	set_up_pList(pList, NULL, -2, pList->count, pList->head, pList->tail, pList->index);
	return NULL;
}