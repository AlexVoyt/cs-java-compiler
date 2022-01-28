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

expression* NewUnaryExpression(token_type Op, expression* Expression)
{
    expression* Result = NewExpression(ExpressionType_Unary);
    Result->Unary.Op = Op;
    Result->Unary.Expression = Expression;
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

expression* NewParenExpression(expression* Expression)
{
    expression* Result = NewExpression(ExpressionType_Paren);
    Result->Paren.Expression = Expression;
    return Result;
}

expression* NewNameExpression(char* Name, u32 Length)
{
    expression* Result = NewExpression(ExpressionType_Name);
    Result->Name.Ident = Name;
    Result->Name.Length = Length;
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

#if 1
expression* NewFunctionCallExpression(expression* Expression, array<expression*> Arguments)
{
    assert(Expression->Type == ExpressionType_Name);

    expression* Result = NewExpression(ExpressionType_FunctionCall);
    Result->FunctionCall.Name = Expression->Name.Ident;
    Result->FunctionCall.Length = Expression->Name.Length;
    Result->FunctionCall.Arguments = Arguments;

    return Result;
}
#endif

declaration* NewDeclaration(declaration_type Type)
{
    declaration* Result = (declaration* )AllocateNode(sizeof(declaration));
    Result->Type = Type;
    return Result;
}

#if 0
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
#endif

statement* NewStatement(statement_type Type)
{
    statement* Result = (statement* )AllocateNode(sizeof(statement));
    Result->Type = Type;
    return Result;
}

statement* NewExpressionStatement(expression* Expression)
{
    statement* Result = NewStatement(StatementType_Expression);
    Result->Expression = Expression;
    return Result;
}

statement* NewDeclarationStatement(declaration* Declaration)
{
    statement* Result = NewStatement(StatementType_Declaration);
    Result->Declaration = Declaration;

    return Result;
}

statement* NewIfStatement(expression* Condition,
                          array<statement*> ThenBlock,
                          array<statement*> ElseBlock,
                          array<else_if> ElseIfs)
{
    statement* Result = NewStatement(StatementType_If);
    Result->If.Condition = Condition;
    Result->If.ThenBlock = ThenBlock;
    Result->If.ElseBlock = ElseBlock;
    Result->If.ElseIfs = ElseIfs;

    return Result;
}
statement* NewForStatement(statement* Init,
                           expression* Condition,
                           statement* Next,
                           array<statement*> Statements)
{
    statement* Result = NewStatement(StatementType_For);
    Result->For.Init = Init;
    Result->For.Condition = Condition;
    Result->For.Next = Next;
    Result->For.Statements = Statements;

    return Result;
}

statement* NewWhileStatement(expression* Condition, array<statement*> Statements)
{
    statement* Result = NewStatement(StatementType_While);
    Result->While.Condition = Condition;
    Result->While.Statements = Statements;

    return Result;
}

statement* NewReturnStatement(expression* Expression)
{
    statement* Result = NewStatement(StatementType_Return);
    Result->Return.Expression = Expression;

    return Result;
}

statement* NewDoWhileStatement(expression* Condition, array<statement*> Statements)
{
    statement* Result = NewStatement(StatementType_DoWhile);
    Result->While.Condition = Condition;
    Result->While.Statements = Statements;

    return Result;
}

statement* NewAssignStatement(expression* Left,
                              token_type AssignOp,
                              expression* Right)
{
    statement* Result = NewStatement(StatementType_Assign);
    Result->Assign.Left = Left;
    Result->Assign.Op = AssignOp;
    Result->Assign.Right = Right;

    return Result;
}

declaration* NewDeclaration(declaration_type Type, u32 NameLength, char* Name)
{
    declaration* Result = (declaration* )AllocateNode(sizeof(declaration));
    Result->Type = Type;
    Result->NameLength = NameLength;
    Result->Name = Name;
    return Result;
}

declaration* NewVariableDeclaration(u32 TypeLength, char* Type,
                                    u32 NameLength, char* Name,
                                    expression* Expression)
{
    declaration* Result = NewDeclaration(DeclarationType_Variable, NameLength, Name);
    Result->Variable.TypeLength = TypeLength;
    Result->Variable.Type = Type;
    Result->Variable.Expression = Expression;

    return Result;
}

declaration* NewFunctionDeclaration(u32 NameLength, char* Name,
                                    access_modifier Modifier, u32 ReturnTypeLength,
                                    char* ReturnType, array<function_param> Params,
                                    array<statement*> Statements)
{
    declaration* Result = NewDeclaration(DeclarationType_Function, NameLength, Name);
    Result->Function.Modifier = Modifier;
    Result->Function.ReturnTypeLength = ReturnTypeLength;
    Result->Function.ReturnType = ReturnType;
    Result->Function.Params = Params;
    Result->Function.Statements = Statements;

    return Result;
}

declaration* NewClassDeclaration(u32 NameLength, char* Name,
                                    array<declaration*> Functions)
{
    declaration* Result = NewDeclaration(DeclarationType_Class, NameLength, Name);
    Result->Class.Functions = Functions;

    return Result;
}

declaration* NewUsingDeclaration(u32 NameLength, char* Name)
{
    declaration* Result = NewDeclaration(DeclarationType_Using, NameLength, Name);

    return Result;
}
