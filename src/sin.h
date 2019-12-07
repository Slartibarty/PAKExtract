//
// SINPAK definitions
//

#pragma once

#include <stdint.h> // Int types
#include <stdlib.h> // _MAX_PATH
#include <stdio.h>  // FILE

#include "pak.h"    // Existing defs

/*
========================================================================

SiN PAKs are the same as Quake ones, but they have 120 chars for the header instead of 56

========================================================================
*/

#define MAX_SINPAK_FILENAME_LENGTH 120
#define SINPAKHEADER	(('K'<<24)+('A'<<16)+('P'<<8)+'S')

typedef struct
{
    char        name[MAX_SINPAK_FILENAME_LENGTH];
    int32_t     filepos, filelen;
} dsinpackfile_t;
