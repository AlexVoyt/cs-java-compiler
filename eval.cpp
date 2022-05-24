#ifndef EVAL_CPP
#define EVAL_CPP

#include "eval.h"

expression* EvalExpr(expression* Expr)
{
    expression* Result = 0;
    switch(Expr->Type)
    {
        case ExpressionType_Integer:
        {
            Result = Expr;
        } break;

        case ExpressionType_Name:
        {
            Result = Expr;
        } break;

        case ExpressionType_FunctionCall:
        {
            // TODO: hacky
            expression* Temp = NewNameExpression(Expr->FunctionCall.Name, Expr->FunctionCall.Length);
            array<expression*> EvaluatedExprs = {};

            for(u32 ArgIndex = 0; ArgIndex < Expr->FunctionCall.Arguments.Size; ArgIndex++)
            {
                AddToArray(&EvaluatedExprs, EvalExpr(Expr->FunctionCall.Arguments.Data[ArgIndex]));
            }

            Result = NewFunctionCallExpression(Temp, EvaluatedExprs);
        } break;

        case ExpressionType_Paren:
        {
            Result = EvalExpr(Expr->Paren.Expression);
        } break;

        case ExpressionType_Unary:
        {
            Result = Expr;
        } break;

        case ExpressionType_Binary:
        {
            expression* Left = EvalExpr(Expr->Binary.Left);
            expression* Right = EvalExpr(Expr->Binary.Right);

            if(Left->Type == ExpressionType_Integer &&
               Right->Type == ExpressionType_Integer)
            {
                u64 EvalResult = 0;
                switch(Expr->Binary.Op)
                {
                    case TokenType_Plus:
                    {
                        EvalResult = Left->Integer + Right->Integer;
                    } break;

                    case TokenType_Minus:
                    {
                        EvalResult = Left->Integer - Right->Integer;
                    } break;

                    case TokenType_Mul:
                    {
                        EvalResult = Left->Integer * Right->Integer;
                    } break;

                    case TokenType_Div:
                    {
                        EvalResult = Left->Integer / Right->Integer;
                    } break;

                    default:
                    {
                        InvalidCodePath("Binary operator");
                    } break;
                }

                Result = NewIntegerExpression(EvalResult);
            }
            else
            {
                Result = NewBinaryExpression(Expr->Binary.Op, Left, Right);
            }

        } break;

        default:
        {
            // assert(0);
        } break;
    }

    return Result;
}

void PerformExprEvaluation(declaration* Decl);
void PerformExprEvaluation(statement* Stmt);
void PerformExprEvaluation(array<statement*> StmtBlock);

void PerformExprEvaluation(statement* Stmt)
{
    switch(Stmt->Type)
    {
        case StatementType_Expression:
        {
            Stmt->Expression = EvalExpr(Stmt->Expression);
        } break;

        case StatementType_Assign:
        {
            Stmt->Assign.Right = EvalExpr(Stmt->Assign.Right);
        } break;

        case StatementType_Declaration:
        {
            PerformExprEvaluation(Stmt->Declaration);
        } break;

        case StatementType_If:
        {
            Stmt->If.Condition = EvalExpr(Stmt->If.Condition);
            PerformExprEvaluation(Stmt->If.ThenBlock);
            PerformExprEvaluation(Stmt->If.ElseBlock);

            for(u32 ElseIfIndex = 0; ElseIfIndex < Stmt->If.ElseIfs.Size; ElseIfIndex++)
            {
                else_if* ElseIf = &Stmt->If.ElseIfs.Data[ElseIfIndex];
                ElseIf->Condition = EvalExpr(ElseIf->Condition);
                PerformExprEvaluation(ElseIf->StatementBlock);
            }

        } break;

        case StatementType_While:
        {
            Stmt->While.Condition = EvalExpr(Stmt->While.Condition);
            PerformExprEvaluation(Stmt->While.Statements);
        } break;

        case StatementType_For:
        {
            PerformExprEvaluation(Stmt->For.Init);
            Stmt->For.Condition = EvalExpr(Stmt->For.Condition);
            PerformExprEvaluation(Stmt->For.Next);
            PerformExprEvaluation(Stmt->For.Statements);
        } break;

        case StatementType_Return:
        {
            if(Stmt->Return.Expression)
            {
                Stmt->Return.Expression = EvalExpr(Stmt->Return.Expression);
            }
        } break;
    }
}

void PerformExprEvaluation(array<statement*> StmtBlock)
{
    for(u32 StatementIndex = 0; StatementIndex < StmtBlock.Size; StatementIndex++)
    {
        statement* Stmt = StmtBlock.Data[StatementIndex];
        PerformExprEvaluation(Stmt);
    }
}

void PerformExprEvaluation(declaration* Decl)
{
    switch(Decl->Type)
    {
        case DeclarationType_None:
        {
        } break;

        case DeclarationType_Variable:
        {
            if(Decl->Variable.Expression)
            {
                Decl->Variable.Expression = EvalExpr(Decl->Variable.Expression);
            }
        } break;

        case DeclarationType_Function:
        {
            for(u32 StatementIndex = 0; StatementIndex < Decl->Function.Statements.Size; StatementIndex++)
            {
                statement* Stmt = Decl->Function.Statements.Data[StatementIndex];
                PerformExprEvaluation(Stmt);
            }
        } break;

        case DeclarationType_Class:
        {
            for(u32 FunctionIndex = 0; FunctionIndex < Decl->Class.Functions.Size; FunctionIndex++)
            {
                declaration* Func = Decl->Class.Functions.Data[FunctionIndex];
                PerformExprEvaluation(Func);
            }
        } break;

        default:
        {
            // assert(0);
        } break;
    }
}



#endif /* EVAL_CPP */
