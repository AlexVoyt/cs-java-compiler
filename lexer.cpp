#ifndef LEXER_CPP
#define LEXER_CPP

#include "term.h"
#include "lexer.h"

bool IsWhitespace(char C)
{
    return (C == ' ');
}

// TODO: \t \r?
bool IsNewLine(char C)
{
    return (C == '\n');
}

// TODO: comments?
void EatWhitespaces(tokenizer* Tokenizer)
{
    while(IsWhitespace(*Tokenizer->At) || IsNewLine(*Tokenizer->At))
    {
        bool NewLine = IsNewLine(*Tokenizer->At);
        Tokenizer->At++;
        if(NewLine)
        {
            Tokenizer->Line++;
        }
    }
}

bool IsNumeric(char C)
{
    return ((C >= '0') && (C <= '9'));
}

bool IsAlpha(char C)
{
    return (((C >= 'a') && (C <= 'z')) ||
            ((C >= 'A') && (C <= 'Z')));
}

// TODO: should add everytime adding something in enum
const char* TokenTypeStr(token* Token)
{
    switch(Token->Type)
    {
        case(TokenType_Unknown): {return "Unknown";} break;
        case(TokenType_EndOfStream): {return "EndOfStream";} break;
        case(TokenType_LParen): {return "LParen";} break;
        case(TokenType_RParen): {return "RParen";} break;
        case(TokenType_LBracket): {return "LBracket";} break;
        case(TokenType_RBracket): {return "RBracket";} break;
        case(TokenType_LBrace): {return "LBrace";} break;
        case(TokenType_RBrace): {return "RBrace";} break;
        case(TokenType_Comma): {return "Comma";} break;
        case(TokenType_Dot): {return "Dot";} break;
        case(TokenType_Apostrophe): {return "Apostrophe";} break;
        case(TokenType_Quote): {return "Quote";} break;
        case(TokenType_Semicolon): {return "Semicolon";} break;
        case(TokenType_Colon): {return "Colon";} break;
        case(TokenType_Plus): {return "Plus";} break;
        case(TokenType_Minus): {return "Minus";} break;
        case(TokenType_Asterisk): {return "Asterisk";} break;
        case(TokenType_Slash): {return "Slash";} break;
        case(TokenType_Percent): {return "Percent";} break;
        case(TokenType_Increment): {return "Increment";} break;
        case(TokenType_Decrement): {return "Decrement";} break;
        case(TokenType_Greater): {return "Greater";} break;
        case(TokenType_Lesser): {return "Lesser";} break;
        case(TokenType_Equal): {return "Equal";} break;
        case(TokenType_GEQ): {return "GEQ";} break;
        case(TokenType_LEQ): {return "LEQ";} break;
        case(TokenType_NEQ): {return "NEQ";} break;
        case(TokenType_LogicalAnd): {return "LogicalAnd";} break;
        case(TokenType_LogicalOr): {return "LogicalOr";} break;
        case(TokenType_LogicalNot): {return "LogicalNot";} break;
        case(TokenType_Assign): {return "Assignment";} break;
        case(TokenType_PlusAssign): {return "PlusAssign";} break;
        case(TokenType_MinusAssign): {return "MinusAssign";} break;
        case(TokenType_MulAssign): {return "MulAssign";} break;
        case(TokenType_DivAssign): {return "DivAssign";} break;
        case(TokenType_ModAssign): {return "ModAssign";} break;
        case(TokenType_ShiftLeft): {return "ShiftLeft";} break;
        case(TokenType_ShiftRight): {return "ShiftRight";} break;
        case(TokenType_BitAnd): {return "BitAnd";} break;
        case(TokenType_BitOr): {return "BitOr";} break;
        case(TokenType_BitXor): {return "BitXor";} break;
        case(TokenType_Int): {return "Int";} break;
        case(TokenType_Identifier): {return "Identifier";} break;
        case(TokenType_Backslash): {return "Backslash";} break;

        default: {InvalidCodePath; return "Invalid code path";} break;
    }
}

#define FOLLOW_CHECK(symbol, type, next_symbol, next_type) case (symbol): \
        { \
            Result.Type = (type); \
            if((*Tokenizer->At) && (*Tokenizer->At == (next_symbol))) \
            { \
                Result.Type = (next_type); \
                Tokenizer->At++; \
            } \
        } break;

#define FOLLOW_CHECK2(symbol, type, s1, t1, s2, t2) case (symbol): \
        { \
            Result.Type = (type); \
            if((*Tokenizer->At) && (*Tokenizer->At == (s1))) \
            { \
                Result.Type = (t1); \
                Tokenizer->At++; \
            } \
            else if((*Tokenizer->At) && (*Tokenizer->At == (s2))) \
            { \
                Result.Type = (t2); \
                Tokenizer->At++; \
            } \
        } break;

