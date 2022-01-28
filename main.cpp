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


int main(int argc, char** argv)
{

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
#if 0
    for(u32 i = 0; i < Tokens.Size; i++)
    {
        printf("%.*s %s\n", Tokens.Data[i].Length, Tokens.Data[i].Content, TokenTypeStr(Tokens.Data[i].Type));
    }
#endif

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



    printf("\n");
    return 0;
}
