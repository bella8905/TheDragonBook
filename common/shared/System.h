#pragma once

#include <windows.h>

#include "FPS.h"

class CInput;
class CGraphics;
class CD3D;
class CApp;

class CSystem
{
public:
    CSystem( HINSTANCE t_instance, LPCWSTR t_appName = L"TheDragonBook" );
    ~CSystem();

private:
    static CSystem* _pActiveInstance;

public:
    static CSystem* GetActiveInstance()
    {
        return _pActiveInstance;
    }

    bool Initialize();
    void ShutDown();
    void Run();

    LRESULT CALLBACK MessageHandler( HWND t_hwnd, UINT t_umsg, WPARAM t_wparam, LPARAM t_lparam );

    CGraphics* GetGraphics()
    {
        return _graphics;
    }

private:
    bool _update();
    void _initWindow( int& t_width, int& t_height );
    void _shutDownWindow();

private:
    LPCWSTR _applicationName;
    HINSTANCE _hinstance;
    HWND _hwnd;

    CInput* _inputs;
    CGraphics* _graphics;
    CFPSCounter _fpsCounter;

protected:
    //////////////////////////////////////////////////////////////////////////
    // create per project app
    //////////////////////////////////////////////////////////////////////////
    virtual CGraphics* _createApp() = 0;
};

static LRESULT CALLBACK WndProc( HWND t_hwnd, UINT t_umsg, WPARAM t_wparam, LPARAM t_lparam );

// public interfaces
CGraphics* System_GetGraphics();
