#include <fstream>
#include <vector>

#include "GeoGenerator.h"
#include "D3DSystem.h"
#include "Utl_LogMsg.h"
#include "Utl_Global.h"
#include "Effect.h"

static const D3D11_INPUT_ELEMENT_DESC LayoutDesc_Unlit[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof( SVertex, _position ) , D3D11_INPUT_PER_VERTEX_DATA , 0 },
    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT , 0, offsetof( SVertex, _color ), D3D11_INPUT_PER_VERTEX_DATA , 0 },
};

static const D3D11_INPUT_ELEMENT_DESC LayoutDesc_Lit[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof( SVertex, _position ) , D3D11_INPUT_PER_VERTEX_DATA , 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT , 0, offsetof( SVertex, _normal ), D3D11_INPUT_PER_VERTEX_DATA , 0 },
};

static const D3D11_INPUT_ELEMENT_DESC LayoutDesc_UnlitTextured[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof( SVertex, _position ) , D3D11_INPUT_PER_VERTEX_DATA , 0 },
    { "TEX0", 0, DXGI_FORMAT_R32G32B32A32_FLOAT , 0, offsetof( SVertex, _tex0 ), D3D11_INPUT_PER_VERTEX_DATA , 0 },
    { "TEX1", 0, DXGI_FORMAT_R32G32B32A32_FLOAT , 0, offsetof( SVertex, _tex1 ), D3D11_INPUT_PER_VERTEX_DATA , 0 },
};

static const D3D11_INPUT_ELEMENT_DESC LayoutDesc_LitTextured[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof( SVertex, _position ) , D3D11_INPUT_PER_VERTEX_DATA , 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT , 0, offsetof( SVertex, _normal ), D3D11_INPUT_PER_VERTEX_DATA , 0 },
    { "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT , 0, offsetof( SVertex, _tangent ), D3D11_INPUT_PER_VERTEX_DATA , 0 },
    { "TEX0", 0, DXGI_FORMAT_R32G32B32A32_FLOAT , 0, offsetof( SVertex, _tex0 ), D3D11_INPUT_PER_VERTEX_DATA , 0 },
    { "TEX1", 0, DXGI_FORMAT_R32G32B32A32_FLOAT , 0, offsetof( SVertex, _tex1 ), D3D11_INPUT_PER_VERTEX_DATA , 0 },
};

static struct
{
    const D3D11_INPUT_ELEMENT_DESC* _desc;
    uint _numOfDesc;
}InputLayoutDesc[LAYOUT_COUNTER] = {
    { LayoutDesc_Unlit, array_size_of( LayoutDesc_Unlit ) },
    { LayoutDesc_Lit, array_size_of( LayoutDesc_Lit ) },
    { LayoutDesc_UnlitTextured, array_size_of( LayoutDesc_UnlitTextured ) },
    { LayoutDesc_LitTextured, array_size_of( LayoutDesc_LitTextured ) },
};

static ID3DX11EffectTechnique* CurrentTechnique = nullptr;

CEffect::CEffect( ID3D11Device* t_device, const std::wstring& t_filename ) : _fx( nullptr ), _IALayout( nullptr )
{
    std::ifstream fin( t_filename, std::ios::binary );

    fin.seekg( 0, std::ios_base::end );
    int size = ( int )fin.tellg();
    fin.seekg( 0, std::ios_base::beg );
    std::vector<char> compiledShader( size );

    fin.read( &compiledShader[0], size );
    fin.close();

    if( FAILED( D3DX11CreateEffectFromMemory( &compiledShader[0], size, 0, t_device, &_fx ) ) )
    {
        LogError << "error creating effect" << LogEndl;
    }

    // log effect info
    LogMsg << "Effect:" << LogEndl;

    D3DX11_EFFECT_DESC desc;
    if( FAILED( _fx->GetDesc( &desc ) ) )
    {
        // log error
    }

    LogMsg << "Global Variables:" << desc.GlobalVariables << LogEndl;
    for( uint i = 0; i < desc.GlobalVariables; ++i )
    {
        auto pv = _fx->GetVariableByIndex( i );
        if( pv != nullptr )
        {
            D3DX11_EFFECT_VARIABLE_DESC variableDesc;
            if( FAILED( pv->GetDesc( &variableDesc ) ) )
            {
                // log error
            }
            LogMsg << "  Name: " << variableDesc.Name << LogEndl;

            // check if key already exists?
            std::string key( variableDesc.Name );
            auto got = _variables.find( key );
            if( got != _variables.end() )
            {
                LogError << "variable already exists" << variableDesc.Name << LogEndl;
            }
            else
            {
                _variables.insert( { std::string( variableDesc.Name ), pv } );
            }
        }
    }

    LogMsg << "Constant Buffers:" << desc.ConstantBuffers << LogEndl;
    for( uint i = 0; i < desc.ConstantBuffers; ++i )
    {
        auto pcb = _fx->GetConstantBufferByIndex( i );
        if( pcb != nullptr )
        {
            D3DX11_EFFECT_VARIABLE_DESC variableDesc;
            if( FAILED( pcb->GetDesc( &variableDesc ) ) )
            {
                // log error
            }
            LogMsg << "  Name: " << variableDesc.Name << LogEndl;
        }
    }

    LogMsg << "Techniques:" << desc.Techniques << LogEndl;
    for( uint i = 0; i < desc.Techniques; ++i )
    {
        auto pt = _fx->GetTechniqueByIndex( i );
        if( pt != nullptr )
        {
            D3DX11_TECHNIQUE_DESC techDesc;
            if( FAILED( pt->GetDesc( &techDesc ) ) )
            {
                // log error
            }
            LogMsg << "  Name: " << techDesc.Name << LogEndl;

            // check if key already exists?
            std::string key( techDesc.Name );
            auto got = _techniques.find( key );
            if( got != _techniques.end() )
            {
                LogError << "variable already exists" << techDesc.Name << LogEndl;
            }
            else
            {
                _techniques.insert( { std::string( techDesc.Name ), pt } );
            }
        }
    }

    LogMsg << "Groups:" << desc.Groups << LogEndl;
    for( uint i = 0; i < desc.Groups; ++i )
    {
        auto pg = _fx->GetGroupByIndex( i );
        if( pg != nullptr )
        {
            D3DX11_GROUP_DESC groupDesc;
            if( FAILED( pg->GetDesc( &groupDesc ) ) )
            {
                // log error
            }
            LogMsg << "  Name: " << groupDesc.Name << LogEndl;
        }
    }
}

