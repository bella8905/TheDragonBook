#pragma once

#include <vector>

#include "Graphics.h"
#include "System.h"

class CCamera;

class CTestApp : public CGraphics
{
public:
	CTestApp() : _gridVertexBuffer( nullptr ), _gridIndexBuffer( nullptr ) {}

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

	ID3D11Buffer* _gridVertexBuffer;
	ID3D11Buffer* _gridIndexBuffer;

private:
    virtual void _initModules();
    virtual void _deinitModules();
    virtual void _initScene();

protected:
    virtual void _update();
    virtual bool _render();
};

