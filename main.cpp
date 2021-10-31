#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#define InvalidCodePath assert(0)

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#include "term.h"
#include "lexer.cpp"

// TODO: error handling
char* ReadFile(const char* Filename)
{
    char* Result = 0;
    FILE* File = fopen(Filename, "r");
    assert(File);

    fseek(File, 0, SEEK_END);
    u32 FileLength = ftell(File);
    fseek(File, 0, SEEK_SET);

    Result = (char*)malloc(FileLength + 1);
    assert(Result);
    fread(Result, FileLength, 1, File);
    Result[FileLength] = 0;

    fclose(File);
    return Result;
}

int main()
{
    // const char* Source = " a3 3a. , \' _ \"* \\ ___ _ds2_ a s ; 732 821 0229 32:3 a82_ [}]asd public 233213093298x2140;3(230)12";
    const char* Source = ReadFile("test2.cs");
    printf("Source string: %s\n", Source);

    tokenizer Tokenizer;
    Tokenizer.At = (char*)Source;
    Tokenizer.Line = 1;
    bool Lexing = true;
    while (Lexing)
    {
        token Token = GetToken(&Tokenizer);
        PrintToken(&Token);
        if (Token.Type == TokenType_EndOfStream)
            Lexing = false;
    }

    return 0;
}
