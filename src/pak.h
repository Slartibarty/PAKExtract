//
// PAK definitions
//

#pragma once

#include <stdint.h> // Int types
#include <stdlib.h> // _MAX_PATH
#include <stdio.h>  // FILE

/*
========================================================================

The .pak files are just a linear collapse of a directory tree

========================================================================
*/

#define IDPAKHEADER     (('K'<<24)+('C'<<16)+('A'<<8)+'P')

// Defines a file inside of a .pak
// Structure is exactly 64 bytes (hence name[56])
typedef struct
{
    char        name[56];
    int32_t     filepos, filelen;
} dpackfile_t;

typedef struct
{
    int32_t     ident;      // == IDPAKHEADER
    int32_t     dirofs;
    int32_t     dirlen;
} dpackheader_t;

#define MAX_FILES_IN_PACK   4096
