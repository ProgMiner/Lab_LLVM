#include <unordered_map>
#include <sstream>
#include <fstream>
#include <memory>

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>

#include "sim.h"
#include "ast.hpp"

extern "C" {
#include "parser.h"
#include "lexer.h"
}


using namespace llvm;

struct compiler_context {

    Module * llvm_module;
    IRBuilder<> ir_builder;
    std::unordered_map<std::string, FunctionType *> llvm_function_types;
    std::unordered_map<std::shared_ptr<ast_expr>, BasicBlock *> llvm_basic_blocks;
    std::vector<std::pair<BasicBlock *, BasicBlock *>> loops;
    Function * current_function = nullptr;

    BasicBlock * create_basic_block() const {
        return BasicBlock::Create(llvm_module->getContext(), "", current_function);
    }

    BasicBlock * get_basic_block(std::shared_ptr<ast_expr> expr) {
        if (auto it = llvm_basic_blocks.find(expr); it != llvm_basic_blocks.end()) {
            return it->second;
        }

        auto * const result = create_basic_block();
        llvm_basic_blocks.emplace(std::move(expr), result);

        return result;
    }

    Value * get_trivial_value() {
        return ir_builder.getInt32(0);
    }
};

struct type_resolver_context {

    std::unordered_map<std::string, std::shared_ptr<ast_decl>> functions;
    std::shared_ptr<ast_decl> current_function;
};

template<typename V>
struct scope {

    const scope<V> * parent;
    std::unordered_map<std::string, V> variables;

    const V & lookup(const std::string & id) const {
        const struct scope<V> * scope = this;

        while (scope) {
            if (auto it = scope->variables.find(id); it != scope->variables.end()) {
                return it->second;
            }

            scope = scope->parent;
        }

        throw std::invalid_argument { "no variable named " + id + " in current scope" };
    }
};

struct compiler_variable {

    std::shared_ptr<ast_type> type;
    Value * llvm_value = nullptr;
};

struct type_resolver_variable {

    std::shared_ptr<ast_type> type;
};

using compiler_scope = scope<compiler_variable>;
using type_resolver_scope = scope<type_resolver_variable>;


[[noreturn]]
static void stack_chk_fail_mock() {
    throw std::runtime_error("stack check failed");
}

static std::shared_ptr<ast_program> parse_program(char * filename) {
    FILE * const input_file = fopen(filename, "r");

    if (!input_file) {
        std::string error { strerror(errno) };
        throw std::runtime_error { "Bad input file" + std::move(error) };
    }

    try {
        const YY_BUFFER_STATE buffer_state = yy_create_buffer(input_file, 4096); // NOLINT(*-misplaced-const)
        yy_switch_to_buffer(buffer_state);

        ast_program * ast = nullptr;
        char * error = nullptr;

        switch (yyparse(&ast, &error)) {
            case 1:
                throw std::runtime_error { "Parsing failed: " + std::string {error} };

            case 2:
                throw std::runtime_error { "Memory exhausted" };
        }

        return std::shared_ptr<ast_program> { ast };
    } catch (...) {
        fclose(input_file);
        throw;
    }
}

