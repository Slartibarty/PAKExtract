//
// File and folder handling
//
// All paths use Unix delimiters, Windows supports either in *most* operations
// Most of these functions avoid using strlen for basically no reason
// would it be faster to use strlen to get to the end of a path, then iterate
// backwards?
// Check older versions of this file for strlen implementations
//

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "filesystem.h"

// Creates any directories needed to store the given filename
void FS_CreatePath(char* path)
{
    char	*ofs;

    for (ofs = path + 1; *ofs; ++ofs)
    {
        if (*ofs == '/') {
            // create the directory
            *ofs = '\0';
            FS_MakeDir(path);
            *ofs = '/';
        }
    }
}

// Gets the filename from a path
// Or uh, the last node technically
const char* FS_GetFileName(const char* path)
{
    const char* lastDelim = path;

    while (*path != '\0')
    {
        if (*path == '/') {
            if (*(path+1) == '\0') {
                // End of the string, don't record this delim
                break;
            }
            lastDelim = path;
        }
        ++path;
    }
    return lastDelim + 1; // Return the start of the filename
}

// Gets the file extension from a path
const char* FS_GetFileExtension(const char* path)
{
    const char* lastPeriod = path;

    while (*path != '\0')
    {
        if (*path == '.') {
            lastPeriod = path;
        }
        ++path;
    }

    // Sanity check
    if (lastPeriod == path) {
        // We never found a period at all!
        // Return the start address
        return lastPeriod;
    }

    return lastPeriod + 1;
}

// Strip the last node (or filename) off of a path
// This KEEPS the last delimiter, it just strips the filename + ext
void FS_StripFileName(char* path)
{
    path += (strlen(path) - 1); // Get to the end!

    // If this is a folder, we might have a delim at the end
    if (*path == '/') {
        *path = '\0';
    }

    while (*path != '/')
    {
        *path = '\0';
        --path;
    }
}

// Strip the file extension off of a path
// If the path has no file extension, we do nothing
void FS_StripFileExtension(char* path)
{
    if (FS_GetFileExtension(path) == path) {
        // We don't have a file extension, early return
        return;
    }

    path += (strlen(path) - 1); // Get to the end!

    while (*path != '.')
    {
        *path = '\0';
        --path;
    }
    
    // If we get here, *path == '.', zero that out too
    *path = '\0';
}

// Returns true if a path is relative
bool FS_IsPathRelative(const char* path)
{
#ifdef _WIN32
    if (path[1] == ':') {
        return false;
    }
#else
    if (path[0] == '/') {
        return false;
    }
#endif
    return true;
}

// MUST pass in a char* with a sizeof _MAX_PATH
void FS_RelativeToAbsolute(char* out, const char* relPath)
{
    // Under normal circumstances, _MAX_PATH should never be exceeded
    // Missing C++ right now.
    FS_GetCWD(out, _MAX_PATH);

    // HACK: Ew, this uses strlen once and GLIBC's strcat implementation to save having to use strcat twice
    const size_t outLength = strlen(out);
    strcpy(out + outLength, "/");
    strcpy(out + outLength + 1, relPath);
}
