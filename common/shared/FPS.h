#include "Utl_Global.h"


class CFPSCounter
{
public:
    CFPSCounter();
    ~CFPSCounter() { }

private:
    double _secondsPerCount;;
    float _deltaTime;

public:
    void Update();
    float GetDeltaTime()
    {
        return _deltaTime;
    }

    float GetFPS() 
    {
        if( _deltaTime > 0 )
        {
            return 1.0f / _deltaTime;
        }
        else 
        {
            return 0;
        }
    }
};