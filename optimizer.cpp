declaration* GetDeclFromCall(array<declaration*> Functions, expression* Call)
{
    assert(Call->Type == ExpressionType_FunctionCall);
    declaration* Result = 0;
    for(u32 DeclIndex = 0; DeclIndex < Functions.Size; DeclIndex++)
    {
        declaration* Decl = Functions.Data[DeclIndex];
        if(AreEqual(Call->FunctionCall.Name, Call->FunctionCall.Length,
                    Decl->Name, Decl->NameLength))
        {
            Result = Decl;
            break;
        }
    }

    assert(Result);
    return Result;
}

void GetFunctionCalls(array<statement*> StatementBlock, array<expression*>* Calls, array<declaration*> Decls);

void GetFunctionCalls(expression* Expr, array<expression*>* Calls, array<declaration*> Decls)
{
    if(Expr->Type == ExpressionType_FunctionCall)
    {
        // PrintNewLine(Expr);
        AddToArray(Calls, Expr);
        declaration* Decl = GetDeclFromCall(Decls, Expr);
        GetFunctionCalls(Decl->Function.Statements, Calls, Decls);
    }
}

void GetFunctionCalls(statement* Statement, array<expression*>* Calls, array<declaration*> Decls)
{
    switch(Statement->Type)
    {
        case StatementType_Expression:
        {
            if(Statement->Expression->Type == ExpressionType_FunctionCall)
            {
                // PrintNewLine(Statement);
                AddToArray(Calls, Statement->Expression);
                declaration* Decl = GetDeclFromCall(Decls, Statement->Expression);
                GetFunctionCalls(Decl->Function.Statements, Calls, Decls);
            }
        } break;

        case StatementType_Assign:
        {
            GetFunctionCalls(Statement->Assign.Right, Calls, Decls);
        } break;

        case StatementType_Declaration:
        {
            if(Statement->Declaration->Type == DeclarationType_Variable)
            {
                if(Statement->Declaration->Variable.Expression)
                {
                    GetFunctionCalls(Statement->Declaration->Variable.Expression, Calls, Decls);
                }
            }
        } break;

        case StatementType_If:
        {
            GetFunctionCalls(Statement->If.Condition, Calls, Decls);
            GetFunctionCalls(Statement->If.ThenBlock, Calls, Decls);
            GetFunctionCalls(Statement->If.ElseBlock, Calls, Decls);
            for(u32 ElseIfIndex = 0; ElseIfIndex < Statement->If.ElseIfs.Size; ElseIfIndex++)
            {
                GetFunctionCalls(Statement->If.ElseIfs.Data[ElseIfIndex].Condition, Calls, Decls);
                GetFunctionCalls(Statement->If.ElseIfs.Data[ElseIfIndex].StatementBlock, Calls, Decls);
            }

        } break;

        case StatementType_While:
        {
            GetFunctionCalls(Statement->While.Condition, Calls, Decls);
            GetFunctionCalls(Statement->While.Statements, Calls, Decls);
        } break;

        case StatementType_DoWhile:
        {
            GetFunctionCalls(Statement->While.Condition, Calls, Decls);
            GetFunctionCalls(Statement->While.Statements, Calls, Decls);
        } break;

        case StatementType_For:
        {
            GetFunctionCalls(Statement->For.Init, Calls, Decls);
            GetFunctionCalls(Statement->For.Condition, Calls, Decls);
            GetFunctionCalls(Statement->For.Next, Calls, Decls);
            GetFunctionCalls(Statement->For.Statements, Calls, Decls);
        } break;

        case StatementType_Return:
        {
            if(Statement->Return.Expression)
            {
                GetFunctionCalls(Statement->Return.Expression, Calls, Decls);
            }
        } break;

    }
}

void GetFunctionCalls(array<statement*> StatementBlock, array<expression*>* Calls, array<declaration*> Decls)
{
    for(u32 StatementIndex = 0; StatementIndex < StatementBlock.Size; StatementIndex++)
    {
        statement* Statement = StatementBlock.Data[StatementIndex];
        GetFunctionCalls(Statement, Calls, Decls);
    }
}
