#if 0
void PrintIndent(u32 IndentLevel)
{
    printf("\n");
    for(u32 i = 0; i < IndentLevel; i++)
    {
        printf("    ");
    }
}
#endif

void Print(expression* Expr, u32 IndentLevel = 0)
{
    assert(Expr && "Print Expr");
    switch(Expr->Type)
    {
        case ExpressionType_Binary:
        {
            // TODO:
            printf("(%s ", TokenTypeStr(Expr->Binary.Op));
            // PrintIndent(IndentLevel + 1);
            Print(Expr->Binary.Left, IndentLevel + 1);
            printf(" ");
            // PrintIndent(IndentLevel + 1);
            Print(Expr->Binary.Right, IndentLevel + 1);
            printf(")");
        } break;

        case ExpressionType_Unary:
        {
            // TODO:
            printf("(%s ", TokenTypeStr(Expr->Binary.Op));
            // PrintIndent(IndentLevel + 1);
            Print(Expr->Unary.Expression, IndentLevel + 1);
            printf(")");
        } break;

        case ExpressionType_Paren:
        {
            // TODO:
            // PrintIndent(IndentLevel + 1);
            Print(Expr->Paren.Expression, IndentLevel + 1);
        } break;

        case ExpressionType_Integer:
        {
            printf("%Iu", Expr->Integer);
        } break;

        case ExpressionType_Float:
        {
            printf("%f", Expr->Float);
        } break;

        case ExpressionType_Name:
        {
            printf("%.*s", Expr->Name.Length, Expr->Name.Ident);
        } break;

        case ExpressionType_FunctionCall:
        {
            printf("(");
            printf("%.*s", Expr->FunctionCall.Length, Expr->FunctionCall.Name);

            for(u32 ArgumentIndex = 0; ArgumentIndex < Expr->FunctionCall.Arguments.Size; ArgumentIndex++)
            {
                printf(" ");
                Print(*(Expr->FunctionCall.Arguments.Data + ArgumentIndex));
            }
            printf(")");
        } break;
    }
}

void Print(declaration* Decl)
{
    assert(Decl && "Print Decl");
    switch(Decl->Type)
    {
        case DeclarationType_Variable:
        {
            printf("[Variable %.*s ", Decl->NameLength, Decl->Name);
            Print(Decl->Variable.Expression);
            printf("]\n");
        } break;

        default:
        {
            InvalidCodePath("Print declaration");
        }
    }
}

void Print(statement* Stmt)
{
    assert(Stmt);
    switch(Stmt->Type)
    {
        case StatementType_If:
        {
            printf("(if ");
            Print(Stmt->If.Condition);
            printf(" (then-block ");
            for(u32 StatementIndex = 0; StatementIndex < Stmt->If.ThenBlock.Size; StatementIndex++)
            {
                Print(*(Stmt->If.ThenBlock.Data + StatementIndex));
            }
            printf(")");
            printf(")");
        } break;

        case StatementType_Declaration:
        {
            printf("(var ");
            printf("%.*s %.*s", Stmt->Declaration->Variable.TypeLength, Stmt->Declaration->Variable.Type,
                                Stmt->Declaration->NameLength, Stmt->Declaration->Name);
            if(Stmt->Declaration->Variable.Expression)
            {
                printf("(init-expr ");
                Print(Stmt->Declaration->Variable.Expression);
                printf(")");
            }
            printf(")");
        }

        case StatementType_Expression:
        {
            printf("(expr ");
            Print(Stmt->Expression);
            printf(")");
        }

        default:
        {
            InvalidCodePath("Print statement");
        }

    }
}

void PrintNewLine(expression* Expr)
{
    Print(Expr);
    printf("\n");
}

void PrintNewLine(statement* Stmt)
{
    Print(Stmt);
    printf("\n");
}

void PrintNewLine(declaration* Decl)
{
    Print(Decl);
    printf("\n");
}

