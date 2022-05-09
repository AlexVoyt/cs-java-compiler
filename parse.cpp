#ifndef PARSE_CPP
#define PARSE_CPP

#include "parse.h"

expression* ParseExpression(token** TokenStream);
statement* ParseStatement(token** TokenStream);
array<statement*> ParseStatementBlock(token** TokenStream);

expression* ParseExpressionOperand(token** TokenStream)
{
    expression* Result;
    if(MatchToken(*TokenStream, TokenType_Int))
    {
        u64 Value = (*TokenStream)->Integer;
        NextToken(TokenStream);

        Result = NewIntegerExpression(Value);
    }
    else if(MatchToken(*TokenStream, TokenType_Identifier))
    {
        char* Name = (*TokenStream)->Content;
        u32 Length = (*TokenStream)->Length;
        NextToken(TokenStream);

        Result = NewNameExpression(Name, Length);
    }
    else if(MatchToken(*TokenStream, TokenType_LParen))
    {
        NextToken(TokenStream);
        Result = ParseExpression(TokenStream);
        ExpectToken(TokenStream, TokenType_RParen);
        Result = NewParenExpression(Result);
    }
    else
    {
        FatalError("Error in ParseExpressionOperand: expected expression, but none find");
        assert(0);
    }

    return Result;
}

expression* ParseExpressionBase(token** TokenStream)
{
    expression* Result = ParseExpressionOperand(TokenStream);
    while((*TokenStream)->Type == TokenType_LParen ||
          (*TokenStream)->Type == TokenType_Dot)
    {
        if(MatchToken(*TokenStream, TokenType_LParen))
        {
            array<expression*> Arguments;
            NextToken(TokenStream);
            if(!MatchToken(*TokenStream, TokenType_RParen))
            {
                AddToArray(&Arguments, ParseExpression(TokenStream));
                while(MatchToken(*TokenStream, TokenType_Comma))
                {
                    NextToken(TokenStream);
                    AddToArray(&Arguments, ParseExpression(TokenStream));
                }
            }
            ExpectToken(TokenStream, TokenType_RParen);

            assert(Result->Type == ExpressionType_Name);
            Result = NewFunctionCallExpression(Result, Arguments);
        }
        else if(MatchToken(*TokenStream, TokenType_Dot))
        {
            NotDone("Parse expression dot");
        }
        else
        {
            InvalidCodePath("ParseExpression");
        }
    }

    return Result;
}

expression* ParseExpressionUnary(token** TokenStream)
{
    if(IsUnaryOp(*TokenStream))
    {
        token_type Op = (*TokenStream)->Type;
        NextToken(TokenStream);
        return NewUnaryExpression(Op, ParseExpressionUnary(TokenStream));
    }
    else
    {
        return ParseExpressionBase(TokenStream);
    }
}

expression* ParseExpressionMul(token** TokenStream)
{
    expression* Result = ParseExpressionUnary(TokenStream);
    while(IsMulOp(*TokenStream))
    {
        token_type Op = (*TokenStream)->Type;
        NextToken(TokenStream);
        Result = NewBinaryExpression(Op, Result, ParseExpressionUnary(TokenStream));
    }
    return Result;
}

expression* ParseExpressionAdd(token** TokenStream)
{
    expression* Result = ParseExpressionMul(TokenStream);
    while(IsAddOp(*TokenStream))
    {
        token_type Op = (*TokenStream)->Type;
        NextToken(TokenStream);
        Result = NewBinaryExpression(Op, Result, ParseExpressionMul(TokenStream));
    }
    return Result;
}

expression* ParseExpressionShift(token** TokenStream)
{
    expression* Result = ParseExpressionAdd(TokenStream);
    while(IsShiftOp(*TokenStream))
    {
        token_type Op = (*TokenStream)->Type;
        NextToken(TokenStream);
        Result = NewBinaryExpression(Op, Result, ParseExpressionAdd(TokenStream));
    }
    return Result;
}

expression* ParseExpressionCompare(token** TokenStream)
{
    expression* Result = ParseExpressionShift(TokenStream);
    while(IsCompareOp(*TokenStream))
    {
        token_type Op = (*TokenStream)->Type;
        NextToken(TokenStream);
        Result = NewBinaryExpression(Op, Result, ParseExpressionShift(TokenStream));
    }
    return Result;
}

expression* ParseExpressionBitAnd(token** TokenStream)
{
    expression* Result = ParseExpressionCompare(TokenStream);
    while(MatchToken(*TokenStream, TokenType_BitAnd))
    {
        NextToken(TokenStream);
        Result = NewBinaryExpression(TokenType_BitAnd, Result, ParseExpressionCompare(TokenStream));
    }
    return Result;
}

