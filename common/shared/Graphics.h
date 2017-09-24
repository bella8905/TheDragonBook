#pragma once

#include <windows.h>

#include "Object.h"

class CD3D;
struct SViewPort;

class CGraphics
{
public:
    CGraphics();
    ~CGraphics();

    bool Initialize( const int& t_clientWidth, const int& t_clientHeight, HWND t_hwnd );
    void ShutDown();
    bool Frame();


    // global settings
    static bool GetIsFullScreen();
    static bool GetIsVsyncEnabled();
    static bool GetIs4xMSAAEnabled();
    static void SetIs4xMSAAEnabled( bool t_bEnabled );
    static void GetDefaultScreenSize( int& t_clientWidth, int& t_clientHeight );
    static void GetScreenSize( int& t_clientWidth, int& t_clientHeight );
    static void SetScreenSize( int t_clientWidth, int t_clientHeight );

    // public interfaces
    void SetViewport( const SViewPort& t_viewport );

protected:
    CD3D* _d3d;

protected:
    virtual void _initModules() { }
    virtual void _deinitModules() { }
    virtual void _initScene() { }

    virtual void _update() = 0;
    virtual bool _render() = 0;
};
