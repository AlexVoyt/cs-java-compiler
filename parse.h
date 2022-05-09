#ifndef PARSE_H
#define PARSE_H

#include "ast.h"

/*      Expressions      */

expression* ParseExpression(token** TokenStream);
statement* ParseStatement(token** TokenStream);
array<statement*> ParseStatementBlock(token** TokenStream);
expression* ParseExpressionOperand(token** TokenStream);
expression* ParseExpressionBase(token** TokenStream);
expression* ParseExpressionUnary(token** TokenStream);
expression* ParseExpressionMul(token** TokenStream);
expression* ParseExpressionAdd(token** TokenStream);
expression* ParseExpressionShift(token** TokenStream);
expression* ParseExpressionCompare(token** TokenStream);
expression* ParseExpressionBitAnd(token** TokenStream);
expression* ParseExpressionBitXor(token** TokenStream);
expression* ParseExpressionBitOr(token** TokenStream);
expression* ParseExpressionLogicalAnd(token** TokenStream);
expression* ParseExpressionLogicalOr(token** TokenStream);
expression* ParseExpression(token** TokenStream);

/*      Declarations      */

declaration* ParseDeclaration(token** TokenStream);
declaration* ParseVariableDeclaration(token** TokenStream);
array<function_param> ParseFunctionParams(token** TokenStream);
declaration* ParseFunctionDeclaration(token** TokenStream);
declaration* ParseUsingDeclaration(token** TokenStream);
declaration* ParseClassDeclaration(token** TokenStream);
declaration* ParseDeclaration(token** TokenStream);

/*      Statements      */

statement* ParseStatement(token** TokenStream);
array<statement*> ParseStatementBlock(token** TokenStream);
statement* ParseIfStatement(token** TokenStream);
statement* ParseExpressionStatement(token** TokenStream);
statement* ParseDeclarationStatement(token** TokenStream);
statement* ParseAssignStatement(token** TokenStream);
statement* ParseSimpleStatement(token** TokenStream);
statement* ParseForStatement(token** TokenStream);
statement* ParseWhileStatement(token** TokenStream);
statement* ParseDoWhileStatement(token** TokenStream);
statement* ParseStatement(token** TokenStream);

#endif /* PARSE_H */
