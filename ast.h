struct expression;
struct statement;
struct declaration;

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

struct expression_list
{
};

struct expression
{
    expression_type Type;
    union
    {
        u64 Integer;
        f64 Float;

        struct
        {
            char* Ident;
            u32 Length;
        } Name;

        struct
        {
            char* Name;
            u32 Length;
            array<expression*> Arguments;
        } FunctionCall;

        struct
        {
            token_type Op;
            expression* Expression;
        } Unary;

        struct
        {
            token_type Op;
            expression* Left;
            expression* Right;
        } Binary;

        struct
        {
            expression* Expression;
        } Paren;

    };
};

enum declaration_type
{
    DeclarationType_None,
    DeclarationType_Variable,
    DeclarationType_Function,
    DeclarationType_Using,
};

struct function_params
{
    char* Name; // TODO: haven't decided yet, will this be null terminated stirngs or length specified
    char* Type;
};

enum using_modifier_type
{
    UsingModifierType_None,
    UsingModifierType_Static,
};

struct declaration
{
    declaration_type Type;
    u32 NameLength;
    char* Name;
    union
    {
        struct
        {
            u32 TypeLength;
            char* Type;
            expression* Expression;
        } Variable;

        struct
        {
            char* ReturnType;
            array<function_params> Params;
        } Function;

        struct
        {
            using_modifier_type ModifierType;
        } Using;
    };
};

enum statement_type
{
    StatementType_Expression,
    StatementType_Declaration,
    StatementType_If,
    StatementType_While,
    StatementType_DoWhile,
    StatementType_For,
};

struct statement_list
{
    statement* First;
    statement* Last;
};

struct else_if
{
    expression* Condition;
    array<statement*> StatementBlock;
};

struct statement
{
    statement_type Type;
    union
    {
        expression* Expression;
        declaration* Declaration;

        struct
        {
            expression* Condition;
            array<statement*> ThenBlock;
            array<statement*> ElseBlock;
            array<else_if> ElseIfs;
        } If;

        struct
        {
            expression* Condition;
            statement_list Statements;
        } While; // NOTE: also do_while

        struct
        {
            statement* Init;
            expression* Condition;
            statement* Next;
            statement_list Statements;
        } For;
    };
};
