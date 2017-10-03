#include "Utl_Math.h"

#include "GeoGenerator.h"

SVertex::SVertex( glm::vec3 t_pos, glm::vec4 t_color )
    : _position( t_pos ), _color( t_color )
    , _normal( 0.f ), _tangent( 0.f ), _texCoord0( 0.f ), _texCoord1( 0.f )
{
}

SVertex::SVertex( glm::vec3 t_pos, glm::vec2 t_texCoord0, glm::vec2 t_texCoord1 )
    : _position( t_pos ), _texCoord0( t_texCoord0 ), _texCoord1( t_texCoord1 )
    , _normal( 0.f ), _tangent( 0.f ), _color( 0.f )
{
}

SVertex::SVertex( glm::vec3 t_pos, glm::vec3 t_normal, glm::vec4 t_color )
    : _position( t_pos ), _normal( t_normal ),  _color( t_color )
    , _texCoord0( 0.f ), _texCoord1( 0.f ) /*, _tangent(  ) */
{
}

SVertex::SVertex( glm::vec3 t_pos, glm::vec3 t_normal, glm::vec3 t_tangent, glm::vec4 t_color )
    : _position( t_pos ), _normal( t_normal ), _tangent( t_tangent ), _color( t_color )
    , _texCoord0( 0.f ), _texCoord1( 0.f )
{
}

SVertex::SVertex( glm::vec3 t_pos, glm::vec3 t_normal, glm::vec3 t_tangent, glm::vec2 t_texCoord0, glm::vec2 t_texCoord1 /*= { 0.f, 0.f }*/ )
    : _position( t_pos ), _normal( t_normal ), _tangent( t_tangent ), _texCoord0( t_texCoord0 ), _texCoord1( t_texCoord1 )
    , _color( 0.f )

{
}

void CGeoGenerator::BuildCube( SMesh& t_meshOut )
{
}

void CGeoGenerator::BuildSphere( float t_radius, uint t_slices, uint t_stacks, glm::vec4 t_color, SMesh& t_meshOut )
{   
    uint numOfVertices = ( t_stacks + 1 ) * t_slices;
    t_meshOut._vertices.resize( numOfVertices );

    // i    , i+1
    // i+2  , i+3

    // iterating through points
    for( uint stackIdx = 0; stackIdx <= t_stacks; ++stackIdx )
    {
        float lat = g_Pi * ( (float)stackIdx / (float)t_stacks );
        float y = cosf( lat );
        float yr = sinf( lat );

        for( uint sliceIdx = 0; sliceIdx < t_slices; ++sliceIdx )
        {
            float lon = 2 * g_Pi * ( (float)sliceIdx / (float)t_slices );
            float x = yr * sinf( lon );
            float z = yr * cosf( lon );

            uint idx = stackIdx * t_slices + sliceIdx;
            t_meshOut._vertices[idx]._position = t_radius * glm::vec3( x, y, z );
            t_meshOut._vertices[idx]._normal = glm::vec3( x, y, z );
            t_meshOut._vertices[idx]._color = t_color;
        }
    }

    // indices
    uint numOfFaces = t_stacks * t_slices;
    uint numOfIndices = numOfFaces * 2 * 3;   // each face has two triangles, each triangle has 3 points;
    t_meshOut._indices.resize( numOfIndices );
    uint indexIdx = 0;
    // iterating through faces
    for( uint stackIdx = 0; stackIdx < t_stacks; ++stackIdx )
    {
        for( uint sliceIdx = 0; sliceIdx < t_slices; ++sliceIdx )
        {
            // 0 - 2 - 3
            t_meshOut._indices[indexIdx++] = stackIdx * t_slices + sliceIdx;
            t_meshOut._indices[indexIdx++] = ( stackIdx + 1 ) * t_slices + sliceIdx;
            t_meshOut._indices[indexIdx++] = sliceIdx == t_stacks - 1 ? ( stackIdx + 1 ) * t_slices : ( stackIdx + 1 ) * t_slices + sliceIdx + 1;
            // 0 - 3 - 1
            t_meshOut._indices[indexIdx++] = stackIdx * t_slices + sliceIdx;
            t_meshOut._indices[indexIdx++] = sliceIdx == t_stacks - 1 ? ( stackIdx + 1 ) * t_slices : ( stackIdx + 1 ) * t_slices + sliceIdx + 1;
            t_meshOut._indices[indexIdx++] = sliceIdx == t_stacks - 1 ? stackIdx * t_slices : stackIdx * t_slices + sliceIdx + 1;
        }
    }
}

// a grid with width and height,
// and m segs on width and n segs on height
void CGeoGenerator::BuildGrid( float t_width, float t_depth, uint t_m, uint t_n, glm::vec4 t_color, SMesh& t_meshOut )
{
    // assert for in params
    uint m = t_m + 1;
    uint n = t_n + 1;
    uint numOfVertices = m * n;
    t_meshOut._vertices.resize( numOfVertices );

    float dx = t_width / ( float )( m - 1 );
    float dz = t_depth / ( float )( n - 1 );

    float halfWidth = 0.5f * t_width;
    float halfDepth = 0.5f * t_depth;

    // i    , i+1
    // i+2  , i+3

    // vertices
    for( uint j = 0; j < n; ++j )
    {
        float z = -halfDepth + ( float )j * dz;
        for( uint i = 0; i < m; ++i )
        {
            float x = -halfWidth + ( float )i * dx;
            t_meshOut._vertices[j * m + i]._position = glm::vec3( x, 0.f, z );
            t_meshOut._vertices[j * m + i]._color = t_color;
        }
    }

    // indices
    uint numOfFaces = ( m - 1 ) * ( n - 1 );
    uint numOfIndices = numOfFaces * 4 * 2; // each face has four lines, each line has two points
    t_meshOut._indices.resize( numOfIndices );

    uint indexIdx = 0;
    for( uint j = 0; j < n - 1; ++j )
    {
        for( uint i = 0; i < m - 1; ++i )
        {
            // 0 - 2
            t_meshOut._indices[indexIdx++] = j * m + i;
            t_meshOut._indices[indexIdx++] = ( j + 1 ) * m + i;
            // 2 - 3
            t_meshOut._indices[indexIdx++] = ( j + 1 ) * m + i;
            t_meshOut._indices[indexIdx++] = ( j + 1 ) * m + i + 1;
            // 3 - 1
            t_meshOut._indices[indexIdx++] = ( j + 1 ) * m + i + 1;
            t_meshOut._indices[indexIdx++] = j * m + i + 1;
            // 1 - 0
            t_meshOut._indices[indexIdx++] = j * m + i + 1;
            t_meshOut._indices[indexIdx++] = j * m + i;
        }
    }
}