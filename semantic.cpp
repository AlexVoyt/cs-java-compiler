static array<declaration*> KnownFunctions = {};

void Check(expression* Expression, array<declaration*>* KnownVariables)
{
    switch(Expression->Type)
    {
        case ExpressionType_Binary:
        {
            Check(Expression->Binary.Left, KnownVariables);
            Check(Expression->Binary.Right, KnownVariables);
        } break;

        case ExpressionType_Unary:
        {
            Check(Expression->Unary.Expression, KnownVariables);
        } break;

        case ExpressionType_Paren:
        {
            Check(Expression->Paren.Expression, KnownVariables);
        } break;

        case ExpressionType_Name:
        {
            bool Found = false;
            for(u32 VarIndex = 0; VarIndex < KnownVariables->Size; VarIndex++)
            {
                declaration* Var = *(KnownVariables->Data + VarIndex);
                if(AreEqual(Var->Name, Var->NameLength, Expression->Name.Ident, Expression->Name.Length))
                {
                    Found = true;
                    break;
                }
            }

            if(!Found)
            {
                printf("Variable %.*s is not defined\n", Expression->Name.Length, Expression->Name.Ident);
            }
        } break;

        case ExpressionType_FunctionCall:
        {
            bool Found = false;
            for(u32 FunIndex = 0; FunIndex < KnownFunctions.Size; FunIndex++)
            {
                declaration* Fun = *(KnownFunctions.Data + FunIndex);
                if(AreEqual(Fun->Name, Fun->NameLength,
                            Expression->FunctionCall.Name, Expression->FunctionCall.Length))
                {
                    if(Fun->Function.Params.Size == Expression->FunctionCall.Arguments.Size)
                    {
                        Found = true;
                        break;
                    }
                    break;
                }
            }

            if(!Found)
            {
                printf("Function %.*s is not defined\n", Expression->FunctionCall.Length,
                                                         Expression->FunctionCall.Name);
            }
        } break;
    }
}

void Check(statement* Statement, array<declaration*>* KnownVariables)
{
    switch(Statement->Type)
    {
        case StatementType_Declaration:
        {
            AddToArray(KnownVariables, Statement->Declaration);
        } break;

        case StatementType_Expression:
        {
            Check(Statement->Expression, KnownVariables);
        } break;

        case StatementType_Assign:
        {
            Check(Statement->Assign.Left, KnownVariables);
            Check(Statement->Assign.Right, KnownVariables);
        } break;

        case StatementType_For:
        {
            u32 KnownVariableSize = KnownVariables->Size;
            bool IsInitDeclaration = Statement->For.Init->Type == StatementType_Declaration;
            if(IsInitDeclaration)
            {
                AddToArray(KnownVariables, Statement->For.Init->Declaration);
            }

            for(u32 StatementIndex = 0; StatementIndex < Statement->For.Statements.Size; StatementIndex++)
            {
                statement* InnerStatement = *(Statement->For.Statements.Data + StatementIndex);
                Check(InnerStatement, KnownVariables);
            }

            KnownVariables->Size = KnownVariableSize;
        } break;

        case StatementType_While:
        case StatementType_DoWhile:
        {
            u32 KnownVariableSize = KnownVariables->Size;
            Check(Statement->While.Condition, KnownVariables);
            for(u32 StatementIndex = 0; StatementIndex < Statement->While.Statements.Size; StatementIndex++)
            {
                statement* InnerStatement = *(Statement->While.Statements.Data + StatementIndex);
                Check(InnerStatement, KnownVariables);
            }

            KnownVariables->Size = KnownVariableSize;
        } break;

        case StatementType_If:
        {
            Check(Statement->If.Condition, KnownVariables);
            u32 KnownVariableSize = KnownVariables->Size;
            for(u32 StatementIndex = 0; StatementIndex < Statement->If.ThenBlock.Size; StatementIndex++)
            {
                statement* InnerStatement = *(Statement->If.ThenBlock.Data + StatementIndex);
                Check(InnerStatement, KnownVariables);
            }
            KnownVariables->Size = KnownVariableSize;

            for(u32 ElseIfIndex = 0; ElseIfIndex < Statement->If.ElseIfs.Size; ElseIfIndex++)
            {
                else_if ElseIf = Statement->If.ElseIfs.Data[ElseIfIndex];

                Check(ElseIf.Condition, KnownVariables);
                KnownVariableSize = KnownVariables->Size;
                for(u32 StatementIndex = 0; StatementIndex < ElseIf.StatementBlock.Size; StatementIndex++)
                {
                    statement* InnerStatement = *(ElseIf.StatementBlock.Data + StatementIndex);
                    Check(InnerStatement, KnownVariables);
                }
                KnownVariables->Size = KnownVariableSize;
            }

            KnownVariableSize = KnownVariables->Size;
            for(u32 StatementIndex = 0; StatementIndex < Statement->If.ElseBlock.Size; StatementIndex++)
            {
                statement* InnerStatement = *(Statement->If.ElseBlock.Data + StatementIndex);
                Check(InnerStatement, KnownVariables);
            }
            KnownVariables->Size = KnownVariableSize;

        } break;

        case StatementType_Return:
        {
            if(Statement->Return.Expression)
            {
                Check(Statement->Return.Expression, KnownVariables);
            }
        } break;
    }
}

void PerformSemAnalyse(declaration* Function)
{
    // TODO: memleak, but who cares
    array<declaration*> KnownVariables = {};
    for(u32 StatementIndex = 0; StatementIndex < Function->Function.Statements.Size; StatementIndex++)
    {
        statement* Statement = *(Function->Function.Statements.Data + StatementIndex);
        for(u32 ArgumentIndex = 0; ArgumentIndex < Function->Function.Params.Size; ArgumentIndex++)
        {
            function_param Param = Function->Function.Params.Data[ArgumentIndex];
            declaration* Decl = NewVariableDeclaration(Param.TypeLength, Param.Type,
                                                       Param.NameLength, Param.Name,
                                                       0);
            AddToArray(&KnownVariables, Decl);
        }
        Check(Statement, &KnownVariables);
    }
}
