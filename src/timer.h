//
// Timer
//

#pragma once

#include <stdint.h>

#ifdef _WIN32

// Start the counter
void StartCounter();

// Get float seconds
double GetFloatSeconds();

// Get float miliseconds
double GetFloatMiliSeconds();

// Get float microseconds
double GetFloatMicroSeconds();

// Get absolute seconds (probably won't use this)
int64_t GetSeconds();

// Get absolute miliseconds
int64_t GetMiliSeconds();

// Get absolute microseconds
int64_t GetMicroSeconds();

#else

// Dummy functions

// Start the counter
void StartCounter() {}

// Get float seconds
double GetFloatSeconds() {}

// Get float miliseconds
double GetFloatMiliSeconds() {}

// Get float microseconds
double GetFloatMicroSeconds() {}

// Get absolute seconds (probably won't use this)
int64_t GetSeconds() {}

// Get absolute miliseconds
int64_t GetMiliSeconds() {}

// Get absolute microseconds
int64_t GetMicroSeconds() {}

#endif
