//**********************************************************
//! TsMeshFactory.h
//! このクラスは一時的なものです。
//!
//! © 2016 Yuki Tsuneyama

#pragma once

class TsGeometryObject;

class TsMeshFactory 
{
public:
    TsMeshFactory();
    virtual ~TsMeshFactory();

    TsBool LoadModelFromFile(TsDevice* pDev, 
                             TsString filename,
                             TsString name);

    //todo Operation check test
    TsTransformBakeAnimation* CreateBakeAnimation( const TsString& filename );
};