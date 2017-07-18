
/*
 * C lex V0.0  identifiers, strings and preprocessor stuff
 * discards comments.
 */

%{
#include "shroud.h"
#include "include.h"
#define BUF_MAX 1000
%}

legal	= [\040-\176\n\r\t\f\v];
letter	= [a-zA-Z_];

digit	= [0-9];
hex_digit = [0-9a-fA-F];
exp_mark = [eE];
hex_mark = [xX];
long_suffix = [lLuU];
float_suffix = [lLfF];
decimal  = [.];
sign = [-+];

blank	= [\n\r\t\b\f\v ];

name	= letter (letter|digit)*;

digits   = digit digit*;
integer  = digits long_suffix*;
hex      = 0 hex_mark hex_digit hex_digit* long_suffix*;
exponent = (exp_mark digits) | (exp_mark sign digits);
real1    = (digits exponent) | (digits exponent float_suffix);
real2    = (decimal digits) | (digits decimal) | (digits decimal digits);
real2a   = real2 (exponent | float_suffix | exponent float_suffix);
number   = integer | hex | real1 | real2 | real2a;

any	= [\000-\377];		/* any character found in a string after a '\' */
not_string_end = [^\\"\n];	/* anything not at the end of a string */
not_char_end   = [^\\'\n];	/* anything not at the end of a character */
string_char    = ("\\" any) | not_string_end;
char_char      = ("\\" any) | not_char_end;
string         = '"' string_char* '"';
character      = "'" char_char*	"'";

any_include = [^>];		/* any character inside #include <> */


%%

%{
	char buffer [BUF_MAX];
	char *translation;
%}

# #include <> preprocessor line
blank* INCLUDE blank* "<" any_include* ">"		/* treat <> like "" */
		{
			lexecho (output);
			gettoken (buffer, BUF_MAX);
			if (include (get_file (buffer)))
			{
				/* we are now reading a system include file */
				is_system_include_file = TRUE;
			}
		}

# all other preprocessor lines
blank* name	{
			print_current_token ();
		}

number		{
			print_current_token ();
		}

name		{
			gettoken (buffer, BUF_MAX);
			translation = translate (buffer);
			/* only print out non-system file tokens */
			if (!is_system_include_file)
			{
				fprintf (output, "%s", translation);
			}
		}

"/*"		{
			do_comment ();
		}

"//"		{
			do_new_comment ();
		}

character	{
			print_current_token ();
		}

string		{
			print_current_token ();
		}

# the default is any legal character not mentioned above

legal		{
			print_current_token ();
		}

%%

/*
 * end of clex.lxi
 */

