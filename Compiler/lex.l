%{
/* ----------------------------------------- *
 *     Anastasas Anastasios  AM:3166         *
 *	   Giakoumis Gewrgios    AM:3157         *
 * ----------------------------------------- */
#include "syn.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int line = 1;

char * prepare_string() {
        char * tmp = malloc(yyleng+1);
        char * tmp2 = strdup(yytext);
        assert(yyleng == strlen(yytext));
        int i = 0;
        while(*tmp2 != '\0'){
            if(*tmp2 == '\\'){
                tmp2++;
                switch(*tmp2){
                    case 'n':
                        *(tmp + i) = '\n';
                        break;
                    case 't':
                        *(tmp + i) = '\t';
                        break;
                    case '\\':
                        *(tmp + i) = '\\';
                        break;
                    case '\"':
                        *(tmp + i) = '\"';
                        break;
                    default:
                        printf("Line:%d Warning: Invalid escape character: '\\%c' \n" ,line ,*tmp2);
                        *(tmp + i) = *tmp2;
                        //i--;
                }
            } else {
                if(*tmp2 != '\n'){
                    *(tmp + i) = *tmp2;
                }else{
                    line++;
                    i--;
                }
            }
            i++;
            tmp2++;
        }
        *(tmp + i) = '\0';

        return tmp;
}
%}

/*  keywords */
if 		"if"
else		"else"
while 		"while"
for		"for"
function	"function"
return 		"return"
break		"break"
continue	"continue"
and		"and"|"&&"
not		"not"
or		"or"|"||"
local		"local"
true		"true"
false		"false"
nil		"nil"

/*  operators */
assign		"="
plus		"+"
minus		"-"
mul		"*"
div		"/"
mod		"%"
equal		"=="
not_equal	"!="
plus_plus	"++"
minus_minus	"--"
greater		">"
less		"<"
greater_equal	">="
less_equal	"<="

/* integer constant */
integer     [0-9]+

/* real constant */
real        {integer}\.{integer}

/* alphanumeric */
string      \"(\\.|[^\\"])*\"

/* punctuation characters */
open_cbrace         \{
close_cbrace        \}
open_sbracket       \[
close_sbracket      \]
open_parenthesis    \(
close_parenthesis   \)
semicolon           \;
comma               \,
colon               \:
colon_colon         \:\:
dot                 \.
dot_dot             \.\.

/* identifier */
id                  [a-zA-Z][a-zA-Z_0-9]*

/* comments */
line_comment        "//".*
comment             "/*"

/* new line */
new_line            \n

/* white spaces */
white_spaces        [\t\r \v]*

/* everything else */
other               .

%option noyywrap
%option yylineno

%%
{if} 			        {return IF;}

{else}			        {return ELSE;}

{while}			        {return WHILE;}

{for}			        {return FOR;}

{function}		        {return FUNCTION;}

{return}		        {return RETURN;}

{break}			        {return BREAK;}

{continue}		        {return CONTINUE;}

{and}			        {return AND;}

{not}			        {return NOT;}

{or}			        {return OR;}

{local}			        {return LOCAL;}

{true}			        {return TRUE;}

{false}			        {return FALSE;}

{nil}	                {return NIL;}

{assign}                {return ASSIGN;}
{plus}                  {return PLUS;}
{minus}                 {return MINUS;}
{mul}                   {return MUL;}
{div}		            {return DIV;}
{mod}		            {return MOD;}
{equal}		            {return EQUAL;}
{not_equal}	            {return NOT_EQUAL;}
{plus_plus}	            {return PLUS_PLUS;}
{minus_minus}	        {return MINUS_MINUS;}
{greater}		        {return GREATER;}
{less}		            {return LESS;}
{greater_equal}	        {return GREATER_EQUAL;}
{less_equal}	        {return LESS_EQUAL;}

{integer}               {yylval.intValue = atoi(yytext);
                         return INTEGER;}

{real}                  {yylval.realValue = atof(yytext);
                         return REAL;}

{string}                {yylval.stringValue = prepare_string();
                         return STRING;}

{open_cbrace}           {return OPEN_CBRACE;}
{close_cbrace}          {return CLOSE_CBRACE;}
{open_sbracket}         {return OPEN_SBRACKET;}
{close_sbracket}        {return CLOSE_SBRACKET;}
{open_parenthesis}      {return OPAR;}
{close_parenthesis}     {return CPAR;}
{semicolon}             {return SEMICOLON;}
{comma}                 {return COMMA;}
{colon}                 {return COLON;}
{colon_colon}           {return COLON_COLON;}
{dot}                   {return DOT;}
{dot_dot}               {return DOT_DOT;}

{id}                    {yylval.stringValue = strdup(yytext);
                         return ID;}

{line_comment}          {}

{comment}               {
                                int commentLines = 0;
                                int cnt = 1;
                                char c;
                                char * str = "COMMENT";
                                while(cnt != 0){
                                    c = input();
                                    if(c == '\n'){
                                        commentLines++;
                                    }
                                    if(c == EOF){
                                        break;
                                    }
                                    if(c == '/'){
                                        c = input();
                                        if(c == '*'){
                                            cnt++;
                                            str = "NESTED COMMENT";
                                            continue;
                                        }
                                        unput(c);
                                    }
                                    if(c == '*'){
                                        c = input();
                                        if(c == '/'){
                                            cnt--;
                                        }else{
                                            unput(c);
                                        }
                                    }
                                }
                                if(cnt == 0){// ta comments tha agnoountai
                                    //insert_token("CORRECT",str);
                                }else{
                                    printf("Error: Not Closing %s, Opened at Line: %d\n", str, line);
                                    //insert_token("WRONG",str);
                                }
                                line += commentLines;
                        }

{new_line}              {//printf("\n");
                         line++;}

{white_spaces}          {}

{other}                 {
                            if(*yytext == '"'){
                                printf("Error: Not Closing String, Opened at Line: %d\n", line);
                                //insert_token("OPENED STRING", "ALPHANUMERIC");
                                char c=input();
                                while(c!=EOF){
                                    c=input();
                                }
                            }else{
                                printf("Error: Undefined character: '%c' , Line: %d\n", *yytext, line);
                                //insert_token(yytext, "OTHER (non-supported)");
                            }
                        }
