void TestLexer()
{
#if 0
    // const char* Source = " a3 3a. , \' _ \"* \\ ___ _ds2_ a s ; 732 821 0229 32:3 a82_ [}]asd public 233213093298x2140;3(230)12";
    const char* Source = ReadFile("test.cs");
    // printf("Source string: %s\n", Source);

    std::vector<token> Tokens;
    tokenizer Tokenizer;
    Tokenizer.At = (char* )Source;
    Tokenizer.Line = 1;
    bool Lexing = true;
    while(Lexing)
    {
        token Token = GetToken(&Tokenizer);
        if(Token.Type == TokenType_EndOfStream)
            Lexing = false;
        Tokens.push_back(Token);
    }
#endif
}

void TestExpressionPrinting()
{
    expression* Expressions[] =
    {
        NewBinaryExpression(TokenType_Plus, NewIntegerExpression(123), NewBinaryExpression(TokenType_Asterisk, NewIntegerExpression(666), NewIntegerExpression(999))),
        NewBinaryExpression(TokenType_Plus, NewBinaryExpression(TokenType_Asterisk, NewIntegerExpression(111), NewIntegerExpression(222)), NewBinaryExpression(TokenType_Plus, NewIntegerExpression(333), NewIntegerExpression(444))),
        NewBinaryExpression(TokenType_Plus, NewFloatExpression(3.14), NewFloatExpression(2.71)),
    };

    for(u32 ExpressionIndex = 0; ExpressionIndex < ArrayCount(Expressions); ExpressionIndex++)
    {
        PrintNewLine(Expressions[ExpressionIndex]);
    }

}

void TestDeclarationPrinting()
{
#if 0
    declaration* Declarations[] =
    {
        NewVariableDeclaration()
    }
#endif
}

void TestExpressionParsing()
{
    char* Expressions[] =
    {
         "Fib(N, Value + 20, Fact(4)) == 20",
         "Value = 20",
         "double Value = 20",
         "Value + 20",
         "20",
         "Value",
         "-23 | 20 - (++0)",
        // "(23 | 20",
        "(23 | 20) * 100 && 64 + 2 ^ (--17 - 89)",
        // "2++", // TODO: postdecrement
        "2 + 2",
        "2 + 2 * 5",
        "2 * 2 + 5",
        "(2 + 2) * 5",
        "(13)",
        // TODO(float) "7.32 > 5 <= 90",
        "-223 == 7",
        "-22",
        "223 == 7 + 5",
        "223 + 7 == 5",
        "7 > 5 <= 90",
    };

    for(u32 ExpressionIndex = 0; ExpressionIndex < ArrayCount(Expressions); ExpressionIndex++)
    {
        array<token> Tokens = Tokenize(Expressions[ExpressionIndex]);
        token* TokenStream = Tokens.Data;
        expression* Expression = ParseExpression(&TokenStream);
        PrintNewLine(Expression);
    }
}

void TestStatementParsing()
{
    char* Statements[] =
    {
        "if(Value == 15) {23+4;}",
    };

    for(u32 StatementIndex = 0; StatementIndex < ArrayCount(Statements); StatementIndex++)
    {
        array<token> Tokens = Tokenize(Statements[StatementIndex]);
        token* TokenStream = Tokens.Data;
        statement* Statement = ParseStatement(&TokenStream);
        PrintNewLine(Statement);
    }
}

void TestParsing()
{
    TestExpressionParsing();
    TestStatementParsing();
}


