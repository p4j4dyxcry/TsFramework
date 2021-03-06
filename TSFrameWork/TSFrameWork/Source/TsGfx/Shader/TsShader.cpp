#include "../TsGfx.h"

// static 変数の定義
TS_INSTANTIATE_NAME_OBJ_LIST( TsVertexShader )
TS_INSTANTIATE_NAME_OBJ_LIST( TsPixelShader )
TS_INSTANTIATE_NAME_OBJ_LIST( TsGeometryShader )
TS_INSTANTIATE_NAME_OBJ_LIST( TsHullShader )
TS_INSTANTIATE_NAME_OBJ_LIST( TsDomainShader )
TS_INSTANTIATE_NAME_OBJ_LIST( TsComputeShader )

template class TsShader<ID3D11VertexShader>;
template class TsShader<ID3D11PixelShader>;
template class TsShader<ID3D11GeometryShader>;
template class TsShader<ID3D11HullShader>;
template class TsShader<ID3D11DomainShader>;

template class TsShader<ID3D11ComputeShader>;

//----------------------------------------------------------
//! Constructor
//----------------------------------------------------------
template<typename T>
TsShader<T>::TsShader()
:m_shader(nullptr),
m_inputLayout(nullptr)
{
    this->TsBinary::TsBinary();
}

//----------------------------------------------------------
//! Destructor
//----------------------------------------------------------
template<typename T>
TsShader<T>::~TsShader()
{
    TsSafeRelease(m_shader);
    TsSafeRelease(m_inputLayout);
};

//----------------------------------------------------------
//! Compile from file
//----------------------------------------------------------
template<typename T>
TsBool TsShader<T>::CompileFromFile(
    __in ID3D11Device* pDev,
    __in const TsLChar* filename,
    __in const TsChar*  entryPoint,
    __in const TsChar*  pProfile)
{
    ID3D10Blob* pBlob = nullptr;
    ID3D10Blob* error = nullptr;

    // 行列を列優先で設定し、古い形式の記述を許可しないようにする
    TsUint Flag1 = D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS;
    // 最適化レベルを設定する
#if defined(DEBUG) || defined(_DEBUG)
    Flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL0;
#else
    Flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif

    // ファイルを元にエフェクトをコンパイルする
    D3DCompileFromFile(filename,
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint,
        pProfile,
        Flag1,
        0,
        &pBlob,
        &error);
    if (error)
    {
        TsDebugLogLoadError(filename);
        TsDebugLogError("Compile Error \n%s\n",(TsChar*)error->GetBufferPointer());
        return TS_FALSE;
    }

    if (pBlob == nullptr)
    {
        TsDebugLogLoadError(filename);
        return TS_FALSE;
    }


    m_size = pBlob->GetBufferSize();
    m_bin = TsNew(TsByte[m_size]);

    CopyMemory(m_bin, pBlob->GetBufferPointer(), m_size);
    TsSafeRelease(pBlob);

    CompileFromMemory(pDev, m_bin, m_size);
    // AddObject(this);
    return TS_TRUE;
};

//----------------------------------------------------------
//! Load From Shader Binary
//----------------------------------------------------------
template<typename T>
TsBool TsShader<T>::LoadFromCSO(ID3D11Device* pd3dDevice, const TsChar* filename)
{
    if (pd3dDevice == nullptr)
        return TS_FALSE;

    TsBool isSuccess = TsBinary::Load(filename);

    if (!isSuccess)
        return TS_FALSE;

    isSuccess = CompileFromMemory(pd3dDevice, m_bin, m_size);

    if (!isSuccess)
        return TS_FALSE;

    TsDebugLogLoadSuccess(filename);

    TsString name;
    name = TSUT::PassToFileName(filename);
    name = name.substr(0, name.rfind('.'));
    SetName(name);
    //	AddObject( this );
    return TS_TRUE;
}

//----------------------------------------------------------
//! Vertex Shader Compile From Memory 
//----------------------------------------------------------
template<>
TsBool TsVertexShader::CompileFromMemory(ID3D11Device* pDev, void* memory, size_t sz)
{
    if (pDev == nullptr) return TS_FALSE;
    HRESULT hr = pDev->CreateVertexShader(memory, sz, nullptr, &m_shader);
    if (FAILED(hr))return TS_FALSE;
    if (CreateInputLayout(pDev) == TS_FALSE)
        return TS_FALSE;
    return TS_TRUE;
}

//----------------------------------------------------------
//! Pixel Shader Compile From Memory 
//----------------------------------------------------------
template<>
TsBool TsPixelShader::CompileFromMemory(ID3D11Device* pDev, void* memory, size_t sz)
{
    if (pDev == nullptr) return TS_FALSE;
    HRESULT hr = pDev->CreatePixelShader(memory, sz, nullptr, &m_shader);
    if (FAILED(hr))return TS_FALSE;
    return TS_TRUE;
}

//----------------------------------------------------------
//! Hull Shader Compile From Memory 
//----------------------------------------------------------
template<>
TsBool TsHullShader::CompileFromMemory(ID3D11Device* pDev, void* memory, size_t sz)
{
    if (pDev == nullptr) return TS_FALSE;
    HRESULT hr = pDev->CreateHullShader(memory, sz, nullptr, &m_shader);
    if (FAILED(hr))return TS_FALSE;
    return TS_TRUE;
}

