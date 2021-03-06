#pragma once

#include <vector>

#include "Graphics.h"
#include "System.h"

class CCamera;
class CEffect;

class CTestApp : public CGraphics
{
public:
    CTestApp() : _gridVertexBuffer( nullptr ), _gridIndexBuffer( nullptr ), _gridNumOfIndices( 0 ), _sphereVertexBuffer( nullptr ), _sphereIndexBuffer( nullptr ), _sphereNumOfIndices( 0 )
    {
    }

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
    uint          _gridNumOfIndices;

    // sphere
    ID3D11Buffer* _sphereVertexBuffer;
    ID3D11Buffer* _sphereIndexBuffer;
    uint          _sphereNumOfIndices;

    // effect
    CEffect*      _effect;

private:
    virtual void _initModules();
    virtual void _deinitModules();
    virtual void _initScene();

    void _initEffect();
    void _deinitEffect();
    void _initGrid();
    void _deinitGrid();

protected:
    virtual void _update();
    virtual bool _render();
};
