%{
    #include <stdlib.h>
    #include "symbolTable.h"
    #include "manage.h"
    #include "quad.h"
    #include "expr.h"


    int yyerror (char* yaccProvidedMessage);
    int yylex (void);

    extern int line;
    extern char* yytext;

%}
%union {
    int intValue;
    double realValue;
    const char * stringValue;
    struct expr * exprValue;
    struct SymbolTableEntry * symValue;

    struct call_info * call_info;
    struct for_info* for_info;
    struct stmt_list_info* stmt_info;
}

%expect 1

%start program

%token IF
%token ELSE
%token WHILE FOR FUNCTION RETURN BREAK CONTINUE AND NOT OR LOCAL TRUE FALSE NIL
%token ASSIGN PLUS MINUS MUL DIV MOD EQUAL NOT_EQUAL PLUS_PLUS MINUS_MINUS GREATER LESS GREATER_EQUAL LESS_EQUAL
%token OPEN_CBRACE CLOSE_CBRACE
%token OPEN_SBRACKET CLOSE_SBRACKET
%token OPAR CPAR
%token SEMICOLON COMMA COLON COLON_COLON DOT DOT_DOT

%token <intValue> INTEGER
%token <realValue> REAL
%token <stringValue> STRING
%token <stringValue> ID

%type <stmt_info> stmts
%type <stmt_info> stmt
%type <stmt_info> loopstmt
%type <stmt_info> endblock
%type <stmt_info> block

%type <exprValue> expr
%type <intValue> M

%type <exprValue> term
%type <exprValue> assignexpr
%type <exprValue> primary
%type <exprValue> lvalue
%type <exprValue> const
%type <exprValue> tableitem
%type <exprValue> member

%type <exprValue> call
%type <call_info> methodcall
%type <call_info> normcall
%type <call_info> callsuffix

%type <exprValue> exp
%type <exprValue> elist
%type <exprValue> elists

%type <exprValue> indexed
%type <exprValue> indexedelem
%type <exprValue> indexedelems


%type <exprValue> objectdef
%type <exprValue> elem

%type <exprValue> assignlvalue

%type <symValue>    funcdef
%type <symValue>    funcprefix
%type <stringValue> funcname
%type <intValue>    funcbody

%type <intValue> openfuncblock

%type <intValue> ifprefix
%type <intValue> elseprefix

%type <stmt_info> ifstmt

%type <intValue> whilestart
%type <intValue> whilecond

%type <intValue> forN
%type <intValue> forM
%type <for_info>  forprefix

%right    ASSIGN
%left     OR
%left     AND
%nonassoc EQUAL NOT_EQUAL
%nonassoc GREATER LESS GREATER_EQUAL LESS_EQUAL
%left     PLUS MINUS
%left     MUL DIV MOD
%right    NOT PLUS_PLUS MINUS_MINUS UMINUS
%left     DOT DOT_DOT
%left     OPEN_SBRACKET CLOSE_SBRACKET
%left     OPAR CPAR


%%

program:    stmts
            ;

stmts:       stmts stmt                                         {$$ = manage_stmts($1,$2);}
            |                                                   {$$ = create_stmt_list_info();}
            ;


stmt:        expr SEMICOLON                                     {$$ = manage_stmt_expr($1);}
            |ifstmt                                             {$$ = manage_stmt_ifstmt($1);}
            |whilestmt                                          {$$ = create_stmt_list_info();}
            |forstmt                                            {$$ = create_stmt_list_info();}
            |returnstmt                                         {$$ = create_stmt_list_info();}
            |BREAK SEMICOLON                                    {$$ = manage_break();}
            |CONTINUE SEMICOLON                                 {$$ = manage_continue();}
            |block                                              {$$ = manage_stmt_block($1);}
            |funcdef                                            {$$ = create_stmt_list_info();}
            |SEMICOLON                                          {$$ = create_stmt_list_info();}
            ;

