%{
    
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
    
#define YY_DECL int alpha_yylex (void* yylval)

struct alpha_token_t {
    int lineNo;
    int tokenNo;
    char* content;
    char* category;
    struct alpha_token_t *next;
};

struct alpha_token_t * head = NULL;
struct alpha_token_t * tail = NULL;

void insert_token(char * content, char * category) {
    struct alpha_token_t * tmp = malloc(sizeof (struct alpha_token_t));
    tmp->lineNo = yylineno;
    tmp->category = strdup(category);
    tmp->next = NULL;
    
    if (head == NULL) {
        tmp->tokenNo = 1;
        head = tmp;
        tail = head;
    } else {
        tail->next = tmp;
        tmp->tokenNo = tail->tokenNo + 1;
        tail = tmp;
    }
    
    if(strcmp(category, "ALPHANUMERIC") == 0){
        char * tmp2 = strdup(content);
        tmp->content = malloc(yyleng);
        int i = 0;
        while(*tmp2 != '\0'){
            if(*tmp2 == '\\'){
                tmp2++;
                switch(*tmp2){
                    case 'n':
                        *(tmp->content+i) = '\n';
                        break;
                    case 't':
                        *(tmp->content+i) = '\t';
                        break;
                    case '\\':
                        *(tmp->content+i) = '\\';
                        break;
                    case '\"':
                        *(tmp->content+i) = '\"';
                        break;
                    default:
			printf("Line:%d, Token:#%d. Invalid escape token: ( \\%c )!\n",yylineno ,tmp->tokenNo ,*tmp2);
                        //tmp2--;  
			i--;       
                }
            } else {
                *(tmp->content+i) = *tmp2;
            }
            
            i++;
            tmp2++;
        }
        *(tmp->content+i) = *tmp2;
    }else{
        tmp->content = strdup(content);
    }
}

void print_tokens() {
    struct alpha_token_t * current_token = head;
    while (current_token != NULL) {
        printf("%5d:\t#%d\t\t", current_token->lineNo, current_token->tokenNo);
        printf("\"%s\"\t\t\t%s\n", current_token->content, current_token->category);
        
        head = current_token;
        current_token = current_token->next;
        
        head->next = NULL;
        free(head->content);
        free(head->category);
        free(head);
    }
    head = NULL;
    tail = NULL;
}

%}

/*  KEYWORDS */
IF 		"if"
ELSE		"else"
WHILE 		"while"
FOR		"for"
FUNCTION	"function"
RETURN 		"return"
BREAK		"break"
CONTINUE	"continue"
AND		"and"
NOT		"not"
OR		"or"
LOCAL		"local"
TRUE		"true"	
FALSE		"false"
NIL		"nil"

/*  OPERATORS */
ASSIGN		"="
PLUS		"+"
MINUS		"-"
MUL		"*"
DIV		"/"
MOD		"%"
EQUAL		"=="
NOT_EQUAL	"!="
PLUS_PLUS	"++"
MINUS_MINUS	"--"
GREATER		">"
LESS		"<"
GREATER_EQUAL	">="
LESS_EQUAL	"<="

/* INTEGER CONSTANT */
INTEGER     [0-9]+

/* REAL CONSTANT */
REAL        {INTEGER}\.{INTEGER}

/* ALPHANUMERIC */
STRING      \"(\\.|[^\\"])*\"

/* PUNCTUATION CHARACTERS */
OPEN_CBRACE         \{
CLOSE_CBRACE        \}
OPEN_SBRACKET       \[
CLOSE_SBRACKET      \]
OPEN_PARENTHESIS    \(
CLOSE_PARENTHESIS   \)
SEMICOLON           \;
COMMA               \,
COLON               \:
COLON_COLON         \:\:
DOT                 \.
DOT_DOT             \.\.

/* IDENTIFIER */
ID                  [a-zA-Z][a-zA-Z_0-9]*

/* COMMENTS */
LINE_COMMENT        "//".*
COMMENT             "/*"

/* WHITE SPACES */
WHITE_SPACES        [\t\r \v\n]*

/* EVERYTHING ELSE */
OTHER               .

%option noyywrap
%option yylineno

%%


{IF} 			{insert_token("if", "KEYWORD");}
				
{ELSE}			{insert_token("else", "KEYWORD");}

{WHILE}			{insert_token("while", "KEYWORD");}

{FOR}			{insert_token("for", "KEYWORD");}

{FUNCTION}		{insert_token("function", "KEYWORD");}

{RETURN}		{insert_token("return", "KEYWORD");}

{BREAK}			{insert_token("break", "KEYWORD");}