//----------------------------------------------------------
//! Geometry Shader Compile From Memory 
//----------------------------------------------------------
template<>
TsBool TsGeometryShader::CompileFromMemory(ID3D11Device* pDev, void* memory, size_t sz)
{
    if (pDev == nullptr) return TS_FALSE;
    HRESULT hr = pDev->CreateGeometryShader(memory, sz, nullptr, &m_shader);
    if (FAILED(hr))return TS_FALSE;
    return TS_TRUE;
}

//----------------------------------------------------------
//! Compute Shader Compile From Memory 
//----------------------------------------------------------
template<>
TsBool TsComputeShader::CompileFromMemory(ID3D11Device* pDev, void* memory, size_t sz)
{
    if (pDev == nullptr) return TS_FALSE;
    HRESULT hr = pDev->CreateComputeShader(memory, sz, nullptr, &m_shader);
    if (FAILED(hr))return TS_FALSE;
    return TS_TRUE;
}

//----------------------------------------------------------
//! Domain Shader Compile From Memory 
//----------------------------------------------------------
template<>
TsBool TsDomainShader::CompileFromMemory(ID3D11Device* pDev, void* memory, size_t sz)
{
    if (pDev == nullptr) return TS_FALSE;
    HRESULT hr = pDev->CreateDomainShader(memory, sz, nullptr, &m_shader);
    if (FAILED(hr))return TS_FALSE;
    return TS_TRUE;
}

//======================================================
//! Create Auto Input Layout 
//======================================================
template<>
TsBool TsVertexShader::CreateInputLayout(ID3D11Device* pDev)
{
    //======================================================
    //! step01 コンパイル済み頂点シェーダからメタデータの取得
    //======================================================
    ID3D11ShaderReflection* pVertexShaderReflection = nullptr;
    if (FAILED(D3DReflect(m_bin,
        m_size,
        IID_ID3D11ShaderReflection,
        (void**)&pVertexShaderReflection)))
    {
        TsDebugLogError("Reflect Error\n");
        return TS_FALSE;
    }

    //======================================================
    //! step02 メタデータからDescを取得
    //======================================================
    D3D11_SHADER_DESC shaderDesc;
    pVertexShaderReflection->GetDesc(&shaderDesc);

    //======================================================
    //! step03 DescからInputLayoutを生成するためのデータを構築
    //======================================================
    TsVector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
    for (TsUint i = 0; i< shaderDesc.InputParameters; i++)
    {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
        pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

        D3D11_INPUT_ELEMENT_DESC elementDesc;
        elementDesc.SemanticName = paramDesc.SemanticName;
        elementDesc.SemanticIndex = paramDesc.SemanticIndex;
        elementDesc.InputSlot = 0;
        elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        elementDesc.InstanceDataStepRate = 0;

        //switch - case 見やすくするため省略
        enum
        {
            F32 = D3D_REGISTER_COMPONENT_FLOAT32,
            S32 = D3D_REGISTER_COMPONENT_SINT32,
            U32 = D3D_REGISTER_COMPONENT_UINT32,
        };
        //1Dデータ
        if (paramDesc.Mask == 1)
        {
            switch (paramDesc.ComponentType)
            {
            case U32:elementDesc.Format = DXGI_FORMAT_R32_UINT; break;
            case S32:elementDesc.Format = DXGI_FORMAT_R32_SINT; break;
            case F32:elementDesc.Format = DXGI_FORMAT_R32_FLOAT; break;
            default: ;
            }
        }
        //2Dデータ
        else if (paramDesc.Mask <= 3)
        {
            switch (paramDesc.ComponentType)
            {
            case U32:elementDesc.Format = DXGI_FORMAT_R32G32_UINT; break;
            case S32:elementDesc.Format = DXGI_FORMAT_R32G32_SINT; break;
            case F32:elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT; break;
            default: ;
            }
        }
        //3Dデータ
        else if (paramDesc.Mask <= 7)
        {
            switch (paramDesc.ComponentType)
            {
            case U32:elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT; break;
            case S32:elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT; break;
            case F32:elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
            default: ;
            }
        }
        //4Dデータ
        else if (paramDesc.Mask <= 15)
        {
            switch (paramDesc.ComponentType)
            {
            case U32:elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT; break;
            case S32:elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT; break;
            case F32:elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
            default: ;
            }
        }

        inputLayoutDesc.push_back(elementDesc);
    }

    //======================================================
    //! step04 DescからInputLayoutを構築
    //======================================================
    HRESULT hr = pDev->CreateInputLayout(&inputLayoutDesc[0],
        inputLayoutDesc.size(),
        m_bin,
        m_size,
        &m_inputLayout);

    TsSafeRelease(pVertexShaderReflection);
    if (FAILED(hr))
    {
        TsDebugLogError("D3D11Device->CreateInputLayout Error\n");
        return TS_FALSE;
    }
    return TS_TRUE;
}



template<typename T>
TsBool TsShader<T>::CreateInputLayout(ID3D11Device* pDev)
{
    return TS_FALSE;
}