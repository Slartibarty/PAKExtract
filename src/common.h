//
// Common
//

#pragma once

#include <string.h>
#include <stdbool.h>

// Definitions

typedef unsigned char   byte;

// Functions

void Com_ReplaceCharacter(char* str, const char toReplace, const char toPlace);

// Convert Windows delimiter to Unix ones
inline void Com_WindowsToUnix(char* path)
{
    #ifdef _WIN32 // Should always be using Unix delimiters on Unix
    Com_ReplaceCharacter(path, '\\', '/');
    #endif
}

// Convert Unix delimiter to Windows ones
inline void Com_UnixToWindows(char* path)
{
    Com_ReplaceCharacter(path, '/', '\\');
}

inline bool Q_stricmp(const char* str1, const char* str2)
{
#ifdef _WIN32
    return (_stricmp(str1, str2) == 0);
#else
    return (strcasecmp(str1, str2) == 0);
#endif
}

inline bool Q_strnicmp(const char* str1, const char* str2, size_t count)
{
#ifdef _WIN32
    return (_strnicmp(str1, str2, count) == 0);
#else
    return (strncasecmp(str1, str2, count) == 0);
#endif
}
