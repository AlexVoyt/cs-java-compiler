expression* ParseExpression(token** TokenStream);

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

bool IsUnaryOp(token* TokenStream)
{
    token_type Type = TokenStream->Type;
    return (Type == TokenType_Increment) ||
           (Type == TokenType_Decrement) ||
           (Type == TokenType_Minus) ||
           (Type == TokenType_Plus);
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

bool IsMulOp(token* TokenStream)
{
    token_type Type = TokenStream->Type;
    return (Type == TokenType_Asterisk) ||
           (Type == TokenType_Slash) ||
           (Type == TokenType_Percent);
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

bool IsAddOp(token* TokenStream)
{
    token_type Type = TokenStream->Type;
    return (Type == TokenType_Plus) ||
           (Type == TokenType_Minus);
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

bool IsShiftOp(token* TokenStream)
{
    token_type Type = TokenStream->Type;
    return (Type == TokenType_ShiftLeft) ||
           (Type == TokenType_ShiftRight);
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

bool IsCompareOp(token* TokenStream)
{
    token_type Type = TokenStream->Type;
    return (Type == TokenType_Greater) ||
           (Type == TokenType_Lesser) ||
           (Type == TokenType_Equal) ||
           (Type == TokenType_GEQ) ||
           (Type == TokenType_LEQ) ||
           (Type == TokenType_NEQ);
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

declaration* ParseDeclaration(token** TokenStream)
{
    declaration* Result = 0;


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
        if(MatchKeyword(*TokenStream, "if"))
        {
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

statement* ParseSimpleStatement(token** TokenStream)
{
    statement* Result = 0;

    if(MatchType(*TokenStream))
    {
        Result = ParseDeclarationStatement(TokenStream);
    }
    else
    {
        Result = ParseExpressionStatement(TokenStream);
    }

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
        NotDone("Statemtnt for");
    }
    else
    {
        Result = ParseSimpleStatement(TokenStream);
        ExpectToken(TokenStream, TokenType_Semicolon);
    }

    return Result;
}







