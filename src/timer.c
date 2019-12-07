//
// Timer
//
// Currently only supported on Windows
//

#ifdef _WIN32

#include "winlite.h"

#include <stdint.h>
#include <math.h>

#include "timer.h"

// Globals

static LARGE_INTEGER	m_StartTime;
static double			m_ToSeconds;
static double			m_ToMiliSeconds;
static double			m_ToMicroSeconds;

// Functions

// Start the counter
void StartCounter()
{
	QueryPerformanceCounter(&m_StartTime);

	LARGE_INTEGER CurrentFrequency;
	QueryPerformanceFrequency(&CurrentFrequency);
	m_ToSeconds = 1.0 / CurrentFrequency.QuadPart;
	m_ToMiliSeconds = 1e3 / CurrentFrequency.QuadPart;
	m_ToMicroSeconds = 1e6 / CurrentFrequency.QuadPart;
}

// Get float seconds
double GetFloatSeconds()
{
	LARGE_INTEGER CurrentTime;
	QueryPerformanceCounter(&CurrentTime);

	return ((CurrentTime.QuadPart - m_StartTime.QuadPart) * m_ToSeconds);
}

// Get float miliseconds
double GetFloatMiliSeconds()
{
	LARGE_INTEGER CurrentTime;
	QueryPerformanceCounter(&CurrentTime);

	return ((CurrentTime.QuadPart - m_StartTime.QuadPart) * m_ToMiliSeconds);
}

// Get float microseconds
double GetFloatMicroSeconds()
{
	LARGE_INTEGER CurrentTime;
	QueryPerformanceCounter(&CurrentTime);

	return ((CurrentTime.QuadPart - m_StartTime.QuadPart) * m_ToMicroSeconds);
}

// Get absolute seconds (probably won't use this)
int64_t GetSeconds()
{
	LARGE_INTEGER CurrentTime;
	QueryPerformanceCounter(&CurrentTime);

	return llround((CurrentTime.QuadPart - m_StartTime.QuadPart) * m_ToSeconds);
}

// Get absolute miliseconds
int64_t GetMiliSeconds()
{
	LARGE_INTEGER CurrentTime;
	QueryPerformanceCounter(&CurrentTime);

	return llround((CurrentTime.QuadPart - m_StartTime.QuadPart) * m_ToMiliSeconds);
}

// Get absolute microseconds
int64_t GetMicroSeconds()
{
	LARGE_INTEGER CurrentTime;
	QueryPerformanceCounter(&CurrentTime);

	return llround((CurrentTime.QuadPart - m_StartTime.QuadPart) * m_ToMicroSeconds);
}

#endif
