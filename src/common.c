//
// Commob
//

#include <stddef.h>

// Replaces every instance of toReplace with toPlace in str
//
void Com_ReplaceCharacter(char* str, const char toReplace, const char toPlace)
{
    while (*str != '\0')
    {
        if (*str == toReplace) {
            *str = toPlace;
        }
        ++str;
    }
}
