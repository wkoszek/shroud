/*
 * stream.h
 *      include file for stream.c
 */

void init_streams (void);
FILE *stack_stream (FILE *s, int line, char *name, int is_system);
FILE *unstack_stream (int *line, char *name, int *is_system);

/*
 * end of stream.h
 */