expression* ParseExpressionBitXor(token** TokenStream)
{
    expression* Result = ParseExpressionBitAnd(TokenStream);
    while(MatchToken(*TokenStream, TokenType_BitXor))
    {
        NextToken(TokenStream);
        Result = NewBinaryExpression(TokenType_BitXor, Result, ParseExpressionBitAnd(TokenStream));
    }
    return Result;
}

expression* ParseExpressionBitOr(token** TokenStream)
{
    expression* Result = ParseExpressionBitXor(TokenStream);
    while(MatchToken(*TokenStream, TokenType_BitOr))
    {
        NextToken(TokenStream);
        Result = NewBinaryExpression(TokenType_BitOr, Result, ParseExpressionBitXor(TokenStream));
    }
    return Result;
}

expression* ParseExpressionLogicalAnd(token** TokenStream)
{
    expression* Result = ParseExpressionBitOr(TokenStream);
    while(MatchToken(*TokenStream, TokenType_LogicalAnd))
    {
        NextToken(TokenStream);
        Result = NewBinaryExpression(TokenType_LogicalAnd, Result, ParseExpressionBitOr(TokenStream));
    }
    return Result;
}

expression* ParseExpressionLogicalOr(token** TokenStream)
{
    expression* Result = ParseExpressionLogicalAnd(TokenStream);
    while(MatchToken(*TokenStream, TokenType_LogicalOr))
    {
        NextToken(TokenStream);
        Result = NewBinaryExpression(TokenType_LogicalOr, Result, ParseExpressionLogicalAnd(TokenStream));
    }
    return Result;
}

expression* ParseExpression(token** TokenStream)
{
    return ParseExpressionLogicalOr(TokenStream);
}
/*      Declarations      */

declaration* ParseDeclaration(token** TokenStream);

declaration* ParseVariableDeclaration(token** TokenStream)
{
    u32 TypeLength = (*TokenStream)->Length;
    char* Type = (*TokenStream)->Content;
    NextToken(TokenStream);

    u32 NameLength = (*TokenStream)->Length;
    char* Name = (*TokenStream)->Content;
    NextToken(TokenStream);

    expression* Expression = 0;
    if(!MatchToken(*TokenStream, TokenType_Semicolon))
    {
        ExpectToken(TokenStream, TokenType_Assign);
        Expression = ParseExpression(TokenStream);
    }

    declaration* Result = NewVariableDeclaration(TypeLength, Type, NameLength, Name, Expression);

    return Result;
}

array<function_param> ParseFunctionParams(token** TokenStream)
{
    array<function_param> Result = {};
    function_param Param = {};

    ExpectToken(TokenStream, TokenType_LParen);
    while(!MatchToken(*TokenStream, TokenType_EndOfStream) &&
          !MatchToken(*TokenStream, TokenType_RParen))
    {
        if(MatchType(*TokenStream))
        {
            Param.TypeLength = (*TokenStream)->Length;
            Param.Type = (*TokenStream)->Content;
            NextToken(TokenStream);

            if((*TokenStream)->Type != TokenType_Identifier)
            {
                FatalError("Expected paramaeter name in function parameter list");
            }

            Param.NameLength = (*TokenStream)->Length;
            Param.Name = (*TokenStream)->Content;
            NextToken(TokenStream);

            if(!MatchToken(*TokenStream, TokenType_RParen))
            {
                ExpectToken(TokenStream, TokenType_Comma);
                if(MatchToken(*TokenStream, TokenType_RParen))
                {
                    FatalError("Expected parameter after comma in function parameter list");
                }
            }
        }
        else
        {
            FatalError("Expected type name in function parameter list");
        }
        AddToArray(&Result, Param);
    }
    ExpectToken(TokenStream, TokenType_RParen);

    return Result;
}

declaration* ParseFunctionDeclaration(token** TokenStream)
{
    declaration* Result = 0;

    u32 NameLength = 0;
    char* Name = 0;
    access_modifier Modifier;
    u32 ReturnTypeLength = 0;
    char* ReturnType = 0;
    array<function_param> Params = {};
    array<statement*> Statements = {};

    bool IsPublic = AreEqual((*TokenStream)->Content, (*TokenStream)->Length, "public");
    bool IsProtected = AreEqual((*TokenStream)->Content, (*TokenStream)->Length, "protected");
    bool IsPrivate = AreEqual((*TokenStream)->Content, (*TokenStream)->Length, "private");
    if(IsPublic)
    {
        Modifier = AccessModifier_Public;
    }
    else if(IsProtected)
    {
        Modifier = AccessModifier_Protected;
    }
    else if(IsPrivate)
    {
        Modifier = AccessModifier_Private;
    }
    else
    {
        FatalError("Expected access modifier");
        InvalidCodePath("Expected access modifier");
    }
    NextToken(TokenStream);

    if(MatchType(*TokenStream) || MatchKeyword(*TokenStream, "void"))
    {
        ReturnType = (*TokenStream)->Content;
        ReturnTypeLength = (*TokenStream)->Length;
    }
    else
    {
        FatalError("Expected return type");
    }
    NextToken(TokenStream);

    Name = (*TokenStream)->Content;
    NameLength = (*TokenStream)->Length;
    NextToken(TokenStream);

    Params = ParseFunctionParams(TokenStream);
    Statements = ParseStatementBlock(TokenStream);

    Result = NewFunctionDeclaration(NameLength, Name,
                                    Modifier, ReturnTypeLength,
                                    ReturnType, Params, Statements);

    return Result;
}

