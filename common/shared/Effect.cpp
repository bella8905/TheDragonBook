#include <fstream>
#include <vector>

#include "D3DSystem.h"
#include "Utl_LogMsg.h"
#include "Utl_Global.h"
#include "Effect.h"

static const ID3DX11EffectTechnique* CurrentTechnique = nullptr;

CEffect::CEffect( ID3D11Device* t_device, const std::wstring& t_filename ) : _fx( nullptr )
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
            fVal->SetMatrix( &t_mat[0][0] );
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

const ID3DX11EffectTechnique* CEffect::GetCurrentTechnique()
{
    return CurrentTechnique;
}