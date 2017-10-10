#include <algorithm>
#include "Utl_Math.h"
#include "glm\gtx\quaternion.hpp"

#include "GeoGenerator.h"

SVertex::SVertex( glm::vec3 t_pos, glm::vec4 t_color )
    : _position( t_pos ), _color( t_color )
    , _normal( 0.f ), _tangent( 0.f ), _tex0( 0.f ), _tex1( 0.f )
{
}

SVertex::SVertex( glm::vec3 t_pos, glm::vec2 t_texCoord0, glm::vec2 t_texCoord1 )
    : _position( t_pos ), _tex0( t_texCoord0 ), _tex1( t_texCoord1 )
    , _normal( 0.f ), _tangent( 0.f ), _color( 0.f )
{
}

SVertex::SVertex( glm::vec3 t_pos, glm::vec3 t_normal, glm::vec4 t_color )
    : _position( t_pos ), _normal( t_normal ), _color( t_color )
    , _tex0( 0.f ), _tex1( 0.f ) /*, _tangent(  ) */
{
}

SVertex::SVertex( glm::vec3 t_pos, glm::vec3 t_normal, glm::vec3 t_tangent, glm::vec4 t_color )
    : _position( t_pos ), _normal( t_normal ), _tangent( t_tangent ), _color( t_color )
    , _tex0( 0.f ), _tex1( 0.f )
{
}

SVertex::SVertex( glm::vec3 t_pos, glm::vec3 t_normal, glm::vec3 t_tangent, glm::vec2 t_texCoord0, glm::vec2 t_texCoord1 /*= { 0.f, 0.f }*/ )
    : _position( t_pos ), _normal( t_normal ), _tangent( t_tangent ), _tex0( t_texCoord0 ), _tex1( t_texCoord1 )
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
        float phi = g_Pi * ( ( float )stackIdx / ( float )t_stacks );
        float y = cosf( phi );
        float yr = sinf( phi );

        for( uint sliceIdx = 0; sliceIdx < t_slices; ++sliceIdx )
        {
            float theta = 2 * g_Pi * ( ( float )sliceIdx / ( float )t_slices );
            float x = yr * sinf( theta );
            float z = yr * cosf( theta );

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

void CGeoGenerator::BuildGeosphere( float t_radius, uint t_numSubs, glm::vec4 t_color, SMesh& t_meshOut )
{
    // Put a cap on the number of subdivisions.
    t_numSubs = std::min( t_numSubs, 5u );

    // Approximate a sphere by tessellating an icosahedron.

    const float x = 0.525731f;
    const float z = 0.850651f;

    glm::vec3 pos[] =
    {
        glm::vec3( -x, 0.0f, z ),  glm::vec3( x, 0.0f, z ),
        glm::vec3( -x, 0.0f, -z ), glm::vec3( x, 0.0f, -z ),
        glm::vec3( 0.0f, z, x ),   glm::vec3( 0.0f, z, -x ),
        glm::vec3( 0.0f, -z, x ),  glm::vec3( 0.0f, -z, -x ),
        glm::vec3( z, x, 0.0f ),   glm::vec3( -z, x, 0.0f ),
        glm::vec3( z, -x, 0.0f ),  glm::vec3( -z, -x, 0.0f )
    };

    uint indices[] =
    {
        1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
        1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
        3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
        10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
    };

    uint numOfVertices = array_size_of( pos );
    uint numOfIndices = array_size_of( indices );
    t_meshOut._vertices.resize( numOfVertices );
    t_meshOut._indices.resize( numOfIndices );

    for( uint i = 0; i < numOfVertices; ++i )
    {
        t_meshOut._vertices[i]._position = pos[i];
    }

    for( uint i = 0; i < numOfIndices; ++i )
    {
        t_meshOut._indices[i] = indices[i];
    }

    for( uint i = 0; i < t_numSubs; ++i )
    {
        _subdivide( t_meshOut );
    }

    // Project vertices onto sphere and scale.
    for( uint i = 0; i < t_meshOut._vertices.size(); ++i )
    {
        // Project onto unit sphere.
        glm::vec3 n = glm::normalize( t_meshOut._vertices[i]._position );

        // Project onto sphere.
        t_meshOut._vertices[i]._position = t_radius * n;
        t_meshOut._vertices[i]._normal = n;
        t_meshOut._vertices[i]._color = t_color;
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

void CGeoGenerator::_subdivide( SMesh& t_meshInOut )
{
    // Save a copy of the input geometry.
    SMesh inputCopy = t_meshInOut;

    t_meshInOut._vertices.resize( 0 );
    t_meshInOut._indices.resize( 0 );

    //       v1
    //       *
    //      / \
    //     /   \
	//  m0*-----*m1
    //   / \   / \
	//  /   \ /   \
	// *-----*-----*
    // v0    m2     v2

    uint numTris = ( uint )inputCopy._indices.size() / 3;
    for( uint i = 0; i < numTris; ++i )
    {
        SVertex v0 = inputCopy._vertices[inputCopy._indices[i * 3 + 0]];
        SVertex v1 = inputCopy._vertices[inputCopy._indices[i * 3 + 1]];
        SVertex v2 = inputCopy._vertices[inputCopy._indices[i * 3 + 2]];

        //
        // Generate the midpoints.
        //

        SVertex m0, m1, m2;

        // For subdivision, we just care about the position component.  We derive the other
        // vertex components in CreateGeosphere.

        m0._position = glm::vec3(
            0.5f*( v0._position.x + v1._position.x ),
            0.5f*( v0._position.y + v1._position.y ),
            0.5f*( v0._position.z + v1._position.z ) );

        m1._position = glm::vec3(
            0.5f*( v1._position.x + v2._position.x ),
            0.5f*( v1._position.y + v2._position.y ),
            0.5f*( v1._position.z + v2._position.z ) );

        m2._position = glm::vec3(
            0.5f*( v0._position.x + v2._position.x ),
            0.5f*( v0._position.y + v2._position.y ),
            0.5f*( v0._position.z + v2._position.z ) );

        //
        // Add new geometry.
        //

        t_meshInOut._vertices.push_back( v0 ); // 0
        t_meshInOut._vertices.push_back( v1 ); // 1
        t_meshInOut._vertices.push_back( v2 ); // 2
        t_meshInOut._vertices.push_back( m0 ); // 3
        t_meshInOut._vertices.push_back( m1 ); // 4
        t_meshInOut._vertices.push_back( m2 ); // 5

        t_meshInOut._indices.push_back( i * 6 + 0 );
        t_meshInOut._indices.push_back( i * 6 + 3 );
        t_meshInOut._indices.push_back( i * 6 + 5 );

        t_meshInOut._indices.push_back( i * 6 + 3 );
        t_meshInOut._indices.push_back( i * 6 + 4 );
        t_meshInOut._indices.push_back( i * 6 + 5 );

        t_meshInOut._indices.push_back( i * 6 + 5 );
        t_meshInOut._indices.push_back( i * 6 + 4 );
        t_meshInOut._indices.push_back( i * 6 + 2 );

        t_meshInOut._indices.push_back( i * 6 + 3 );
        t_meshInOut._indices.push_back( i * 6 + 1 );
        t_meshInOut._indices.push_back( i * 6 + 4 );
    }
}