CEffect::~CEffect()
{
    ReleaseCOM( _fx );
    ReleaseCOM( _IALayout );
}

bool CEffect::SetParameter( std::string t_name, float t_scalar )
{
    bool ret = false;
    auto got = _variables.find( t_name );
    if( got != _variables.end() )
    {
        auto fVal = got->second->AsScalar();
        if( fVal != nullptr )
        {
            fVal->SetFloat( t_scalar );
            ret = true;
        }
        else
        {
            LogError << "parameter not a scalar: " << t_name << LogEndl;
        }
    }
    else
    {
        LogError << "parameter not found: " << t_name << LogEndl;
    }

    return ret;
}

// vector
bool CEffect::SetParameter( std::string t_name, glm::vec4 t_vector )
{
    bool ret = false;
    auto got = _variables.find( t_name );
    if( got != _variables.end() )
    {
        auto fVal = got->second->AsVector();
        if( fVal != nullptr )
        {
            fVal->SetFloatVector( &t_vector[0] );
            ret = true;
        }
        else
        {
            LogError << "parameter not a vector: " << t_name << LogEndl;
        }
    }
    else
    {
        LogError << "parameter not found: " << t_name << LogEndl;
    }

    return ret;
}

bool CEffect::SetParameter( std::string t_name, glm::vec3 t_vector )
{
    bool ret = false;
    auto got = _variables.find( t_name );
    if( got != _variables.end() )
    {
        auto fVal = got->second->AsVector();
        if( fVal != nullptr )
        {
            fVal->SetRawValue( &t_vector[0], 0, sizeof( t_vector ) );
            ret = true;
        }
        else
        {
            LogError << "parameter not a vector: " << t_name << LogEndl;
        }
    }
    else
    {
        LogError << "parameter not found: " << t_name << LogEndl;
    }

    return ret;
}

bool CEffect::SetParameter( std::string t_name, glm::mat4 t_mat )
{
    bool ret = false;
    auto got = _variables.find( t_name );
    if( got != _variables.end() )
    {
        auto fVal = got->second->AsMatrix();
        if( fVal != nullptr )
        {
            // hlsl now uses col major matrix convention
            // unless a declare to use row major matrix is explicitly put.
            // Effect framework expects row major matrix ( which is somewhat the usual conduct for most D3D applications )
            // and it transposes the matrix before uploading it to gpu
            // Just use set matrix transpose as a quick fix.
            fVal->SetMatrixTranspose( &t_mat[0][0] );
            ret = true;
        }
        else
        {
            LogError << "parameter not a matrix: " << t_name << LogEndl;
        }
    }
    else
    {
        LogError << "parameter not found: " << t_name << LogEndl;
    }

    return ret;
}

bool CEffect::SetParameter( std::string t_name, void* t_rawVal, uint t_size )
{
    bool ret = false;
    auto got = _variables.find( t_name );
    if( got != _variables.end() )
    {
        auto fVal = got->second;
        if( fVal != nullptr )
        {
            fVal->SetRawValue( t_rawVal, 0, t_size );
            ret = true;
        }
        else
        {
            LogError << "parameter is null: " << t_name << LogEndl;
        }
    }
    else
    {
        LogError << "parameter not found: " << t_name << LogEndl;
    }

    return ret;
}

bool CEffect::SetTechniqueAsCurrent( std::string t_name )
{
    bool ret = false;
    auto got = _techniques.find( t_name );
    if( got != _techniques.end() )
    {
        auto tech = got->second;
        if( tech != nullptr )
        {
            CurrentTechnique = tech;
            ret = true;
        }
        else
        {
            LogError << "technique is null: " << t_name << LogEndl;
        }
    }
    else
    {
        LogError << "technique not found: " << t_name << LogEndl;
    }

    return ret;
}

ID3DX11EffectTechnique* Effect_GetCurrentTechnique()
{
    return CurrentTechnique;
}

void CEffect::InitIALayout( ID3D11Device* t_device, ID3D11DeviceContext* t_deviceContext, Effect_IALayoutType t_IAType )
{
    if( t_device == nullptr || _fx == nullptr ) return;
    D3DX11_PASS_DESC passDesc;
    ID3DX11EffectTechnique* technique = _techniques.begin()->second;
    if( technique == nullptr ) return;
    ID3DX11EffectPass* pass = technique->GetPassByIndex( 0 );
    if( pass == nullptr ) return;
    HR( pass->GetDesc( &passDesc ) );
    HR( t_device->CreateInputLayout( InputLayoutDesc[t_IAType]._desc, InputLayoutDesc[t_IAType]._numOfDesc,
        passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &_IALayout ) );

    t_deviceContext->IASetInputLayout( _IALayout );
}