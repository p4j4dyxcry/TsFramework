//**********************************************************
//! Ts3DModelLoader.h
//! 3Dモデル形式をロードするベースクラス.
//!
//! © 2016 Yuki Tsuneyama

#pragma once


class TsCommon3DMaterial
{
    TsString m_name;
    TsFloat3 m_diffuse;
    TsFloat3 m_ambient;
    TsFloat3 m_specluer;
    TsF32    m_power;
    TsF32    m_alpha;
};

class TsCommon3DMesh
{
    TsString      m_name;
    TsVector3   * m_pPositions;
    TsVector3   * m_pNormals;
    TsVector2   * m_pTexcoords;
    TsVector3   * m_pBinormals;
    TsVector3   * m_pTangents;
    TsFloat3    * m_pColors;
    TsFloat4    * m_pBoneIndex;
    TsVector4   * m_pWeights;
    TsUint      * m_pIndex;
    TsTransForm * m_pTransoform;
    TsCommon3DMaterial* m_materialRefName;
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

    virtual TsUint GetMeshData( TsCommon3DMesh* pOut );
    virtual TsUint GetMaterialData( TsCommon3DMaterial * pOut );
    virtual TsUint GetSkeletonData( TsSkeleton* pOut );
    virtual TsUint GetTransformData( TsTransForm* );
private:
    //=========================================================================
    //! propery
    //=========================================================================

    TsCommon3DMesh*         m_pMeshs;
    TsCommon3DMaterial*     m_pMaterials;
    TsSkeleton*             m_pSkeleton;
    TsTransForm*            m_pTransform;
    
};
