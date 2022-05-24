#if 0
enum declaration_type
{
    DeclarationType_None,
    DeclarationType_Variable,
    DeclarationType_Function,
    DeclarationType_Class,
    DeclarationType_Using,
};

enum access_modifier
{
    AccessModifier_Public,
    AccessModifier_Protected,
    AccessModifier_Private,
};

enum statement_type
{
    StatementType_Expression,
    StatementType_Assign,
    StatementType_Declaration,
    StatementType_If,
    StatementType_While,
    StatementType_DoWhile,
    StatementType_For,
    StatementType_Return,
};

enum expression_type
{
    ExpressionType_None,
    ExpressionType_Integer,
    ExpressionType_Float,
    ExpressionType_Name,
    ExpressionType_FunctionCall,
    /* ExpressionType_Field, */ // TODO: do we have structs? Methods are calls or fields?
    ExpressionType_Unary,
    ExpressionType_Binary,
    ExpressionType_Paren,
};
#endif

#define GenF(...) fprintf(stdout, __VA_ARGS__)
#define GenLineF(...) (GenLine(), GenF(__VA_ARGS__))

static u32 GenIndent = 0;

void GenLine()
{
    u32 IndentStrength = 4;
    GenF("\n");
    for(u32 i = 0; i < GenIndent*IndentStrength; i++)
    {
        GenF(" ");
    }
}

void Gen(statement* Stmt);
void Gen(declaration* Decl);
void Gen(expression* Expr);
void Gen(array<statement*> StatementBlock);

char* ToJavaType(u32 Length, char* Type)
{
    if(AreEqual(Type, Length, "int"))
    {
        return "int";
    }

    if(AreEqual(Type, Length, "bool"))
    {
        return "boolean";
    }

    if(AreEqual(Type, Length, "char"))
    {
        return "char";
    }

    if(AreEqual(Type, Length, "short"))
    {
        return "short";
    }
    if(AreEqual(Type, Length, "sbyte"))

    {
        return "byte";
    }

    if(AreEqual(Type, Length, "int"))
    {
        return "int";
    }

    if(AreEqual(Type, Length, "long"))
    {
        return "long";
    }

    if(AreEqual(Type, Length, "float"))
    {
        return "float";
    }

    if(AreEqual(Type, Length, "double"))
    {
        return "double";
    }

    if(AreEqual(Type, Length, "void"))
    {
        return "void";
    }

    InvalidCodePath("ToJavaType");
    return 0;
}

void Gen(expression* Expr)
{
    switch(Expr->Type)
    {
        case ExpressionType_None:
        {
            InvalidCodePath("Generate code for none expression");
        } break;

        case ExpressionType_Integer:
        {
            GenF("%lld", Expr->Integer);
        } break;

        case ExpressionType_Float:
        {
            GenF("%f", Expr->Float);
        } break;

        case ExpressionType_Name:
        {
            GenF("%.*s", Expr->Name.Length, Expr->Name.Ident);
        } break;

        case ExpressionType_FunctionCall:
        {
            GenF("%.*s", Expr->FunctionCall.Length, Expr->FunctionCall.Name);
            GenF("(");
            for(u32 ArgIndex = 0; ArgIndex < Expr->FunctionCall.Arguments.Size; ArgIndex++)
            {
                expression* Arg = *(Expr->FunctionCall.Arguments.Data + ArgIndex);
                Gen(Arg);
                if(ArgIndex != Expr->FunctionCall.Arguments.Size - 1)
                {
                    GenF(", ");
                }
            }
            GenF(")");
        } break;

        case ExpressionType_Unary:
        {
            GenF(TokenTypeStr(Expr->Unary.Op));
            Gen(Expr->Unary.Expression);
        } break;

        case ExpressionType_Binary:
        {
            Gen(Expr->Binary.Left);
            GenF(" %s ", TokenTypeStr(Expr->Binary.Op));
            Gen(Expr->Binary.Right);
        } break;

        case ExpressionType_Paren:
        {
            GenF("(");
            Gen(Expr->Paren.Expression);
            GenF(")");
        } break;
    }
}

