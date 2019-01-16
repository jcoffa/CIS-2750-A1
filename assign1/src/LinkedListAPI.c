/************************************
 *  Name: Joseph Coffa              *
 *  Student #: 1007320              *
 *  Due Date: February 4, 2019      *
 *                                  *
 *  Assignment 1, CIS*2750          *
 *  LinkedListAPI.c                 *
 ************************************/

// FIXME NO PATHS IN INCLUDE STATEMENTS
//       Use shared libraries after you figure out how to do that
#include "../../LinkedListAPI.h"
#include "../include/Parsing.h"

/** Function to initialize the list metadata head with the appropriate function pointers.
* This function verifies that its arguments are not NULL, allocates a new List struct, and initializes it using
* the arguments
*@pre function pointer arguments must not be NULL
*@post List structure has been allocated and initialized
*@return On success returns newly allocated List struct. Returns NULL if any of the arguments are invalid or malloc fails
*@param printFunction - function pointer to print a single node of the list
*@param deleteFunction - function pointer to delete a single piece of data from the list
*@param compareFunction - function pointer to compare two nodes of the list in order to test for equality or order
**/
List* initializeList(char* (*printFunction)(void* toBePrinted),void (*deleteFunction)(void* toBeDeleted),int (*compareFunction)(const void* first,const void* second)) {
    List *toReturn = malloc(sizeof(List));

    // return NULL if any parameters are invalid or malloc fails
    if (!toReturn || !printFunction || !deleteFunction || !compareFunction) {
        return NULL;
    }

    // initialize elements of the List
    toReturn->head = NULL;
    toReturn->tail = NULL;
    toReturn->length = 0;
    toReturn->deleteData = deleteFunction;
    toReturn->compare = compareFunction;
    toReturn->printData = printFunction;

    return toReturn;
}



/**Function for creating a node for the linked list.
* This node contains abstracted (void *) data as well as previous and next
* pointers to connect to other nodes in the list
*@pre data should be of same size of void pointer on the users machine to avoid size conflicts. data must be valid.
*data must be cast to void pointer before being added.
*@post data is valid to be added to a linked list
*@return On success returns a node that can be added to a linked list. On failure, returns NULL.
*@param data - a void * pointer to any data type.  Data must be allocated on the heap.
**/
Node* initializeNode(void* data) {
    Node *toReturn = malloc(sizeof(Node));

    // initialize elements of the Node
    toReturn->data = data;
    toReturn->previous = NULL;
    toReturn->next = NULL;

    return toReturn;
}



/**Inserts a Node at the front of a linked list.  List metadata is updated
* so that head and tail pointers are correct.
*@pre 'List' type must exist and be used in order to keep track of the linked list.
*@param list pointer to the List struct
*@param toBeAdded - a pointer to data that is to be added to the linked list
**/
void insertFront(List* list, void* toBeAdded) {
    Node *toInsert = initializeNode(toBeAdded);

    toInsert->next = list->head;

    // if the list is not empty, the current head node must be updated
    if (list->head) {
        list->head->previous = toInsert;
    } else {
        // the list is empty, meaning the new node is also the tail
        list->tail = toInsert;
    }

    list->head = toInsert;
    list->length += 1;
}



/**Inserts a Node at the back of a linked list.
*List metadata is updated so that head and tail pointers are correct.
*@pre 'List' type must exist and be used in order to keep track of the linked list.
*@param list pointer to the List struct
*@param toBeAdded - a pointer to data that is to be added to the linked list
**/
void insertBack(List* list, void* toBeAdded) {
    Node *toInsert = initializeNode(toBeAdded);

    toInsert->previous = list->tail;

    // if the list is not empty, the current tail node must be updated
    if (list->tail) {
        list->tail->next = toInsert;
    } else {
        // the list is empty, meaning the new node is also the head
        list->head = toInsert;
    }

    list->tail = toInsert;
    list->length += 1;
}



/** Deletes the entire linked list, freeing all memory associated with the list, including the list struct itself.
* Uses the supplied function pointer to release allocated memory for the data.
* @pre 'List' type must exist and be used in order to keep track of the linked list.
* @param list pointer to the List struct
**/
void freeList(List* list) {
    // FIXME this leaks memory, most likely due to the clearList() function
    clearList(list);
    free(list);
}

/** Clears the list: frees the contents of the list - Node structs and data stored in them -
 * without deleting the List struct
 * uses the supplied function pointer to release allocated memory for the data
 * @pre 'List' struct must exist and be used in order to keep track of the linked list.
 * @post List struct still exists, list head = list tail = NULL, list length = 0
 * @param list pointer to the List-type dummy node
 * @return  on success: NULL, on failure: head of list
**/
void clearList(List* list) {
    Node *toDelete;
    ListIterator iter = createIterator(list);

    while (iter.current) {
        toDelete = iter.current;
        nextElement(&iter);
        list->deleteData(toDelete->data);
        free(toDelete);
    }

    list->length = 0;
}


