#ifndef _CGI_LIB_H_
#define _CGI_LIB_H_
/********************************************************************************************
 * Author: Noel V Aguilar
 * Version: 1.4
 * Filename: cgi-lib.h
 * Program Library: CGI-LIB
 *
 * This header file contains all the functions that can be used to build a cgi program.
 *
 * This is the accompanying header file for the cgi-lib library. These function will assist
 * you in manipulating data that is transferred over the web. I have attempted to write
 * the code using ANSI standards.  This code can be compiled on a standard ANSI compiler
 * without any problems.  I have not included any system specific functions in order to make
 * it platform independent.
 ********************************************************************************************
 *                                                                           
 * Feel free to use, copy, reference, or modify this program in any way.     
 * Support public domain freeware.                                           
 *                                                                           
 * For updates or to report bugs go to my home page:
 * http://www.geocities.com/SiliconValley/Vista/6493/
 * The web page for the library is:
 * http://www.geocities.com/SiliconValley/Vista/6493/cgi-lib.html
 ********************************************************************************************
 * Disclaimer:                                                                              *
 * THE INFORMATION, CODE AND EXECUTABLE PROVIDED ARE PROVIDED "AS IS" WITHOUT               *
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED               *
 * TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR                *
 * PURPOSE.  IN NO EVENT SHALL NOEL V AGUILAR BE LIABLE FOR ANY DAMAGES                     *
 * WHATSOEVER INCLUDING DIRECT, INDIRECT, INCIDENTAL, CONSEQUENTIAL, LOSS OF                *
 * BUSINESS PROFITS OR SPECIAL DAMAGES.  ALL THE INFORMATION, CODE AND                      *
 * EXECUTABLES ARE FOR EDUCATIONAL PURPOSES ONLY.                                           *
 ********************************************************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list-lib.h"


/******************************************************/
/* begin the definitions of our environment variables */
/******************************************************/
#define GATEWAY_INTERFACE getenv("GATEWAY_INTERFACE")
#define SERVER_PROTOCOL getenv("SERVER_PROTOCOL")
#define PATH_INFO getenv("PATH_INFO")
#define PATH_TRANSLATED getenv("PATH_TRANSLATED")
#define QUERY_STRING getenv("QUERY_STRING")
#define CONTENT_TYPE getenv("CONTENT_TYPE")
#define CONTENT_LENGTH getenv("CONTENT_LENGTH")
#define REQUEST_METHOD getenv("REQUEST_METHOD")
#define SERVER_SOFTWARE getenv("SERVER_SOFTWARE")
#define SERVER_NAME getenv("SERVER_NAME")
#define SERVER_ADMIN getenv("SERVER_ADMIN")
#define SERVER_PORT getenv("SERVER_PORT")
#define SCRIPT_NAME getenv("SCRIPT_NAME")
#define DOCUMENT_ROOT getenv("DOCUMENT_ROOT")
#define REMOTE_HOST getenv("REMOTE_HOST")
#define REMOTE_ADDR getenv("REMOTE_ADDR")
#define REMOTE_USER getenv("REMOTE_USER")
#define REMOTE_GROUP getenv("REMOTE_GROUP")
#define AUTH_TYPE getenv("AUTH_TYPE")
#define REMOTE_IDENT getenv("REMOTE_IDENT")
#define HTTP_ACCEPT getenv("HTTP_ACCEPT")
#define HTTP_COOKIE getenv("HTTP_COOKIE")
#define HTTP_ACCEPT_LANGUAGE getenv("HTTP_ACCEPT_LANGUAGE")
#define HTTP_REFERER getenv("HTTP_REFERER")
#define HTTP_USER_AGENT getenv("HTTP_USER_AGENT")



/**************************************************/
/* Define our global functions that can be called */
/**************************************************/
/*
	is_form_empty() -	This sub will return true if the form is empty otherwise it will
						return false.  This sub can be used to test to see if the form is
						empty so we don't waste time by calling the cgi_input_parse() 
						function.
*/
int is_form_empty();

