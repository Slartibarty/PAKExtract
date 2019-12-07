//
// PAK extractor
//
// FUTUREPROOF: extracted PAK directories might overrun Windows max path length of 260
//

#include <stdlib.h> // Malloc
#include <stdio.h>
#include <string.h> // strcpy, strcat
#include <stdbool.h>

#include "common.h"
#include "filesystem.h"
#include "timer.h"
#include "pak.h"
#include "sin.h"

typedef enum
{
    FMT_NONE,
    FMT_PAK,    // Quake 1 & 2 PAK
    FMT_SIN,    // SiN PAK
    FMT_MAX
} format_t;

struct
{
    format_t    format;
    char        outDir[_MAX_PATH];
    char*       inputFile;
} g_globals =
{
    .format = FMT_NONE,
    .outDir = "",
    .inputFile = NULL
};

void ExtractAllPAKFiles(void* pakFileDesc, int numPakFiles, FILE* pakHandle, bool sinPak)
{
    FILE*   newFile;
    int     fileNum;
    byte*   rawData;
    char    nameBuf[_MAX_PATH];
    dpackfile_t*    localPakDesc = NULL;	// Shut the compiler up
    dsinpackfile_t* sinLocalPakDesc = NULL;	// Shut the compiler up

    const char* name;
    int32_t*    filepos;
    int32_t*    filelen;

    if (sinPak) {
        sinLocalPakDesc = (dsinpackfile_t*)pakFileDesc;
    }
    else {
        localPakDesc = (dpackfile_t*)pakFileDesc;
    }

    double startTime = GetFloatSeconds();

    for (fileNum = 0; fileNum < numPakFiles; ++fileNum)
    {
        if (sinPak) {
            name = (sinLocalPakDesc[fileNum].name);
            filepos = &(sinLocalPakDesc[fileNum].filepos);
            filelen = &(sinLocalPakDesc[fileNum].filelen);
        }
        else {
            name = (localPakDesc[fileNum].name);
            filepos = &(localPakDesc[fileNum].filepos);
            filelen = &(localPakDesc[fileNum].filelen);
        }

        rawData = malloc(*filelen);

        fseek(pakHandle, *filepos, SEEK_SET);
        fread(rawData, 1, *filelen, pakHandle);

        // Create output path (this kinda sucks)
        strcpy(nameBuf, g_globals.outDir);
        strcat(nameBuf, name);

        FS_CreatePath(nameBuf);
        newFile = fopen(nameBuf, "wb");
        if (!newFile) {
            printf("ERROR: Couldn't write to %s\n", nameBuf);
            free(rawData);
            free(pakFileDesc); // Free data from LoadPAK
            exit(1);
        }
        fwrite(rawData, 1, *filelen, newFile);

        free(rawData);
        fclose(newFile);
    }

    double endTime = GetFloatSeconds();
    
    printf("Wrote %d files in %.2f seconds\n", fileNum, endTime - startTime);
}

bool LoadPAK(const char* filename, bool sinPak)
{
    FILE*   pakHandle;
    int     numPakFiles;   // Number of files in the pak
    dpackheader_t   pakHeader;
    void*           pakFile;

    const int	    pakFileSize = sinPak ? sizeof(dsinpackfile_t) : sizeof(dpackfile_t);

    // Open the file
    pakHandle = fopen(filename, "rb");
    if (!pakHandle) {
        printf("LoadPAK ERROR: %s failed to open (does it exist?)...\n", filename);
        return false;
    }

    printf("LoadPAK: Operating on %s\n", filename);

    // Read in the header
    fread(&pakHeader, 1, sizeof(pakHeader), pakHandle);
    if (pakHeader.ident != (sinPak ? SINPAKHEADER : IDPAKHEADER)) {
        printf("LoadPAK ERROR: %s is not a \"PACK\" file...\n", filename);
        return false;
    }

    // How many files are in this PAK?
    numPakFiles = pakHeader.dirlen / pakFileSize;
    // Report how many files we have
    printf("LoadPAK: PAK holds %d files\n", numPakFiles);

    pakFile = malloc(numPakFiles * pakFileSize);

    // Read the file descriptors in
    fseek(pakHandle, pakHeader.dirofs, SEEK_SET);
    fread(pakFile, 1, pakHeader.dirlen, pakHandle);

    // Done, we now have everything we need to start reading

    ExtractAllPAKFiles(pakFile, numPakFiles, pakHandle, sinPak);

    free(pakFile);
    fclose(pakHandle);

    return true;
}