expr:        assignexpr                                         {$$ = manage_expr_assignexpr($1);}
            |expr PLUS expr                                     {$$ = manage_exprPLUSexpr($1,$3);}
            |expr MINUS expr                                    {$$ = manage_exprMINUSexpr($1,$3);}
            |expr MUL expr                                      {$$ = manage_exprMULexpr($1,$3);}
            |expr DIV expr                                      {$$ = manage_exprDIVexpr($1,$3);}
            |expr MOD expr                                      {$$ = manage_exprMODexpr($1,$3);}
            |expr GREATER expr                                  {$$ = manage_exprGREATERexpr($1,$3);}
            |expr GREATER_EQUAL expr                            {$$ = manage_exprGREATER_EQUALexpr($1,$3);}
            |expr LESS expr                                     {$$ = manage_exprLESSexpr($1,$3);}
            |expr LESS_EQUAL expr                               {$$ = manage_exprLESS_EQUALexpr($1,$3);}
            |expr EQUAL expr                                    {$$ = manage_exprEQUALexpr($1,$3);}
            |expr NOT_EQUAL expr                                {$$ = manage_exprNOT_EQUALexpr($1,$3);}

            |expr AND                                           {$1 = manage_andprefix($1);}
             M expr                                             {$$ = manage_exprANDexpr($1,$4,$5);}

            |expr OR                                            {$1 = manage_orprefix($1);}
             M expr                                             {$$ = manage_exprORexpr($1,$4,$5);}

            |term                                               {$$ = manage_expr_term($1);}
            ;

M:                                                              {$$ = manage_M();}
            ;

term:        OPAR expr CPAR                                     {$$ = manage_term_expr($2);}
            |MINUS expr %prec UMINUS                            {$$ = manage_term_uminus($2);}
            |NOT expr                                           {$$ = manage_term_NOT($2);}
            |PLUS_PLUS lvalue		                            {$$ = manage_term_PP_lvalue($2);}
            |lvalue PLUS_PLUS	                                {$$ = manage_term_lvalue_PP($1);}
            |MINUS_MINUS lvalue		                            {$$ = manage_term_MM_lvalue($2);}
            |lvalue MINUS_MINUS	                                {$$ = manage_term_lvalue_MM($1);}
            |primary                                            {$$ = manage_term_primary($1);}
            ;


assignexpr:  assignlvalue ASSIGN expr                           {$$ = manage_assignexpr($1,$3);}
             ;

assignlvalue:   lvalue                                          {$$ = manage_assignlvalue($1);}
                ;

primary:    lvalue                                              {$$ = emit_iftableitem($1);}
            |call                                               {$$ = manage_primary_call($1);}
            |objectdef                                          {$$ = manage_primary_object($1);}
            |OPAR funcdef CPAR                                  {$$ = manage_primary_funcdef($2);}
            |const                                              {$$ = manage_primary_const($1);}
            ;


lvalue:      ID						        {$$ = manage_lvalue_ID($1);}
            |LOCAL ID		                                {$$ = manage_lvalue_LOCAL_ID($2);}
            |COLON_COLON ID		                        {$$ = manage_lvalue_CCOLON_ID($2);}
            |member                                             {$$ = manage_lvalue_member($1);}
            ;

member:      tableitem                                          {$$ = manage_member_tableitem($1);}
            |call DOT ID                                        {$$ = manage_tableitem_lvalue_dot_id($1,$3);}
            |call OPEN_SBRACKET expr CLOSE_SBRACKET             {$$ = manage_tableitem_lvalue_expr($1,$3);}
            ;

tableitem:   lvalue DOT ID                                      {$$ = manage_tableitem_lvalue_dot_id($1,$3);}
            |lvalue OPEN_SBRACKET expr CLOSE_SBRACKET           {$$ = manage_tableitem_lvalue_expr($1,$3);}
            ;

call:        call OPAR elist CPAR                               {$$ = make_call($1,$3);}
            |lvalue callsuffix                                  {$$ = manage_call_lvalue_callsuffix($1,$2);}
            |OPAR funcdef CPAR OPAR elist CPAR                  {$$ = manage_call_funcdef($2,$5);}
            ;

callsuffix:  normcall                                           {$$ = manage_callsufix_normcall($1);}
            |methodcall                                         {$$ = manage_callsufix_methodcall($1);}
            ;

normcall:   OPAR elist CPAR                                     {$$ = manage_normcall($2);}
            ;

methodcall: DOT_DOT ID OPAR elist CPAR                          {$$ = manage_methodcall($2,$4);}
            ;

elist:       exp elists                                         {$$ = manage_elist_expr($1,$2);}
            |                                                   {$$ = manage_elist_empty();}
            ;