declaration* ParseUsingDeclaration(token** TokenStream)
{
    declaration* Result = 0;
    u32 Length = (*TokenStream)->Length;
    char* Name = (*TokenStream)->Content;
    ExpectToken(TokenStream, TokenType_Identifier);

    while(!MatchToken(*TokenStream, TokenType_EndOfStream) &&
          !MatchToken(*TokenStream, TokenType_Semicolon))
    {
        if(MatchToken(*TokenStream, TokenType_Dot))
        {
            NextToken(TokenStream);
            Length += 1 + (*TokenStream)->Length;
            ExpectToken(TokenStream, TokenType_Identifier);
        }
    }

    ExpectToken(TokenStream, TokenType_Semicolon);

    Result = NewUsingDeclaration(Length, Name);

    return Result;
}

declaration* ParseClassDeclaration(token** TokenStream)
{
    declaration* Result = 0;
    u32 ClassNameLength = 0;
    char* ClassName = 0;
    array<declaration*> Functions = {};

    ClassName = (*TokenStream)->Content;
    ClassNameLength = (*TokenStream)->Length;
    ExpectToken(TokenStream, TokenType_Identifier);
    ExpectToken(TokenStream, TokenType_LBrace);

    while(!MatchToken(*TokenStream, TokenType_EndOfStream) &&
          !MatchToken(*TokenStream, TokenType_RBrace))
    {
        AddToArray(&Functions, ParseFunctionDeclaration(TokenStream));
    }

    ExpectToken(TokenStream, TokenType_RBrace);

    Result = NewClassDeclaration(ClassNameLength, ClassName, Functions);
    return Result;
}

declaration* ParseDeclaration(token** TokenStream)
{
    declaration* Result = 0;

    if(MatchKeyword(*TokenStream, "class"))
    {
        NextToken(TokenStream);
        Result = ParseClassDeclaration(TokenStream);
    }
    else if(MatchKeyword(*TokenStream, "using"))
    {
        NextToken(TokenStream);
        Result = ParseUsingDeclaration(TokenStream);
    }

    if(!Result)
    {
        FatalError("Unexpected declaration: require using or class keywords");
    }
    assert(Result);
    return Result;
}

/*      Statements      */

statement* ParseStatement(token** TokenStream);

array<statement*> ParseStatementBlock(token** TokenStream)
{
    array<statement*> Result = {};
    ExpectToken(TokenStream, TokenType_LBrace);
    while(!MatchToken(*TokenStream, TokenType_EndOfStream) &&
          !MatchToken(*TokenStream, TokenType_RBrace))
    {
        AddToArray(&Result, ParseStatement(TokenStream));
    }
    ExpectToken(TokenStream, TokenType_RBrace);

    return Result;
}

statement* ParseIfStatement(token** TokenStream)
{
    ExpectToken(TokenStream, TokenType_LParen);
    expression* Condition = ParseExpression(TokenStream);
    ExpectToken(TokenStream, TokenType_RParen);

    array<statement*> ThenBlock = ParseStatementBlock(TokenStream);
    array<statement*> ElseBlock = {};
    array<else_if> ElseIfs = {};
    while(MatchKeyword(*TokenStream, "else"))
    {
        NextToken(TokenStream);
        if(MatchKeyword(*TokenStream, "if"))
        {
            NextToken(TokenStream);
            ExpectToken(TokenStream, TokenType_LParen);
            expression* ElseIfCond = ParseExpression(TokenStream);
            ExpectToken(TokenStream, TokenType_RParen);
            array<statement*> ElseIfBlock = ParseStatementBlock(TokenStream);

            else_if ElseIf = {};
            ElseIf.Condition = ElseIfCond;
            ElseIf.StatementBlock = ElseIfBlock;
            AddToArray(&ElseIfs, ElseIf);
        }
        else
        {
            ElseBlock = ParseStatementBlock(TokenStream) ;
            break;
        }
    }

    statement* Result = NewIfStatement(Condition, ThenBlock, ElseBlock, ElseIfs);
    return Result;
}

