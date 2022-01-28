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
    DeclarationType_Class,
    DeclarationType_Using,
};

struct function_param
{
    u32 NameLength;
    char* Name;
    u32 TypeLength;
    char* Type;
};

enum access_modifier
{
    AccessModifier_Public,
    AccessModifier_Protected,
    AccessModifier_Private,
};

char* ToString(access_modifier Modifier)
{
    switch(Modifier)
    {
        case AccessModifier_Public:
        {
            return "public";
        } break;

        case AccessModifier_Protected:
        {
            return "protected";
        } break;

        case AccessModifier_Private:
        {
            return "private";
        } break;
    }

    InvalidCodePath("ToString access modifier");
    return 0;
}


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
            access_modifier Modifier;
            u32 ReturnTypeLength;
            char* ReturnType;
            array<function_param> Params;
            array<statement*> Statements;
        } Function;

        struct
        {
            array<declaration*> Functions;
        } Class;

#if 1
        struct
        {
        } Using;
#endif
    };
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
            token_type Op;
            expression* Left;
            expression* Right;
        } Assign;

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
            array<statement*> Statements;
        } While; // NOTE: also do_while

        struct
        {
            statement* Init;
            expression* Condition;
            statement* Next;
            array<statement*> Statements;
        } For;

        struct
        {
            expression* Expression;
        } Return;
    };
};