{CONTINUE}		{insert_token("continue", "KEYWORD");}

{AND}			{insert_token("and", "KEYWORD");}

{NOT}			{insert_token("not", "KEYWORD");}

{OR}			{insert_token("or", "KEYWORD");}

{LOCAL}			{insert_token("local", "KEYWORD");}

{TRUE}			{insert_token("true", "KEYWORD");}

{FALSE}			{insert_token("false", "KEYWORD");}

{NIL}			{insert_token("nil", "KEYWORD");}

{ASSIGN}                  {insert_token("=", "OPERATOR");}
{PLUS}                    {insert_token("+", "OPERATOR");}
{MINUS}                   {insert_token("-", "OPERATOR");}
{MUL}                     {insert_token("*", "OPERATOR");}
{DIV}		        {insert_token("/", "OPERATOR");}
{MOD}		        {insert_token("%", "OPERATOR");}
{EQUAL}		        {insert_token("==", "OPERATOR");}
{NOT_EQUAL}	        {insert_token("!=", "OPERATOR");}
{PLUS_PLUS}	        {insert_token("++", "OPERATOR");}
{MINUS_MINUS}	        {insert_token("--", "OPERATOR");}
{GREATER}		        {insert_token(">", "OPERATOR");}
{LESS}		        {insert_token("<", "OPERATOR");}
{GREATER_EQUAL}	        {insert_token(">=", "OPERATOR");}
{LESS_EQUAL}	        {insert_token("<=", "OPERATOR");}

{INTEGER}                 {insert_token(yytext, "INTEGER CONSTANT");}

{REAL}                    {insert_token(yytext, "REAL CONSTANT");}

{STRING}                  {insert_token(yytext, "ALPHANUMERIC");}

{OPEN_CBRACE}             {insert_token("{", "PUNCTUATION CHARACTER");}
{CLOSE_CBRACE}            {insert_token("}", "PUNCTUATION CHARACTER");}
{OPEN_SBRACKET}           {insert_token("[", "PUNCTUATION CHARACTER");}
{CLOSE_SBRACKET}          {insert_token("]", "PUNCTUATION CHARACTER");}
{OPEN_PARENTHESIS}        {insert_token("(", "PUNCTUATION CHARACTER");}
{CLOSE_PARENTHESIS}       {insert_token(")", "PUNCTUATION CHARACTER");}
{SEMICOLON}               {insert_token(";", "PUNCTUATION CHARACTER");}
{COMMA}                   {insert_token(",", "PUNCTUATION CHARACTER");}
{COLON}                   {insert_token(":", "PUNCTUATION CHARACTER");}
{COLON_COLON}             {insert_token("::", "PUNCTUATION CHARACTER");}
{DOT}                     {insert_token(".", "PUNCTUATION CHARACTER");}
{DOT_DOT}                 {insert_token("..", "PUNCTUATION CHARACTER");}

{ID}                      {insert_token(yytext, "IDENTIFIER");}

{LINE_COMMENT}            {insert_token("//", "LINE COMMENT");}

{COMMENT}                 {
                            int cnt=1;
                            char c;
                            char * str = "COMMENT";
                            while(cnt!=0){
                                c = input();
                                if(c==EOF){
                                    break;
                                }
                                if(c == '/'){
                                    c=input();
                                    if(c== '*'){
                                        cnt++;
                                        str="NESTED COMMENT";
                                        continue;
                                    }
                                }
                                if(c == '*'){
                                    c=input();
                                    if(c=='/'){
                                        cnt--;
                                    }
                                }
                            }
                            if(cnt==0){
                                insert_token("CORRECT",str);
                            }else{
                                insert_token("WRONG",str);
                            }
                          }

{WHITE_SPACES}            {insert_token(" ", "WHITE SPACES");}

{OTHER}                   {
				if(*yytext == '"'){
					insert_token("OPENED STRING", "ALPHANUMERIC");
					char c=input();
					while(c!=EOF){
						c=input();
					}
				}else{
					insert_token(yytext, "OTHER (non-supported)");
				}
			    }

%%

int main(int argc, char *argv[])
{
  	FILE *fp;
	if(argc==1){
    		printf("Give an input from keyboard .. \n");
	    	yyin=stdin;
  	}
  	else if (argc!=2){
   		printf("Wrong parameters! \n");
    		exit(1);
  	}
  	else{
    		if(!(fp=fopen(argv[1],"r")))
    		{
      			printf("Cannot open source file !\n");
      			exit(1);
    		}
    	yyin=fp;
	}

	alpha_yylex(NULL);
	print_tokens();

}