void Gen(declaration* Decl)
{
    switch(Decl->Type)
    {
        case DeclarationType_None:
        {
        } break;

        case DeclarationType_Variable:
        {
            // TODO: generate java types, nor C#
            GenF("%s %.*s", ToJavaType(Decl->Variable.TypeLength, Decl->Variable.Type),
                            Decl->NameLength, Decl->Name);
            if(Decl->Variable.Expression)
            {
                GenF(" = ");
                Gen(Decl->Variable.Expression);
            }
            GenF(";");
        } break;

        case DeclarationType_Function:
        {
            // TODO: generate java types, nor C#
            GenF("%s %s %.*s(", ToString(Decl->Function.Modifier),
                                ToJavaType(Decl->Function.ReturnTypeLength, Decl->Function.ReturnType),
                                Decl->NameLength, Decl->Name);
            for(u32 ParamIndex = 0; ParamIndex < Decl->Function.Params.Size; ParamIndex++)
            {
                function_param Param = Decl->Function.Params.Data[ParamIndex];
                GenF("%.*s %.*s", Param.TypeLength, Param.Type, Param.NameLength, Param.Name);
                if(ParamIndex != Decl->Function.Params.Size - 1)
                {
                    GenF(", ");
                }
            }
            GenF(")");
            Gen(Decl->Function.Statements);
            GenLine();
        } break;

        case DeclarationType_Class:
        {
            GenF("class %.*s", Decl->NameLength, Decl->Name);
            GenLine();
            GenF("{");
            GenIndent++;
            GenLine();
            for(u32 FunctionIndex = 0; FunctionIndex < Decl->Class.Functions.Size; FunctionIndex++)
            {
                Gen(*(Decl->Class.Functions.Data + FunctionIndex));
                if(FunctionIndex != Decl->Class.Functions.Size - 1)
                {
                    GenLine();
                }
            }
            GenIndent--;
            GenLineF("}");
        } break;

        case DeclarationType_Using:
        {
            GenF("import %.*s;", Decl->NameLength, Decl->Name);
        } break;
    }
}

void Gen(array<statement*> StatementBlock)
{
    GenLineF("{");
    GenIndent++;
    GenLine();
    for(u32 StatementIndex = 0; StatementIndex < StatementBlock.Size; StatementIndex++)
    {
        Gen(*(StatementBlock.Data + StatementIndex));
        if(StatementIndex != StatementBlock.Size - 1)
        {
            GenLine();
        }
    }
    GenIndent--;
    GenLineF("}");
}

void Gen(statement* Stmt)
{
    switch(Stmt->Type)
    {
        case StatementType_Expression:
        {
            Gen(Stmt->Expression);
            GenF(";");
        } break;

        case StatementType_Assign:
        {
            Gen(Stmt->Assign.Left);
            GenF(" %s ", TokenTypeStr(Stmt->Assign.Op));
            Gen(Stmt->Assign.Right);
            GenF(";");
        } break;

        case StatementType_Declaration:
        {
            Gen(Stmt->Declaration);
        } break;

        case StatementType_If:
        {
            GenF("if(");
            Gen(Stmt->If.Condition);
            GenF(")");
            Gen(Stmt->If.ThenBlock);
            GenLine();

            for(u32 ElseIfIndex = 0; ElseIfIndex < Stmt->If.ElseIfs.Size; ElseIfIndex++)
            {
                else_if ElseIf = Stmt->If.ElseIfs.Data[ElseIfIndex];
                GenF("else if(");
                Gen(ElseIf.Condition);
                GenF(")");
                Gen(ElseIf.StatementBlock);
                GenLine();
            }

            if(Stmt->If.ElseBlock.Size > 0)
            {
                GenF("else ");
                Gen(Stmt->If.ElseBlock);
            }
        } break;

        case StatementType_While:
        {
            GenF("while(");
            Gen(Stmt->While.Condition);
            GenF(")");
            Gen(Stmt->While.Statements);
        } break;

        case StatementType_DoWhile:
        {
            GenF("do");
            Gen(Stmt->While.Statements);
            GenF(" while(");
            Gen(Stmt->While.Condition);
            GenF(");");
            GenLine();
        } break;

        case StatementType_For:
        {
            GenF("for(");
            Gen(Stmt->For.Init);
            GenF("; ");
            Gen(Stmt->For.Condition);
            GenF("; ");
            Gen(Stmt->For.Next);
            GenF(")");
            Gen(Stmt->For.Statements);
        } break;

        case StatementType_Return:
        {
            GenF("return");
            if(Stmt->Return.Expression)
            {
                GenF(" ");
                Gen(Stmt->Return.Expression);
            }
            GenF(";");
        } break;
    }
}
