#pragma once

#include <vector>

#include "Graphics.h"
#include "System.h"

class CCamera;

class CTestApp : public CGraphics
{
private:
    struct SCENE
    {
        std::vector<CObject> _objects;
        CCamera* _pFreeflyCam;

        SCENE() : _pFreeflyCam( nullptr )
        {

        }

        void Init();
        void Deinit();
        void Update();
        void Draw();

    } _scene;

private:
    virtual void _initModules();
    virtual void _deinitModules();
    virtual void _initScene();

protected:
    virtual void _update();
    virtual bool _render();
};

class CTestSystem : public CSystem
{
public:
    CTestSystem( HINSTANCE t_instance ) : CSystem( t_instance )
    {
    }

protected:
    virtual CGraphics* _createApp()
    {
        return new CTestApp();
    }
};