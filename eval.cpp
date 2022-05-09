#ifndef EVAL_CPP
#define EVAL_CPP

#include "eval.h"

expression* EvalExpr(expression* Expr)
{
    expression* Result = {};
    switch(Expr->Type)
    {
        case ExpressionType_Integer:
        {
            Result = NewIntegerExpression(Expr->Integer);
        } break;

        case ExpressionType_Name:
        {
            Result = NewNameExpression(Expr->Name.Ident, Expr->Name.Length);
        } break;

        case ExpressionType_Paren:
        {
            Result = EvalExpr(Expr->Paren.Expression);
        } break;

        case ExpressionType_Unary:
        {
            // TODO
            assert(0 && "Not Implemented");
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


#endif /* EVAL_CPP */
