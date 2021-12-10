#include "ast.h"

// TODO: better assertions
void* AllocateNode(size_t size)
{
    assert(size > 0);
    void* Result = calloc(1, size);
    assert(Result);
    return Result;
}

/* ---------- Expressions ---------- */

expression* NewExpression(expression_type Type)
{
    expression* Result = (expression* )AllocateNode(sizeof(expression));
    Result->Type = Type;
    return Result;
}

expression* NewBinaryExpression(token_type Op, expression* Left, expression* Right)
{
    // TODO:
    // assert(IsBinary(Op));
    expression* Result = NewExpression(ExpressionType_Binary);
    Result->Binary.Op = Op;
    Result->Binary.Left = Left;
    Result->Binary.Right = Right;
    return Result;
}

expression* NewIntegerExpression(u64 Value)
{
    expression* Result = NewExpression(ExpressionType_Integer);
    Result->Integer = Value;
    return Result;
}

expression* NewFloatExpression(f64 Value)
{
    expression* Result = NewExpression(ExpressionType_Float);
    Result->Float = Value;
    return Result;
}

// TODO:
#if 1
expression* NewFunctionCallExpression(char* FunctionName, expression* E0 = 0,  expression* E1 = 0, expression* E2 = 0)
{
    expression* Result = NewExpression(ExpressionType_FunctionCall);
    Result->FunctionCall.Name = FunctionName;
    Result->FunctionCall.Params[0] = E0;
    Result->FunctionCall.Params[1] = E1;
    Result->FunctionCall.Params[2] = E2;
    return Result;
}
#endif

declaration* NewDeclaration(declaration_type Type)
{
    declaration* Result = (declaration* )AllocateNode(sizeof(declaration));
    Result->Type = Type;
    return Result;
}






// TODO: for now just return IntegerLiteral
expression* ParseIntegerExpression(token* TokenStream)
{
    expression* Result = NewIntegerExpression(TokenStream->Integer);

    return Result;
}

#if 0

expression* ParseExpressionAdd(token* TokenStream)
{
}

expression* ParseExpressionMul(token* TokenStream)
{
}
#endif

declaration* ParseVariableDeclaration(token* TokenStream)
{
    // TODO: refactor
    declaration* Result = (declaration* )malloc(sizeof(*Result));
    Result->Type = DeclarationType_Variable;

    Result->Variable.Type = TokenStream->Content;
    NextToken(&TokenStream);

    Result->Name = TokenStream->Content;
    Result->NameLength = TokenStream->Length;
    NextToken(&TokenStream);

    ExpectToken(&TokenStream, TokenType_Assign);
    Result->Variable.Expr = ParseIntegerExpression(TokenStream);

    NextToken(&TokenStream);
    ExpectToken(&TokenStream, TokenType_Semicolon);

    return Result;
}
