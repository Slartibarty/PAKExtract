//
// File and folder handling
//

#pragma once

#include <stdbool.h>

#ifdef _WIN32
#include <direct.h>
#else

#endif

#include "common.h"

// Inlines

inline int FS_MakeDir(const char* path)
{
#ifdef _WIN32
    return _mkdir(path);
#else
    return mkdir(path);
#endif
}

// TODO: Considering ignoring the return value...
inline char* FS_GetCWD(char* buf, int bufsize)
{
#ifdef _WIN32
    char* retVal = _getcwd(buf, bufsize);
    Com_WindowsToUnix(buf); // This func returns \\ delimiters on Windows
    return retVal;
#else
    return getcwd(buf, bufsize);
#endif
}

inline int FS_SetCWD(const char* path)
{
#ifdef _WIN32
    return _chdir(path);
#else
    return chdir(path);
#endif
}

// Functions

// Creates any directories needed to store the given filename
void FS_CreatePath(char* path);

// Gets the filename from a path
// Or uh, the last node technically
const char* FS_GetFileName(const char* path);

// Gets the file extension from a path
const char* FS_GetFileExtension(const char* path);

// Strip the last node (or filename) off of a path
// This KEEPS the last delimiter, it just strips the filename + ext
void FS_StripFileName(char* path);

// Strip the file extension off of a path
// If the path has no file extension, we do nothing
void FS_StripFileExtension(char* path);

// Returns true if a path is relative
bool FS_IsPathRelative(const char* path);

// MUST pass in a char* with a sizeof _MAX_PATH
void FS_RelativeToAbsolute(char* out, const char* relPath);
