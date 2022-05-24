#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <set>

#include "term.h"
#include "utils.cpp"
#include "lexer.cpp"
#include "ast.cpp"
#include "parse.cpp"
#include "print.cpp"
#include "semantic.cpp"
#include "eval.cpp"
#include "codegen.cpp"

#include "test.cpp"
struct program
{
    array<declaration*> Declarations;
    array<declaration*> Functions;
};

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
        TestExpressionEvaluation();
        TestCodeGeneration();
        return 0;
    }

    program Program;

    char* FileContent = ReadFile(argv[1]);
    array<token> Tokens = Tokenize(FileContent);
    token* TokenStream = Tokens.Data;
    bool SemOk = true;

    while(!MatchToken(TokenStream, TokenType_EndOfStream))
    {
        declaration* Decl = ParseDeclaration(&TokenStream);
        AddToArray(&Program.Declarations, Decl);

        if(Decl->Type == DeclarationType_Class)
        {
            for(u32 FunctionIndex = 0; FunctionIndex < Decl->Class.Functions.Size; FunctionIndex++)
            {
                declaration* Func = *(Decl->Class.Functions.Data + FunctionIndex);
                AddToArray(&KnownFunctions, Func);
                SemOk = SemOk && PerformSemAnalyse(Func);
                AddToArray(&Program.Functions, Func);
            }
        }
    }

    if(!SemOk)
    {
        FatalError("Errors during semantic analysis");
    }

    bool MainFound = false;
    declaration* Main = 0;

    for(u32 DeclIndex = 0; DeclIndex < Program.Declarations.Size; DeclIndex++)
    {
        declaration* Decl = Program.Declarations.Data[DeclIndex];
        if(Decl->Type != DeclarationType_Class)
            continue;

        for(u32 FunctionIndex = 0; FunctionIndex < Decl->Class.Functions.Size; FunctionIndex++)
        {
            declaration* Func = Decl->Class.Functions.Data[FunctionIndex];
            if(!AreEqual(Func->Name, Func->NameLength, "Main"))
                continue;

            MainFound = true;
            Main = Func;
        }
    }

    if(!MainFound)
    {
        FatalError("Main not found");
    }

    for(u32 DeclIndex = 0; DeclIndex < Program.Declarations.Size; DeclIndex++)
    {
        PerformExprEvaluation(Program.Declarations.Data[DeclIndex]);
        Gen(Program.Declarations.Data[DeclIndex]);
    }

    printf("\n");
    return 0;
}
