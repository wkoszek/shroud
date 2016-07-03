#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>

#include "shroud.h"

struct mapping 
{
	char *old;
	char *new;
}
;

#define MAX_MAPPINGS	5000


struct mapping mappings [MAX_MAPPINGS];
int number_of_mappings;
int mappings_loaded;

void save_mapping (struct mapping *mappings, char *filename);
void load_mapping (struct mapping *mappings, char *filename);
int equal_mappings (struct mapping *x, struct mapping *y);
char *duplicate (char *string);

/* translate a name using the mappings */
char *translate (char *name)
{
	int previous_mappings;
	struct mapping temp_mapping;
	char temp [10];
	struct mapping *result;

	previous_mappings = number_of_mappings;
	temp_mapping . old = name;

	result = (struct mapping *) lsearch ((char *) &temp_mapping, (char *) mappings, &number_of_mappings, sizeof (struct mapping), equal_mappings);

	if (previous_mappings != number_of_mappings)
	{
		/* a new entry has been added */
		if (number_of_mappings >= MAX_MAPPINGS-1)
		{
			fprintf (stderr, "Can only map %d names\n", MAX_MAPPINGS);
			exit (1);
		}
		mappings [previous_mappings] . old = duplicate (name);
		if (is_system_include_file)
		{
			/* keep the same name */
			mappings [previous_mappings] . new = mappings [previous_mappings] . old;
		}
		else
		{
			/* create a new, unreadable name */
			sprintf (temp, "_%d", previous_mappings);
			mappings [previous_mappings] . new = duplicate (temp);
		}
		return mappings [previous_mappings] . new;
	}
	else
	{
		return result -> new;
	}
}

/* load up the mappings from the specified file */
void load_mapping (struct mapping *mappings, char *filename)
{
	FILE *f;

	char old [100];
	char new [100];

	number_of_mappings = 0;

	f = fopen (filename, "r");
	if (f != NULL)
	{
		while (fscanf (f, "%s %s", old, new) == 2 && number_of_mappings < MAX_MAPPINGS-1)
		{
			mappings [number_of_mappings] . old = duplicate (old);
			if (strcmp (old, new))
			{
				/* the mappings are different */
				mappings [number_of_mappings] . new = duplicate (new);
			}
			else
			{
				/* they are the same, save some space */
				mappings [number_of_mappings] . new = mappings [number_of_mappings] . old;
			}
			number_of_mappings++;
		}
		fclose (f);
	}
	fprintf (stderr, "%d mappings loaded.\n", number_of_mappings);

	/* save this for later use */
	mappings_loaded = number_of_mappings;
}

/* save the mappings to the specified file */
void save_mapping (struct mapping *mappings, char *filename)
{
	FILE *f;
	int i;

	/* only write out if new mappings were added */
	if (mappings_loaded != number_of_mappings)
	{
		f = fopen (filename, "w");
		if (f != NULL)
		{
			for (i = 0; i < number_of_mappings; i++)
			{
				fprintf  (f, "%s %s\n", mappings [i] . old, mappings [i] . new);
			}
			fprintf (stderr, "%d mappings saved.\n", number_of_mappings);
			fclose (f);
		}
		else
		{
			fprintf (stderr, "Can't save to %s", filename);
			exit (1);
		}
	}
}

/* compares two mappings based on their old names */
int equal_mappings (struct mapping *x, struct mapping *y)
{
	return strcmp (x -> old, y -> old);
}


void load_mappings (void)
{
	load_mapping (mappings, "shroud.map");
}

void save_mappings (void)
{
	save_mapping (mappings, "shroud.map");
}


/* duplicate a string and exit if there is no memory left */
char *duplicate (char *string)
{
	char *result;
	
	result = strdup (string);
	if (result == NULL)
	{
		fprintf (stderr, "out of memory.  Terminating.\n");
		exit (1);
	}
	return result;
}


