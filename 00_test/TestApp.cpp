#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

#include "Utl_Global.h"
#include "Utl_Math.h"
#include "D3DSystem.h"
#include "Shader.h"
#include "Geometry.h"
#include "View.h"
#include "Camera.h"


#include "TestApp.h"

void CTestApp::SCENE::Init()
{
    // init objects
    CObject box( GEO_BOX );
    glm::mat4 rot_x30 = glm::rotate( glm::mat4(), 30 * g_o2Pi, glm::vec3( 1, 0, 0 ) );
    box.SetRot( Matrix_ToMat3( rot_x30 ) );
    _objects.push_back( box );


    // view matrix
    glm::vec3 camPos( 0.f, 0.f, -2.0f );
    glm::vec3 camTarget( 0.f, 0.f, 0.f );
    float dist = glm::distance( camPos, camTarget );
    _pFreeflyCam = new CFreeFlyCamera( ToPositon( camPos ), ToPositon( camTarget ), glm::vec4( 0.f, 1.f, 0.f, 0.f ) );

    CView view;
    // SViewPort viewport;
    // view.SetViewport( viewport );
    _pFreeflyCam->SetToView( &view );
    // view.SetCameraPostionFaceAndUp( ToPositon( camPos ), ToDirection( camFace ) );
    view.SetHorizontalFieldOfView( DegToRad( 80.f ) );
    View_SetAsActive( &view );
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
    for( uint i = 0, numOfObj = _objects.size(); i < numOfObj; ++i )
    {
        Shaders_BindShader( &_objects[i] );
        _objects[i].Draw();
    }
}


void CTestApp::_initScene()
{
    _scene.Init();
}

void CTestApp::_initModules()
{
    // init geo
    CGeoContainer::GetInstance().Init( _d3d->GetDevice(), _d3d->GetDeviceContext() );
    // init shader
    g_mvpVS.Init( _d3d->GetDevice(), _d3d->GetDeviceContext() );
    g_simpleColorPS.Init( _d3d->GetDevice(), _d3d->GetDeviceContext() );
    Shaders_SetAsActive( VS, &g_mvpVS );
    Shaders_SetAsActive( PS, &g_simpleColorPS );
}

void CTestApp::_deinitModules()
{
    // shaders
    g_mvpVS.Deinit();
    g_simpleColorPS.Deinit();
    Shaders_Clear();

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

    _scene.Draw();

    _d3d->EndScene();
    return true;
}