inline void PrintUsage(void)
{
    puts("Usage: pakextract.exe -format (pak/sin) -outdir \"c:\\myfolder\" <pakfile>");
}

bool HandleCommandLine(int argc, char** argv)
{
    // Subtract 1 from argc to compensate for the last arg being the pakfile
    // Do this in all relevant places
    for (int i = 1; i < argc-1; ++i)
    {
        if (Q_stricmp(argv[i], "-outdir")) {
            if (++i >= argc-1) {
                puts("ERROR: Expected argument after -outdir");
                PrintUsage();
                return false;
            }
            strcpy(g_globals.outDir, argv[i]);
            Com_WindowsToUnix(g_globals.outDir);
            continue;
        }
        if (Q_stricmp(argv[i], "-format")) {
            if (++i >= argc-1) {
                puts("ERROR: Expected argument after -format");
                PrintUsage();
                return false;
            }
            if (Q_stricmp(argv[i], "pak")) {
                g_globals.format = FMT_PAK;
            }
            else if (Q_stricmp(argv[i], "sin")) {
                g_globals.format = FMT_SIN;
            }
            else {
                puts("ERROR: Invalid parameter supplied for -format");
                PrintUsage();
                return false;
            }
        }
    }

    // Nothing? This must be the input file
    // Last arg is always the input file
    g_globals.inputFile = argv[argc-1];
    Com_WindowsToUnix(g_globals.inputFile); // Yeah I'm modifying argv

    if (g_globals.outDir[0] == '\0') {
        // Generate an outDir from the input file
        if (FS_IsPathRelative(g_globals.inputFile)) {
            FS_RelativeToAbsolute(g_globals.outDir, g_globals.inputFile);
        }
        else {
            strcpy(g_globals.outDir, g_globals.inputFile);
        }
        // We strip the extension for both paths, do it here instead
        FS_StripFileExtension(g_globals.outDir);
    }

    // We need to add a delimiter to the end of the outdir
    // so that the appended PAK file names work
    const size_t odLength = strlen(g_globals.outDir) - 1;

    // Bonus sanity check
    // This will almost certainly never execute
    if (odLength >= _MAX_PATH) {
        return false;
    }

    if (g_globals.outDir[odLength] != '/') {
        g_globals.outDir[odLength + 1] = '/';
        g_globals.outDir[odLength + 2] = '\0';
    }

    // Infer the format from the extension
    if (g_globals.format == FMT_NONE) {
        const char* extension = FS_GetFileExtension(g_globals.inputFile);
        if (Q_stricmp(extension, "sin")) {
            g_globals.format = FMT_SIN;
        }
        else {
            g_globals.format = FMT_PAK;
        }
    }

    return true;
}

int main(int argc, char** argv)
{
    puts("--------");
    puts("PAK Extractor - by Slartibarty");
    puts("Build date: " __DATE__ " " __TIME__);
    puts("--------");
    
    StartCounter();

    // exename, pak
    if (argc < 2) {
        puts("ERROR: Too few arguments! Must specify a pakfile!");
        return 1;
    }

    if (!HandleCommandLine(argc, argv)) {
        return 1;
    }

    // Load the thingy
    if (!LoadPAK(g_globals.inputFile, (g_globals.format == FMT_SIN))) {
        puts("ERROR: PAK loading failed\n");
        return 1;
    }

    return 0;
}
