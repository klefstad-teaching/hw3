%token Pass Break Continue Return Ident If Else Elif For While Def Arrow Class Or And Not In Is True False None Number String GE LE EQ NE
%token NOTUSED Except Lambda With As Finally Nonlocal Assert Yield From Global Raise Del Try Import
%token Indent Dedent Newline EndMarker
%token-table

%{
    /*
    UCI CS 142B Homework 2
    Name: Peter Anteater
    UCI NET ID: panteate
    ID Number: 12345678
    */

#include "all.h"
using namespace std;

#define token_image(tok)   yytname[YYTRANSLATE(tok)]
#define yyerror(s) cout << "PARSE ERROR r:" << row << " c:" << col << ": " << s << endl
#define scan_error(tok) cout << "SCAN ERROR r:" << row << " c:" << col << ": " << tok << endl
#define YYDEBUG 1

int yylex(void);


int row = 0;
int col = 0;
int HW = 0;

%}

/*
%union {
    Type ty;
    TypeList tyli;
    Expr ex;
    ExprList exli;
    Stmt st;
    StmtList stli;
    char *cstr;
    int in;
}

%type <ex> expr
%type <exli> exprs exprs_option
%type <st> stmt
%type <stli> stmts
%type <ty> type
%type <tyli> types
%type <cstr> Ident String strings
%type <in> Number

*/

%%

file: 
    stmts EndMarker { if ( HW == 3 ) cout << $1 << endl; }
|   EndMarker
;

stmts:
    stmt stmts
|   stmt
;

stmt:
    compound_stmt
|   simple_stmt
;

simple_stmt:
    small_stmt Newline
|   small_stmt ';' simple_stmt
;

small_stmt:
    decl_stmt
|   assign_stmt
|   call_stmt
|   Return expr
|   Pass
|   Break
|   Continue
;

compound_stmt:
    func_def
|   class_def
|   if_stmt
|   for_stmt
|   while_stmt
;

type:
    Ident
|   String
|   '[' type ']'
;

decl_stmt:
    Ident ':' type init_option
;

init_option:
    '=' expr
|   EMPTY
;

call_stmt:
    call_expr
;

if_stmt:
    If expr ':' block elif_stmts
;

elif_stmts:
    Elif expr ':' block elif_stmts
|   Else ':' block
|   EMPTY
;

while_stmt:
    While expr ':' block
;

for_stmt:
    For Ident In expr ':' block
;

func_def:
    Def Ident '(' params_option ')' ret_type_option ':' block
;

ret_type_option:
    Arrow type
|   EMPTY
;

params_option:
    params
|   EMPTY
;

params:
    param ',' params
|   param
;

param:
    Ident ':' type
|   Ident
;

class_def:
    Class Ident base_classes ':' block
; 

base_classes:
    '(' types ')'
|   EMPTY
;

block:
    Newline Indent stmts Dedent
|   simple_stmt
;

assign_stmt:
    assign_expr
;

assign_expr:
    name '=' expr
;

expr:
    disjunction
;

disjunction:
    disjunction Or conjunction
|   conjunction

conjunction:
    conjunction And inversion
|   inversion
;

inversion:
    Not inversion
|   relation
;

relation:
    relation EQ sum
|   relation NE sum
|   relation '<' sum
|   relation LE sum
|   relation '>' sum
|   relation GE sum
|   relation In sum
|   relation Not In sum
|   relation Is sum
|   relation Is Not sum
|   sum
;

sum:
    sum '+' term
|   sum '-' term
|   term
;

term:
    term '*' factor
|   term '/' factor
|   term '%' factor
|   factor
;

factor:
    '+' factor
|   '-' factor
|   primary
;


primary:
    name 
|   atom
;

name:
    Ident
|   name '.' Ident
|   name '[' expr ']'
|   call_expr
;

call_expr:
    name '(' exprs_option ')'
;

atom:
    True
|   False
|   None
|   Number
|   strings
|   '[' exprs_option ']'
|   '(' expr ')'
;

strings:
    String strings
|   String
;

exprs:
    expr ',' exprs
|   expr
;

exprs_option:
    exprs
|   EMPTY
;

types:
    type ',' types
|   type
;

EMPTY: ;

%%


#include "scan.cpp"


int scan1_main()
{
    while (int token = yylex())
    {
        if (token != Newline && token != Indent && token != Dedent && token != EndMarker)
        {
            cout << "Token = " << token_image(token) << "\t\t";
            cout << "Lexeme = \"" << yytext << "\"";
            cout << endl;
        }
    }
    return 0;
}

int scan2_main()
{
    while (int token = yylex())
    {
        cout << "Token = " << token_image(token) << "\t\t";
        if (token != Newline && token != Indent && token != Dedent && token != EndMarker)
            cout << "Lexeme = \"" << yytext << "\"";
        cout << endl;
    }
    return 0;
}

void parse_main()
{
    yyparse();
}

int main(int argc, char *argv[])
{
    int opt;
    while (true)
        switch ( opt = getopt(argc, argv, "0123456789") )
        {
            case '0':
                scan1_main();
                break;
            case '1':
                scan2_main();
                break;
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                HW = opt - '0';
                parse_main();
                break;
            case -1:
                exit(0);
            default:
                cerr << "Unknown program option: " << static_cast<char>(opt) << endl;
        }
    return 0;
}

