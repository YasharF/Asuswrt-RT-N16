/********************************************************************************************
 * Author: Noel V Aguilar
 * Version: 1.4
 * Filename: cgi-priv.h
 *
 * This is the accompanying header file that goes with cgi-lib.c. These function will assist
 * you in manipulating data that is transferred over the web. I have attempted to write
 * the code using ANSI standards.  This could should be compiled on a standard ANSI compiler
 * without any problems.  I have not included any system specific functions in order to make
 * it platform independent.
 *
 *	This private header file contains declarations that are not necessary for the 
 *	development of CGI's but are Only necessary for the library.
 ********************************************************************************************/


/**************************************************/
/* used for defining if a function was successful */
/**************************************************/
#ifndef SUCCESS
#define SUCCESS 1
#endif

/******************************************************/
/* used for defining if a function was not successful */
/******************************************************/
#ifndef FAILURE
#define FAILURE 0
#endif


/***********************************************************/
/* this is the structure of our linked list                */
/**********************************************************/
struct cgi_structure
{
  char *key;
  char *value;
};


/***********************************************************/
/* we can refer to our structure using the CGI_LIST decl.*/
/***********************************************************/
typedef struct cgi_structure CGI_LIST;


/******************************************************/
/* Define the words to look for in our REQUEST_METHOD */
/******************************************************/
#define GET "GET"
#define POST "POST"
