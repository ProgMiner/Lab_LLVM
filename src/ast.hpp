#pragma once

#include <string>
#include <vector>
#include <memory>

#include "ast.h"


struct ast_arg;

struct ast_program {

    std::vector<std::shared_ptr<ast_decl>> decls;
};

struct ast_decl {

    std::string id;
    std::vector<ast_arg> args;
    std::shared_ptr<ast_type> return_type;
    std::shared_ptr<ast_expr> body;
};

struct ast_arg {

    std::string id;
    std::shared_ptr<ast_type> type;
};

struct ast_arg_list {

    std::vector<ast_arg> values;
};

struct ast_type {

    virtual ~ast_type() noexcept = default;
};

struct ast_type_int : ast_type {

    static inline const std::shared_ptr<ast_type_int> instance = std::make_shared<ast_type_int>();
};

struct ast_type_array : ast_type {

    std::shared_ptr<ast_type> type;

    explicit ast_type_array(std::shared_ptr<ast_type> type)
        : type(std::move(type))
    {}
};

struct ast_expr {

    std::shared_ptr<ast_type> type;

    ast_expr() noexcept = default;

    virtual ast_expr * clone() = 0;

    virtual ~ast_expr() noexcept = default;
};

struct ast_expr_seq : ast_expr {

    std::shared_ptr<ast_expr> lhs;
    std::shared_ptr<ast_expr> rhs;

    ast_expr_seq(std::shared_ptr<ast_expr> lhs, std::shared_ptr<ast_expr> rhs)
        : lhs(std::move(lhs))
        , rhs(std::move(rhs))
    {}

    ast_expr_seq * clone() override {
        return new ast_expr_seq { lhs, rhs };
    }
};

struct ast_expr_var : ast_expr {

    std::string id;
    std::shared_ptr<ast_type> var_type;
    std::shared_ptr<ast_expr> value;

    ast_expr_var(std::string id, std::shared_ptr<ast_type> var_type, std::shared_ptr<ast_expr> value)
        : id(std::move(id))
        , var_type(std::move(var_type))
        , value(std::move(value))
    {}

    ast_expr_var * clone() override {
        return new ast_expr_var { id, var_type, value };
    }
};

struct ast_expr_var_array : ast_expr_var {

    std::shared_ptr<ast_type> element_type;
    int32_t size;

    ast_expr_var_array(std::string id, std::shared_ptr<ast_type> element_type, int32_t size)
        : ast_expr_var(std::move(id), std::make_shared<ast_type_array>(element_type), nullptr)
        , element_type(std::move(element_type))
        , size(size)
    {}

    ast_expr_var_array * clone() override {
        return new ast_expr_var_array { id, var_type, element_type, size };
    }

private:

    ast_expr_var_array(
        std::string id,
        std::shared_ptr<ast_type> var_type,
        std::shared_ptr<ast_type> element_type,
        int32_t size
    )
        : ast_expr_var(std::move(id), std::move(var_type), nullptr)
        , element_type(std::move(element_type))
        , size(size)
    {}
};

struct ast_expr_if : ast_expr {

    std::shared_ptr<ast_expr> cond;
    std::shared_ptr<ast_expr> then_branch;
    std::shared_ptr<ast_expr> else_branch;

    ast_expr_if(
        std::shared_ptr<ast_expr> cond,
        std::shared_ptr<ast_expr> then_branch,
        std::shared_ptr<ast_expr> else_branch
    )
        : cond(std::move(cond))
        , then_branch(std::move(then_branch))
        , else_branch(std::move(else_branch))
    {}

    ast_expr_if * clone() override {
        return new ast_expr_if { cond, then_branch, else_branch };
    }
};

struct ast_expr_while : ast_expr {

    std::shared_ptr<ast_expr> cond;
    std::shared_ptr<ast_expr> body;

    ast_expr_while(std::shared_ptr<ast_expr> cond, std::shared_ptr<ast_expr> body)
        : cond(std::move(cond))
        , body(std::move(body))
    {}

    ast_expr_while * clone() override {
        return new ast_expr_while { cond, body };
    }
};

struct ast_expr_do : ast_expr {

    std::shared_ptr<ast_expr> body;
    std::shared_ptr<ast_expr> cond;

