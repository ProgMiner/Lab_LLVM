%parse-param {struct ast_program ** result} {char ** error}

%{
#include <stdio.h>

#include "ast.h"

int yylex();

void yyerror(struct ast_program ** result, char ** error, const char * str);
%}

%code requires {
#include "ast.h"
}

%locations

%token T_INT T_ID
%token TL_FUNCTION "function"
%token TL_INT "int"
%token TL_VAR "var"
%token TL_IF "if"
%token TL_ELSE "else"
%token TL_WHILE "while"
%token TL_DO "do"
%token TL_FOR "for"
%token TL_RETURN "return"
%token TL_BREAK "break"
%token TL_CONTINUE "continue"
%token TL_PLUS_ASN "+="
%token TL_MINUS_ASN "-="
%token TL_MUL_ASN "*="
%token TL_DIV_ASN "/="
%token TL_REM_ASN "%="
%token TL_BAND_ASN "&="
%token TL_BXOR_ASN "^="
%token TL_BOR_ASN "|="
%token TL_SHL_ASN "<<="
%token TL_SHR_ASN ">>="
%token TL_AND_ASN "&&="
%token TL_OR_ASN "||="
%token TL_SHL "<<"
%token TL_SHR ">>"
%token TL_AND "&&"
%token TL_OR "||"
%token TL_EQ "=="
%token TL_NE "!="
%token TL_LE "<="
%token TL_GE ">="
%token TL_INC "++"
%token TL_DEC "--"

%precedence P_IF
%left "else"
%precedence P_RETURN
%nonassoc '=' "+=" "-=" "*=" "/=" "%=" "&=" "^=" "|=" "<<=" ">>=" "&&=" "||="
%left "||"
%left "&&"
%nonassoc "==" "!=" '<' "<=" '>' ">="
%left "<<" ">>"
%left '|'
%left '^'
%left '&'
%left '+' '-'
%left '*' '/' '%'
%precedence P_UNARY
%precedence '[' '('

%union {
    struct ast_program * program;
    struct ast_decl * decl;
    struct ast_arg_list * arg_list;
    struct ast_arg_c arg;
    struct ast_type * type;
    struct ast_expr * expr;
    struct ast_expr_list * expr_list;
    int32_t int_;
    char * id;
}

%type<program> declarations
%type<decl> declaration signature
%type<arg_list> arg_list arg_list1
%type<arg> arg
%type<type> type_colon_opt type_colon type
%type<expr> stmts stmt stmt_block stmt_var stmt_for_init expr lvalue_expr expr_binary expr_unary
%type<expr_list> expr_list expr_list1
%type<int_> T_INT
%type<id> T_ID

%%

file
    : declarations YYEOF    { *result = $1; }
    ;

declarations
    : /* empty */               { $$ = ast_program_new(); }
    | declarations declaration  { $$ = $1; ast_program_add_decl($1, $2); }
    ;

declaration
    : signature ';'         { $$ = $1; }
    | signature stmt_block  { $$ = $1; ast_decl_set_body($1, $2); }
    ;

signature
    : "function" T_ID '(' arg_list ')' type_colon_opt { $$ = ast_decl_new($2, $4, $6); }
    ;

arg_list
    : /* empty */   { $$ = ast_arg_list_new(); }
    | arg_list1
    ;

arg_list1
    : arg               { $$ = ast_arg_list_new(); ast_arg_list_add($$, $1); }
    | arg_list1 ',' arg { $$ = $1; ast_arg_list_add($$, $3); }
    ;

arg
    : T_ID type_colon { $$ = (struct ast_arg_c) { .id = $1, .type = $2 }; }
    ;

type_colon_opt
    : /* empty */ { $$ = NULL; }
    | type_colon
    ;

type_colon
    : ':' type { $$ = $2; }
    ;

type
    : "int"         { $$ = ast_type_new_int(); }
    | '[' type ']'  { $$ = ast_type_new_array($2); }
    ;

stmts
    : /* empty */   { $$ = NULL; }
    /* not efficient at all, but much simpler: */
    | stmt stmts    { $$ = ast_expr_new_seq($1, $2); }
    ;

