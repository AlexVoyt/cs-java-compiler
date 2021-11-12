enum expression_type
{
    ExpressionType_IntegerLiteral,
};

struct expression
{
    expression_type Type;
    union
    {
        struct
        {
            u64 Value;
        } IntegerLiteral;
    };
};

enum declaration_type
{
    DeclarationType_Variable,
};

struct declaration
{
    declaration_type Type;
    char* Name;
    u32 NameLength;
    union
    {
        struct
        {
            char* Type; 
            expression* Expr;
        } VariableDeclaration;
    };
};

// TODO: do we need this function? It's probably easier to just increment
// pointer than to use this(???)
void NextToken(token** Stream)
{
    (*Stream)++;
}

bool MatchToken(token* Token, token_type Type)
{
    if(Token->Type == Type)
        return true;
    else
        return false;
}

// TODO: this function should check current token and assert that it 
// has certain type, otherwise we delete our program
void ExpectToken(token** Token, token_type ExpectedType)
{
    if(MatchToken(*Token, ExpectedType))
    {
        NextToken(Token);
    }
    else
    {
        // TODO: FatalError should be printf with exit
        // FatalError("Expect %s, got %s", ExpectedType, (*Token)->Type);
        FatalError("In expect token");
    }
}

// TODO: for now just return IntegerLiteral
expression* ParseExpression(token* TokenStream)
{
    expression* Result = (expression *)malloc(sizeof(*Result));
    Result->Type = ExpressionType_IntegerLiteral;
    Result->IntegerLiteral.Value = TokenStream->Integer;

    return Result;
}

declaration* ParseVariableDeclaration(token* TokenStream)
{
    declaration* Result = (declaration* )malloc(sizeof(*Result));
    Result->Type = DeclarationType_Variable;

    Result->VariableDeclaration.Type = TokenStream->Content;
    NextToken(&TokenStream);

    Result->Name = TokenStream->Content;
    Result->NameLength = TokenStream->Length;
    NextToken(&TokenStream);

    ExpectToken(&TokenStream, TokenType_Assign);
    Result->VariableDeclaration.Expr = ParseExpression(TokenStream);

    NextToken(&TokenStream);
    ExpectToken(&TokenStream, TokenType_Semicolon);

    return Result;
}
