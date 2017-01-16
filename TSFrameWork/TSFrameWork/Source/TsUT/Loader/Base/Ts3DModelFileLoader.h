//**********************************************************
//! Ts3DModelLoader.h
//! 3Dモデル形式をロードするベースクラス.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

class TsMeshObject;

struct TsCommon3DMaterial
{
    TsString m_name;
    TsFloat3 m_diffuse;
    TsFloat3 m_ambient;
    TsFloat3 m_specluer;
    TsF32    m_power;
    TsF32    m_alpha;

    TsString m_albedoTexture;
    TsString m_normalTexture;
    TsString m_specluerTexture;
    TsString m_specluerPowerTexture;


    TsCommon3DMaterial()
       : m_power(0),
         m_alpha(1.0f),
         m_diffuse(1,1,1)
    {
    
    };
};

struct TsCommon3DMesh
{
    TsString            m_name;
    TsVector3         * m_pPositions;
    TsVector3         * m_pNormals;
    TsVector2         * m_pTexcoords;
    TsVector3         * m_pBinormals;
    TsVector3         * m_pTangents;
    TsFloat4          * m_pColors;
    TsInt4            * m_pBoneIndex;
    TsVector4         * m_pWeights;
    TsUint            * m_pIndex;
    TsTransForm       * m_pTransoform;
    TsCommon3DMaterial* m_pMaterialRef;

    TsAABB3D      m_aabb;

    TsUint m_vertexCount;
    TsUint m_indexCount;

    TsCommon3DMesh()
    {
        m_pPositions = nullptr;
        m_pNormals = nullptr;
        m_pTexcoords = nullptr;
        m_pBinormals = nullptr;
        m_pTangents = nullptr;
        m_pColors = nullptr;
        m_pBoneIndex = nullptr;
        m_pWeights = nullptr;
        m_pIndex = nullptr;
        m_pTransoform = nullptr;
        m_pMaterialRef = nullptr;

        m_vertexCount = 0;
        m_indexCount = 0;
    }

};

//=========================================================================
//! TsPlaneGeometry 
//=========================================================================
class Ts3DModelLoader : public TsNameObject
{
public:
    //=========================================================================
    //! public method
    //=========================================================================

    //! Constructor
    Ts3DModelLoader();

    //! Destructor
    virtual ~Ts3DModelLoader();

    virtual TsBool LoadFile( const TsChar*filename );
    virtual TsBool SaveFile( const TsChar*filename );

    //----------------------------------------------------------
    //! メッシュ配列を取得
    // @param
    // out     pOut メッシュの先頭ポインタ
    // return メッシュの数
    //----------------------------------------------------------
    virtual TsCommon3DMesh* GetMeshData()const;

    //----------------------------------------------------------
    //! マテリアル配列を取得
    // @param
    // out     pOut メッシュの先頭ポインタ
    // return メッシュの数
    //----------------------------------------------------------
    virtual TsCommon3DMaterial* GetMaterialData()const;

    //----------------------------------------------------------
    //! スケルトン配列を取得
    // @param
    // out     pOut メッシュの先頭ポインタ
    // return メッシュの数
    //----------------------------------------------------------
    virtual TsSkeleton* GetSkeletonData()const;

    //----------------------------------------------------------
    //! RootTransformを取得
    // @param
    // out     pOut メッシュの先頭ポインタ
    // return メッシュの数
    //----------------------------------------------------------
    virtual TsTransForm* GetRootTransformData()const;

    TsUint GetMeshCount()const
    {
        return m_meshCount;
    }

    TsUint GetMaterialCount()const
    {
        return m_materialCount;
    }

    virtual TsBool CreateCommonData()
    {
        return TS_FALSE;
    }

    virtual TsBool Encode( TsMeshObject* pMesh )
    {
        return TS_FALSE;
    }

protected:
    //=========================================================================
    //! propery
    //=========================================================================

    TsCommon3DMesh*         m_pMeshs;
    TsCommon3DMaterial*     m_pMaterials;
    TsSkeleton*             m_pSkeleton;
    TsTransForm*            m_pTransform;
    TsUint                  m_meshCount;
    TsUint                  m_materialCount;    
};