static std::shared_ptr<ast_expr> fill_null_expr(const std::shared_ptr<ast_expr> & expr) {
    if (!expr) {
        return std::make_shared<ast_expr_literal>(0);
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_seq>(expr)) {
        if (!ptr->lhs) {
            return fill_null_expr(ptr->rhs);
        }

        if (!ptr->rhs) {
            return fill_null_expr(ptr->lhs);
        }

        ptr->lhs = fill_null_expr(ptr->lhs);
        ptr->rhs = fill_null_expr(ptr->rhs);
        return ptr;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_var>(expr)) {
        // cannot fill NULL value of array type

        if (ptr->value) {
            ptr->value = fill_null_expr(ptr->value);
        }

        return ptr;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_if>(expr)) {
        ptr->cond = fill_null_expr(ptr->cond);
        ptr->then_branch = fill_null_expr(ptr->then_branch);
        ptr->else_branch = fill_null_expr(ptr->else_branch);
        return ptr;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_while>(expr)) {
        ptr->cond = fill_null_expr(ptr->cond);
        ptr->body = fill_null_expr(ptr->body);
        return ptr;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_do>(expr)) {
        ptr->body = fill_null_expr(ptr->body);
        ptr->cond = fill_null_expr(ptr->cond);
        return ptr;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_for>(expr)) {
        ptr->cond = fill_null_expr(ptr->cond);
        ptr->body = fill_null_expr(ptr->body);
        ptr->post = fill_null_expr(ptr->post);
        return ptr;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_name>(expr)) {
        return ptr;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_literal>(expr)) {
        return ptr;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_subscript>(expr)) {
        ptr->value = fill_null_expr(ptr->value);
        ptr->index = fill_null_expr(ptr->index);
        return ptr;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_assign>(expr)) {
        ptr->target = std::dynamic_pointer_cast<ast_lvalue_expr>(fill_null_expr(ptr->target));
        ptr->value = fill_null_expr(ptr->value);
        return ptr;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_binary>(expr)) {
        ptr->lhs = fill_null_expr(ptr->lhs);
        ptr->rhs = fill_null_expr(ptr->rhs);
        return ptr;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_unary>(expr)) {
        ptr->value = fill_null_expr(ptr->value);
        return ptr;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_call>(expr)) {
        for (auto & arg : ptr->args) {
            arg = fill_null_expr(arg);
        }

        return ptr;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_return>(expr)) {
        ptr->value = fill_null_expr(ptr->value);
        return ptr;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_break>(expr)) {
        return ptr;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_continue>(expr)) {
        return ptr;
    }

    throw std::invalid_argument { "unsupported expr" };
}

static std::shared_ptr<ast_type> unify_types(
    const std::shared_ptr<ast_type> & lhs,
    const std::shared_ptr<ast_type> & rhs
) {
    if (lhs->equals(rhs.get())) {
        return lhs;
    }

    return ast_type_int::instance;
}

static std::shared_ptr<ast_expr> coerce_type(
    const std::shared_ptr<ast_expr> & expr,
    const std::shared_ptr<ast_type> & type
) {
    if (type->equals(expr->type.get())) {
        return expr;
    }

    if (!ast_type_int::instance->equals(type.get())) {
        throw std::invalid_argument { "coercion is only supported for int type" };
    }

    return std::make_shared<ast_expr_coerce_ptr_to_int>(expr);
}

