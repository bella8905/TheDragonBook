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

void CGeoGenerator::BuildGrid( float t_width, float t_depth, uint t_m, uint t_n, glm::vec4 t_color, SMesh& t_meshOut )
{
    // assert for in params
    uint numOfVertices = t_m * t_n;
    t_meshOut._vertices.resize( numOfVertices );

    float dx = t_width / (float)( t_m - 1 );
    float dz = t_depth / (float)( t_n - 1 );

    float halfWidth = 0.5f * t_width;
    float halfDepth = 0.5f * t_depth;

    // i    , i+1
    // i+2  , i+3
    for( uint j = 0; j < t_n; ++j )
    {
        float z = -halfDepth + (float)j * dz;
        for( uint i = 0; i < t_m; ++i )
        {
            float x = -halfWidth + (float)i * dx;
            t_meshOut._vertices[j * t_m + i]._position = glm::vec3( x, 0.f, z );
            t_meshOut._vertices[j * t_m + i]._color = t_color;
        }
    }

    // indices
    uint numOfIndices = ( t_m - 1 ) * ( t_n - 1 ) * 2 * 3;
    t_meshOut._indices.resize( numOfIndices );

    uint indexIdx = 0;
    for( uint j = 0; j < t_n - 1; ++j )
    {
        for( uint i = 0; i < t_m - 1; ++i )
        {
            t_meshOut._indices[indexIdx++] = j * t_m + i;
            t_meshOut._indices[indexIdx++] = ( j + 1 ) * t_m + i;
            t_meshOut._indices[indexIdx++] = ( j + 1 ) * t_m + i + 1;
            t_meshOut._indices[indexIdx++] = ( j + 1 ) * t_m + i + 1;
            t_meshOut._indices[indexIdx++] = j * t_m + i + 1;
            t_meshOut._indices[indexIdx++] = j * t_m + i;
        }
    }
}
