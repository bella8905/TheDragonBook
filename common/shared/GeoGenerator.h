#pragma once

#include <vector>

#include "glm/glm.hpp"
#include "Utl_Global.h"
#include "Vertex.h"

class CGeoGenerator
{
public:
    CGeoGenerator() {}
    ~CGeoGenerator() {}

public:
    void BuildCube( SMesh& t_meshOut );
    void BuildSphere( float t_radius, uint t_slices, uint t_stacks, glm::vec4 t_color, SMesh& t_meshOut );
    // the triangles of the sphere do not have equal areas
    // and it has poles.
    // A geosphere approximates a sphere using triangles
    // with almost equal areas as well as equal side lengths
    void BuildGeosphere( float t_radius, uint t_numSubs, glm::vec4 t_color, SMesh& t_meshOut );

    // build a grid with m x n vertices
    void BuildGrid( float t_width, float t_depth, uint t_m, uint t_n, glm::vec4 t_color, SMesh& t_meshOut );

private:
    void _subdivide( SMesh& t_meshInOut );
};
