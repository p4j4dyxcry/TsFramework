//!*******************************************************
//! TsGeometoryBinalizer.h
//!
//! ジオメトリバイナライザ.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsMaterialBinalizer;
class TsSkeletonBinalizer;
class TsTransformBinalizer;

class TsGeometoryBinalizer : public TsBinalizer
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsGeometoryBinalizer();
    virtual ~TsGeometoryBinalizer();
#pragma pack(1)
    struct CommonRef
    {
        TsChar name[128];
        TsChar matName[128];
        TsU64  transformPtr;
        TsAABB3D aabb;
    };
#pragma pack()

    TsBool Binalize(std::ofstream& ofs, 
                    TsGeometryObject** pData,
                    TsUint count);
    TsBool Decode( std::ifstream& ifs );

    TsGeometryObject** GetGeometry()const
    {
        return m_pGeometoryObject;
    }

    TsUint GetGeometoryCount()const
    {
        return m_geometoryCount;
    }

    //--------------------------------------------------------------
    // * メモ　バイナリデータからメッシュを作成する際は下記の手順
    //  ① Decode()
    //　② BuildGeometory()
    //  ③ Bind...

    //! 頂点バッファ・インデックスバッファを含むメッシュ作成
    TsBool BuildGeometory(TsDevice * pDev);

    //! トランスフォームをバインドする
    //  引数 : バインドする対象の含まれるRootTransform
    TsBool BindTransform(TsTransformBinalizer* pTransformBinalizer);

    //! マテリアルをバインドする
    TsBool BindMaterial(TsMaterialBinalizer * pMaterialBinalizer);

    //! スケルトンをバインドする
    TsBool BindSkeleton(TsSkeletonBinalizer* pSkeletonBinalizer);
    
protected:

    //! ジオメトリ情報を一時的に格納する構造体
    //  バイナリからデコードするときのみに使用する。
    struct GeometoryRef
    {
        TsVertexSkin * m_pVertexArray = nullptr;
        TsUint       * m_pIndexArray = nullptr;
        TsInt          m_vertexCount = 0;
        TsInt          m_indexCount = 0;
    };

    //! ジオメトリと参照情報(Material,Skeleton,Transform)をバインドする構造体
    //  バイナリからデコードするときのみに使用する。


    //----------------------------------------------------------
    // peropery
    //----------------------------------------------------------
    TsGeometryObject**          m_pGeometoryObject;
    TsVector<CommonRef>         m_pCommonRef;
    TsVector<GeometoryRef>      m_pGeometoryRef;
    TsUint                      m_geometoryCount;
};