static void resolve_types(
    type_resolver_context & ctx,
    type_resolver_scope & parent_scope,
    const std::shared_ptr<ast_expr> & expr
) {
    if (!expr) {
        return;
    }

    type_resolver_scope scope { &parent_scope };

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_seq>(expr)) {
        resolve_types(ctx, scope, ptr->lhs);
        resolve_types(ctx, scope, ptr->rhs);
        expr->type = ptr->rhs->type;
        return;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_var>(expr)) {
        resolve_types(ctx, scope, ptr->value);

        if (!ptr->var_type) {
            ptr->var_type = ptr->value->type;
        }

        parent_scope.variables[ptr->id] = { ptr->var_type };
        expr->type = ast_type_int::instance;
        return;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_if>(expr)) {
        resolve_types(ctx, scope, ptr->cond);
        resolve_types(ctx, scope, ptr->then_branch);
        resolve_types(ctx, scope, ptr->else_branch);

        std::shared_ptr<ast_type> type = unify_types(ptr->then_branch->type, ptr->else_branch->type);
        ptr->then_branch = coerce_type(ptr->then_branch, type);
        ptr->else_branch = coerce_type(ptr->else_branch, type);
        ptr->type = type;
        return;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_while>(expr)) {
        resolve_types(ctx, scope, ptr->cond);
        resolve_types(ctx, scope, ptr->body);
        ptr->type = ast_type_int::instance;
        return;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_do>(expr)) {
        resolve_types(ctx, scope, ptr->body);
        resolve_types(ctx, scope, ptr->cond);
        ptr->type = ast_type_int::instance;
        return;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_for>(expr)) {
        resolve_types(ctx, scope, ptr->cond);
        resolve_types(ctx, scope, ptr->body);
        resolve_types(ctx, scope, ptr->post);
        ptr->type = ast_type_int::instance;
        return;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_name>(expr)) {
        ptr->type = scope.lookup(ptr->id).type;
        return;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_literal>(expr)) {
        ptr->type = ast_type_int::instance;
        return;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_subscript>(expr)) {
        resolve_types(ctx, scope, ptr->value);
        resolve_types(ctx, scope, ptr->index);

        ptr->index = coerce_type(ptr->index, ast_type_int::instance);

        if (auto ptr1 = std::dynamic_pointer_cast<ast_type_array>(ptr->value->type)) {
            ptr->type = ptr1->type;
        } else {
            throw std::invalid_argument { "subscript is allowed only on array types" };
        }

        return;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_assign>(expr)) {
        resolve_types(ctx, scope, ptr->target);
        resolve_types(ctx, scope, ptr->value);

        ptr->value = coerce_type(ptr->value, ptr->target->type);
        ptr->type = ptr->value->type;
        return;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_binary>(expr)) {
        resolve_types(ctx, scope, ptr->lhs);
        resolve_types(ctx, scope, ptr->rhs);

        ptr->lhs = coerce_type(ptr->lhs, ast_type_int::instance);
        ptr->rhs = coerce_type(ptr->rhs, ast_type_int::instance);
        ptr->type = ast_type_int::instance;
        return;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_unary>(expr)) {
        resolve_types(ctx, scope, ptr->value);

        ptr->value = coerce_type(ptr->value, ast_type_int::instance);
        ptr->type = ast_type_int::instance;
        return;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_call>(expr)) {
        const auto & decl = ctx.functions.at(ptr->id);

        if (ptr->args.size() != decl->args.size()) {
            throw std::invalid_argument { "number of arguments doesn't correspond to number of function parameters" };
        }

        for (std::size_t i = 0; i < ptr->args.size(); ++i) {
            resolve_types(ctx, scope, ptr->args[i]);
            ptr->args[i] = coerce_type(ptr->args[i], decl->args[i].type);
        }

        ptr->type = decl->return_type;
        return;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_return>(expr)) {
        resolve_types(ctx, scope, ptr->value);

        ptr->value = coerce_type(ptr->value, ctx.current_function->return_type);
        ptr->type = ast_type_int::instance;
        return;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_break>(expr)) {
        ptr->type = ast_type_int::instance;
        return;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_continue>(expr)) {
        ptr->type = ast_type_int::instance;
        return;
    }

    throw std::invalid_argument { "unsupported expr" };
}

static void resolve_types(type_resolver_context & ctx, const std::shared_ptr<ast_decl> & decl) {
    if (!decl->body) {
        return;
    }

    type_resolver_scope initial_scope { nullptr };
    for (const auto & arg : decl->args) {
        initial_scope.variables[arg.id] = { arg.type };
    }

    ctx.current_function = decl;

    decl->body = fill_null_expr(decl->body);
    resolve_types(ctx, initial_scope, decl->body);

    decl->body = coerce_type(decl->body, decl->return_type);
}

static void resolve_types(const std::shared_ptr<ast_program> & program) {
    type_resolver_context ctx;

    for (const auto & decl : program->decls) {
        if (!decl->return_type) {
            decl->return_type = ast_type_int::instance;
        }

        ctx.functions[decl->id] = decl;
    }

    for (const auto & decl : program->decls) {
        resolve_types(ctx, decl);
    }
}

