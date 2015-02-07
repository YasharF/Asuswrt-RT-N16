/**********************************************************************************
 *	Author: Noel V Aguilar
 *  Version: 1.4
 *  Filename: list-lib.c
 *
 *	This file contains all the functions that will be used in order to allow the
 *	user to create a linked list.
 *
 *	True will be equal to a non-zero number and False will be equal to 0. Within
 *  my code I have made True the value of 1 and False the value of 0.
 *
 *  The Data being inserted does not need to have memory already allocated to it,
 *  the library will allocate memory, it is better if you use an automatatic variable,
 *  if you choose not to use an automatic variable then additional memory will be
 *  used up by the list in order to copy the data into the memory space allocated
 *  by the library.
 *
 *  IMPORTANT: This version of list-lib is a cut down version of the original,
 *             this library version has the exact code as the full library version
 *             except that it only contains the functions used by CGI-LIB
 *
 **********************************************************************************/


#include "list-lib.h"
#include <stdio.h>
#include <string.h>

/***************************** FUNCTION DEFINITIONS *******************************/

/*
	CreateList -	Allocates storage for a structure of type LIST and
					returns its address or NULL if no memory is
					available.  If n > 0 then the list will be limited
					to at most n entries; otherwise the list may grow
					indefinitely, limited only by the available memory
					on the heap.
*/
LIST *CreateList (long n)
{
	LIST *NewList;

	NewList = (LIST *)malloc(sizeof(LIST));
	if(!NewList)
		return NULL;
	NewList->Count = 0;
	NewList->MaxEntries = n;
	NewList->ListHead = NULL;

	return NewList;
}


/*----------------------------------------------------------------------------------*/


/*
	DestroyList -	Returns the storage for every entry in the list, as
					well as for the list itself, to the heap.
*/
void DestroyList(LIST *List)
{
	entry_t *CurrentEntry=NULL;
	entry_t *NextLink=NULL;

	if(!List)
		return;
	/* check to see if there is something in the list */
	if(List->Count == 0 || List->ListHead == NULL)
		return;

	CurrentEntry = List->ListHead;
#if 1
	while(CurrentEntry)
	{
		NextLink = CurrentEntry->nextlink;
		if(CurrentEntry->data){
			free(CurrentEntry->data);
		}
		free(CurrentEntry);
		CurrentEntry = NextLink;
	}
	free(List);
	List=NULL;
#else
	while(CurrentEntry->nextlink != NULL)
	{
		NextLink = CurrentEntry->nextlink;
		free(CurrentEntry->data);
		free(CurrentEntry);
		CurrentEntry = NextLink;
	}
	free(CurrentEntry->data);
	free(CurrentEntry);
	List=NULL;
#endif
}


/*----------------------------------------------------------------------------------*/


/*
	GetCount -	Returns the count of the number of entries that have
				been added to the list so far.
*/
long GetCount(LIST *List)
{
	return List->Count;
}


/*----------------------------------------------------------------------------------*/


/*
	InsertEntry -	Insert a new entry at the appropriate position
					within the list.  The entry to be added and its
					size, in bytes, are provided as the 2nd and 3rd
					arguments.  Returns true or false, depending on
					whether the entry was added successfully.

	The function supplied as the fourth argument is used by
	InsertEntry to determine whether the data being added is less
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
int InsertEntry(LIST *List, void *Data, unsigned long Size, int(*function)(void *Data1, void *Data2))
{
	entry_t *NewEntry=NULL;
	entry_t *CurrentEntry=NULL;
	entry_t *PrevEntry=NULL;
	char *NewData=NULL;
	int funcValue=0;


	/* check that we have not gone beyond the maximum number of entries allowed */
	if((List->Count != 0) && (List->Count == List->MaxEntries))
		return 0;


	CurrentEntry = List->ListHead;


	if((NewEntry = (entry_t *)malloc(sizeof(entry_t))) == NULL)
		return 0;

	if((NewData = malloc(Size)) == NULL){
		free(NewEntry);
		return 0;
	}

	/* copy all the data */
	memcpy(NewData, Data, Size);

	/* give the pointer of the data into the NewEntry */
	NewEntry->data = NewData;
	NewEntry->nextlink = NULL;
	/* Begin figuring out where to insert the node */
	if(CurrentEntry == NULL)	/* if no nodes have been added */
	{
		NewEntry->nextlink = NULL;
		List->ListHead = NewEntry;
		List->Count++;
		return 1;
	}
	else						/* the list contains at least one node */
	{
		while(CurrentEntry->nextlink != NULL)
		{
			if((funcValue=(function)(Data,CurrentEntry->data)) == 1) /* value is greater */
			{
				PrevEntry = CurrentEntry;
				CurrentEntry = CurrentEntry->nextlink;
				continue;
			}
			else if (funcValue == -1) /* value is less than */
			{
				if(CurrentEntry == List->ListHead)
				{
					NewEntry->nextlink = CurrentEntry;
					List->ListHead = NewEntry;
					List->Count++;
					return 1;
				}
				else
				{
					NewEntry->nextlink = CurrentEntry;
					PrevEntry->nextlink = NewEntry;
					List->Count++;
					return 1;
				}
			}
			else if (funcValue == 0) /* values are equal */
			{
					NewEntry->nextlink = CurrentEntry->nextlink;
					CurrentEntry->nextlink = NewEntry;
					List->Count++;
					return 1;
			}
		}

		/* if we are here then it is the last entry.
		if we still get -1 as a value from the function then the NewData
		is smaller than anything else and we have to stick it at the beginning
		*/
		if((funcValue=((function)(Data,CurrentEntry->data))) == 1 || funcValue == 0) /* value is greater */
		{
			NewEntry->nextlink = NULL;
			CurrentEntry->nextlink = NewEntry;
			List->Count++;
			return 1;
		}
		else if(funcValue == -1)  /* value is less than */
		{
			if(CurrentEntry == List->ListHead)
			{
				NewEntry->nextlink = CurrentEntry;
				List->ListHead = NewEntry;
				List->Count++;
				return 1;
			}
			else
			{
				NewEntry->nextlink = CurrentEntry;
				PrevEntry->nextlink = NewEntry;
				List->Count++;
				return 1;
			}
		}
	}

	/* This statement is being added purely to avoid warnings from
	   compilers.  This statement should never be executed.
	*/
	return 0;
}


