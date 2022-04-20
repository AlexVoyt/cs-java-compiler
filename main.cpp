#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#define global_variable static

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

#define str(x) #x
#define xstr(x) str(x)
#define ArrayCount(x) sizeof((x))/sizeof((x)[0])

#include "term.h"
#include "util.cpp"
#include "lexer.cpp"
#include "ast.cpp"
#include "parse.cpp"
#include "print.cpp"
#include "test.cpp"
#include "semantic.cpp"
#include "eval.cpp"


int main(int argc, char** argv)
{

#if 0
    if(argc != 2)
    {
        printf("usage: compiler [--test] [filename]\n");
        return 1;
    }

    if(strcmp(argv[1], "--test") == 0)
    {
        TestParsing();
        return 0;
    }

    char* FileContent = ReadFile(argv[1]);
    array<token> Tokens = Tokenize(FileContent);
    token* TokenStream = Tokens.Data;
    array<declaration*> Program;

    while(!MatchToken(TokenStream, TokenType_EndOfStream))
    {
        AddToArray(&Program, ParseDeclaration(&TokenStream));
    }

    for(u32 DeclarationIndex = 0; DeclarationIndex < Program.Size; DeclarationIndex++)
    {
        declaration* Decl = *(Program.Data + DeclarationIndex);
        if(Decl->Type == DeclarationType_Class)
        {
            for(u32 FunctionIndex = 0; FunctionIndex < Decl->Class.Functions.Size; FunctionIndex++)
            {
                declaration* Func = *(Decl->Class.Functions.Data + FunctionIndex);
                AddToArray(&KnownFunctions, Func);
                PerformSemAnalyse(Func);
                // PrintNewLine(Func);
            }
        }
    }
#endif
    char* Expressions[] =
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
        PrintNewLine(Expression);
            expression* EvaluatedExpr = EvalExpr(Expression);
            PrintNewLine(EvaluatedExpr);
        printf("\n");
    }



    printf("\n");
    return 0;
}
