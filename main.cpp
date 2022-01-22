#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
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

#include "term.h"
#include "util.cpp"
#include "lexer.cpp"
#include "ast.cpp"
#include "parse.cpp"
#include "print.cpp"
#include "test.cpp"


#if 0
struct keyword_table
{
};

global_variable keyword_table KeywordTable;
#endif

int main()
{
    TestLexer();
    // TestExpressionPrinting();
    // TestDeclarationPrinting();
    TestParsing();

    printf("\n");
    return 0;
}
