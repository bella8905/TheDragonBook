#include <windows.h>
#include <profileapi.h>


#include "FPS.h"

CFPSCounter::CFPSCounter() : _deltaTime( 0.0f ), _fps( 0 )
{
    long long int countsPerSec;
    QueryPerformanceFrequency( ( LARGE_INTEGER* )&countsPerSec );
    _secondsPerCount = 1.0 / countsPerSec;
}

void CFPSCounter::Update()
{
    static long long int prevTime = 0;
    static float timeElapsed = 0.0;
    static uint fps = 0;
    long long int curTime;
    QueryPerformanceCounter( (LARGE_INTEGER*)&curTime );

    _deltaTime = ( float )( curTime - prevTime ) * _secondsPerCount;
    prevTime = curTime;

    fps++;
    timeElapsed += _deltaTime;
    if( timeElapsed >= 1.0 )
    {
        _fps = fps;
        fps = 0;
        timeElapsed = 0;
    }
}