static Type * compile(compiler_context & ctx, const std::shared_ptr<ast_type> & type) {
    if (std::dynamic_pointer_cast<ast_type_int>(type)) {
        return ctx.ir_builder.getInt32Ty();
    }

    if (std::dynamic_pointer_cast<ast_type_array>(type)) {
        return ctx.ir_builder.getPtrTy();
    }

    throw std::invalid_argument { "unsupported type" };
}

static Value * compile_lvalue(
        compiler_context & ctx,
        const std::shared_ptr<ast_lvalue_expr> & expr,
        compiler_scope & parent_scope,
        BasicBlock * next_basic_block
);

static Value * compile_lvalue0(
        compiler_context & ctx,
        const std::shared_ptr<ast_lvalue_expr> & expr,
        compiler_scope & scope,
        BasicBlock * next_basic_block
);

static Value * compile(
        compiler_context & ctx,
        std::shared_ptr<ast_expr> expr,
        compiler_scope & parent_scope,
        BasicBlock * next_basic_block
) {
    compiler_scope scope {&parent_scope };

    while (true) {
        if (!expr) {
            throw std::invalid_argument { "null expr" };
        }

        auto * const basic_block = ctx.get_basic_block(expr);
        ctx.ir_builder.SetInsertPoint(basic_block);

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_seq>(expr)) {
            ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->lhs));

            compile(ctx, ptr->lhs, scope, ctx.get_basic_block(ptr->rhs));
            expr = ptr->rhs;
            continue;
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_var>(expr)) {
            auto * const var = ctx.ir_builder.CreateAlloca(compile(ctx, ptr->var_type));

            auto * const store_basic_block = ctx.create_basic_block();

            Value * value = nullptr;
            if (auto ptr1 = std::dynamic_pointer_cast<ast_expr_var_array>(expr)) {
                auto * const size = ctx.ir_builder.getInt32(ptr1->size);

                value = ctx.ir_builder.CreateAlloca(compile(ctx, ptr1->element_type), size);
                ctx.ir_builder.CreateBr(store_basic_block);
            } else if (ptr->value) {
                ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->value));

                value = compile(ctx, ptr->value, scope, store_basic_block);
            }

            if (value) {
                ctx.ir_builder.SetInsertPoint(store_basic_block);
                ctx.ir_builder.CreateStore(value, var);
            }

            ctx.ir_builder.CreateBr(next_basic_block);

            parent_scope.variables[ptr->id] = { ptr->var_type, var };
            return ctx.get_trivial_value();
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_if>(expr)) {
            ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->cond));

            auto * const br_basic_block = ctx.create_basic_block();
            auto * const result_basic_block = ctx.create_basic_block();

            auto * const cond = compile(ctx, ptr->cond, scope, br_basic_block);

            ctx.ir_builder.SetInsertPoint(br_basic_block);
            auto * const cond_bool = ctx.ir_builder.CreateIsNotNull(cond);
            ctx.ir_builder.CreateCondBr(
                cond_bool,
                ctx.get_basic_block(ptr->then_branch),
                ctx.get_basic_block(ptr->else_branch)
            );

            auto * const then_result = compile(ctx, ptr->then_branch, scope, result_basic_block);
            auto * const else_result = compile(ctx, ptr->else_branch, scope, result_basic_block);

            ctx.ir_builder.SetInsertPoint(result_basic_block);
            auto * const result = ctx.ir_builder.CreatePHI(compile(ctx, ptr->type), 2);
            result->addIncoming(then_result, ctx.get_basic_block(ptr->then_branch));
            result->addIncoming(else_result, ctx.get_basic_block(ptr->else_branch));
            ctx.ir_builder.CreateBr(next_basic_block);
            return result;
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_while>(expr)) {
            ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->cond));

            auto * const br_basic_block = ctx.create_basic_block();

            auto * const cond = compile(ctx, ptr->cond, scope, br_basic_block);

            ctx.ir_builder.SetInsertPoint(br_basic_block);
            auto * const cond_bool = ctx.ir_builder.CreateIsNotNull(cond);
            ctx.ir_builder.CreateCondBr(cond_bool, ctx.get_basic_block(ptr->body), next_basic_block);

            ctx.loops.emplace_back(ctx.get_basic_block(ptr->cond), next_basic_block);
            compile(ctx, ptr->body, scope, ctx.get_basic_block(ptr->cond));
            ctx.loops.pop_back();

            return ctx.get_trivial_value();
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_do>(expr)) {
            ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->body));

            ctx.loops.emplace_back(ctx.get_basic_block(ptr->cond), next_basic_block);
            compile(ctx, ptr->body, scope, ctx.get_basic_block(ptr->cond));
            ctx.loops.pop_back();

            auto * const br_basic_block = ctx.create_basic_block();
            auto * const cond = compile(ctx, ptr->cond, scope, br_basic_block);

            ctx.ir_builder.SetInsertPoint(br_basic_block);
            auto * const cond_bool = ctx.ir_builder.CreateIsNotNull(cond);
            ctx.ir_builder.CreateCondBr(cond_bool, ctx.get_basic_block(ptr->body), next_basic_block);
            return ctx.get_trivial_value();
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_for>(expr)) {
            ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->cond));

            auto * const br_basic_block = ctx.create_basic_block();

            auto * const cond = compile(ctx, ptr->cond, scope, br_basic_block);

            ctx.ir_builder.SetInsertPoint(br_basic_block);
            auto * const cond_bool = ctx.ir_builder.CreateIsNotNull(cond);
            ctx.ir_builder.CreateCondBr(cond_bool, ctx.get_basic_block(ptr->body), next_basic_block);

            ctx.loops.emplace_back(ctx.get_basic_block(ptr->post), next_basic_block);
            compile(ctx, ptr->body, scope, ctx.get_basic_block(ptr->post));
            ctx.loops.pop_back();

            compile(ctx, ptr->post, scope, ctx.get_basic_block(ptr->cond));
            return ctx.get_trivial_value();
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_lvalue_expr>(expr)) {
            auto * const load_basic_block = ctx.create_basic_block();

            auto * const result_ptr = compile_lvalue0(ctx, ptr, scope, load_basic_block);

            ctx.ir_builder.SetInsertPoint(load_basic_block);
            auto * const result = ctx.ir_builder.CreateLoad(compile(ctx, ptr->type), result_ptr);
            ctx.ir_builder.CreateBr(next_basic_block);
            return result;
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_literal>(expr)) {
            ctx.ir_builder.CreateBr(next_basic_block);

            return ctx.ir_builder.getInt32(ptr->value);
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_assign>(expr)) {
            ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->target));

            auto * const target = compile_lvalue(ctx, ptr->target, scope, ctx.get_basic_block(ptr->value));

            auto * const store_basic_block = ctx.create_basic_block();

            auto * const value = compile(ctx, ptr->value, scope, store_basic_block);

            ctx.ir_builder.SetInsertPoint(store_basic_block);
            ctx.ir_builder.CreateStore(value, target);
            ctx.ir_builder.CreateBr(next_basic_block);
            return value;
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_binary>(expr)) {
            ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->lhs));

            auto * const lhs = compile(ctx, ptr->lhs, scope, ctx.get_basic_block(ptr->rhs));

            auto * const binop_basic_block = ctx.create_basic_block();

            auto * const rhs = compile(ctx, ptr->rhs, scope, binop_basic_block);

            ctx.ir_builder.SetInsertPoint(binop_basic_block);
            Value * result;

            switch (ptr->op) {
                case AST_EXPR_BINOP_ADD:
                    result = ctx.ir_builder.CreateAdd(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_SUB:
                    result = ctx.ir_builder.CreateSub(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_MUL:
                    result = ctx.ir_builder.CreateMul(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_DIV:
                    result = ctx.ir_builder.CreateSDiv(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_REM:
                    result = ctx.ir_builder.CreateSRem(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_BAND:
                    result = ctx.ir_builder.CreateAnd(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_BXOR:
                    result = ctx.ir_builder.CreateXor(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_BOR:
                    result = ctx.ir_builder.CreateOr(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_SHL:
                    result = ctx.ir_builder.CreateShl(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_SHR:
                    result = ctx.ir_builder.CreateAShr(lhs, rhs);
                    break;

                case AST_EXPR_BINOP_AND: {
                    auto * const lhs_bool = ctx.ir_builder.CreateIsNotNull(lhs);
                    auto * const rhs_bool = ctx.ir_builder.CreateIsNotNull(rhs);

                    auto * const result_bool = ctx.ir_builder.CreateAnd(lhs_bool, rhs_bool);
                    result = ctx.ir_builder.CreateZExt(result_bool, ctx.ir_builder.getInt32Ty());
                    break;
                }

                case AST_EXPR_BINOP_OR: {
                    auto * const result0 = ctx.ir_builder.CreateOr(lhs, rhs);
                    auto * const result_bool = ctx.ir_builder.CreateIsNotNull(result0);
                    result = ctx.ir_builder.CreateZExt(result_bool, ctx.ir_builder.getInt32Ty());
                    break;
                }

                case AST_EXPR_BINOP_EQ: {
                    auto * const result_bool = ctx.ir_builder.CreateICmpEQ(lhs, rhs);
                    result = ctx.ir_builder.CreateZExt(result_bool, ctx.ir_builder.getInt32Ty());
                    break;
                }

                case AST_EXPR_BINOP_NE: {
                    auto * const result_bool = ctx.ir_builder.CreateICmpNE(lhs, rhs);
                    result = ctx.ir_builder.CreateZExt(result_bool, ctx.ir_builder.getInt32Ty());
                    break;
                }

                case AST_EXPR_BINOP_LT: {
                    auto * const result_bool = ctx.ir_builder.CreateICmpSLT(lhs, rhs);
                    result = ctx.ir_builder.CreateZExt(result_bool, ctx.ir_builder.getInt32Ty());
                    break;
                }

                case AST_EXPR_BINOP_LE: {
                    auto * const result_bool = ctx.ir_builder.CreateICmpSLE(lhs, rhs);
                    result = ctx.ir_builder.CreateZExt(result_bool, ctx.ir_builder.getInt32Ty());
                    break;
                }

                case AST_EXPR_BINOP_GT: {
                    auto * const result_bool = ctx.ir_builder.CreateICmpSGT(lhs, rhs);
                    result = ctx.ir_builder.CreateZExt(result_bool, ctx.ir_builder.getInt32Ty());
                    break;
                }

                case AST_EXPR_BINOP_GE: {
                    auto * const result_bool = ctx.ir_builder.CreateICmpSGE(lhs, rhs);
                    result = ctx.ir_builder.CreateZExt(result_bool, ctx.ir_builder.getInt32Ty());
                    break;
                }
            }

            ctx.ir_builder.CreateBr(next_basic_block);
            return result;
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_unary>(expr)) {
            ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->value));

            auto * const unop_basic_block = ctx.create_basic_block();

            auto * const value = compile(ctx, ptr->value, scope, unop_basic_block);

            ctx.ir_builder.SetInsertPoint(unop_basic_block);
            Value * result;

            switch (ptr->op) {
                case AST_EXPR_UNOP_NEG:
                    result = ctx.ir_builder.CreateNeg(value);
                    break;

                case AST_EXPR_UNOP_BINV:
                    result = ctx.ir_builder.CreateXor(value, ctx.ir_builder.getInt32(-1));
                    break;

                case AST_EXPR_UNOP_INV: {
                    auto * const result_bool = ctx.ir_builder.CreateIsNull(value);
                    result = ctx.ir_builder.CreateZExt(result_bool, ctx.ir_builder.getInt32Ty());
                    break;
                }
            }

            ctx.ir_builder.CreateBr(next_basic_block);
            return result;
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_call>(expr)) {
            std::vector<Value *> args(ptr->args.size());

            if (!ptr->args.empty()) {
                ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->args[0]));

                for (std::size_t i = 0; i < args.size() - 1; ++i) {
                    args[i] = compile(ctx, ptr->args[i], scope, ctx.get_basic_block(ptr->args[i + 1]));
                }

                auto * const call_basic_block = ctx.create_basic_block();

                args[args.size() - 1] = compile(ctx, ptr->args[args.size() - 1], scope, call_basic_block);

                ctx.ir_builder.SetInsertPoint(call_basic_block);
            }

            auto * const result = ctx.ir_builder.CreateCall(
                ctx.llvm_module->getOrInsertFunction(ptr->id, ctx.llvm_function_types[ptr->id]),
                args
            );

            ctx.ir_builder.CreateBr(next_basic_block);
            return result;
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_return>(expr)) {
            ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->value));

            auto * const ret_basic_block = ctx.create_basic_block();

            auto * const result = compile(ctx, ptr->value, scope, ret_basic_block);

            ctx.ir_builder.SetInsertPoint(ret_basic_block);
            ctx.ir_builder.CreateRet(result);

            // dummy value
            return ctx.ir_builder.getInt32(0);
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_break>(expr)) {
            if (ctx.loops.empty()) {
                throw std::invalid_argument { "break is only allowed in loops" };
            }

            ctx.ir_builder.CreateBr(ctx.loops.rbegin()->second);

            // dummy value
            return ctx.ir_builder.getInt32(0);
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_continue>(expr)) {
            if (ctx.loops.empty()) {
                throw std::invalid_argument { "continue is only allowed in loops" };
            }

            ctx.ir_builder.CreateBr(ctx.loops.rbegin()->first);

            // dummy value
            return ctx.ir_builder.getInt32(0);
        }

        if (auto ptr = std::dynamic_pointer_cast<ast_expr_coerce_ptr_to_int>(expr)) {
            ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->value));

            auto * const coerce_basic_block = ctx.create_basic_block();

            auto * const result_ptr = compile(ctx, ptr->value, scope, coerce_basic_block);

            ctx.ir_builder.SetInsertPoint(coerce_basic_block);
            auto * const result = ctx.ir_builder.CreatePtrToInt(result_ptr, compile(ctx, ptr->type));
            ctx.ir_builder.CreateBr(next_basic_block);

            return result;
        }

        throw std::invalid_argument { "unsupported expr" };
    }
}

