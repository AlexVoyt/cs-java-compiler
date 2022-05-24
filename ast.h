#ifndef AST_H
#define AST_H

#include "lexer.h"

struct expression;
struct statement;
struct declaration;

enum expression_type
{
    ExpressionType_None,
    ExpressionType_Integer,
    ExpressionType_Float,
    ExpressionType_Name,
    ExpressionType_FunctionCall,
    /* ExpressionType_Field, */ // TODO: do we have structs? Methods are calls or fields?
    ExpressionType_Unary,
    ExpressionType_Binary,
    ExpressionType_Paren,
};

struct expression
{
    expression_type Type;
    union
    {
        u64 Integer;
        f64 Float;

        struct
        {
            char* Ident;
            u32 Length;
        } Name;

        struct
        {
            char* Name;
            u32 Length;
            array<expression*> Arguments;
        } FunctionCall;

        struct
        {
            token_type Op;
            expression* Expression;
        } Unary;

        struct
        {
            token_type Op;
            expression* Left;
            expression* Right;
        } Binary;

        struct
        {
            expression* Expression;
        } Paren;

    };
};

enum declaration_type
{
    DeclarationType_None,
    DeclarationType_Variable,
    DeclarationType_Function,
    DeclarationType_Class,
    DeclarationType_Using,
};

struct function_param
{
    u32 NameLength;
    char* Name;
    u32 TypeLength;
    char* Type;
};

enum access_modifier
{
    AccessModifier_Public,
    AccessModifier_Protected,
    AccessModifier_Private,
};

const char* ToString(access_modifier Modifier)
{
    switch(Modifier)
    {
        case AccessModifier_Public:
        {
            return "public";
        } break;

        case AccessModifier_Protected:
        {
            return "protected";
        } break;

        case AccessModifier_Private:
        {
            return "private";
        } break;
    }

    InvalidCodePath("ToString access modifier");
    return 0;
}

struct declaration
{
    declaration_type Type;
    u32 NameLength;
    char* Name;
    union
    {
        struct
        {
            u32 TypeLength;
            char* Type;
            expression* Expression;
        } Variable;

        struct
        {
            access_modifier Modifier;
            u32 ReturnTypeLength;
            char* ReturnType;
            array<function_param> Params;
            array<statement*> Statements;
        } Function;

        struct
        {
            array<declaration*> Functions;
        } Class;

#if 1
        struct
        {
        } Using;
#endif
    };
};

enum statement_type
{
    StatementType_Expression,
    StatementType_Assign,
    StatementType_Declaration,
    StatementType_If,
    StatementType_While,
    StatementType_DoWhile,
    StatementType_For,
    StatementType_Return,
};

struct else_if
{
    expression* Condition;
    array<statement*> StatementBlock;
};

struct statement
{
    statement_type Type;
    union
    {
        expression* Expression;
        declaration* Declaration;

        struct
        {
            token_type Op;
            expression* Left;
            expression* Right;
        } Assign;

        struct
        {
            expression* Condition;
            array<statement*> ThenBlock;
            array<statement*> ElseBlock;
            array<else_if> ElseIfs;
        } If;

        struct
        {
            expression* Condition;
            array<statement*> Statements;
        } While; // NOTE: also do_while

        struct
        {
            statement* Init;
            expression* Condition;
            statement* Next;
            array<statement*> Statements;
        } For;

        struct
        {
            expression* Expression;
        } Return;
    };
};

void* AllocateNode(size_t size);
expression* NewExpression(expression_type Type);
expression* NewUnaryExpression(token_type Op, expression* Expression);
expression* NewBinaryExpression(token_type Op, expression* Left, expression* Right);
expression* NewParenExpression(expression* Expression);
expression* NewNameExpression(char* Name, u32 Length);
expression* NewIntegerExpression(u64 Value);
expression* NewFloatExpression(f64 Value);
expression* NewFunctionCallExpression(expression* Expression, array<expression*> Arguments);
statement* NewStatement(statement_type Type);
statement* NewExpressionStatement(expression* Expression);
statement* NewDeclarationStatement(declaration* Declaration);
statement* NewIfStatement(expression* Condition,
                          array<statement*> ThenBlock,
                          array<statement*> ElseBlock,
                          array<else_if> ElseIfs);
statement* NewForStatement(statement* Init,
                           expression* Condition,
                           statement* Next,
                           array<statement*> Statements);
statement* NewWhileStatement(expression* Condition, array<statement*> Statements);
statement* NewReturnStatement(expression* Expression);
statement* NewDoWhileStatement(expression* Condition, array<statement*> Statements);
statement* NewAssignStatement(expression* Left,
                              token_type AssignOp,
                              expression* Right);
declaration* NewDeclaration(declaration_type Type, u32 NameLength, char* Name);
declaration* NewVariableDeclaration(u32 TypeLength, char* Type,
                                    u32 NameLength, char* Name,
                                    expression* Expression);
declaration* NewFunctionDeclaration(u32 NameLength, char* Name,
                                    access_modifier Modifier, u32 ReturnTypeLength,
                                    char* ReturnType, array<function_param> Params,
                                    array<statement*> Statements);
declaration* NewClassDeclaration(u32 NameLength, char* Name,
                                    array<declaration*> Functions);
declaration* NewUsingDeclaration(u32 NameLength, char* Name);
#endif /* AST_H */
