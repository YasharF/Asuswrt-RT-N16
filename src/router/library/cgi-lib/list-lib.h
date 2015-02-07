#ifndef _LIST_H_
#define _LIST_H_
/**********************************************************************************
 *	Author: Noel V Aguilar
 *  Version: 1.4
 *  Filename: list-lib.h
 *	
 *	Include this header file in your program in order to be able to use the linked
 *	list library.
 *
 *  When declaring variables that will maintain the linked list, all you will need
 *  is one pointer variable per linked list of type: LIST
 *
 *	True will be equal to a non-zero number and False will be equal to 0. Within
 *  my code I have made True the value of 1 and False the value of 0.
 *
 *  the Data being inserted does not need to have memory already allocated to it,
 *  the library will allocate memory, it is better if you use a automatatic variable,
 *  if you choose not to use an automatic variable then additional memory will be
 *  used up by the list in order to copy the data into the memory space allocated
 *  by the program.
 *
 *  IMPORTANT: This version of list-lib is a cut down version of the original,
 *             this library version has the exact code as the full library version
 *             except that it only contains the functions used by CGI-LIB
 *
 **********************************************************************************/


#include <string.h>
#include <stdlib.h>


/*************************** STRUCTURE PROTOTYPES ******************************/

/* node structure declaration */
typedef struct entry
{
	void *data;   /* the data for one entry */
	struct entry *nextlink;  /* the next link entry in the list */
}entry_t;



/* actual linked list structure declaration */
typedef struct list
{
	entry_t *ListHead;   /* pointer to the 1st entry in the list */
	long MaxEntries;	 /* maximum entries the list may have */
	long Count;			 /* number of entries in list right now */
}LIST;


/**************************** FUNCTION PROTOTYPES ******************************/

/*
	CreateList -	Allocates storage for a structure of type LIST and
					returns its address or NULL if no memory is
					available.  If n > 0 then the list will be limited
					to at most n entries; otherwise the list may grow
					indefinitely, limited only by the available memory
					on the heap.
*/
LIST *CreateList (long n);

/*
	DestroyList -	Returns the storage for every entry in the list, as
					well as for the list itself, to the heap.
*/
void DestroyList(LIST *List);

/*
	GetCount -	Returns the count of the number of entries that have
				been added to the list so far.
*/
long GetCount(LIST *List);

/*
	InsertEntry -	Insert a new entry at the appropriate position
					within the list.  The entry to be added and its
					size, in bytes, are provided as the 2nd and 3rd
					arguments.  Returns true or false, depending on
					whether the entry was added successfully.

	The function supplied as the fourth argument is used by
	InserEntry to determine whether the data being added is less
	that, greather than, or equal to another node in the list. This
	function must return -1 if its first argument precededs the
	second, 0 if both arguments are equal, and 1 if the second
	argument precededs the first. The first argument to the defined
	function will be the data, and the second argument will be the 
	current node data.
	
	When inserting the data, if the data is less than then the data will
	be inserted before the node, if the values are equal then the data
	will be inserted after node, and if the values are equal then the 
	data will be inserted after the first equal value, unless the user of
	the library details so that it will take into account the appearence of
	more than one field and compares these values also.

*/
int InsertEntry(LIST *List, void *Data, unsigned long Size, int(*function)(void *Data1, void *Data2));

/*
	FindEntry -		Locates an entry within the list and returns the address of
					the entry, or NULL if the entry was not found.  The data being
					sought is supplied as the 2nd argument.

	The function supplied as the third argument is called by FindEntry to determine
	whether the data being sought is identical to an existing entry in the list.
	This function must return true if its two arguments are equal, and False
	otherwise.  The first argument will be data to be found and the second argument
	passed to the functionwill be the current node of the data we are on.
*/
entry_t *FindEntry(LIST *List, void *Data, int(*function)(void *Data1, void *Data2));
/*
	FindEntry -		Locates an entry within the list and returns 
					the address of the data, or	NULL if the entry was not found.
					The data being sought is supplied as the 2nd argument.

	The function supplied as the third argument is called by FindEntry to determine
	whether the data being sought is identical to an existing entry in the list.
	This function must return True if its two arguments are equal, and False 
	otherwise.  The first argument will be data to be found and the second argument
	passed to the function will the current node data that we are on.
*/
void *FindEntryData(LIST *List, void *Data, int(*function)(void *Data1, void *Data2));

/*
	DeleteEntry -	Removes the first entry from the list whose
					content matches the 2nd argument.  Returns true
					if an entry was deleted; false otherwise.

  The function supplied as the third argument is called by DeleteEntry
  to determine whether an entry in the list matches the data
  provided.  This function must return True if its two arguments
  identical, and False otherwise.  The first argument to the function 
  will be the Data to be deleted, and the second argument will be the
  node data that we are currently on.
*/
int DeleteEntry(LIST *List, void *Data, int(*function)(void *Data1, void *Data2));

/*
	TraverseList -	Walks the entire list from head to tail and calls the
					function supplied as the second argument once for
					each entry in the list.

	The function supplied as the second argument can be used to alter or
	display the contents of the entry, as you see fit.
*/
void TraverseList(LIST *List, void (*function)(void *Data));
#endif
