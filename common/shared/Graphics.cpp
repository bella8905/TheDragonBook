#include "D3DSystem.h"

#include "Graphics.h"



// global video settings
static struct SGraphicSettings
{
    bool _bFullScreen;
    bool _bVsyncEnabled;
    bool _4xMSAAEnabled;
    // http://www.directxtutorial.com/Lesson.aspx?lessonid=11-1-4
    int _defaultClientWidth;    // default client width
    int _defaultClientHeight;   // default client height

    int _currentClientWidth;        // current client width
    int _currentClientHeight;       // current client height


    SGraphicSettings()
    {
        _bFullScreen = false;
        _bVsyncEnabled = true;
        _4xMSAAEnabled = false;
        _defaultClientWidth = 800;
        _defaultClientHeight = 600;
    }

} Graphics_GlobalSettings;



bool CGraphics::GetIsFullScreen()
{
    return Graphics_GlobalSettings._bFullScreen;
}

bool CGraphics::GetIsVsyncEnabled()
{
    return Graphics_GlobalSettings._bVsyncEnabled;
}

bool CGraphics::GetIs4xMSAAEnabled()
{
    return Graphics_GlobalSettings._4xMSAAEnabled;
}

void CGraphics::SetIs4xMSAAEnabled( bool t_bEnabled )
{
    Graphics_GlobalSettings._4xMSAAEnabled = t_bEnabled;
}

void CGraphics::GetDefaultScreenSize( int& t_clientWidth, int& t_clientHeight )
{
    t_clientWidth = Graphics_GlobalSettings._defaultClientWidth;
    t_clientHeight = Graphics_GlobalSettings._defaultClientHeight;
}

void CGraphics::GetScreenSize( int& t_clientWidth, int& t_clientHeight )
{
    t_clientWidth = Graphics_GlobalSettings._currentClientWidth;
    t_clientHeight = Graphics_GlobalSettings._currentClientHeight;
}

void CGraphics::SetScreenSize( int t_clientWidth, int t_clientHeight )
{
    Graphics_GlobalSettings._currentClientWidth = t_clientWidth;
    Graphics_GlobalSettings._currentClientHeight = t_clientHeight;
}

CGraphics::CGraphics() : _d3d( nullptr )
{

}

CGraphics::~CGraphics()
{

}



bool CGraphics::Initialize( const int& t_clientWidth, const int& t_clientHeight, HWND t_hwnd )
{
    // Create the Direct3D object.
    _d3d = new CD3D();

    if( !_d3d )
    {
        return false;
    }

    // Initialize the Direct3D object.
    if( !_d3d->Initialize( t_hwnd, t_clientWidth, t_clientHeight, GetIsVsyncEnabled(), GetIsFullScreen() ) )
    {
        return false;
    }

    _initModules();
    _initScene();

    return true;
}

void CGraphics::ShutDown()
{
    _deinitModules();

    if( _d3d )
    {
        _d3d->ShutDown();
        delete _d3d;
        _d3d = nullptr;
    }
}

bool CGraphics::Frame()
{
    _update();
    return _render();
}