static Value * compile_lvalue(
        compiler_context & ctx,
        const std::shared_ptr<ast_lvalue_expr> & expr,
        compiler_scope & parent_scope,
        BasicBlock * next_basic_block
) {
    compiler_scope scope {&parent_scope };

    return compile_lvalue0(ctx, expr, scope, next_basic_block);
}

static Value * compile_lvalue0(
        compiler_context & ctx,
        const std::shared_ptr<ast_lvalue_expr> & expr,
        compiler_scope & scope,
        BasicBlock * next_basic_block
) {
    auto * const basic_block = ctx.get_basic_block(expr);
    ctx.ir_builder.SetInsertPoint(basic_block);

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_name>(expr)) {
        ctx.ir_builder.CreateBr(next_basic_block);

        return scope.lookup(ptr->id).llvm_value;
    }

    if (auto ptr = std::dynamic_pointer_cast<ast_expr_subscript>(expr)) {
        ctx.ir_builder.CreateBr(ctx.get_basic_block(ptr->value));

        auto * const value = compile(ctx, ptr->value, scope, ctx.get_basic_block(ptr->index));

        auto * const gep_basic_block = ctx.create_basic_block();

        auto * const index = compile(ctx, ptr->index, scope, gep_basic_block);

        ctx.ir_builder.SetInsertPoint(gep_basic_block);
        auto * const result = ctx.ir_builder.CreateInBoundsGEP(compile(ctx, ptr->type), value, index);
        ctx.ir_builder.CreateBr(next_basic_block);

        return result;
    }

    throw std::invalid_argument { "unsupported lvalue expr" };
}

