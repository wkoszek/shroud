#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lex.h>
#include <getopt.h>

#include "shroud.h"
#include "stream.h"
#include "mapping.h"

#define VERSION		"Shroud V1.0"

void usage (void);
void error (char *message);
void first_stream (char *file);
char *fixup (char *string);


int is_system_include_file;
char prefix [MAX_NAME];		/* storage for prefix path names */
char input_name [MAX_NAME];	/* storage for the name of the file being read */
char output_name [MAX_NAME];	/* storage for the name of the file being written to */
FILE *output;

void main (int argcount, char *argvalue [])
{
	int c;
	int option;
	char *include_path;
	char *output_directory;

	if (argcount > 2)
	{
		while ((option = getopt (argcount, argvalue, "e:E:h:H:")) != EOF) 
		{
			switch (tolower (option))
			{
			case 'h':
				strncat (prefix,optarg, MAX_NAME);
				strncat (prefix,";", MAX_NAME);
				break;
			case 'e':
				if ((include_path = getenv (optarg)) != NULL) 
				{
					strncat (prefix,include_path, MAX_NAME);
					strncat (prefix,";", MAX_NAME);
					free (include_path);
				}
				else
				{
					fprintf (stderr, "-e %s is not available.\n", optarg);
				}
				break;
			default:
				usage ();
			}
		}
	}
	else
	{
		usage ();
	}

	load_mappings ();
	output_directory = fixup (argvalue [optind]);
	optind ++;
	while (optind < argcount)
	{
		is_system_include_file = FALSE;
		init_streams ();
		first_stream (argvalue [optind]);

		/* get the output name */
		strncpy (output_name, output_directory, MAX_NAME);
		strncat (output_name, argvalue [optind], MAX_NAME);
		optind ++;

		output = fopen (output_name, "w");
		if (output == NULL)
		{
			fprintf (stderr, "Can't write to %s\n", output_name);
			exit (1);
		}
		while ((c = yylex ()) != 0)
		{
			if (c == LEXERR)
			{
				error ("Syntax error.");
				exit (1);
			}
		}
		fclose (output);
	}
	save_mappings ();
	exit (0);
}




/*
 * process an old style comment 
 */

void do_comment (void)
{
	int lno;
	int c;

	lno = yyline;           /* save the line number */


	c = lexchar ();
	while (c != EOF) 
	{
		while (c == '*') 
		{
			c=lexchar ();
			if (c == '/') 
			{
				return;
			}
		}
		c = lexchar();
	}
	yyline = lno;
	error("This comment goes to the end of file\n");
}

/*
 *  process a comment that ends with "\n"
 */

void do_new_comment(void)
{
	int c;

	c = lexchar ();
	while (c != EOF && c != '\n')
	{
		c = lexchar ();
	}
}

/*
 * get a character for lex.
 * This one counts lines and unstacks the streams as required.
 */

int lexgetc (void)
{
	int ch;

	do 
	{
		/*
		 * test for the end of the original file
		 */
		if (lexin == NULL) 
		{
			yyline = 0;               /* set line number to something reasonable */
			return EOF;
		}

		if ( (ch = getc(lexin)) == '\n') 
		{
			yyline ++;
		}

		if (ch == EOF) 
		{
			/* close the file */
			fclose (lexin);

			/* unstack the next one */
			lexin = unstack_stream(&yyline, input_name, &is_system_include_file);
		}
	}
	while (ch == EOF)
		;

	return ch;
}


/* print the current token if we are in a non-system file */
void print_current_token (void)
{
	if (!is_system_include_file)
	{
		lexecho (output);
	}
}

/* give the usage information and quit */
void usage (void)
{
	fprintf (stderr, "%s\n", VERSION);
	fprintf (stderr, "usage: shroud [-e ENVIRONMENT_VARIABLE] [-h PATH] /output_directory files\n");
	exit (1);

}

/* give an error message and quit */
void error (char *message)
{
	fprintf (stderr, "%s(%d) : %s.  Terminating.\n", input_name, yyline, message);
	exit (1);
}

/* set up the first stream */
void first_stream (char *file)
{
	FILE *fd;

	fd = fopen (file, "r");
	if (fd != NULL)
	{
		lexin = fd;
		yyline = 1;
		strncpy (input_name, file, MAX_NAME);
		/* reset the lex variables */
		llinit ();
	}
	else
	{
		fprintf (stderr, "Can't open %s\n", file);
		exit (1);
	}
}


/* copy the string and make sure there is a '/' on the end of it */
char *fixup (char *string)
{
	int length;
	char *result;
	
	length = strlen (string);
	
	switch (string [length])
	{
	case '/':
#ifdef MSDOS
	case '\\':
#endif
		result = strdup (string);
		break;
		
	default:
		result = malloc (length + 2);
		if (result != NULL)
		{
			strcpy (result, string);
			strcat (result, "/");
		}
		break;
	}
	
	if (result == NULL)
	{
		fprintf (stderr, "Severe memory shortage.\n");
		exit (1);
	}
	return result;
}