/*
	cgi_env() -	This function will print out the current environment variables on the
				sytem.  This function will not send out the mime-types automatically.
*/
void cgi_env(void);

/* cgi_err() -	This function will compose an error message for the application to use.  It
				sends the proper mime types to the server when used and it terminates the
				program.  The *msg1 variable is the name that will go between the H1 tags
				and the *msg2 variable is the one that will go between the H2 tags.  This
				function will also terminate the program and send a message to stderr.
*/
void cgi_err(char *msg1, char *msg2);

/* cgi_input_parse() -	This function will take the raw data passed from the browser to the
						server and adds each associated name and value to the linked list
						entries.  It parses the information transmitted using both the
						associated key and value to the linked list entries.  It parses
						both teh GET and POST method.

  This function "should be initialized at the beginning of the calling program", in order
  to get the data into the linked list and be able to use it.  The value returned by the
  function will be a pointer to the beginning of the list or a NULL if the list was not
  created for whatever reason.  The user needs only to create a pointer variable of type
  LIST.
*/
LIST *cgi_input_parse(void);

/*
	list_print() -	This function will print out the key/value pairs in our linked list.
					They will be formatted in a Definition List without any headers
					being assigned.  The head pointer the user has must be passed to it.
*/
void list_print(LIST *head);


/*
	list_clear() -	The list_clear() function will clear the linked list and free up
					the memory.
*/
void list_clear(LIST *head);

/*
	list_count() -	The list_count() function will return the number of items that
					exist in the linked list.
*/
long list_count(LIST *head);

/*
	list_delete_entry() -	This function will delete the specified entry in the
							list, if NULL is supplied for the value then the first
							key found will be deleted, if NULL is supplied for the
							key then the first matching value will be deleted, if 
							both the key and value are supplied then the
							key and value must match in order for the node to be 
							deleted.  A zero value will be returned if the 
							deletion was not sucessful, and a non-zero value will
							be returned if it was successful.
*/
int list_delete_entry(LIST *head, char *key, char *value);

/*
	list_delete_key_multi() -	This function will delete the list of specified
								entries based on the key passed.  This assumes that
								multiple entries with this one named key exist.  A
								zero will be returned if the deletion was not
								successful, and a non-zero value will be returned
								if it was successful at deleting any items, 
								assuming any were found.
*/
int list_delete_key_multi(LIST *head, char *key);

/*
	list_delete_val_multi() -	This function will delete the list of specified
								entries based on the value passed.  This assumes
								that multiple entries with this one name value
								exist.  A zero will be returned if the deletion was
								not successful, and a non-zero value will be 
								returned if it was successuful at deleting any items,
								assuming any were found.
*/
int list_delete_val_multi(LIST *head, char *value);


/* 
	find_val() -	This function will search the linked list and return the
					value in the first node that it finds that matches the
					specified key. If none is found then NULL is returned.
*/
char *find_val(LIST *head, char *key);

/*
	find_key() -	This function will search the linked list and return the
					key in the first node that it finds that matches the 
					specified value. If none is found then NULL is returned.
*/
char *find_key(LIST *head, char *value);

/*
	find_val_multi() -	This function will return  the number of values found, If 0 are
						returned then it means that no values were found.  The actual 
						values are being held in the Vals variable, in order to use this
						variable the address of a **Var variable must be passed.  Space
						is being allocated for Vals, so it is upto the user to free it
						up when they are done.  You must pass the value to be searched for.
*/
long find_val_multi(LIST *head, char *key, char ***Vals);

/*
	find_key_multi() -	This function will return the number of values found.  If 0 is
						returned then it means that no values were found.  The actual
						values are being held in the Vals variable, in order to use this
						variable the address of a **Val variable must be passed.  Space
						is being allocated for Vals, so it is upto the user to free it 
						up when they are done.  The key to be searched for must be given.
*/
long find_key_multi(LIST *head, char *value, char ***Vals);

#endif /* _CGI_LIB_H_ */
