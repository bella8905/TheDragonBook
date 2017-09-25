#include <windows.h>
#include <profileapi.h>


#include "FPS.h"

CFPSCounter::CFPSCounter() : _deltaTime( 0.0f )
{
    long long int countsPerSec;
    QueryPerformanceFrequency( ( LARGE_INTEGER* )&countsPerSec );
    _secondsPerCount = 1.0 / countsPerSec;
}

void CFPSCounter::Update()
{
    static long long int prevTime = 0;
    long long int curTime;
    QueryPerformanceCounter( (LARGE_INTEGER*)&curTime );

    _deltaTime = ( float )( curTime - prevTime ) * _secondsPerCount;
    prevTime = curTime;
}