/** Uses the comparison function pointer to place the element in the
* appropriate position in the list.
* should be used as the only insert function if a sorted list is required.
*@pre List exists and has memory allocated to it. Node to be added is valid.
*@post The node to be added will be placed immediately before or after the first occurrence of a related node
*@param list - a pointer to the List struct
*@param toBeAdded - a pointer to data that is to be added to the linked list
**/
void insertSorted(List* list, void* toBeAdded) {
    Node *toInsert = initializeNode(toBeAdded);

    ListIterator iter = createIterator(list);

    while (iter.current) {
        if (list->compare(toInsert->data, (iter.current)->data) > 0) {
            // TODO iterate ...
        } else {
            // TODO check if toInsert will become the new head or tail
            if (iter.current == list->head) {
                // toInsert will become the new head
            } else if (iter.current == list->tail) {
                // toInsert will become the new tail
            }
        }
    }
}



/** Removes data from the list, deletes the node and frees the memory,
 * changes pointer values of surrounding nodes to maintain list structure.
 * returns the data
 * You can assume that the list contains no duplicates
 *@pre List must exist and have memory allocated to it
 *@post toBeDeleted will have its memory freed if it exists in the list.
 *@param list - a pointer to the List struct
 *@param toBeDeleted - a pointer to data that is to be removed from the list
 *@return on success: void * pointer to data  on failure: NULL
 **/
void* deleteDataFromList(List* list, void* toBeDeleted);



/**Returns a pointer to the data at the front of the list. Does not alter list structure.
 *@pre The list exists and has memory allocated to it
 *@param list - a pointer to the List struct
 *@return pointer to the data located at the head of the list
 **/
void* getFromFront(List* list) {
    if (!(list->head)) {
        return NULL;
    }

    return list->head->data;
}



/**Returns a pointer to the data at the back of the list. Does not alter list structure.
 *@pre The list exists and has memory allocated to it
 *@param list - a pointer to the List struct
 *@return pointer to the data located at the tail of the list
 **/
void* getFromBack(List* list) {
    if (!(list->tail)) {
        return NULL;
    }

    return list->tail->data;
}



/**Returns a string that contains a string representation of
the list traversed from  head to tail. Utilize the list's printData function pointer to create the string.
returned string must be freed by the calling function.
 *@pre List must exist, but does not have to have elements.
 *@param list - a pointer to the List struct
 *@return on success: char * to string representation of list (must be freed after use).  on failure: NULL
 **/
char* toString(List* list) {
    // TODO untested
    char *toReturn = malloc(1000);
    strcpy(toReturn, "");

    ListIterator iter = createIterator(list);

    // concatenate all the list elements together in one string
    while (iter.current) {
        concat(toReturn, (char *)nextElement(&iter));
        concat(toReturn, ", ");
    }

    // remove the last comma at the end of the list
    char *lastComma = strrchr(toReturn, ',');
    if (lastComma) {
        *lastComma = '\0';
    }

    return toReturn;
}


/** Function for creating an iterator for the linked list.
 * Newly created iterator points to the head of the list.
 *@pre List exists and is valid
 *@post List remains unchanged.  The iterator has been allocated and points to the head of the list.
 *@return The newly created iterator object.
 *@param list - pointer to the List struct to iterate over.
**/
ListIterator createIterator(List* list) {
    ListIterator toReturn;
    toReturn.current = list->head;

    return toReturn;
}


/** Function that returns the next element of the list through the iterator.
* This function returns the data at head of the list the first time it is called after
* the iterator was created.
* Every subsequent call returns the data associated with the next element.
* Returns NULL once the end of the iterator is reached.
*@pre List exists and is valid.  Iterator exists and is valid.
*@post List remains unchanged.  The iterator points to the next element on the list.
*@return The data associated with the list element that the iterator pointed to when the function was called.
*@param iter - a pointer to an iterator for a List struct.
**/
void* nextElement(ListIterator* iter) {
    void *toReturn = iter->current->data;

    iter->current = iter->current->next;
    return toReturn;
}


/**Returns the number of elements in the list.
 *@pre List must exist, but does not have to have elements.
 *@param list - a pointer to the List struct.
 *@return on success: number of elements in the list (0 or more).  on failure: -1 (e.g. list not initlized correctly)
 **/
int getLength(List* list) {
    if (list->length >= 0) {
        return list->length;
    }

    return -1;
}


/** Function that searches for an element in the list using a comparator function.
 * If an element is found, a pointer to the data of that element is returned
 * Returns NULL if the element is not found.
 *@pre List exists and is valid.  Comparator function has been provided.
 *@post List remains unchanged.
 *@return The data associated with the list element that matches the search criteria.  If element is not found, return NULL.
 *@param list - a pointer to the List sruct
 *@param customCompare - a pointer to comparator function for customizing the search
 *@param searchRecord - a pointer to search data, which contains seach criteria
 *Note: while the arguments of customCompare() and searchRecord are all void, it is assumed that records they point to are
 *      all of the same type - just like arguments to the compare() function in the List struct
 **/
void* findElement(List* list, bool (*customCompare)(const void* first,const void* second), const void* searchRecord);
