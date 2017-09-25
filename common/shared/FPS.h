#include "Utl_Global.h"


class CFPSCounter
{
public:
    CFPSCounter();
    ~CFPSCounter() { }

private:
    double _secondsPerCount;;
    float _deltaTime;
    uint  _fps;

public:
    void Update();
    float GetDeltaTime()
    {
        return _deltaTime;
    }

    uint GetFPS() 
    {
        return _fps;
    }
};