#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "Utl_Global.h"

struct SVertex
{
    glm::vec3 _position;
    glm::vec3 _normal;
    glm::vec3 _tangent;
    glm::vec4 _color;
    glm::vec2 _tex0;
    glm::vec2 _tex1;

    //////////////////////////////////////////////////////////////////////////
    // constructors
    //////////////////////////////////////////////////////////////////////////

    // default
    SVertex() {}
    // prelit vertex
    SVertex( glm::vec3 t_pos, glm::vec4 t_color );
    SVertex( glm::vec3 t_pos, glm::vec2 t_texCoord0, glm::vec2 t_texCoord1 = { 0.f, 0.f } );

    // lit vertex
    SVertex( glm::vec3 t_pos, glm::vec3 t_normal, glm::vec4 t_color );
    SVertex( glm::vec3 t_pos, glm::vec3 t_normal, glm::vec3 t_tangent, glm::vec4 t_color );
    SVertex( glm::vec3 t_pos, glm::vec3 t_normal, glm::vec3 t_tangent, glm::vec2 t_texCoord0, glm::vec2 t_texCoord1 = { 0.f, 0.f } );
};

struct SMesh
{
    std::vector<SVertex> _vertices;
    std::vector<uint>    _indices;
};
