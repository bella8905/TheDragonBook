#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

#include "Utl_Global.h"
#include "Utl_Math.h"
#include "Utl_LogMsg.h"
#include "D3DSystem.h"
#include "Shader.h"
#include "Geometry.h"
#include "View.h"
#include "Camera.h"
#include "GeoGenerator.h"
#include "Effect.h"

#include "TestApp.h"

void CTestApp::SCENE::Init()
{
    // init objects
    CObject box( GEO_BOX );
    glm::mat4 rot_x30 = glm::rotate( glm::mat4(), 30 * g_o2Pi, glm::vec3( 1, 0, 0 ) );
    box.SetRot( Matrix_ToMat3( rot_x30 ) );
    _objects.push_back( box );

    // view matrix
    glm::vec3 camPos( 0.f, 2.f, 10.0f );
    glm::vec3 camTarget( 0.f, 0.f, 0.f );
    _pFreeflyCam = new CFreeFlyCamera( ToPositon( camPos ), ToPositon( camTarget ), glm::vec4( 0.f, 1.f, 0.f, 0.f ) );

    CView view;
    // SViewPort viewport;
    // view.SetViewport( viewport );
    _pFreeflyCam->SetToView( &view );
    // view.SetCameraPostionFaceAndUp( ToPositon( camPos ), ToDirection( camFace ) );
    view.SetHorizontalFieldOfView( DegToRad( 80.f ) );
    View_SetAsActive( &view );

    //     // test
    //     glm::vec4 point{ 0.f, 0.f, 0.f, 1.f };
    //     // pos in camera view
    //     point = view.GetWorld2ViewMatrix() * point;
    //     // pos in ndc
    //     point = view.GetView2ProjMatrix() * point;
}

void CTestApp::SCENE::Deinit()
{
    if( _pFreeflyCam )
    {
        delete _pFreeflyCam;
        _pFreeflyCam = nullptr;
    }
}

void CTestApp::SCENE::Update()
{
    _pFreeflyCam->UpdateControl( 1.f / 60.f );
    _pFreeflyCam->SetToView( View_GetActive() );
}

void CTestApp::SCENE::Draw()
{
    for( uint i = 0, numOfObj = ( uint )_objects.size(); i < numOfObj; ++i )
    {
        Shaders_BindShader( &_objects[i] );
        _objects[i].Draw();
    }
}

void CTestApp::_initEffect()
{
    _effect = new CEffect( _d3d->GetDevice(), L"unlit.fxo" );
    _effect->InitIALayout( _d3d->GetDevice(), _d3d->GetDeviceContext(), UNLIT_COLORED );
    _effect->SetTechniqueAsCurrent( "UnlitTech" );
}

void CTestApp::_deinitEffect()
{
    if( _effect != nullptr )
    {
        delete _effect;
        _effect = nullptr;
    }
}

void CTestApp::_initGrid()
{
    glm::vec4 red( 1.f, 0.f, 0.f, 1.f );
    glm::vec4 grey( 0.3f, 0.3f, 0.3f, 1.f );

    CGeoGenerator geoGen;
    SMesh grid;
    float gridWidth = 10;
    float gridDepth = 10;
    uint m = 10;
    uint n = 10;

    geoGen.BuildGrid( gridWidth, gridDepth, m, n, grey, grid );
    _gridNumOfIndices = ( uint )grid._indices.size();
    if( !_d3d->CreateBufferFromMeshData( grid, &_gridVertexBuffer, &_gridIndexBuffer ) )
    {
        LogError << "error creating grid" << LogEndl;
    }

    SMesh sphere;
    geoGen.BuildGeosphere( 3.f, 3, red, sphere );
    _sphereNumOfIndices = ( uint )sphere._indices.size();
    if( !_d3d->CreateBufferFromMeshData( sphere, &_sphereVertexBuffer, &_sphereIndexBuffer ) )
    {
        LogError << "error creating sphere" << LogEndl;
    }
}

void CTestApp::_deinitGrid()
{
    // release grid buffer
    ReleaseCOM( _gridVertexBuffer );
    ReleaseCOM( _gridIndexBuffer );
    ReleaseCOM( _sphereVertexBuffer );
    ReleaseCOM( _sphereIndexBuffer );
}

void CTestApp::_initScene()
{
    assert( _d3d != nullptr );

    _scene.Init();
}

void CTestApp::_initModules()
{
    // init geo
    CGeoContainer::GetInstance().Init( _d3d->GetDevice(), _d3d->GetDeviceContext() );
    // init shader
    _initEffect();

    _initGrid();
}

void CTestApp::_deinitModules()
{
    _deinitGrid();
    // shaders
    _deinitEffect();

    // geo container
    CGeoContainer::GetInstance().Deinit();
}

void CTestApp::_update()
{
    _scene.Update();
}

bool CTestApp::_render()
{
    if( !_d3d ) return false;

    _d3d->BeginScene( 0.8f, 0.8f, 0.8f, 1.f );

    _effect->SetParameter( "view", View_GetActive()->GetWorld2ViewMatrix() );
    _effect->SetParameter( "projection", View_GetActive()->GetView2ProjMatrix() );

    D3DX11_TECHNIQUE_DESC techDesc;
    Effect_GetCurrentTechnique()->GetDesc( &techDesc );
    for( uint p = 0; p < techDesc.Passes; ++p )
    {
        ID3DX11EffectPass* pass = Effect_GetCurrentTechnique()->GetPassByIndex( p );
        pass->Apply( 0, _d3d->GetDeviceContext() );

        _d3d->SetRS( WIREFRAME );
        _effect->SetParameter( "model", glm::mat4( 1.f ) );
        _d3d->DrawIndexed( _gridVertexBuffer, _gridIndexBuffer, _gridNumOfIndices, D3D11_PRIMITIVE_TOPOLOGY_LINELIST );

        _d3d->SetRS( FILLED );
        _d3d->DrawIndexed( _sphereVertexBuffer, _sphereIndexBuffer, _sphereNumOfIndices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
        // _scene.Draw();
    }
    _d3d->EndScene();
    return true;
}