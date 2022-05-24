#ifndef LEXER_CPP
#define LEXER_CPP

#include "term.h"
#include "lexer.h"

// TODO: should add everytime adding something in enum
const char* TokenTypeStr(token_type Type)
{
    switch(Type)
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
        case(TokenType_Semicolon): {return ";";} break;
        case(TokenType_Colon): {return ":";} break;
        case(TokenType_Plus): {return "+";} break;
        case(TokenType_Minus): {return "-";} break;
        case(TokenType_Asterisk): {return "*";} break;
        case(TokenType_Slash): {return "/";} break;
        case(TokenType_Percent): {return "%";} break;
        case(TokenType_Increment): {return "++";} break;
        case(TokenType_Decrement): {return "--";} break;
        case(TokenType_Greater): {return ">";} break;
        case(TokenType_Lesser): {return "<";} break;
        case(TokenType_Equal): {return "==";} break;
        case(TokenType_GEQ): {return ">=";} break;
        case(TokenType_LEQ): {return "<=";} break;
        case(TokenType_NEQ): {return "!=";} break;
        case(TokenType_LogicalAnd): {return "&&";} break;
        case(TokenType_LogicalOr): {return "||";} break;
        case(TokenType_LogicalNot): {return "!=";} break;
        case(TokenType_Assign): {return "=";} break;
        case(TokenType_PlusAssign): {return "+=";} break;
        case(TokenType_MinusAssign): {return "-=";} break;
        case(TokenType_MulAssign): {return "*=";} break;
        case(TokenType_DivAssign): {return "/=";} break;
        case(TokenType_ModAssign): {return "%=";} break;
        case(TokenType_ShiftLeft): {return "<<";} break;
        case(TokenType_ShiftRight): {return ">>";} break;
        case(TokenType_BitAnd): {return "&";} break;
        case(TokenType_BitOr): {return "|";} break;
        case(TokenType_BitXor): {return "^";} break;
        case(TokenType_Int): {return "Int";} break;
        case(TokenType_Identifier): {return "Identifier";} break;
        case(TokenType_Backslash): {return "Backslash";} break;

        default: {InvalidCodePath("Token to str"); return "Invalid code path";} break;
    }
}

const char* TokenTypeStr(token* Token)
{
    return TokenTypeStr(Token->Type);
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
        printf("Line: %3d, Type: %12s, Length: %3d, Value: %llu\n",
                Token->Line, TokenTypeStr(Token), Token->Length, Token->Integer);

    RESET_TERM_COLOR();
}

// TODO: do we need this function? It's probably easier to just increment
// pointer than to use this(???)
void NextToken(token** Stream)
{
    (*Stream)++;
}

bool MatchToken(token* Token, token_type Type)
{
    if(Token->Type == Type)
        return true;
    else
        return false;
}
bool MatchKeyword(token* Token, char* Keyword)
{
    if((Token->Type == TokenType_Identifier) && AreEqual(Token->Content, Token->Length, Keyword))
        return true;
    else
        return false;
}

bool MatchAccessModifier(token* Token)
{
    return MatchKeyword(Token, "public") ||
           MatchKeyword(Token, "protected") ||
           MatchKeyword(Token, "private");
}

bool MatchType(token* Token)
{
    bool Result = false;
    if(Token->Type == TokenType_Identifier)
    {
        Result = AreEqual(Token->Content, Token->Length, "int") ||
                 AreEqual(Token->Content, Token->Length, "long") ||
                 AreEqual(Token->Content, Token->Length, "short") ||
                 AreEqual(Token->Content, Token->Length, "char") ||
                 AreEqual(Token->Content, Token->Length, "bool") ||
                 AreEqual(Token->Content, Token->Length, "double") ||
                 AreEqual(Token->Content, Token->Length, "float") ||
                 AreEqual(Token->Content, Token->Length, "sbyte");
    }

    return Result;
}

// TODO: this function should check current token and assert that it
// has certain type, otherwise we delete our program
void ExpectToken(token** Token, token_type ExpectedType)
{
    if(MatchToken(*Token, ExpectedType))
    {
        NextToken(Token);
    }
    else
    {
        // TODO: FatalError should be printf with exit
        printf("In line %d: expect %s, got %s",
                (*Token)->Line,
                TokenTypeStr(ExpectedType),
                TokenTypeStr((*Token)->Type));
        exit(1); // TODO: skip tokens
        // FatalError("In expect token");
    }
}

array<token> Tokenize(const char* Source)
{
    array<token> Result = {};
    tokenizer Tokenizer;
    Tokenizer.At = (char* )Source;
    Tokenizer.Line = 1;
    bool Lexing = true;
    while(Lexing)
    {
        token Token = GetToken(&Tokenizer);
        if(Token.Type == TokenType_EndOfStream)
            Lexing = false;
        AddToArray(&Result, Token);
    }

    return Result;
}

bool IsAssignOp(token* TokenStream)
{
    token_type Type = TokenStream->Type;

    return (Type == TokenType_Assign) ||
           (Type == TokenType_PlusAssign) ||
           (Type == TokenType_MinusAssign) ||
           (Type == TokenType_MulAssign) ||
           (Type == TokenType_DivAssign) ||
           (Type == TokenType_ModAssign);
}

bool IsCompareOp(token* TokenStream)
{
    token_type Type = TokenStream->Type;
    return (Type == TokenType_Greater) ||
           (Type == TokenType_Lesser) ||
           (Type == TokenType_Equal) ||
           (Type == TokenType_GEQ) ||
           (Type == TokenType_LEQ) ||
           (Type == TokenType_NEQ);
}

bool IsShiftOp(token* TokenStream)
{
    token_type Type = TokenStream->Type;
    return (Type == TokenType_ShiftLeft) ||
           (Type == TokenType_ShiftRight);
}

bool IsAddOp(token* TokenStream)
{
    token_type Type = TokenStream->Type;
    return (Type == TokenType_Plus) ||
           (Type == TokenType_Minus);
}

bool IsMulOp(token* TokenStream)
{
    token_type Type = TokenStream->Type;
    return (Type == TokenType_Asterisk) ||
           (Type == TokenType_Slash) ||
           (Type == TokenType_Percent);
}

bool IsUnaryOp(token* TokenStream)
{
    token_type Type = TokenStream->Type;
    return (Type == TokenType_Increment) ||
           (Type == TokenType_Decrement) ||
           (Type == TokenType_Minus) ||
           (Type == TokenType_Plus);
}


#endif /* LEXER_CPP */
