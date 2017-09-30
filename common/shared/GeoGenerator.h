#include <vector>
#include "glm/glm.hpp"
#include "Utl_Global.h"

struct SVertex
{
    glm::vec3 _position;
    glm::vec3 _normal;
    glm::vec3 _tangent;
    glm::vec4 _color;
    glm::vec2 _texCoord0;
    glm::vec2 _texCoord1;

    SVertex() : _position( 0.f ), _normal( 0.f, 0.f, -1.f ), _tangent( 1.f, 0.f, 0.f ),
        _color( 1.f, 0.f, 0.f, 1.f ), _texCoord0( 0.f, 0.f ), _texCoord1( 0.f, 0.f )
    {

    }

    //////////////////////////////////////////////////////////////////////////
    // constructors
    //////////////////////////////////////////////////////////////////////////

    // prelit vertex
    SVertex( glm::vec3 t_pos, glm::vec4 t_color );
    SVertex( glm::vec3 t_pos, glm::vec2 t_texCoord0, glm::vec2 t_texCoord1 = { 0.f, 0.f }  );

    // lit vertex
    SVertex( glm::vec3 t_pos, glm::vec3 t_normal, glm::vec3 t_tangent, glm::vec4 t_color );
    SVertex( glm::vec3 t_pos, glm::vec3 t_normal, glm::vec3 t_tangent, glm::vec2 t_texCoord0, glm::vec2 t_texCoord1 = { 0.f, 0.f } );
};

struct SMesh
{
    std::vector<SVertex> _vertices;
    std::vector<uint>    _indices;
};

class CGeoGenerator
{
public:
    CGeoGenerator() { }
    ~CGeoGenerator() { }

public:
    void BuildUnitCube( SMesh& t_meshOut );
    void BuildUnitSphere( SMesh& t_meshOut );

    // build a grid with m x n vertices
    void BuildGrid( float t_width, float t_depth, uint t_m, uint t_n, glm::vec4 t_color, SMesh& t_meshOut );
};
