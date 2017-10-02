#pragma once

#include "glm\fwd.hpp"

#include "Input.h"

class CView;
struct SRay;

/////////////////////////////////////////////////////////////////
//
//  Camera Base
//
//  We use right handedness convention.
//  and we name them right (for x-axis), up (for the y-axis)
//  and forward for the (z-axis).
//
//  forward = neg( facing )
//  https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/lookat-function
//
/////////////////////////////////////////////////////////////////
class CCamera
{
public:
    CCamera( glm::vec4 t_pos = glm::vec4( 0.f, 0.f, 0.f, 1.f ), glm::vec4 t_forward = glm::vec4( 0.f, 0.f, -1.f, 0.f ), glm::vec4 t_up = glm::vec4( 0.f, 1.f, 0.f, 0.f ) );
    virtual ~CCamera();

protected:
    /////////////////////////////////////////////////////////////////
    //
    //  camera position
    //
    /////////////////////////////////////////////////////////////////
    glm::vec4 _pos;
    /////////////////////////////////////////////////////////////////
    //
    //  camera facing direction
    //
    /////////////////////////////////////////////////////////////////
    glm::vec4 _forward;
    /////////////////////////////////////////////////////////////////
    //
    //  camera up direction
    //
    /////////////////////////////////////////////////////////////////
    glm::vec4 _up;

public:
    void Setup( const glm::vec4& t_pos, const glm::vec4& t_forward, const glm::vec4& t_up );

    glm::vec4 GetPos()
    {
        return _pos;
    }
    glm::vec4 GetForward()
    {
        return _forward;
    }

    void SetToView( CView* t_view );
    virtual void UpdateControl( double t_delta )
    {
    }
};

/////////////////////////////////////////////////////////////////
//
//  Freefly Camera
//
/////////////////////////////////////////////////////////////////
class CFreeFlyCamera : public CCamera
{
public:
    CFreeFlyCamera( glm::vec4 t_pos = glm::vec4( 0.f, 0.f, 0.f, 1.f ),
                    glm::vec4 t_aim = glm::vec4( 0.f, 0.f, 0.f, 1.f ),
                    glm::vec4 t_up = glm::vec4( 0.f, 1.f, 0.f, 0.f ) );

    virtual  ~CFreeFlyCamera()
    {
    }

protected:
    enum CAMERA_BEHAVIOR
    {
        CAMERA_PAN = 0,
        CAMERA_ORBIT,
        CAMERA_ZOOM,

        CAMERA_BEHAVIOR_COUNTER,
    };

    struct S_CAMERA_BEHAVIOR_DATA
    {
        int _cursorX;
        int _cursorY;
        CInput::KEY _key;
        float _speed;
    }_cameraHehaviors[CAMERA_BEHAVIOR_COUNTER];

    glm::vec4 _aim;

protected:
    SRay _getCameraCursorRayFromCursorPos( const double& t_x, const double& t_y );

public:
    void Update( double t_delta );

    /////////////////////////////////////////////////////////////////
    //
    //  Update camera controls
    //
    /////////////////////////////////////////////////////////////////
    virtual void UpdateControl( double t_delta );
};
