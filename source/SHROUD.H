/* shroud.h:
	include file for shroud.c
*/


void do_new_comment(void);
void do_comment (void);
char *translate (char *name);
void print_current_token (void);

#define MAX_NAME	100	/* maximum length of file name */

#define TRUE	1
#define FALSE	0

extern int is_system_include_file;
extern char prefix [];
extern char input_name [];
extern FILE *output;
