#ifndef UTILS_CPP
#define UTILS_CPP

#include "utils.h"

void FatalError(const char* ErrorMsg)
{
    printf("%s\n", ErrorMsg);
    exit(1);
}

// TODO: error handling
char* ReadFile(const char* Filename)
{
    char* Result = 0;
    FILE* File = fopen(Filename, "rb");
    if(!File)
    {
        FatalError("Could not open file");
        exit(2);
    }
    assert(File);

    fseek(File, 0, SEEK_END);
    u32 FileLength = ftell(File);
    fseek(File, 0, SEEK_SET);

    Result = (char* )malloc(FileLength + 1);
    assert(Result);
    fread(Result, FileLength, 1, File);
    Result[FileLength] = 0;

    fclose(File);
    return Result;
}

template<class T>
void AddToArray(array<T>* Array, T Element)
{
    if(Array->Data)
    {
        if(Array->Size == Array->Capacity)
        {
            Array->Capacity *= 2;
            Array->Data = (T* )realloc(Array->Data, Array->Capacity * sizeof(T));
        }
    }
    else
    {
        Array->Size = 0;
        Array->Capacity = 10;
        Array->Data = (T* )malloc(Array->Capacity * sizeof(T));
    }

    Array->Data[Array->Size] = Element;
    Array->Size++;
}

bool IsWhitespace(char C)
{
    return (C == ' ');
}

// TODO: \t \r?
bool IsNewLine(char C)
{
    return (C == '\n') || (C == '\r');
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


// NOTE: left string is length based, right is null terminated
bool AreEqual(char* Left, u32 LeftLength, char* Right)
{
    bool Result = true;
    u32 Index = 0;
    for(; Index < LeftLength; Index++)
    {
        if(Left[Index] == Right[Index])
        {
        }
        else
        {
            Result = false;
            break;
        }
    }

    if(Right[Index])
    {
        Result = false;
    }

    return Result;
}

bool AreEqual(char* Left, u32 LeftLength, char* Right, u32 RightLength)
{
    if(LeftLength != RightLength)
    {
        return false;
    }

    for(u32 Index = 0; Index < LeftLength; Index++)
    {
        if(Left[Index] == Right[Index])
        {
        }
        else
        {
            return false;
        }
    }

    return true;
}

#endif /* UTILS_CPP */
