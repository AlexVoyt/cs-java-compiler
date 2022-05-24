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
#include "optimizer.cpp"

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

#if 1
    array<expression*> FunctionsCalledFromMain = {};
    GetFunctionCalls(Main->Function.Statements, &FunctionsCalledFromMain,
                     Program.Functions);
    // Print(FunctionsCalledFromMain);

    array<declaration*> FunctionsToGenerate = {};
    AddToArray(&FunctionsToGenerate, Main);

    // TODO: Hacky
    declaration* Class = Program.Declarations.Data[0];
    assert(Class->Type == DeclarationType_Class);
    for(u32 CallIndex = 0; CallIndex < FunctionsCalledFromMain.Size; CallIndex++)
    {
        expression* Call = FunctionsCalledFromMain.Data[CallIndex];
        bool ToGenerate = true;
        u32 FuncIndex = 0;
        declaration* Decl = GetDeclFromCall(Class->Class.Functions, Call);

        // PrintNewLine(Decl);
        for(u32 GenerateIndex = 0; GenerateIndex < FunctionsToGenerate.Size; GenerateIndex++)
        {
            declaration* GenDecl = FunctionsToGenerate.Data[GenerateIndex];
            if(AreEqual(GenDecl->Name, GenDecl->NameLength,
                        Decl->Name, Decl->NameLength))
            {
                ToGenerate = false;
            }
        }
#if 0
        printf("%d ", ToGenerate);
        printf("%d ", FuncIndex);
        printf("%d ", FunctionsToGenerate.Size);
#endif
        // PrintNewLine(Decl);
        if(ToGenerate)
        {
            if(!AreEqual(Decl->Name, Decl->NameLength, "Main"))
            {
                AddToArray(&FunctionsToGenerate, Decl);
            }
        }
        else
        {
            // Already in to be generated
        }
    }

    // Print(FunctionsToGenerate);
#endif

    Program.Declarations.Data[0]->Class.Functions = FunctionsToGenerate;
    for(u32 DeclIndex = 0; DeclIndex < Program.Declarations.Size; DeclIndex++)
    {
        PerformExprEvaluation(Program.Declarations.Data[DeclIndex]);
        Gen(Program.Declarations.Data[DeclIndex]);
    }

    printf("\n");
    return 0;
}
