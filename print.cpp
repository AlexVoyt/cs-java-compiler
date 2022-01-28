// TODO: Maybe this should not be static
static u32 Indent = 0;
void PrintStatementBlock(array<statement*> Block);

#if 1
void PrintIndent()
{
    u32 IndentStrength = 4;
    printf("\n");
    for(u32 i = 0; i < Indent*IndentStrength; i++)
    {
        printf(" ");
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
            printf("(var %.*s %.*s ", Decl->Variable.TypeLength, Decl->Variable.Type, Decl->NameLength, Decl->Name);
            if(Decl->Variable.Expression)
            {
                Print(Decl->Variable.Expression);
            }
            printf(")");
        } break;

        case DeclarationType_Function:
        {
            printf("(%s fun %.*s %.*s ", ToString(Decl->Function.Modifier),
                                         Decl->Function.ReturnTypeLength, Decl->Function.ReturnType,
                                         Decl->NameLength, Decl->Name);
            printf("(");
            for(u32 ParamIndex = 0; ParamIndex < Decl->Function.Params.Size; ParamIndex++)
            {
                function_param Param = Decl->Function.Params.Data[ParamIndex];
                printf(" %.*s %.*s ", Param.TypeLength, Param.Type, Param.NameLength, Param.Name);
            }
            printf(" ) ");
            Indent++;
            PrintIndent();
            PrintStatementBlock(Decl->Function.Statements);
            Indent--;
            printf(")");
        } break;

        case DeclarationType_Class:
        {
            printf("(class %.*s ", Decl->NameLength, Decl->Name);
            Indent++;
            for(u32 FunctionIndex = 0; FunctionIndex < Decl->Class.Functions.Size; FunctionIndex++)
            {
                PrintIndent();
                Print(*(Decl->Class.Functions.Data + FunctionIndex));
            }
            Indent--;
            printf(")");
        } break;

        case DeclarationType_Using:
        {
            printf("(using %.*s )", Decl->NameLength, Decl->Name);
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
            PrintIndent();
            PrintStatementBlock(Stmt->If.ThenBlock);
            for(u32 ElseIfIndex = 0; ElseIfIndex < Stmt->If.ElseIfs.Size; ElseIfIndex++)
            {
                else_if ElseIf = Stmt->If.ElseIfs.Data[ElseIfIndex];
                PrintIndent();
                printf("else if ");
                Print(ElseIf.Condition);
                PrintIndent();
                PrintStatementBlock(ElseIf.StatementBlock);
            }

            if(Stmt->If.ElseBlock.Size > 0)
            {
                PrintIndent();
                printf("else ");
                PrintIndent();
                PrintStatementBlock(Stmt->If.ElseBlock);
            }

        } break;

        case StatementType_Declaration:
        {
            Print(Stmt->Declaration);
        } break;

        case StatementType_Expression:
        {
            printf("(expr ");
            Print(Stmt->Expression);
            printf(")");
        } break;

        case StatementType_For:
        {
            printf("(for ");
            Print(Stmt->For.Init);
            printf(" ");
            Print(Stmt->For.Condition);
            printf(" ");
            Print(Stmt->For.Next);
            PrintIndent();
            PrintStatementBlock(Stmt->For.Statements);
            printf(")");
        } break;

        case StatementType_While:
        {
            printf("(while ");
            Print(Stmt->While.Condition);
            printf(" ");
            PrintIndent();
            PrintStatementBlock(Stmt->While.Statements);
            printf(")");
        } break;

        case StatementType_DoWhile:
        {
            printf("(do ");
            PrintIndent();
            PrintStatementBlock(Stmt->While.Statements);
            PrintIndent();
            printf("(while ");
            Print(Stmt->While.Condition);
            printf(")");
        } break;

        case StatementType_Assign:
        {
            printf("(%s ", TokenTypeStr(Stmt->Assign.Op));
            Print(Stmt->Assign.Left);
            printf(" ");
            Print(Stmt->Assign.Right);
            printf(")");
        } break;

        case StatementType_Return:
        {
            printf("(return ");
            if(Stmt->Return.Expression)
            {
                Print(Stmt->Return.Expression);
            }
            printf(")");
        } break;

        default:
        {
            InvalidCodePath("Print statement");
        }

    }
}

void PrintStatementBlock(array<statement*> Block)
{
    printf("(statement-block");
    Indent++;
    for(u32 StatementIndex = 0; StatementIndex < Block.Size; StatementIndex++)
    {
        PrintIndent();
        Print(*(Block.Data + StatementIndex));
    }
    Indent--;
    printf(")");
}

void PrintNewLine(expression* Expr)
{
    Print(Expr);
    printf("\n\n");
}

void PrintNewLine(statement* Stmt)
{
    Print(Stmt);
    printf("\n\n");
}

void PrintNewLine(declaration* Decl)
{
    Print(Decl);
    printf("\n\n");
}

