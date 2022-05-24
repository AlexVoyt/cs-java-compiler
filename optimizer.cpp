void GetFunctionCalls(expression* Expr, array<expression*>* Calls)
{
    if(Expr->Type == ExpressionType_FunctionCall)
    {
        // PrintNewLine(Expr);
        AddToArray(Calls, Expr);
    }
}

void GetFunctionCalls(array<statement*> StatementBlock, array<expression*>* Calls);

void GetFunctionCalls(statement* Statement, array<expression*>* Calls)
{
    switch(Statement->Type)
    {
        case StatementType_Expression:
        {
            if(Statement->Expression->Type == ExpressionType_FunctionCall)
            {
                // PrintNewLine(Statement);
                AddToArray(Calls, Statement->Expression);
            }
        } break;

        case StatementType_Assign:
        {
            GetFunctionCalls(Statement->Assign.Right, Calls);
        } break;

        case StatementType_Declaration:
        {
            if(Statement->Declaration->Type == DeclarationType_Variable)
            {
                if(Statement->Declaration->Variable.Expression)
                {
                    GetFunctionCalls(Statement->Declaration->Variable.Expression, Calls);
                }
            }
        } break;

        case StatementType_If:
        {
            GetFunctionCalls(Statement->If.Condition, Calls);
            GetFunctionCalls(Statement->If.ThenBlock, Calls);
            GetFunctionCalls(Statement->If.ElseBlock, Calls);
            for(u32 ElseIfIndex = 0; ElseIfIndex < Statement->If.ElseIfs.Size; ElseIfIndex++)
            {
                GetFunctionCalls(Statement->If.ElseIfs.Data[ElseIfIndex].Condition, Calls);
                GetFunctionCalls(Statement->If.ElseIfs.Data[ElseIfIndex].StatementBlock, Calls);
            }

        } break;

        case StatementType_While:
        {
            GetFunctionCalls(Statement->While.Condition, Calls);
            GetFunctionCalls(Statement->While.Statements, Calls);
        } break;

        case StatementType_DoWhile:
        {
            GetFunctionCalls(Statement->While.Condition, Calls);
            GetFunctionCalls(Statement->While.Statements, Calls);
        } break;

        case StatementType_For:
        {
            GetFunctionCalls(Statement->For.Init, Calls);
            GetFunctionCalls(Statement->For.Condition, Calls);
            GetFunctionCalls(Statement->For.Next, Calls);
            GetFunctionCalls(Statement->For.Statements, Calls);
        } break;

        case StatementType_Return:
        {
            if(Statement->Return.Expression)
            {
                GetFunctionCalls(Statement->Return.Expression, Calls);
            }
        } break;

    }
}

void GetFunctionCalls(array<statement*> StatementBlock, array<expression*>* Calls)
{
    for(u32 StatementIndex = 0; StatementIndex < StatementBlock.Size; StatementIndex++)
    {
        statement* Statement = StatementBlock.Data[StatementIndex];
        GetFunctionCalls(Statement, Calls);
    }
}

#if 0
    array<expression*> FunctionsCalledFromMain = {};
    GetFunctionCalls(Main->Function.Statements, &FunctionsCalledFromMain);
    // Print(FunctionsCalledFromMain);

    array<declaration*> FunctionsToGenerate = {};
    AddToArray(&FunctionsToGenerate, Main);
    // Print(FunctionsToGenerate);

    // TODO: Hacky
    declaration* Class = Program.Declarations.Data[0];

    for(u32 CallIndex = 0; CallIndex < FunctionsCalledFromMain.Size; CallIndex++)
    {
        expression* Call = FunctionsCalledFromMain.Data[CallIndex];
        bool ToGenerate = true;
        u32 FuncIndex = 0;
        declaration* Decl = 0;

        for(; FuncIndex < Class->Class.Functions.Size; FuncIndex++)
        {
            Decl = Class->Class.Functions.Data[FuncIndex];

            if(AreEqual(Call->FunctionCall.Name, Call->FunctionCall.Length,
                        Decl->Name, Decl->NameLength))
            {
                for(u32 GenerateIndex = 0; GenerateIndex < FunctionsToGenerate.Size; GenerateIndex++)
                {
                    declaration* GenDecl = FunctionsToGenerate.Data[GenerateIndex];
                    if(AreEqual(GenDecl->Name, GenDecl->NameLength,
                                Decl->Name, Decl->NameLength))
                    {
                        ToGenerate = false;
                        break;
                    }
                }
            }
        }

#if 0
        printf("%d ", FuncIndex);
        printf("%d ", FunctionsToGenerate.Size);
        PrintNewLine(Decl);
#endif
        if(ToGenerate)
        {
            if(!AreEqual(Decl->Name, Decl->NameLength, "Main"))
            {
                AddToArray(&FunctionsToGenerate, Decl);
            }
        }
        else
        {
            // Already in to be generated
        }
    }

    Print(FunctionsToGenerate);
#endif
