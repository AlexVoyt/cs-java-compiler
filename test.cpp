#ifndef TEST_CPP
#define TEST_CPP

#include "parse.h"
#include "print.h"
#include "eval.h"

void TestLexer()
{
#if 0
    const char* Source = " a3 3a. , \' _ \"* \\ ___ _ds2_ a s ; 732 821 0229 32:3 a82_ [}]asd public 233213093298x2140;3(230)12";
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

void TestExpressionParsing()
{
    const char* Expressions[] =
    {
         "Fib(N, Value + 20, Fact(4)) == 20",
         "Value = 20",
         "double Value = 20",
         "Value + 20",
         "20",
         "Value",
         "-23 | 20 - (++0)",
        "(23 | 20) * 100 && 64 + 2 ^ (--17 - 89)",
        "2 + 2",
        "2 + 2 * 5",
        "2 * 2 + 5",
        "(2 + 2) * 5",
        "(13)",
        "-223 == 7",
        "-22",
        "223 == 7 + 5",
        "223 + 7 == 5",
        "7 > 5 <= 90",
    };

    for(u32 ExpressionIndex = 0;
        ExpressionIndex < ArrayCount(Expressions);
        ExpressionIndex++)
    {
        array<token> Tokens = Tokenize(Expressions[ExpressionIndex]);
        token* TokenStream = Tokens.Data;
        expression* Expression = ParseExpression(&TokenStream);
        PrintNewLine(Expression);
    }
}

void TestStatementParsing()
{
    const char* Statements[] =
    {
        "if(Value == 15) \
        {\
            X = 13 << Y;\
        }",

        "if(Value == 15) \
        {\
            Fib(Y-X);\
        }\
        else if (Z != false) \
        {\
        }\
        else\
        {\
            Fact(123);\
        }",

        "if(Value == 15) \
        {\
            Fib(Y-X);\
        }\
        else if (Z == true) \
        {\
        }\
        else if (Fact(14141)) \
        {\
        }\
        else\
        {\
            Fact(123);\
        }",

        "if(Value == 15)\
        {\
            Fib(Y-X);\
            int X = 20;\
            int Y = X+15*Value;\
            float Z;\
            X = Abs(-321);\
        }",

        "for(int Index = 0; Index < 100; ++Index)\
        {\
            X += Index;\
        }",

        "for(Index = 0; Index < 100; ++Index)\
        {\
            X += Index;\
        }",

        "for(Index = 0; Index < 100; Index += 3)\
        {\
            X += Index;\
        }",

        "while(Index >= 124)\
        {\
            SomeFunction();\
            Index -= AnotherFunction();\
        }",

        "do\
        {\
            SomeFunction();\
            Index -= AnotherFunction();\
        } while(Index >= 124);",

    };

    for(u32 StatementIndex = 0; StatementIndex < ArrayCount(Statements); StatementIndex++)
    {
        array<token> Tokens = Tokenize(Statements[StatementIndex]);
        token* TokenStream = Tokens.Data;
        statement* Statement = ParseStatement(&TokenStream);
        PrintNewLine(Statement);
    }
}

void TestDeclarationParsing()
{
    const char* Declarations[] =
    {
        "class Main\
        {\
            public void EndlessLoop() \
            {\
                int Index = 100 + 90;\
                long Variable = 3;\
                bool Hm;\
                while(Index > 0) \
                {\
                    Variable *= Index;\
                }\
            }\
            private long Fact(int n) \
            {\
                if(n == 1) \
                {\
                    return 1;\
                }\
                return n*Fact(n-1);\
            }\
        }",

        "class Another\
        {\
            private int GetElement(int Index)\
            {\
                return (Index >> 13);\
            }\
        }",
        "using System;",
        "using System.Math;",
        "using System.Console.Something.Class;",
    };

    for(u32 DeclarationIndex = 0; DeclarationIndex < ArrayCount(Declarations); DeclarationIndex++)
    {
        array<token> Tokens = Tokenize(Declarations[DeclarationIndex]);
        token* TokenStream = Tokens.Data;
        declaration* Declaration = ParseDeclaration(&TokenStream);
        PrintNewLine(Declaration);
    }
}

void TestCompleteProgram()
{
    const char* Programs[] =
    {
        "using System;\
        using System.Math;\
        using System.Console.Something.Class;\
        class Main\
        {\
            public void EndlessLoop() \
            {\
                int Index = 100 + 90;\
                long Variable = 3;\
                bool Hm;\
                while(Index > 0) \
                {\
                    Variable *= Index;\
                }\
            }\
            private long Fact(int n) \
            {\
                if(n == 1) \
                {\
                    return 1;\
                }\
                return n*Fact(n-1);\
            }\
        }\
        class Another\
        {\
            private int GetElement(int Index)\
            {\
                return (Index >> 13);\
            }\
        }",
    };

    for(u32 ProgramIndex = 0; ProgramIndex < ArrayCount(Programs); ProgramIndex++)
    {
        array<token> Tokens = Tokenize(Programs[ProgramIndex]);
        token* TokenStream = Tokens.Data;
        array<declaration*> Program;
        while(!MatchToken(TokenStream, TokenType_EndOfStream))
        {
            AddToArray(&Program, ParseDeclaration(&TokenStream));
        }

        for(u32 DeclarationIndex = 0; DeclarationIndex < Program.Size; DeclarationIndex++)
        {
            PrintNewLine(*(Program.Data + DeclarationIndex));
        }
    }

}

void TestParsing()
{
    TestExpressionParsing();
    TestStatementParsing();
    TestDeclarationParsing();
    TestCompleteProgram();
}

void TestExpressionEvaluation()
{
    printf("=== Test expression evaluation ===\n\n");
    const char* Expressions[] =
    {
        "2",
        "2 + 2",
        "2 + 2 * 5",
        "2 * 2 + 5",
        "(2 + 2) * 5",
        "(2 + Result) * 5",
        "(2 + 2) * Result",
    };

    for(u32 ExpressionIndex = 0; ExpressionIndex < ArrayCount(Expressions); ExpressionIndex++)
    {
        array<token> Tokens = Tokenize(Expressions[ExpressionIndex]);
        token* TokenStream = Tokens.Data;

        expression* Expression = ParseExpression(&TokenStream);
        printf("Original expression: ");
        PrintNewLine(Expression);

        expression* EvaluatedExpr = EvalExpr(Expression);
        printf("Evaluated expression: ");
        PrintNewLine(EvaluatedExpr);

        printf("====\n");
    }
}

void TestExpressionCodeGeneration()
{
    const char* Expressions[] =
    {
         "Fib(N, Value + 20, Fact(4)) == 20",
         "Value = 20",
         "Value + 20",
         "20",
         "Value",
         "-23 | 20 - (++0)",
        "(23 | 20) * 100 && 64 + 2 ^ (--17 - 89)",
        "2 + 2",
        "2 + 2 * 5",
        "2 * 2 + 5",
        "(2 + 2) * 5",
        "(13)",
        "-223 == 7",
        "-22",
        "223 == 7 + 5",
        "223 + 7 == 5",
        "7 > 5 <= 90",
    };

    for(u32 ExpressionIndex = 0;
        ExpressionIndex < ArrayCount(Expressions);
        ExpressionIndex++)
    {
        array<token> Tokens = Tokenize(Expressions[ExpressionIndex]);
        token* TokenStream = Tokens.Data;
        expression* Expression = ParseExpression(&TokenStream);

        Gen(Expression);
        GenLine();
    }
}

void TestStatementCodeGeneration()
{
    const char* Statements[] =
    {
        "if(Value == 15) \
        {\
            X = 13 << Y;\
        }",

        "if(Value == 15) \
        {\
            Fib(Y-X);\
        }\
        else if (Z != false) \
        {\
        }\
        else\
        {\
            Fact(123);\
        }",

        "if(Value == 15) \
        {\
            Fib(Y-X);\
        }\
        else if (Z == true) \
        {\
        }\
        else if (Fact(14141)) \
        {\
        }\
        else\
        {\
            Fact(123);\
        }",

        "if(Value == 15)\
        {\
            Fib(Y-X);\
            int X = 20;\
            int Y = X+15*Value;\
            float Z;\
            X = Abs(-321);\
        }",

        "for(int Index = 0; Index < 100; ++Index)\
        {\
            X += Index;\
        }",

        "for(Index = 0; Index < 100; ++Index)\
        {\
            X += Index;\
        }",

        "for(Index = 0; Index < 100; Index += 3)\
        {\
            X += Index;\
        }",

        "while(Index >= 124)\
        {\
            SomeFunction();\
            Index -= AnotherFunction();\
        }",

        "do\
        {\
            SomeFunction();\
            Index -= AnotherFunction();\
        } while(Index >= 124);",

    };

    for(u32 StatementIndex = 0; StatementIndex < ArrayCount(Statements); StatementIndex++)
    {
        array<token> Tokens = Tokenize(Statements[StatementIndex]);
        token* TokenStream = Tokens.Data;
        statement* Statement = ParseStatement(&TokenStream);

        Gen(Statement);
        GenLine();
    }
}

void TestDeclarationCodeGeneration()
{
    const char* Declarations[] =
    {
        "class Main\
        {\
            public void EndlessLoop() \
            {\
                int Index = 100 + 90;\
                long Variable = 3;\
                bool Hm;\
                while(Index > 0) \
                {\
                    Variable *= Index;\
                }\
            }\
            private long Fact(int n) \
            {\
                if(n == 1) \
                {\
                    return 1;\
                }\
                return n*Fact(n-1);\
            }\
        }",

        "class Another\
        {\
            private int GetElement(int Index)\
            {\
                return (Index >> 13);\
            }\
        }",
        "using System;",
        "using System.Math;",
        "using System.Console.Something.Class;",
    };

    for(u32 DeclarationIndex = 0; DeclarationIndex < ArrayCount(Declarations); DeclarationIndex++)
    {
        array<token> Tokens = Tokenize(Declarations[DeclarationIndex]);
        token* TokenStream = Tokens.Data;
        declaration* Declaration = ParseDeclaration(&TokenStream);

        Gen(Declaration);
        GenLine();
    }
}

void TestCompleteProgramCodeGeneration()
{
    const char* Programs[] =
    {
        "using System;\
        using System.Math;\
        using System.Console.Something.Class;\
        class Main\
        {\
            public void EndlessLoop() \
            {\
                int Index = 100 + 90;\
                long Variable = 3;\
                bool Hm;\
                while(Index > 0) \
                {\
                    Variable *= Index;\
                }\
                return;\
            }\
            private long Fact(int n, bool k) \
            {\
                if(n == 1) \
                {\
                    return 1;\
                }\
                return n*Fact(n-1);\
            }\
        }\
        class Another\
        {\
            private int GetElement(int Index)\
            {\
                return (Index >> 13);\
            }\
        }",
    };

    for(u32 ProgramIndex = 0; ProgramIndex < ArrayCount(Programs); ProgramIndex++)
    {
        array<token> Tokens = Tokenize(Programs[ProgramIndex]);
        token* TokenStream = Tokens.Data;
        array<declaration*> Program;
        while(!MatchToken(TokenStream, TokenType_EndOfStream))
        {
            AddToArray(&Program, ParseDeclaration(&TokenStream));
        }

        for(u32 DeclarationIndex = 0; DeclarationIndex < Program.Size; DeclarationIndex++)
        {
            Gen(*(Program.Data + DeclarationIndex));
            GenLine();
        }
    }

}

void TestCodeGeneration()
{
    TestExpressionCodeGeneration();
    TestStatementCodeGeneration();
    TestDeclarationCodeGeneration();
    TestCompleteProgramCodeGeneration();
}

#endif /* TEST_CPP */
