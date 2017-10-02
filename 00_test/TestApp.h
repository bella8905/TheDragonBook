#pragma once

#include <vector>

#include "Graphics.h"
#include "System.h"

class CCamera;

class CTestApp : public CGraphics
{
public:
    CTestApp() : _gridVertexBuffer( nullptr ), _gridIndexBuffer( nullptr ), _numOfIndices( 0 ) {}

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

    // grid
    ID3D11Buffer* _gridVertexBuffer;
    ID3D11Buffer* _gridIndexBuffer;
    uint          _numOfIndices;

private:
    virtual void _initModules();
    virtual void _deinitModules();
    virtual void _initScene();

    void _initGrid();
    void _deinitGrid();
    void _drawGrid();

protected:
    virtual void _update();
    virtual bool _render();
};
