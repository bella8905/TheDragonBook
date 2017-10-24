#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Utl_LogMsg.h"
#include "Model.h"

CModel::CModel( std::string t_filename ) : _bDumpSceneInfo( true )
{
    _loadModelFromFile( t_filename );
}

void CModel::_loadModelFromFile( std::string t_filename )
{
    // read from model file
    Assimp::Importer importer;
    // remove components
    importer.SetPropertyInteger( AI_CONFIG_PP_RVC_FLAGS,
        aiComponent_CAMERAS |
        aiComponent_LIGHTS |
        aiComponent_ANIMATIONS |
        aiComponent_COLORS             // remove vertex colors so meshes with different vertex colors but same other attributes can be joined
    );
    const aiScene* scene = importer.ReadFile( t_filename,
        aiProcess_Triangulate |
        aiProcess_SortByPType |                                          // sort by primitive type
        aiProcess_JoinIdenticalVertices |
        aiProcess_PreTransformVertices |                                 // remove node hierarchy. animations are removed. SUPPORT SCENE GRAPH LATER?
        aiProcess_GenNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_RemoveRedundantMaterials |                             // remove redundant/unreferenced materials
        aiProcess_RemoveComponent |
        aiProcess_FlipUVs |                                              // for D3D, flip uv
        aiProcess_ValidateDataStructure                                  // validate scene
    );

    if( !scene )
    {
        LogError << "can't read model: " << t_filename << LogEndl;
        return;
    }

    if( _bDumpSceneInfo )
    {
        // scene processing
        LogMsg << "***************Model loaded: " << t_filename << LogEndl;
        LogMsg << "   " << scene->mNumAnimations << " animations" << LogEndl;
        LogMsg << "   " << scene->mNumCameras << " cameras" << LogEndl;
        LogMsg << "   " << scene->mNumLights << " lights" << LogEndl;
        LogMsg << "   " << scene->mNumMaterials << " materials" << LogEndl;
        LogMsg << "   " << scene->mNumMeshes << " meshes" << LogEndl;
        LogMsg << "   " << scene->mNumTextures << " textures" << LogEndl;
    }

    _initScene( scene );
    // initMeshes( scene );
    // initMaterials( scene );
}

void CModel::_initScene( const aiScene* t_aiScene )
{
    for( uint i = 0; i < t_aiScene->mNumMeshes; ++i )
    {
        const aiMesh* assMesh = t_aiScene->mMeshes[i];
        SPrim prim;
        _initPrim( assMesh, prim, _vertices, _indices );
        _prims.push_back( prim );
    }
}

void CModel::_initPrim( const aiMesh* t_aiMesh, SPrim& t_prim, std::vector<SVertex>& t_vertices, std::vector<uint>& t_indices )
{
    if( t_aiMesh == nullptr ) return;
    t_prim._vertexStart = t_vertices.size();
    t_prim._indexStart = t_indices.size();

    // vertices
    uint numOfVertices = t_aiMesh->mNumVertices;
    t_prim._vertexCount = numOfVertices;

    std::vector<SVertex> vertices( numOfVertices );
    for( int i = 0; i < numOfVertices; ++i )
    {
        // position
        const aiVector3D* pos = &( t_aiMesh->mVertices[i] );
        vertices[i]._position = glm::vec3( ( float )pos->x, ( float )pos->y, ( float )pos->z );
        // normal
        if( t_aiMesh->HasNormals() )
        {
            const aiVector3D* normal = &( t_aiMesh->mNormals[i] );
            vertices[i]._normal = glm::vec3( ( float )normal->x, ( float )normal->y, ( float )normal->z );
        }
        // tangent
        if( t_aiMesh->HasTangentsAndBitangents() )
        {
            const aiVector3D* tangent = &( t_aiMesh->mTangents[i] );
            vertices[i]._tangent = glm::vec3( ( float )tangent->x, ( float )tangent->y, ( float )tangent->z );
        }
        // tex0
        if( t_aiMesh->HasTextureCoords( 0 ) )
        {
            const aiVector3D* tex = &( t_aiMesh->mTextureCoords[0][i] );
            vertices[i]._tex0 = glm::vec2( ( float )tex->x, ( float )tex->y );
        }
        // tex1
        if( t_aiMesh->HasTextureCoords( 1 ) )
        {
            const aiVector3D* tex = &( t_aiMesh->mTextureCoords[1][i] );
            vertices[i]._tex1 = glm::vec2( ( float )tex->x, ( float )tex->y );
        }
    }

    t_vertices.insert( t_vertices.end(), vertices.begin(), vertices.end() );

    // indices
    std::vector<uint> indices;
    if( t_aiMesh->HasFaces() )
    {
        for( unsigned int i = 0; i < t_aiMesh->mNumFaces; ++i )
        {
            const aiFace* face = &( t_aiMesh->mFaces[i] );
            for( unsigned int j = 0; j < face->mNumIndices; ++j )
            {
                indices.push_back( face->mIndices[j] );
            }
        }
    }
    t_prim._indexCount = indices.size();
    t_indices.insert( t_indices.end(), indices.begin(), indices.end() );
}

void CModel::_initMaterial( const aiMaterial* t_aiMat, SMaterial& t_material )
{
}