//!*******************************************************
//! TsDrawQueue.h
//!
//! RenderQueue. the class is geomtry list.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsDrawQueue
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    
    //! constructor
    TsDrawQueue();

    //! destructor
    virtual ~TsDrawQueue();

    //! Queueにgeometryを追加する
    TsBool Add( TsDrawObject* );

    //! Find
    TsDrawObject* FindGeometoryByName( TsString name );
    TsDrawObject* FindGeometoryByIndex(TsInt index);
    //! Remove
    TsBool Remove( TsDrawObject* );

    TsBool Begin( TsDeviceContext * pDc ,
                  TsBool zEnable ,
                  TsBool zWriteEnable ,
                  D3D11_CULL_MODE cullMopde );

    TsBool End();

    TsBool Render( TsDeviceContext* ) const;

private:

    //----------------------------------------------------------
    // private method
    //----------------------------------------------------------
    TsDrawQueue& operator = ( const TsDrawQueue& ) = delete;
    TsDrawQueue( const TsDrawQueue& ) = delete;

    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    TsVector<TsDrawObject*> m_drawObjList;
};