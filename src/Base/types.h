#ifndef GamesEngineDefs_h
#define GamesEngineDefs_h

#include <stdio.h>
#include <assert.h>

namespace GamesEngine
{
    typedef char char8;
    typedef char byte;
    typedef char Bool;
    typedef unsigned char Uint8;
    typedef unsigned short Uint16;
    typedef unsigned int Uint32;
    typedef unsigned long Uint64;
    typedef char int8;
    typedef short int16;
    typedef int int32;
    typedef long int64;
    typedef float float32;
    typedef double float64;
    typedef Uint64 size_t;
    
    enum
    {
        RET_INVALID_PARAMS=-1,
        RET_FAILED=0,
        RET_SUCC=1
    };
    
    
    enum
    {
        LOG_NONE = 0,
        LOG_ERROR = 10,
        LOG_BASIC = 20,
        LOG_WARNING = 30,
        LOG_INFO = 40,
        LOG_ALL = 1000
    };
    
    const Uint32 LOG_MAXLEVEL = LOG_ALL;
    const Uint32 ASSERT_LEVEL = LOG_NONE;
    
}

#define DEBUG_LOG(SUBJECT, LEVEL, ARGS)                     \
if(LOG_MAXLEVEL >= LEVEL)                                   \
{                                                           \
printf("[%s] [%s] ", #SUBJECT, #LEVEL);                 \
printf ARGS;                                            \
printf("\n");                                           \
}                                                           \
if(ASSERT_LEVEL >= LEVEL)                                   \
{                                                           \
assert(false);                                          \
}


#define PRINT_SIZE(___TYPENAME___)      \
printf("size of %s : %ld\n", #___TYPENAME___, sizeof(___TYPENAME___));

#define ARRAY_SIZE(array, ___TYPENAME___)   \
sizeof(array)/sizeof(___TYPENAME___)



#endif /* type_defs_h */
