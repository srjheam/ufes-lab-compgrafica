#pragma once

#define STR_LEN(str) (sizeof(str) - 1)  // -1 to exclude null terminator

#if defined(__GNUC__)
    #define UNUSED_PARAM(x) __attribute__((unused)) x
#else 
    #define UNUSED_PARAM(x) x
#endif
