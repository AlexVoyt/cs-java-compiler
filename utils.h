#ifndef UTILS_H
#define UTILS_H

#include "stdio.h"
#include "stdint.h"
#include "assert.h"
#include <stdlib.h>

#define global_variable static

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

#define str(x) #x
#define xstr(x) str(x)
#define ArrayCount(x) sizeof((x))/sizeof((x)[0])

#define InvalidCodePath(Message) assert(!"Invalid Code Path" && (Message))
#define NotDone(Message) assert(!"Not Done" && (Message))

const u64 UnsignedLongMax = 9223372036854775807;
void FatalError(const char* ErrorMsg);
char* ReadFile(const char* Filename);

bool IsWhitespace(char C);
bool IsNewLine(char C);
bool IsNumeric(char C);
bool IsAlpha(char C);
bool AreEqual(char* Left, u32 LeftLength, char* Right);
bool AreEqual(char* Left, u32 LeftLength, char* Right, u32 RightLength);

template<class T>
struct array
{
    u32 Size = 0;
    u32 Capacity = 0;
    T* Data = 0;
};

template<class T>
void AddToArray(array<T>* Array, T Element);

#endif /* UTILS_H */
