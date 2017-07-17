/*
 * stream.c
 */

#include <stdio.h>
#include <string.h>
#include "shroud.h"
#include "stream.h"

#define MAX_STREAMS 10    /* total amount of streams in a stack */

struct slot 
{
	FILE *stream;
	int line;
	int is_system;
	char name [MAX_NAME];
}
;

struct stream_stack 
{
	struct slot stack [MAX_STREAMS];
	int stack_pointer;
}
;

static struct stream_stack streams;


/* sets the stream stack to be empty */

void init_streams ()
{
	streams.stack_pointer = 0;
}



/*
 * saves the old stream s, and returns it.
 * it returns NULL if too many streams are open already.
 */

FILE *stack_stream (FILE *s, int line, char *name, int is_system)
{
	/* check for stack overflow */
	if (streams.stack_pointer >= MAX_STREAMS) 
	{
		return NULL;
	}

	/* save old stream */
	streams.stack [streams.stack_pointer].stream  = s;
	streams.stack [streams.stack_pointer].line = line;
	streams.stack [streams.stack_pointer].is_system = is_system;
	strncpy (streams.stack [streams.stack_pointer].name, name, MAX_NAME);
	streams.stack_pointer++;

	return s;

}





/* restores the next stream from the stack.  returns NULL if there
   are no streams on the stack.
 */

FILE *unstack_stream (int *line, char *name, int *is_system)
{

	/* check for stack underflow */
	if (-- streams.stack_pointer < 0 ) 
	{
		return NULL;
	}

	*line = streams.stack [streams.stack_pointer].line;
	*is_system = streams.stack [streams.stack_pointer].is_system;
	strncpy (name, streams.stack [streams.stack_pointer].name, MAX_NAME);

	/* return old stream */
	return streams.stack [streams.stack_pointer].stream;

}
/*
 * end of stream.c
 */

