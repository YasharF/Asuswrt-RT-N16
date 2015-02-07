#ifndef _HTML_LIB_H_
#define _HTML_LIB_H
/***************************************************************************************
 *  Author: Noel V Aguilar
 *  Version: 1.4
 *  Filename: html-lib.h
 *  Program Library: CGI-LIB
 *
 *  This is the header file that must be used when referencing this C HTML
 *  library.  Below is a list of possible functions that can be used. Include
 *  this header file in your program in order to use these functions.  These
 *  function will only assist you in using html and not data manipulation.
 ***************************************************************************************
 *                                                                           
 * Feel free to use, copy, reference, or modify this program in any way.     
 * Support public domain freeware.                                           
 *                                                                           
 * For updates or to report bugs go to my home page:
 * http://www.geocities.com/SiliconValley/Vista/6493/
 * The web page for the library is:
 * http://www.geocities.com/SiliconValley/Vista/6493/cgi-lib.html
 ***************************************************************************************
 * Disclaimer:                                                                         *
 * THE INFORMATION, CODE AND EXECUTABLE PROVIDED ARE PROVIDED "AS IS" WITHOUT          *
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED          *
 * TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR           *
 * PURPOSE.  IN NO EVENT SHALL NOEL V AGUILAR BE LIABLE FOR ANY DAMAGES                *
 * WHATSOEVER INCLUDING DIRECT, INDIRECT, INCIDENTAL, CONSEQUENTIAL, LOSS OF           *
 * BUSINESS PROFITS OR SPECIAL DAMAGES.  ALL THE INFORMATION, CODE AND                 *
 * EXECUTABLES ARE FOR EDUCATIONAL PURPOSES ONLY.                                      *
 ***************************************************************************************/

#include <stdio.h>

/*
	mime_header() -	This function will take any text passed to it and it will diplay it as
					the mime type.
*/
void mime_header(char *mime);

/*
	html_begin() -	This function will diplay the top of the head of the html page all the
					way upto the body tag.  The title and body attributes may be passed, if
					for any of these fields are not desired, then pass NULL.
*/
void html_begin(char *title, char *attributes);

/*
	h1() -	This function will diplay whatever text is given between the tags.
*/
void h1(char *str);

/*
	h2() -	This function will diplay whatever text is given between the tags.
*/
void h2(char *str);

/*
	h3() -	This function will diplay whatever text is given between the tags.
*/
void h3(char *str);

/*
	h4() -	This function will diplay whatever text is given between the tags.
*/
void h4(char *str);

/*
	h5() -	This function will diplay whatever text is given between the tags.
*/
void h5(char *str);

/*
	h6() -	This function will diplay whatever text is given between the tags.
*/
void h6(char *str);

/*
	hidden() -	This function will print out hidden fields to the html page.  The Name and
				value of the hidden field must be passed.
*/
void hidden(char *name, char *value);

/*
	html_end() -	This function will do nothing more than to just give the ending tags
					of the html page.
*/
void html_end(void);

/*
	location() -	This function will redirect a browser to another URL.
*/
void location(char *loc);
#endif
