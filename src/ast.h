#pragma once

#ifdef __cplusplus
#include <cstdint>
#include <cstring>
#else
#include <stdint.h>
#include <string.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

enum ast_expr_binop {

    AST_EXPR_BINOP_PLUS = 0,
    AST_EXPR_BINOP_MINUS,
    AST_EXPR_BINOP_MUL,
    AST_EXPR_BINOP_DIV,
    AST_EXPR_BINOP_REM,
    AST_EXPR_BINOP_BAND,
    AST_EXPR_BINOP_BXOR,
    AST_EXPR_BINOP_BOR,
    AST_EXPR_BINOP_SHL,
    AST_EXPR_BINOP_SHR,
    AST_EXPR_BINOP_AND,
    AST_EXPR_BINOP_OR,
    AST_EXPR_BINOP_EQ,
    AST_EXPR_BINOP_NE,
    AST_EXPR_BINOP_LT,
    AST_EXPR_BINOP_LE,
    AST_EXPR_BINOP_GT,
    AST_EXPR_BINOP_GE,
};

enum ast_expr_unop {

    AST_EXPR_UNOP_NEG = 0,
    AST_EXPR_UNOP_BINV,
    AST_EXPR_UNOP_INV,
    AST_EXPR_UNOP_INC,
    AST_EXPR_UNOP_DEC,
};

struct ast_program;
struct ast_decl;
struct ast_arg_list;
struct ast_type;
struct ast_expr;
struct ast_expr_list;

struct ast_arg_c {

    char * id;
    struct ast_type * type;
};

struct ast_program * ast_program_new(void);
void ast_program_add_decl(struct ast_program * program, struct ast_decl * decl);

struct ast_decl * ast_decl_new(char * id, struct ast_arg_list * args, struct ast_type * type);
void ast_decl_set_body(struct ast_decl * decl, struct ast_expr * body);

struct ast_arg_list * ast_arg_list_new(void);
void ast_arg_list_add(struct ast_arg_list * list, struct ast_arg_c arg);

struct ast_type * ast_type_new_int(void);
struct ast_type * ast_type_new_array(struct ast_type * type);

struct ast_expr * ast_expr_new_seq(struct ast_expr * lhs, struct ast_expr * rhs);
struct ast_expr * ast_expr_new_var(char * id, struct ast_type * type, struct ast_expr * value);
struct ast_expr * ast_expr_new_var_array(char * id, struct ast_type * type, int32_t size);
struct ast_expr * ast_expr_new_if(struct ast_expr * cond, struct ast_expr * then_branch, struct ast_expr * else_branch);
struct ast_expr * ast_expr_new_while(struct ast_expr * cond, struct ast_expr * body);
struct ast_expr * ast_expr_new_do(struct ast_expr * body, struct ast_expr * cond);
struct ast_expr * ast_expr_new_for(
    struct ast_expr * init,
    struct ast_expr * cond,
    struct ast_expr * post,
    struct ast_expr * body
);
struct ast_expr * ast_expr_new_name(char * id);
struct ast_expr * ast_expr_new_literal(int32_t value);
struct ast_expr * ast_expr_new_subscript(struct ast_expr * value, struct ast_expr * index);
struct ast_expr * ast_expr_new_assign(struct ast_expr * target, struct ast_expr * value);
struct ast_expr * ast_expr_new_binop(struct ast_expr * lhs, enum ast_expr_binop op, struct ast_expr * rhs);
struct ast_expr * ast_expr_new_unop(enum ast_expr_unop op, struct ast_expr * value);
struct ast_expr * ast_expr_new_call(char * id, struct ast_expr_list * args);
struct ast_expr * ast_expr_new_return(struct ast_expr * value);
struct ast_expr * ast_expr_new_break(void);
struct ast_expr * ast_expr_new_continue(void);
struct ast_expr * ast_expr_clone(struct ast_expr * expr);

struct ast_expr_list * ast_expr_list_new(void);
void ast_expr_list_add(struct ast_expr_list * list, struct ast_expr * expr);

#ifdef __cplusplus
}
#endif
