/*
 * include.c
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <lex.h>
#include "shroud.h"
#include "stream.h"


FILE *search_open(char *file, char *file_prefix);


/*
 * include file into lexin input stream
 * return true if lex is now reading from that file 
 */

int include(char *file)
{
	FILE *fd;

	/* open the included file */
	if ((fd = search_open(file, prefix)) == NULL) 
	{       
		fprintf (stderr, "Can't find the include file '%s'.\n", file);
		return FALSE;
	}
	else
	{
		if (!stack_stream (lexin, yyline, input_name, is_system_include_file)) 
		{
			fprintf (stderr, "include files are nested too deeply");
			exit (1);
		}
		lexin = fd;
		yyline = 1;
		strncpy (input_name, file, MAX_NAME);
		return TRUE;
	}
}

/*
 * search_open - search through the include path to find a file
 */
FILE *search_open(char *file, char *file_prefix)
{
	FILE *fd;
	char *path, *name;
	int i;
	char tmp[64];                /* buffer for the file and path name */

	if ((fd = fopen(file, "r")) != NULL) 
	{         /* can we open it w/o search */
		return fd;
	}
	else
	{
		for (path = file_prefix; *path; )    /* for each path */
		{
			/* skip any leading semi-colons */
			while (*path == ';' && *path) 
			{
				path++;
			}

			/* build the path name */
			for (i = 0; *path != ';' && *path; i++) 
			{
				tmp[i] = *path++;
			}

			/* terminate the path properly */
			if (tmp[i-1] != '\\' && tmp[i-1] != '/' && tmp[i-1] != ':') 
			{
				tmp[i++] = '\\';
			}

			/* now tack on the file name */
			for (name = file; *name; ) 
			{      
				tmp[i++] = *name++;
			}

			/* terminate the string */
			tmp[i] = '\0';

			/* try to open it */
			if ((fd = fopen(tmp, "r")) != NULL) 
			{
				return fd;
			}
		}
	}

	return NULL;                 /* file was not found, return NULL */
}

/*
 * parses a string for an imbedded filename of the form
 * "name" or <name>.
 * it returns the name with no " or <> or NULL if not found.
 */

char * get_file (char *string)
{
	char *head;                   /* the head of the file name */
	char *tail;                     /* the tail of the file name */

	if ((head = strchr (string, '<')) != NULL &&    /* find the first < */
	  (tail = strchr (++head, '>')) != NULL) 
	{    /* and the > after it */
		*tail = '\0';                  /* terminate the string at the > */
		return head;
	}

	if ((head = strchr (string, '\"')) != NULL &&   /* find the first " */
	  (tail = strchr (++head, '\"')) != NULL) 
	{   /* and the " after it */
		*tail = '\0';                  /* terminate the string at the " */
		return head;
	}

	return NULL;

}

/*
 * end of include.c
 */