stmt
    : expr ';' { $$ = $1; }
    | stmt_var
    | stmt_block
    | "if" '(' expr ')' stmt %prec P_IF     { $$ = ast_expr_new_if($3, $5, NULL); }
    | "if" '(' expr ')' stmt "else" stmt    { $$ = ast_expr_new_if($3, $5, $7); }
    | "while" '(' expr ')' stmt             { $$ = ast_expr_new_while($3, $5); }
    | "do" stmt "while" '(' expr ')'        { $$ = ast_expr_new_do($2, $5); }
    | "for" '(' stmt_for_init ';' expr ';' expr ')' stmt { $$ = ast_expr_new_for($3, $5, $7, $9); }
    ;

stmt_block
    : '{' stmts '}' { $$ = $2; }
    ;

stmt_var
    : "var" T_ID type_colon ';'                 { $$ = ast_expr_new_var($2, $3, NULL); }
    | "var" T_ID type_colon_opt '=' expr ';'    { $$ = ast_expr_new_var($2, $3, $5); }
    | "var" T_ID type_colon '[' T_INT ']' ';'   { $$ = ast_expr_new_var_array($2, $3, $5); }
    ;

stmt_for_init
    : /* empty */ { $$ = NULL; }
    | expr
    | "var" T_ID type_colon_opt '=' expr { $$ = ast_expr_new_var($2, $3, $5); }
    ;

expr
    : T_ID  { $$ = ast_expr_new_name($1); }
    | T_INT { $$ = ast_expr_new_literal($1); }
    | '(' expr ')'      { $$ = $2; }
    | expr '[' expr ']' { $$ = ast_expr_new_subscript($1, $3); }
    | expr_binary
    | expr_unary
    | T_ID '(' expr_list ')' { $$ = ast_expr_new_call($1, $3); }
    | "if" '(' expr ')' expr "else" expr { $$ = ast_expr_new_if($3, $5, $7); }
    | "return" %prec P_RETURN       { $$ = ast_expr_new_return(NULL); }
    | "return" expr %prec P_RETURN  { $$ = ast_expr_new_return($2); }
    | "break"       { $$ = ast_expr_new_break(); }
    | "continue"    { $$ = ast_expr_new_continue(); }
    ;

lvalue_expr
    : T_ID              { $$ = ast_expr_new_name($1); }
    | expr '[' expr ']' { $$ = ast_expr_new_subscript($1, $3); }
    ;

