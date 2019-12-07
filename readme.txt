PakExtract
By Slartibarty
Written in C
07/12/2019
------------
Extractor for Quake, Quake 2, Half-Life and SiN pak files.

Dev note: SiN's pak format is the same as all the others, but the max filename length in dpackfile_t is 120 instead of 56.

==== Options ====

-format <pak/sin> signifies which format you wish to load.
If you don't supply -format, the format will be auto-detected

-outdir <path> The directory you want to extract the root of the pak file to.
To avoid confusion, a parameter of "C:\test\out\" will create "C:\test\out\maps" "C:\test\out\models", etc.
If you don't supply -outdir, the output directory will be a subfolder with the name of your input file (sans extension)

The last parameter MUST be the pak file you want to extract, this can be a relative path or absolute

==== Example Usage ====

>PAKExtract pak0.pak

Creates "/pak0/models" etc.

