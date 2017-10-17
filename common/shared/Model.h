#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "d3d11.h"
#include "Vertex.h"
#include "glm/glm.hpp"
#include "Utl_Global.h"

class CEffect;
struct aiMesh;
struct aiScene;
struct aiMaterial;

class CModel
{
public:
    CModel( std::string t_filename );
    ~CModel()
    {
    }

private:
    struct SMaterial
    {
        glm::vec3 _ambient;
        glm::vec3 _diffuse;
        glm::vec3 _specular;
        float     _specularExp;
        glm::vec3 _reflectivity;

        CEffect* _effect;
        ID3D11ShaderResourceView* _diffuseMapSRV;
        ID3D11ShaderResourceView* _normalMapSRV;

        SMaterial() : _effect( nullptr ), _diffuseMapSRV( nullptr ), _normalMapSRV( nullptr ) {}
        bool IsEqual( const SMaterial& t_rhs )
        {
            return _ambient == t_rhs._ambient && _diffuse == t_rhs._diffuse && _specular == t_rhs._specular && _specularExp == t_rhs._specularExp &&
                _reflectivity == t_rhs._reflectivity && _effect == t_rhs._effect && _diffuseMapSRV == t_rhs._diffuseMapSRV && _normalMapSRV == t_rhs._normalMapSRV;
        }
    };

    // a model can have a list of prims
    // a prim is a group of triangles that can all be rendered using the same material.
    // ie., same effect, textures ad render state
    struct SPrim
    {
        uint _vertexStart;
        uint _vertexCount;
        uint _indexStart;
        uint _indexCount;

        SMaterial* _material;

        SPrim() : _material( nullptr ), _vertexStart( 0 ), _vertexCount( 0 ), _indexStart( 0 ), _indexCount( 0 )
        {}
    };

    std::vector<SMaterial> _materials;
    std::vector<SVertex> _vertices;
    std::vector<uint> _indices;
    std::vector<SPrim> _prims;

    // TODO:
    // have a texture manager and move the collection there.
    std::unordered_map<std::string, ID3D11ShaderResourceView*> textures;

private:
    void _loadModelFromFile( std::string t_filename );
    void _initScene( const aiScene* t_aiScene );
    void _initPrim( const aiMesh* t_aiMesh, SPrim& t_prim );
    void _initMaterial( const aiMaterial* t_aiMat, SMaterial& t_material );
};