expr_binary
    : lvalue_expr '=' expr  { $$ = ast_expr_new_assign($1, $3); }
    | lvalue_expr "+=" expr { $$ = ast_expr_new_assign($1, ast_expr_new_binop(ast_expr_clone($1), AST_EXPR_BINOP_PLUS, $3)); }
    | lvalue_expr "-=" expr { $$ = ast_expr_new_assign($1, ast_expr_new_binop(ast_expr_clone($1), AST_EXPR_BINOP_MINUS, $3)); }
    | lvalue_expr "*=" expr { $$ = ast_expr_new_assign($1, ast_expr_new_binop(ast_expr_clone($1), AST_EXPR_BINOP_MUL, $3)); }
    | lvalue_expr "/=" expr { $$ = ast_expr_new_assign($1, ast_expr_new_binop(ast_expr_clone($1), AST_EXPR_BINOP_DIV, $3)); }
    | lvalue_expr "%=" expr { $$ = ast_expr_new_assign($1, ast_expr_new_binop(ast_expr_clone($1), AST_EXPR_BINOP_REM, $3)); }
    | lvalue_expr "&=" expr { $$ = ast_expr_new_assign($1, ast_expr_new_binop(ast_expr_clone($1), AST_EXPR_BINOP_BAND, $3)); }
    | lvalue_expr "^=" expr { $$ = ast_expr_new_assign($1, ast_expr_new_binop(ast_expr_clone($1), AST_EXPR_BINOP_BXOR, $3)); }
    | lvalue_expr "|=" expr { $$ = ast_expr_new_assign($1, ast_expr_new_binop(ast_expr_clone($1), AST_EXPR_BINOP_BOR, $3)); }
    | lvalue_expr "<<=" expr { $$ = ast_expr_new_assign($1, ast_expr_new_binop(ast_expr_clone($1), AST_EXPR_BINOP_SHL, $3)); }
    | lvalue_expr ">>=" expr { $$ = ast_expr_new_assign($1, ast_expr_new_binop(ast_expr_clone($1), AST_EXPR_BINOP_SHR, $3)); }
    | lvalue_expr "&&=" expr { $$ = ast_expr_new_assign($1, ast_expr_new_binop(ast_expr_clone($1), AST_EXPR_BINOP_AND, $3)); }
    | lvalue_expr "||=" expr { $$ = ast_expr_new_assign($1, ast_expr_new_binop(ast_expr_clone($1), AST_EXPR_BINOP_OR, $3)); }
    | expr '+' expr { $$ = ast_expr_new_binop($1, AST_EXPR_BINOP_PLUS, $3); }
    | expr '-' expr { $$ = ast_expr_new_binop($1, AST_EXPR_BINOP_MINUS, $3); }
    | expr '*' expr { $$ = ast_expr_new_binop($1, AST_EXPR_BINOP_MUL, $3); }
    | expr '/' expr { $$ = ast_expr_new_binop($1, AST_EXPR_BINOP_DIV, $3); }
    | expr '%' expr { $$ = ast_expr_new_binop($1, AST_EXPR_BINOP_REM, $3); }
    | expr '&' expr { $$ = ast_expr_new_binop($1, AST_EXPR_BINOP_BAND, $3); }
    | expr '^' expr { $$ = ast_expr_new_binop($1, AST_EXPR_BINOP_BXOR, $3); }
    | expr '|' expr { $$ = ast_expr_new_binop($1, AST_EXPR_BINOP_BOR, $3); }
    | expr "<<" expr { $$ = ast_expr_new_binop($1, AST_EXPR_BINOP_SHL, $3); }
    | expr ">>" expr { $$ = ast_expr_new_binop($1, AST_EXPR_BINOP_SHR, $3); }
    | expr "&&" expr { $$ = ast_expr_new_binop($1, AST_EXPR_BINOP_AND, $3); }
    | expr "||" expr { $$ = ast_expr_new_binop($1, AST_EXPR_BINOP_OR, $3); }
    | expr "==" expr { $$ = ast_expr_new_binop($1, AST_EXPR_BINOP_EQ, $3); }
    | expr "!=" expr { $$ = ast_expr_new_binop($1, AST_EXPR_BINOP_NE, $3); }
    | expr '<' expr { $$ = ast_expr_new_binop($1, AST_EXPR_BINOP_LT, $3); }
    | expr "<=" expr { $$ = ast_expr_new_binop($1, AST_EXPR_BINOP_LE, $3); }
    | expr '>' expr { $$ = ast_expr_new_binop($1, AST_EXPR_BINOP_GT, $3); }
    | expr ">=" expr { $$ = ast_expr_new_binop($1, AST_EXPR_BINOP_GE, $3); }
    ;

expr_unary
    : '-' expr %prec P_UNARY    { $$ = ast_expr_new_unop(AST_EXPR_UNOP_NEG, $2); }
    | '~' expr %prec P_UNARY    { $$ = ast_expr_new_unop(AST_EXPR_UNOP_BINV, $2); }
    | '!' expr %prec P_UNARY    { $$ = ast_expr_new_unop(AST_EXPR_UNOP_INV, $2); }
    | "++" expr %prec P_UNARY   { $$ = ast_expr_new_unop(AST_EXPR_UNOP_INC, $2); }
    | "--" expr %prec P_UNARY   { $$ = ast_expr_new_unop(AST_EXPR_UNOP_DEC, $2); }
    ;

expr_list
    : /* empty */   { $$ = ast_expr_list_new(); }
    | expr_list1
    ;

expr_list1
    : expr { $$ = ast_expr_list_new(); ast_expr_list_add($$, $1); }
    | expr_list1 ',' expr { $$ = $1; ast_expr_list_add($$, $3); }
    ;

%%

void yyerror(struct ast_program ** result, char ** error, const char * str) {
    free(*error);

    *error = malloc(strlen(str) + 56);
    sprintf(*error, "at %d:%d: %s", yylloc.first_line, yylloc.first_column, str);
}