elists:      COMMA exp elists                                   {$$ = manage_elist_expr($2,$3);}
            |                                                   {$$ = manage_elist_empty();}
            ;

exp:       expr                                                 {$$ = manage_exp($1);}
            ;

objectdef:  OPEN_SBRACKET elem CLOSE_SBRACKET                   {$$ = manage_object_elem($2);}
            ;

elem:        elist                                              {$$ = manage_elem_elist($1);}
            |indexed                                            {$$ = manage_elem_indexed($1);}
            ;

indexed:      indexedelem indexedelems                          {$$ = (expr_p)manage_indexed($1,$2);}
              ;

indexedelems: COMMA indexedelem indexedelems                    {$$ = (expr_p)manage_indexed($2,$3);}
              |                                                 {$$ = manage_indexedelems_empty();}
              ;

indexedelem:  OPEN_CBRACE exp COLON exp CLOSE_CBRACE            {$$ = manage_indexedelem($2,$4);}
              ;


block:        openblock endblock                                {$$ = manage_block($2);}
              ;

openblock:      OPEN_CBRACE                                     {manage_openblock();}

endblock:       stmts CLOSE_CBRACE                              {$$ = manage_endblock($1);}

funcdef:        funcprefix funcargs funcbody                    {$$ = manage_funcdef($1,$3);}
                ;

funcprefix:     FUNCTION funcname                               {$$ = manage_funcprefix($2);}
                ;

funcargs:       OPAR idlist CPAR                                {manage_funcargs();}
                ;

funcbody:       openfuncblock endblock                          {$$ = manage_funcbody($1);}
                ;

openfuncblock:  OPEN_CBRACE                                     {$$ = manage_openfuncblock();}
                ;

funcname:   ID								                    {$$ = manage_funcname_ID($1);}
            |											        {$$ = manage_funcname_empty();}
            ;

const:       INTEGER                                            {$$ = manage_const_INTEGER($1);}
            |REAL                                               {$$ = manage_const_REAL($1);}
            |STRING                                             {$$ = manage_const_STRING($1);}
            |NIL                                                {$$ = manage_const_NIL();}
            |TRUE                                               {$$ = manage_const_TRUE();}
            |FALSE                                              {$$ = manage_const_FALSE();}
            ;

idlist:     formalid idlists
            |
            ;

formalid:      ID                                               {manage_formalid($1);}
            ;

idlists:    COMMA formalid idlists
            |
            ;


ifprefix:    IF OPAR expr CPAR                                  {$$ = manage_ifprefix($3);}
            ;

ifstmt:      ifprefix stmt                                      {$$ = manage_ifstmt($1,$2);}
            |ifprefix stmt elseprefix stmt                      {$$ = manage_ifelsestmt($1,$2,$3,$4);}
            ;

elseprefix: ELSE                                                {$$ = manage_elseprefix();}
            ;

loopstart:                                                      {manage_loopstart();}
            ;

loopend:                                                        {manage_loopend();}
            ;

loopstmt:   loopstart stmt loopend                              {$$ = manage_loopstmt($2);}
            ;


whilestart: WHILE                                               {$$ = manage_whilestart();}
            ;

whilecond:  OPAR expr CPAR                                      {$$ = manage_whilecond($2);}
            ;

whilestmt:  whilestart whilecond loopstmt                       {manage_whilestmt($1,$2,$3);}
            ;

forN:                                                           {$$ = manage_forN();}
            ;

forM:                                                           {$$ = manage_forM();}
            ;

forprefix:  FOR OPAR elist SEMICOLON forM expr SEMICOLON        {$$ = manage_forprefix($5,$6);}
            ;

forstmt:    forprefix forN elist CPAR forN loopstmt forN        {manage_forstmt($1,$2,$5,$6,$7);}
            ;

returnstmt: RETURN retvalue SEMICOLON
            ;

retvalue:   expr                                                {manage_retval_expr($1);}
            |                                                   {manage_retval_empty();}
            ;
%%

int yyerror (char * yaccProvideMessage){
    fprintf(stderr, "\n%s: at line %d, before token: %s\n",
        yaccProvideMessage, line, yytext);
    fprintf(stderr, "INPUT NOT VALID\n");
    exit(1);
}
