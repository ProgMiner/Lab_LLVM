#include "ast.hpp"

#include <stdexcept>


struct ast_program * ast_program_new(void) {
    return new ast_program;
}

void ast_program_add_decl(struct ast_program * program, struct ast_decl * decl) {
    program->decls.push_back(std::shared_ptr<ast_decl> { decl });
}

struct ast_decl * ast_decl_new(char * id, struct ast_arg_list * args, struct ast_type * type) {
    const auto result = new ast_decl { id, std::move(args->values), std::shared_ptr<ast_type> { type } };

    free(id);
    delete args;
    return result;
}
void ast_decl_set_body(struct ast_decl * decl, struct ast_expr * body) {
    decl->body = std::shared_ptr<ast_expr> { body };
}

struct ast_arg_list * ast_arg_list_new(void) {
    return new ast_arg_list;
}

void ast_arg_list_add(struct ast_arg_list * list, struct ast_arg_c arg) {
    list->values.emplace_back(ast_arg { arg.id, std::shared_ptr<ast_type> { arg.type } });
    free(arg.id);
}

struct ast_type * ast_type_new_int(void) {
    return new ast_type_int {};
}

struct ast_type * ast_type_new_array(struct ast_type * type) {
    return new ast_type_array { std::shared_ptr<ast_type> { type } };
}

struct ast_expr * ast_expr_new_seq(struct ast_expr * lhs, struct ast_expr * rhs) {
    if (!lhs) {
        return rhs;
    }

    if (!rhs) {
        return lhs;
    }

    return new ast_expr_seq {
        std::shared_ptr<ast_expr> { lhs },
        std::shared_ptr<ast_expr> { rhs },
    };
}

struct ast_expr * ast_expr_new_var(char * id, struct ast_type * type, struct ast_expr * value) {
    const auto result = new ast_expr_var {
        id,
        std::shared_ptr<ast_type> { type },
        std::shared_ptr<ast_expr> { value },
    };

    free(id);
    return result;
}

struct ast_expr * ast_expr_new_var_array(char * id, struct ast_type * type, int32_t size) {
    const auto result = new ast_expr_var_array {
        id,
        std::shared_ptr<ast_type> { type },
        size,
    };

    free(id);
    return result;
}

struct ast_expr * ast_expr_new_if(struct ast_expr * cond, struct ast_expr * then_branch, struct ast_expr * else_branch) {
    return new ast_expr_if {
        std::shared_ptr<ast_expr> { cond },
        std::shared_ptr<ast_expr> { then_branch },
        std::shared_ptr<ast_expr> { else_branch },
    };
}

struct ast_expr * ast_expr_new_while(struct ast_expr * cond, struct ast_expr * body) {
    return new ast_expr_while {
        std::shared_ptr<ast_expr> { cond },
        std::shared_ptr<ast_expr> { body },
    };
}

struct ast_expr * ast_expr_new_do(struct ast_expr * body, struct ast_expr * cond) {
    return new ast_expr_do {
        std::shared_ptr<ast_expr> { body },
        std::shared_ptr<ast_expr> { cond },
    };
}

struct ast_expr * ast_expr_new_for(
        struct ast_expr * init,
        struct ast_expr * cond,
        struct ast_expr * post,
        struct ast_expr * body
) {
    return ast_expr_new_seq(
        init,
        new ast_expr_for {
            std::shared_ptr<ast_expr> { cond },
            std::shared_ptr<ast_expr> { body },
            std::shared_ptr<ast_expr> { post },
        }
    );
}

struct ast_expr * ast_expr_new_name(char * id) {
    const auto result = new ast_expr_name { id };

    free(id);
    return result;
}

struct ast_expr * ast_expr_new_literal(int32_t value) {
    return new ast_expr_literal { value };
}

struct ast_expr * ast_expr_new_subscript(struct ast_expr * value, struct ast_expr * index) {
    return new ast_expr_subscript {
        std::shared_ptr<ast_expr> { value },
        std::shared_ptr<ast_expr> { index },
    };
}

struct ast_expr * ast_expr_new_assign(struct ast_expr * target, struct ast_expr * value) {
    if (auto * const ptr = dynamic_cast<ast_lvalue_expr *>(target)) {
        return new ast_expr_assign {
            std::shared_ptr<ast_lvalue_expr> { ptr },
            std::shared_ptr<ast_expr> { value },
        };
    }

    throw std::invalid_argument { "assignment target must be lvalue" };
}

struct ast_expr * ast_expr_new_binop(struct ast_expr * lhs, enum ast_expr_binop op, struct ast_expr * rhs) {
    return new ast_expr_binary {
        op,
        std::shared_ptr<ast_expr> { lhs },
        std::shared_ptr<ast_expr> { rhs },
    };
}

struct ast_expr * ast_expr_new_unop(enum ast_expr_unop op, struct ast_expr * value) {
    return new ast_expr_unary {
        op,
        std::shared_ptr<ast_expr> { value },
    };
}

struct ast_expr * ast_expr_new_call(char * id, struct ast_expr_list * args) {
    const auto result = new ast_expr_call {
        id,
        std::move(args->values),
    };

    free(id);
    delete args;
    return result;
}

struct ast_expr * ast_expr_new_return(struct ast_expr * value) {
    return new ast_expr_return {
        std::shared_ptr<ast_expr> { value },
    };
}

struct ast_expr * ast_expr_new_break(void) {
    return new ast_expr_break;
}

struct ast_expr * ast_expr_new_continue(void) {
    return new ast_expr_continue;
}

struct ast_expr * ast_expr_clone(struct ast_expr * expr) {
    return expr->clone();
}

struct ast_expr_list * ast_expr_list_new(void) {
    return new ast_expr_list;
}

void ast_expr_list_add(struct ast_expr_list * list, struct ast_expr * expr) {
    list->values.push_back(std::shared_ptr<ast_expr> { expr });
}
