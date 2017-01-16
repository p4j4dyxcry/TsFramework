//!*******************************************************
//! Ts3DMeshConverter.h
//!
//! ファイル - メッシュのコンバータ.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class Ts3DMeshConverter
{
public:
    static TsMeshObject* ConvertFromFile( TsDevice* pDev,
                                          const TsChar * filename);

    static TsBool ConvertFileFromMesh( TsMeshObject* pMesh,
                                 const TsChar * output);

protected:
    //----------------------------------------------------------
    // peropery
    //----------------------------------------------------------
    static Ts3DModelLoader* ExtencionToModelLoader(const TsChar* extencion);

    static TsMaterial *     ConvertMaterial(TsDevice* pDev,
                                            const TsChar * filename,
                                            TsCommon3DMaterial* pCommonMaterial);

    static TsGeometryObject* ConvertGeometory( TsDevice* pDev,
                                               TsCommon3DMesh* pCommonMesh);

    static TsVertexSkin ConvertVertx(TsCommon3DMesh* pMesh, TsUint index);

    static TsString GetCachePath(const TsChar * filename);
};