static void compile(compiler_context & ctx, const std::shared_ptr<ast_decl> & decl) {
    if (!decl->body) {
        return;
    }

    ctx.llvm_basic_blocks.clear();
    ctx.loops.clear();

    auto * const function = Function::Create(
        ctx.llvm_function_types.at(decl->id),
        Function::ExternalLinkage,
        decl->id,
        ctx.llvm_module
    );

    ctx.current_function = function;

    auto * const first_basic_block = ctx.create_basic_block();
    ctx.ir_builder.SetInsertPoint(first_basic_block);

    compiler_scope initial_scope {nullptr };

    {
        auto it = function->args().begin();
        for (const auto &arg: decl->args) {
            auto * const llvm_arg = it++;

            llvm_arg->setName(arg.id);

            auto * var = ctx.ir_builder.CreateAlloca(compile(ctx, arg.type));
            ctx.ir_builder.CreateStore(llvm_arg, var);

            initial_scope.variables[arg.id] = { arg.type, var };
        }
    }

    ctx.ir_builder.CreateBr(ctx.get_basic_block(decl->body));

    auto * const last_basic_block = ctx.create_basic_block();

    auto * const result = compile(ctx, decl->body, initial_scope, last_basic_block);

    ctx.ir_builder.SetInsertPoint(last_basic_block);
    ctx.ir_builder.CreateRet(result);
}

