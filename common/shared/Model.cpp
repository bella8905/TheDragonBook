#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include "Model.h"

CModel::CModel( std::string t_filename )
{
    _loadModelFromFile( t_filename );
}

void CModel::_loadModelFromFile( std::string t_filename )
{
}

void CModel::_initScene( const aiScene* t_aiScene )
{
}

void CModel::_initPrim( const aiMesh* t_aiMesh, SPrim& t_prim )
{
}

void CModel::_initMaterial( const aiMaterial* t_aiMat, SMaterial& t_material )
{
}