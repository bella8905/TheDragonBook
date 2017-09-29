#include "GeoGenerator.h"




SVertex::SVertex( glm::vec3 t_pos, glm::vec4 t_color )
    : _position( t_pos ), _color( t_color )
    , _normal( 0.f ), _tangent( 0.f ), _texCoord0( 0.f ), _texCoord1( 0.f )
{

}

SVertex::SVertex( glm::vec3 t_pos, glm::vec2 t_texCoord0, glm::vec2 t_texCoord1 = { 0.f, 0.f } )
    : _position( t_pos ), _texCoord0( t_texCoord0 ), _texCoord1( t_texCoord1 )
    , _normal( 0.f ), _tangent( 0.f ), _color( 0.f )
{

}

SVertex::SVertex( glm::vec3 t_pos, glm::vec3 t_normal, glm::vec3 t_tangent, glm::vec4 t_color ) 
    : _position( t_pos ), _normal( t_normal ), _tangent( t_tangent ), _color( t_color )
    , _texCoord0( 0.f ), _texCoord1( 0.f )
{

}

SVertex::SVertex( glm::vec3 t_pos, glm::vec3 t_normal, glm::vec3 t_tangent, glm::vec2 t_texCoord0, glm::vec2 t_texCoord1 = { 0.f, 0.f } )
    : _position( t_pos ), _normal( t_normal ), _tangent( t_tangent ), _texCoord0( t_texCoord0 ), _texCoord1( t_texCoord1 )
    , _color( 0.f )
    
{

}


void CGeoGenerator::BuildUnitCube( SMesh& t_meshOut )
{

}

void CGeoGenerator::BuildUnitSphere( SMesh& t_meshOut )
{

}

void CGeoGenerator::BuildGrid( float t_width, float  , uint t_m, uint t_n, SMesh& t_meshOut )
{

}