static void compile(const std::shared_ptr<ast_program> & program, Module * module) {
    compiler_context ctx { module, IRBuilder { module->getContext() } };

    for (const auto & decl: program->decls) {
        std::vector<Type *> params;

        for (const auto & arg : decl->args) {
            params.emplace_back(compile(ctx, arg.type));
        }

        ctx.llvm_function_types[decl->id] = FunctionType::get(
            compile(ctx, decl->return_type),
            params,
            false
        );
    }

    for (const auto & item: program->decls) {
        compile(ctx, item);
    }
}

int main(int argc, char * argv[]) {
    if (argc < 2) {
        errs() << "usage: " << argv[0] << " <input file>\n";
        return 1;
    }

    std::shared_ptr<ast_program> program;

    try {
        program = parse_program(argv[1]);
    } catch (const std::exception & e) {
        errs() << e.what() << '\n';
        return 2;
    }

    LLVMContext context;
    auto * const module = new Module("main", context);

    resolve_types(program);
    compile(program, module);

    std::error_code err;
    raw_fd_stream fos { "main.gen.ll", err };
    module->print(fos, nullptr);

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();

    ExecutionEngine * const ee = EngineBuilder(std::unique_ptr<Module>(module)).create();

    ee->InstallLazyFunctionCreator([&](const std::string & name) -> void * {
        if (name == "sim_set_pixel") {
            return reinterpret_cast<void *>(sim_set_pixel);
        }

        if (name == "sim_flush") {
            return reinterpret_cast<void *>(sim_flush);
        }

        if (name == "sim_rand") {
            return reinterpret_cast<void *>(sim_rand);
        }

        if (name == "__stack_chk_fail") {
            return reinterpret_cast<void *>(stack_chk_fail_mock);
        }

        errs() << "Not found " << name << "\n";
        return nullptr;
    });

    ee->finalizeObject();

    auto * const main_function = module->getFunction("main");
    if (!main_function) {
        throw std::invalid_argument { "main function is not defined" };
    }

    const GenericValue result = ee->runFunction(main_function, {});

    return static_cast<int>(*result.IntVal.getRawData());
}