token GetToken(tokenizer* Tokenizer)
{
    EatWhitespaces(Tokenizer);

    token Result = {};
    Result.Content = Tokenizer->At;
    Result.Line = Tokenizer->Line;

    char* C = Tokenizer->At;
    Tokenizer->At++;
    switch(*C)
    {
        case '(':{Result.Type = TokenType_LParen;} break;
        case ')':{Result.Type = TokenType_RParen;} break;
        case '[':{Result.Type = TokenType_LBracket;} break;
        case ']':{Result.Type = TokenType_RBracket;} break;
        case '{':{Result.Type = TokenType_LBrace;} break;
        case '}':{Result.Type = TokenType_RBrace;} break;

        case ';':{Result.Type = TokenType_Semicolon;} break;
        case ':':{Result.Type = TokenType_Colon;} break;
        case '.':{Result.Type = TokenType_Dot;} break;
        case ',':{Result.Type = TokenType_Comma;} break;
        case '\'':{Result.Type = TokenType_Apostrophe;} break;
        case '"':{Result.Type = TokenType_Quote;} break;
        case '\\':{Result.Type = TokenType_Backslash;} break;

        FOLLOW_CHECK('*', TokenType_Asterisk, '=', TokenType_MulAssign);
        FOLLOW_CHECK('/', TokenType_Slash, '=', TokenType_DivAssign);
        FOLLOW_CHECK('%', TokenType_Percent, '=', TokenType_ModAssign);
        FOLLOW_CHECK('=', TokenType_Assign, '=', TokenType_Equal);
        FOLLOW_CHECK2('+', TokenType_Plus, '+', TokenType_Increment, '=', TokenType_PlusAssign);
        FOLLOW_CHECK2('-', TokenType_Minus, '-', TokenType_Decrement, '=', TokenType_MinusAssign);

        FOLLOW_CHECK2('>', TokenType_Greater, '>', TokenType_ShiftRight, '=', TokenType_GEQ);
        FOLLOW_CHECK2('<', TokenType_Lesser, '<', TokenType_ShiftLeft, '=', TokenType_LEQ);
        FOLLOW_CHECK('!', TokenType_LogicalNot, '=', TokenType_NEQ);
        FOLLOW_CHECK('&', TokenType_BitAnd, '&', TokenType_LogicalAnd);
        FOLLOW_CHECK('|', TokenType_BitOr, '|', TokenType_LogicalOr);
        case '^':{Result.Type = TokenType_BitXor;} break;
        // TODO: Logical/Bit Assign?

        default:
        {
            if(*C == 0)
            {
                Result.Type = TokenType_EndOfStream;
            }
            else if(IsAlpha(*C) || (*C == '_'))
            {
                Result.Type = TokenType_Identifier;
                while(*Tokenizer->At && 
                     (IsAlpha(*Tokenizer->At) || IsNumeric(*Tokenizer->At) || (*Tokenizer->At == '_')))
                {
                    Tokenizer->At++;
                }
            }
            else if(IsNumeric(*C))
            {
                // TODO: float support, error on invalid number, scientific notation
                Result.Type = TokenType_Int;
                u32 Digit = (*C - '0');
                u64 Number = Digit;
                while(*Tokenizer->At && 
                     (IsNumeric(*Tokenizer->At)))
                {
                    Digit = (*Tokenizer->At - '0');
                    if(Number > (UnsignedLongMax - Digit)/10)
                        FatalError("Maximum integer number overflow");
                    Number = Number*10+Digit;
                    Tokenizer->At++;
                }
                Result.Integer = Number;
            }
            else
            {
                Result.Type = TokenType_Unknown;
            }

        } break;
    }

    Result.Length = Tokenizer->At - C;
    return Result;
} 
#undef FOLLOW_CHECK
#undef FOLLOW_CHECK2

void PrintToken(token* Token)
{
    if(Token->Type == TokenType_Unknown)
    {
        SET_TERM_COLOR(TERM_RED);
    }

    if(Token->Type != TokenType_Int)
        printf("Line: %3d, Type: %12s, Length: %3d, Content: %.*s\n", 
                Token->Line, TokenTypeStr(Token), Token->Length, Token->Length, Token->Content);
    else
        printf("Line: %3d, Type: %12s, Length: %3d, Value: %ld\n",
                Token->Line, TokenTypeStr(Token), Token->Length, Token->Integer);

    RESET_TERM_COLOR();
}


#endif /* LEXER_CPP */
