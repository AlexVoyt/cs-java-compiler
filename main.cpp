#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <vector>
#define InvalidCodePath assert(0)
#define global_variable static

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

const u64 UnsignedLongMax = 9223372036854775807;
void FatalError(const char* ErrorMsg)
{
    printf("%s\n", ErrorMsg);
    exit(1);
}

#include "term.h"
#include "lexer.cpp"
#include "ast.cpp"


// TODO: error handling
char* ReadFile(const char* Filename)
{
    char* Result = 0;
    FILE* File = fopen(Filename, "r");
    assert(File);

    fseek(File, 0, SEEK_END);
    u32 FileLength = ftell(File);
    fseek(File, 0, SEEK_SET);

    Result = (char* )malloc(FileLength + 1);
    assert(Result);
    fread(Result, FileLength, 1, File);
    Result[FileLength] = 0;

    fclose(File);
    return Result;
}

struct keyword_table
{
};

global_variable keyword_table KeywordTable;


using namespace std;
int main()
{
    // const char* Source = " a3 3a. , \' _ \"* \\ ___ _ds2_ a s ; 732 821 0229 32:3 a82_ [}]asd public 233213093298x2140;3(230)12";
    const char* Source = ReadFile("test.cs");
    // printf("Source string: %s\n", Source);

    vector<token> Tokens;
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

#if 0
    for(token Token : Tokens)
        if(Token.Type == TokenType_Int)
            PrintToken(&Token);
#endif

    declaration* Decl = ParseVariableDeclaration(Tokens.data());
    printf("Type: %d\n", Decl->Type);
    printf("Name: %.*s\n", Decl->NameLength, Decl->Name);
    printf("Expression: \n");
    printf("{\n");
    printf("    Type: %d\n", Decl->VariableDeclaration.Expr->Type);
    printf("    Value: %ld\n", Decl->VariableDeclaration.Expr->IntegerLiteral.Value);
    printf("}\n");

    return 0;
}
