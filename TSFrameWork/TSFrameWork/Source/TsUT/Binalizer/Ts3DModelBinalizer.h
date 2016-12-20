//!*******************************************************
//! Ts3DModelBinalizer.h
//!
//! 3Dモデルを独自形式でBinalize.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsDevice;
class TsMeshObject;

class Ts3DModelBinalizer : public TsBinalizer
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    Ts3DModelBinalizer();

    TsBool LoadBinaly( TsDevice* pDev,
                        const TsChar* filename);

    TsBool SaveBinaly(TsDevice* pDev,
                      const TsChar* filename,
                      TsMeshObject* pMeshObjects,
                      TsUint meshCount);

    TsMeshObject* GetMesh()
    {
        return m_pMeshObjects;
    }

    TsUint GetMeshCount()
    {
        return m_meshCount;
    }

protected:

    //----------------------------------------------------------
    // peropery
    //----------------------------------------------------------
    TsMeshObject* m_pMeshObjects;
    TsUint         m_meshCount;
};