#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"
#include "d3dx11Effect.h"

enum Effect_IALayoutType
{
    UNLIT_COLORED,
    LIT_COLORED,
    UNLIT_TEXTURED,
    LIT_TEXTURED,

    //
    LAYOUT_COUNTER,
};

class CEffect
{
public:
    CEffect( ID3D11Device* t_device, const std::wstring& t_filename );
    ~CEffect();

private:
    CEffect( const CEffect& rhs );
    CEffect& operator=( const CEffect& rhs );

private:
    // techniques
    std::unordered_map<std::string, ID3DX11EffectTechnique*> _techniques;
    // global const variables
    std::unordered_map<std::string, ID3DX11EffectVariable*> _variables;

    ID3D11InputLayout* _IALayout;

protected:
    ID3DX11Effect* _fx;

public:
    // techniques
    bool SetTechniqueAsCurrent( std::string t_name );

    // set parameters
    // scalar
    bool SetParameter( std::string t_name, float t_scalar );
    // vector
    bool SetParameter( std::string t_name, glm::vec4 t_vector );
    bool SetParameter( std::string t_name, glm::vec3 t_vector );
    // matrix
    bool SetParameter( std::string t_name, glm::mat4 t_mat );
    // raw
    bool SetParameter( std::string t_name, void* t_rawVal, uint t_size );

    ID3DX11EffectTechnique* GetCurrentTechnique();

    void InitIALayout( ID3D11Device* t_device, ID3D11DeviceContext* t_deviceContext, Effect_IALayoutType t_IAType );
};
