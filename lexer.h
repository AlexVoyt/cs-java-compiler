#ifndef LEXER_H
#define LEXER_H

struct tokenizer
{
    char* At;
    u32 Line;
};

enum token_type
{
    /*
    Arithmetic:  + - * / % ++ -- + -
    Comparison:  < > == <= >= !=
    Logical:     && || !
    Assignement: = += -+ *= /= %=
    Bit:      & | << >> ^ ~
    Braces:      ( ) { } [ ]
    Syntax:      : ; , . ' / \
    */

    // Utility
    TokenType_Unknown,
    TokenType_EndOfStream,

    // Braces
    TokenType_LParen,
    TokenType_RParen,
    TokenType_LBracket,
    TokenType_RBracket,
    TokenType_LBrace,
    TokenType_RBrace,

    // Syntax
    TokenType_Comma,
    TokenType_Dot,
    TokenType_Apostrophe,
    TokenType_Quote,
    TokenType_Semicolon,
    TokenType_Colon,
    TokenType_Backslash,

    // Arithmetic
    TokenType_Plus,
    TokenType_Minus,
    TokenType_Asterisk,
    TokenType_Slash,
    TokenType_Percent,
    TokenType_Increment,
    TokenType_Decrement,

    // Comparison
    TokenType_Greater,
    TokenType_Lesser,
    TokenType_Equal,
    TokenType_GEQ,
    TokenType_LEQ,
    TokenType_NEQ,

    // Logical
    TokenType_LogicalAnd,
    TokenType_LogicalOr,
    TokenType_LogicalNot,

    // Assignment
    TokenType_Assign,
    TokenType_PlusAssign,
    TokenType_MinusAssign,
    TokenType_MulAssign,
    TokenType_DivAssign,
    TokenType_ModAssign,

    // Bit
    TokenType_ShiftLeft,
    TokenType_ShiftRight,
    TokenType_BitAnd,
    TokenType_BitOr,
    TokenType_BitXor,
    TokenType_BitNot,

    TokenType_Int,
    TokenType_Float,
    TokenType_Identifier,
};

struct token
{
    token_type Type;
    u32 Length;
    u32 Line;
    char* Content;
    union
    {
        u64 Integer;
    };
};

const char* TokenTypeStr(token* Token);
token GetToken(tokenizer* Tokenizer);
void PrintToken(token* Token);

bool IsWhitespace(char C);
bool IsNewLine(char C);
void EatWhitespaces(tokenizer* Tokenizer);
bool IsNumeric(char C);
bool IsAlpha(char C);


#endif /* LEXER_H */
