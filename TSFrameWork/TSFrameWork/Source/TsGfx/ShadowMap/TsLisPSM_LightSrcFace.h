//**********************************************************
//! TsLisPSM_LightSrcFace.h
//! LisPSMの光源発生面.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

class TsLisPSM_LightSrcFace
{
public:
    void Init( const TsVector3& lightDir ,
               const TsVector3& eyePos ,
               const TsAABB3D& sceneAABB )
    {
        // 光源方向にある面を選択する。
        // 任意の座標pを面に投影する計算において、投影が無限遠となることを避ける
        // ために、光線と面に1度以上の傾きが無い場合には、その面は光線発生面とし
        // ない。
        TsVector3 vt;
        vt = lightDir.Normalized();
        const TsF32 th = sin( TsRadian( 1.0f ) );

        TsVector3 minScene = sceneAABB.GetMin();
        TsVector3 maxScene = sceneAABB.GetMax();

        // YZ平面
        if( th < fabsf( vt.x ) )
        {
            if( 0.0f < vt.x )
            {
                m_vtNormal[m_numFace] = TsVector3( 1.0f , 0.0f , 0.0f );
                m_vtPos[m_numFace] = TsVector3( minScene.x , 0.0f , 0.0f );
            }
            else
            {
                m_vtNormal[m_numFace] = TsVector3( -1.0f , 0.0f , 0.0f );
                m_vtPos[m_numFace] = TsVector3( maxScene.x , 0.0f , 0.0f );
            }
            m_numFace++;
        }

        // XZ平面
        if( th < fabsf( vt.y ) )
        {
            if( 0.0f < vt.y )
            {
                m_vtNormal[m_numFace] = TsVector3( 0.0f , 1.0f , 0.0f );
                m_vtPos[m_numFace] = TsVector3( 0.0f , minScene.y , 0.0f );
            }
            else
            {
                m_vtNormal[m_numFace] = TsVector3( 0.0f , -1.0f , 0.0f );
                m_vtPos[m_numFace] = TsVector3( 0.0f , maxScene.y , 0.0f );
            }
            m_numFace++;
        }

        // XY平面
        if( th < fabsf( vt.z ) )
        {
            if( 0.0f < vt.z )
            {
                m_vtNormal[m_numFace] = TsVector3( 0.0f , 0.0f , 1.0f );
                m_vtPos[m_numFace] = TsVector3( 0.0f , 0.0f , minScene.z );
            }
            else
            {
                m_vtNormal[m_numFace] = TsVector3( 0.0f , 0.0f , -1.0f );
                m_vtPos[m_numFace] = TsVector3( 0.0f , 0.0f , maxScene.z );
            }
            m_numFace++;
        }
    }

    void Transform( const TsMatrix& mtx )
    {
        TsVector3 temp;
        for( TsInt i = 0; i < m_numFace; i++ )
        {
            m_vtNormal[i] = mtx.TransformNormal( m_vtNormal[i] );
            m_vtPos[i] = mtx.TransformPoint( m_vtPos[i] );
        }
    }

    TsVector3 Project( const TsVector3& vt )const
    {
        // 座標を-Y方向に投影して、最も近い面との交点を計算する。
        TsF32 maxY = -FLT_MAX;
        for( TsInt i = 0; i < m_numFace; i++ )
        {
            const TsVector3 &n = m_vtNormal[i];
            const TsVector3 &o = m_vtPos[i];
            TsF32 a = ( ( o.x - vt.x ) * n.x ) + ( ( o.z - vt.z ) * n.z );
            TsF32 y = ( a / n.y ) + o.y;

            if( maxY < y ) maxY = y;
        }

        return TsVector3( vt.x , maxY , vt.z );
    }

protected:

    // ! 有効な光線発生面の数
    // ! 最低1枚、最大3枚となる。
    TsInt       m_numFace = 0;

    // ! 光線発生面の向き
    TsVector3   m_vtNormal[3];

    // ! 光線発生面の位置
    TsVector3   m_vtPos[3];

    //! 平面の方程式のD
    TsF32       m_planeD[3];
};
