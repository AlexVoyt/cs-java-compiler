enum expression_type
{
    ExpressionType_Integer,
    ExpressionType_Binary,
};

struct expression
{
    expression_type Type;
    union
    {
        struct
        {
            u64 Value;
        } Integer;

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
    DeclarationType_Variable,
};

struct declaration
{
    declaration_type Type;
    char* Name;
    u32 NameLength;
    union
    {
        struct
        {
            char* Type; 
            expression* Expr;
        } Variable;
    };
};

