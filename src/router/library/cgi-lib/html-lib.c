/**********************************************************************************************
 *  Author: Noel V Aguilar
 *  Version: 1.4
 *  Filename: html-lib.c
 *
 *  This C HTML library will facilitate the creation of CGI scripts using
 *  C.  In order to access the functions contained within this library
 *  please include the "html-lib.h" file in your source. This file will only assist in
 *  is using html references and will not do any data manipulation.
***********************************************************************************************/

#include "html-lib.h"


/*----------------------------------------------------------------------------------------*/


/*
	mime_header() -	This function will take any text passed to it and it will diplay it as
					the mime type.
*/
void mime_header(char *mime)
{
  printf("Content-type: %s\n\n",mime);
}


/*----------------------------------------------------------------------------------------*/


/*
	html_begin() -	This function will diplay the top of the head of the html page all the
					way upto the body tag.  The title and body attributes may be passed, if
					for any of these fields are not desired, then pass NULL.
*/
void html_begin(char *title, char *attributes)
{
  puts("<HTML>");
  puts("<HEAD>");
  (title == NULL) ? puts("<TITLE></TITLE>") : printf("<TITLE>%s</TITLE>\n",title);
  puts("</HEAD>");
  (attributes == NULL) ? puts("<BODY>") : printf("<BODY %s>\n",attributes);
}


/*----------------------------------------------------------------------------------------*/


/*
	h1() -	This function will diplay whatever text is given between the tags.
*/
void h1(char *str)
{
  printf("<H1>%s</H1>\n",str);
}


/*----------------------------------------------------------------------------------------*/


/*
	h2() -	This function will diplay whatever text is given between the tags.
*/
void h2(char *str)
{
  printf("<H2>%s</H2>\n",str);
}


/*----------------------------------------------------------------------------------------*/


/*
	h3() -	This function will diplay whatever text is given between the tags.
*/
void h3(char *str)
{
  printf("<H3>%s</H3>\n",str);
}


/*----------------------------------------------------------------------------------------*/


/*
	h4() -	This function will diplay whatever text is given between the tags.
*/
void h4(char *str)
{
  printf("<H4>%s</H4>\n",str);
}


/*----------------------------------------------------------------------------------------*/


/*
	h5() -	This function will diplay whatever text is given between the tags.
*/
void h5(char *str)
{
  printf("<H5>%s</H5>\n",str);
}


/*----------------------------------------------------------------------------------------*/


/*
	h6() -	This function will diplay whatever text is given between the tags.
*/
void h6(char *str)
{
  printf("<H6>%s</H6>\n",str);
}


/*----------------------------------------------------------------------------------------*/


/*
	hidden() -	This function will print out hidden fields to the html page.  The Name and
				value of the hidden field must be passed.
*/
void hidden(char *name, char *value)
{
  printf("<INPUT TYPE=hidden NAME=\"%s\" VALUE=\"%s\">\n",name,value);
}


/*----------------------------------------------------------------------------------------*/


/*
	html_end() -	This function will do nothing more than to just give the ending tags
					of the html page.
*/
void html_end(void)
{
  puts("</BODY>");
  puts("</HTML>");
}


/*----------------------------------------------------------------------------------------*/


/*
	location() -	This function will redirect a browser to another URL.
*/
void location(char *loc)
{
  printf("Location: %s\n\n",loc);
}