/*----------------------------------------------------------------------------------*/

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
entry_t *FindEntry(LIST *List, void *Data, int(*function)(void *Data1, void *Data2))
{
	entry_t *CurrentEntry=NULL;

	/* exit the function if empty list or no nodes exist */
        //Melinda modify
	if(!List || List->ListHead == NULL || List->Count == 0)
		return NULL;

	CurrentEntry = List->ListHead;

	while(CurrentEntry->nextlink != NULL)
	{
		if((function)(Data, CurrentEntry->data) == 0) /* arguments do not match */
		{
			CurrentEntry = CurrentEntry->nextlink;
			continue;
		}
		else /* arguments match */
			return CurrentEntry;
	}
	if((function)(Data, CurrentEntry->data) == 0) /* arguments do not match */
		return NULL;
	else  /* arguments match */
		return CurrentEntry;

}


/*----------------------------------------------------------------------------------*/


/*
	FindEntryData -	Locates data within an entry within the list and returns 
					the address of the data, or	NULL if the data was not found.
					The data being sought is supplied as the 2nd argument.

	The function supplied as the third argument is called by FindEntry to determine
	whether the data being sought is identical to an existing entry in the list.
	This function must return True if its two arguments are equal, and False 
	otherwise.  The first argument will be data to be found and the second argument
	passed to the function will the current node data that we are on.
*/
void *FindEntryData(LIST *List, void *Data, int(*function)(void *Data1, void *Data2))
{
	entry_t *CurrentEntry=NULL;

	/* exit the function if empty list or no nodes exist */
        //Melinda modify
	if(!List || List->ListHead == NULL || List->Count == 0)
		return NULL;

	CurrentEntry = List->ListHead;

	while(CurrentEntry->nextlink != NULL)
	{
		if((function)(Data, CurrentEntry->data) == 0) /* arguments do not match */
		{
			CurrentEntry = CurrentEntry->nextlink;
			continue;
		}
		else /* arguments match */
			return CurrentEntry->data;
	}
	if((function)(Data, CurrentEntry->data) == 0) /* arguments do not match */
		return NULL;
	else  /* arguments match */
		return CurrentEntry->data;
}


/*----------------------------------------------------------------------------------*/


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
int DeleteEntry(LIST *List, void *Data, int(*function)(void *Data1, void *Data2))
{
	entry_t *CurrentEntry=NULL;
	entry_t *PrevEntry=NULL;

	CurrentEntry = List->ListHead;

	/* check that the list is not empty */
	if(CurrentEntry == NULL || List->Count == 0)
		return 0;
	
	while(CurrentEntry->nextlink != NULL)
	{
		if(((function)(Data, CurrentEntry->data)) == 0) /* no match */
		{
			PrevEntry = CurrentEntry;
			CurrentEntry = CurrentEntry->nextlink;
			continue;
		}
		else /* there is a match */
		{
			/* make sure that it is not at the beginning */
			if(CurrentEntry == List->ListHead)
			{
				List->ListHead = CurrentEntry->nextlink;
				free(CurrentEntry->data);
				free(CurrentEntry);
			}
			else
			{
				PrevEntry->nextlink = CurrentEntry->nextlink;
				free(CurrentEntry->data);
				free(CurrentEntry);
			}
			List->Count--;
			return 1;
		}
	}
	/* the following is done for the last node */
	if(((function)(Data, CurrentEntry->data)) == 0) /* no match */
		return 0;
	else /* there is a match */
	{
		if(CurrentEntry == List->ListHead)
		{
			List->ListHead = CurrentEntry->nextlink;
			free(CurrentEntry->data);
			free(CurrentEntry);
		}
		else
		{
			PrevEntry->nextlink = CurrentEntry->nextlink;
			free(CurrentEntry->data);
			free(CurrentEntry);
		}
		List->Count--;
		return 1;
	}

}


/*----------------------------------------------------------------------------------*/


/*
	TraverseList -	Walks the entire list from head to tail and calls the
					function supplied as the second argument once for
					each entry in the list.

	The function supplied as the second argument can be used to alter or
	display the contents of the entry, as you see fit.
*/
void TraverseList(LIST *List, void (*function)(void *Data))
{
	entry_t *CurrentEntry;

	/* exit the function if empty list or no nodes exist */
	if(List->ListHead == NULL || List->Count == 0)
		return;

	CurrentEntry = List->ListHead;
	while(CurrentEntry->nextlink != NULL)
	{
		(function)(CurrentEntry->data);
		CurrentEntry = CurrentEntry->nextlink;
	}
	(function)(CurrentEntry->data);


}