    ast_expr_do(std::shared_ptr<ast_expr> body, std::shared_ptr<ast_expr> cond)
        : body(std::move(body))
        , cond(std::move(cond))
    {}

    ast_expr_do * clone() override {
        return new ast_expr_do { body, cond };
    }
};

struct ast_expr_for : ast_expr {

    std::shared_ptr<ast_expr> cond;
    std::shared_ptr<ast_expr> body;
    std::shared_ptr<ast_expr> post;

    ast_expr_for(std::shared_ptr<ast_expr> cond, std::shared_ptr<ast_expr> body, std::shared_ptr<ast_expr> post)
            : cond(std::move(cond))
            , body(std::move(body))
            , post(std::move(post))
    {}

    ast_expr_for * clone() override {
        return new ast_expr_for { cond, body, post };
    }
};

struct ast_lvalue_expr : ast_expr {};

struct ast_expr_name : ast_lvalue_expr {

    std::string id;

    explicit ast_expr_name(std::string id): id(std::move(id)) {}

    ast_expr_name * clone() override {
        return new ast_expr_name { id };
    }
};

struct ast_expr_literal : ast_expr {

    int32_t value;

    explicit ast_expr_literal(int32_t value): value(value) {}

    ast_expr_literal * clone() override {
        return new ast_expr_literal { value };
    }
};

struct ast_expr_subscript : ast_lvalue_expr {

    std::shared_ptr<ast_expr> value;
    std::shared_ptr<ast_expr> index;

    ast_expr_subscript(std::shared_ptr<ast_expr> value, std::shared_ptr<ast_expr> index)
        : value(std::move(value))
        , index(std::move(index))
    {}

    ast_expr_subscript * clone() override {
        return new ast_expr_subscript { value, index };
    }
};

struct ast_expr_assign : ast_expr {

    std::shared_ptr<ast_lvalue_expr> target;
    std::shared_ptr<ast_expr> value;

    ast_expr_assign(std::shared_ptr<ast_lvalue_expr> target, std::shared_ptr<ast_expr> value)
        : target(std::move(target))
        , value(std::move(value))
    {}

    ast_expr_assign * clone() override {
        return new ast_expr_assign { target, value };
    }
};

struct ast_expr_binary : ast_expr {

    ast_expr_binop op;
    std::shared_ptr<ast_expr> lhs;
    std::shared_ptr<ast_expr> rhs;

    ast_expr_binary(ast_expr_binop op, std::shared_ptr<ast_expr> lhs, std::shared_ptr<ast_expr> rhs)
        : op(op)
        , lhs(std::move(lhs))
        , rhs(std::move(rhs))
    {}

    ast_expr_binary * clone() override {
        return new ast_expr_binary { op, lhs, rhs };
    }
};

struct ast_expr_unary : ast_expr {

    ast_expr_unop op;
    std::shared_ptr<ast_expr> value;

    ast_expr_unary(ast_expr_unop op, std::shared_ptr<ast_expr> value)
        : op(op)
        , value(std::move(value))
    {}

    ast_expr_unary * clone() override {
        return new ast_expr_unary { op, value };
    }
};

struct ast_expr_call : ast_expr {

    std::string id;
    std::vector<std::shared_ptr<ast_expr>> args;

    ast_expr_call(std::string id, std::vector<std::shared_ptr<ast_expr>> args)
        : id(std::move(id))
        , args(std::move(args))
    {}

    ast_expr_call * clone() override {
        return new ast_expr_call { id, args };
    }
};

struct ast_expr_return : ast_expr {

    std::shared_ptr<ast_expr> value;

    explicit ast_expr_return(std::shared_ptr<ast_expr> value): value(std::move(value)) {}

    ast_expr_return * clone() override {
        return new ast_expr_return { value };
    }
};

struct ast_expr_break : ast_expr {

    ast_expr_break() noexcept = default;

    ast_expr * clone() override {
        return new ast_expr_break;
    }
};

struct ast_expr_continue : ast_expr {

    ast_expr_continue() noexcept = default;

    ast_expr * clone() override {
        return new ast_expr_continue;
    }
};

struct ast_expr_list {

    std::vector<std::shared_ptr<ast_expr>> values;
};
