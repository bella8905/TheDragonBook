#pragma once

#include <d3d11.h>
#include <windows.h>
#include <string>

struct SMesh;

/////////////////////////////////////////////////////////////////
// Convenience macro for releasing COM objects.
/////////////////////////////////////////////////////////////////
#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

struct SViewPort;

enum RasterizerState
{
    FILLED,
    WIREFRAME,
};

class CD3D
{
public:
    CD3D();
    ~CD3D();

public:
    virtual bool Initialize( HWND t_hwnd );
    virtual void ShutDown();

    ID3D11Device* GetDevice()
    {
        return _device;
    }
    ID3D11DeviceContext* GetDeviceContext()
    {
        return _deviceContext;
    }

    void BeginScene( float, float, float, float );
    void EndScene();

    void SetViewport( const SViewPort& t_viewport );
private:
    struct VIDEOCARD_INFO
    {
        int         _videoCardMemory;
        int         _dedicatedSystemMemory;
        int         _sharedSystemMemory;
        std::string _videoCardDescription;
        //         struct
        //         {
        //             UINT _numerator;
        //             UINT _denominator;
        //         }_refreshRate;

        void PrintInfo();
    } _videoCardInfo;

    UINT                     _4xMsaaQuality;
    IDXGISwapChain*          _swapChain;
    ID3D11Device*            _device;
    ID3D11DeviceContext*     _deviceContext;
    ID3D11RenderTargetView*  _renderTargetView;
    ID3D11Texture2D*         _depthStencilBuffer;
    ID3D11DepthStencilState* _depthStencilState;
    ID3D11DepthStencilView*  _depthStencilView;

    ID3D11RasterizerState*   _rsFilled;
    ID3D11RasterizerState*   _rsWireframe;

private:
    bool _createPresetRS();
    void _destroyPresetRS();

    // initiaize stages
    bool _updateVideoCardInfo();
    bool _createDevice();
    bool _checkAndSetupMSAA();
    bool _createSwapChain( HWND t_hwnd );
    bool _createRenderTargetView();
    bool _createDepthStensilBuffer();
    bool _bindViews();
    bool _setupRasterizer();
    void _setDefaultViewport();

public:
    bool CreateBufferFromMeshData( const SMesh& t_mesh, ID3D11Buffer** t_vertexBufferOut, ID3D11Buffer** t_indexBufferOut = nullptr );
    void SetRS( RasterizerState t_rs );

    // draw
    void Draw( ID3D11Buffer*& t_vertexBuffer, UINT t_numOfVertices, UINT t_vertexOffset = 0 );
    void DrawIndexed( ID3D11Buffer*& t_vertexBuffer, ID3D11Buffer*& t_indexBuffer, UINT t_numOfIndices, D3D11_PRIMITIVE_TOPOLOGY t_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, UINT t_vertexOffset = 0, UINT t_indexOffset = 0 );
};
