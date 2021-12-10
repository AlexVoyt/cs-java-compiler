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
        char* Name;

        struct
        {
            char* Name;
            expression* Params[3]; // TODO: for testing purposes
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
            char* Type;
            expression* Expr;
        } Variable;

        struct
        {
            char* ReturnType;
            function_params* Params;
        } Function;

        struct
        {
            using_modifier_type ModifierType;
        } Using;
    };
};

enum statement_type
{
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
            statement_list Statements;
            // TODO: else if's
        } If;

        struct
        {
            expression* Condition;
            statement_list Statements;
        } While;

        struct
        {
            // TODO: actually same fields as While, should we use While struct or duplicate?
        } DoWhile;

        struct
        {
            statement* Init;
            expression* Condition;
            statement* Next;
            statement_list Statements;
        } For;
    };
};