statement* ParseExpressionStatement(token** TokenStream)
{
    statement* Result = 0;
    Result = NewExpressionStatement(ParseExpression(TokenStream));

    return Result;
}

statement* ParseDeclarationStatement(token** TokenStream)
{
    statement* Result = 0;
    Result = NewDeclarationStatement(ParseVariableDeclaration(TokenStream));

    return Result;
}

statement* ParseAssignStatement(token** TokenStream)
{
    statement* Result = 0;

    expression* Left = ParseExpression(TokenStream);

    assert(IsAssignOp(*TokenStream));
    token_type AssignOp = (*TokenStream)->Type;
    NextToken(TokenStream);

    expression* Right = ParseExpression(TokenStream);

    Result = NewAssignStatement(Left, AssignOp, Right);

    return Result;
}

statement* ParseSimpleStatement(token** TokenStream)
{
    statement* Result = 0;

    if(MatchType(*TokenStream))
    {
        Result = ParseDeclarationStatement(TokenStream);
    }
    else
    {
        token* SearchAssignStream = *TokenStream + 1;
        if(IsAssignOp(SearchAssignStream))
        {
            Result = ParseAssignStatement(TokenStream);
        }
        else
        {
            Result = ParseExpressionStatement(TokenStream);
        }
    }

    return Result;
}

statement* ParseForStatement(token** TokenStream)
{
    statement* Result = 0;

    statement* Init = 0;
    expression* Condition = 0;
    statement* Next = 0;
    array<statement*> Statements = {};

    ExpectToken(TokenStream, TokenType_LParen);

    Init = ParseSimpleStatement(TokenStream);
    ExpectToken(TokenStream, TokenType_Semicolon);

    Condition = ParseExpression(TokenStream);
    ExpectToken(TokenStream, TokenType_Semicolon);

    Next = ParseSimpleStatement(TokenStream);

    ExpectToken(TokenStream, TokenType_RParen);
    Statements = ParseStatementBlock(TokenStream);

    Result = NewForStatement(Init, Condition, Next, Statements);

    return Result;
}

statement* ParseWhileStatement(token** TokenStream)
{
    statement* Result = 0;

    expression* Condition = 0;
    array<statement*> Statements = {};

    ExpectToken(TokenStream, TokenType_LParen);
    Condition = ParseExpression(TokenStream);
    ExpectToken(TokenStream, TokenType_RParen);

    Statements = ParseStatementBlock(TokenStream);

    Result = NewWhileStatement(Condition, Statements);

    return Result;
}

statement* ParseDoWhileStatement(token** TokenStream)
{
    statement* Result = 0;

    expression* Condition = 0;
    array<statement*> Statements = {};

    Statements = ParseStatementBlock(TokenStream);

    if(!MatchKeyword(*TokenStream, "while"))
    {
        FatalError("Expected while keyword after do block");
    }
    NextToken(TokenStream);

    ExpectToken(TokenStream, TokenType_LParen);
    Condition = ParseExpression(TokenStream);
    ExpectToken(TokenStream, TokenType_RParen);
    ExpectToken(TokenStream, TokenType_Semicolon);

    Result = NewDoWhileStatement(Condition, Statements);

    return Result;
}

statement* ParseStatement(token** TokenStream)
{
    statement* Result = 0;
    if(MatchKeyword(*TokenStream, "if"))
    {
        NextToken(TokenStream);
        Result = ParseIfStatement(TokenStream);
    }
    else if(MatchKeyword(*TokenStream, "for"))
    {
        NextToken(TokenStream);
        Result = ParseForStatement(TokenStream);
    }
    else if(MatchKeyword(*TokenStream, "while"))
    {
        NextToken(TokenStream);
        Result = ParseWhileStatement(TokenStream);
    }
    else if(MatchKeyword(*TokenStream, "do"))
    {
        NextToken(TokenStream);
        Result = ParseDoWhileStatement(TokenStream);
    }
    else if(MatchKeyword(*TokenStream, "return"))
    {
        NextToken(TokenStream);
        expression* Expression = 0;
        if(!MatchToken(*TokenStream, TokenType_Semicolon))
        {
            Expression = ParseExpression(TokenStream);
        }
        ExpectToken(TokenStream, TokenType_Semicolon);

        Result = NewReturnStatement(Expression);
    }
    else
    {
        Result = ParseSimpleStatement(TokenStream);
        ExpectToken(TokenStream, TokenType_Semicolon);
    }

    return Result;
}

#endif /* PARSE_CPP */
