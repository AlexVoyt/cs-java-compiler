void PrintIndent(u32 IndentLevel)
{
    printf("\n");
    for(u32 i = 0; i < IndentLevel; i++)
    {
        printf("    ");
    }
}

void Print(expression* Expr, u32 IndentLevel = 0)
{
    assert(Expr);
    switch(Expr->Type)
    {
        case ExpressionType_Binary:
        {
            // TODO: 
            printf("[Binary %s ", TokenTypeStr(Expr->Binary.Op));
            PrintIndent(IndentLevel + 1);
            Print(Expr->Binary.Left, IndentLevel + 1);
            printf(" ");
            PrintIndent(IndentLevel + 1);
            Print(Expr->Binary.Right, IndentLevel + 1);
            printf("]");
        } break;

        case ExpressionType_Integer:
        {
            printf("%ld", Expr->Integer.Value);
        } break;
    }
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
