#ifndef PRINT_H
#define PRINT_H

#include "utils.h"
#include "ast.h"

void PrintStatementBlock(array<statement*> Block);
void PrintIndent();
void Print(expression* Expr, u32 IndentLevel);
void Print(declaration* Decl);
void Print(statement* Stmt);
void PrintStatementBlock(array<statement*> Block);
void PrintNewLine(expression* Expr);
void PrintNewLine(statement* Stmt);
void PrintNewLine(declaration* Decl);

#endif /* PRINT_H */
