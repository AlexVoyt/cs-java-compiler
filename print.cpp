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
    assert(Expr);
    switch(Expr->Type)
    {
        case ExpressionType_Binary:
        {
            // TODO:
            printf("[%s ", TokenTypeStr(Expr->Binary.Op));
            // PrintIndent(IndentLevel + 1);
            Print(Expr->Binary.Left, IndentLevel + 1);
            printf(" ");
            // PrintIndent(IndentLevel + 1);
            Print(Expr->Binary.Right, IndentLevel + 1);
            printf("]");
        } break;

        case ExpressionType_Integer:
        {
            printf("%Iu", Expr->Integer);
        } break;

        case ExpressionType_Float:
        {
            printf("%f", Expr->Float);
        } break;

        case ExpressionType_FunctionCall:
        {
            printf("[Called %s", Expr->FunctionCall.Name);
            // TODO: change once we decide what should we use as function params
            for(u32 i = 0; i < 3; i++)
            {
                if(Expr->FunctionCall.Params[i])
                {
                    printf(" ");
                    Print(Expr->FunctionCall.Params[i]);
                }
            }
            printf("]");
        } break;
    }
}

void PrintNewLine(expression* Expr)
{
    Print(Expr);
    printf("\n");
}

void Print(declaration* Decl)
{
    assert(Decl);
    switch(Decl->Type)
    {
        case DeclarationType_Variable:
        {
            printf("[Variable %.*s ", Decl->NameLength, Decl->Name);
            Print(Decl->Variable.Expr);
            printf("]\n");
        } break;
    }
}
