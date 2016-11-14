//**********************************************************
//! TsLisPSM_ViewVolume.h
//! LisPSM用の視錐体計算用クラス.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsLisPSM_ViewVolume
{
public:

    TsLisPSM_ViewVolume(){}

    //=========================================================================
    //! Copy Constructor
    //=========================================================================
    TsLisPSM_ViewVolume( const TsLisPSM_ViewVolume& vv )
    {
        for( TsInt i = 0; i < m_numFace; i++ )
        {
            m_numVertices[i] = vv.m_numVertices[i];
            for( TsInt v = 0; v < m_numVertices[i]; v++ )
            {
                m_vt[i][v] = vv.m_vt[i][v];
            }
        }
        m_vtMin = vv.m_vtMin;
        m_vtMax = vv.m_vtMax;
    }

    void Init( const TsMatrix& proj ,
               const TsMatrix& view )
    {
        // フラスタムの角をワールド座標系で
        TsVector3 viewFrustum[10];

        TsMatrix invViewProj = ( view * proj ).Inversed();

        // near clip と far clip を取得する
        TsF32 n , f , nn , ff;
        {
            TsF32 a = proj._33;
            TsF32 b = proj._43;
            n = -b / fabsf( a );
            f = b / ( 1.0f - fabsf( a ) );
            nn = 0;
            ff = f;
        }

        // w除算前の射影座標系からビュー座標系へ変換する行列を用いてフラスタムの
        // 角の座標を求める。
        // xとyは必ず視野錐の辺上にあるので(x/w)および(y/w)は奥行き(z)にかか
        // わらず+-1.0になる。ただし(z/w)は mtxProjection の near と far に
        // よるので、変数nnとffに計算しておく。
        TsVector4 vt[] =
        {
            TsVector4( -n , n , nn , n ) ,	// 0:left-top-near
            TsVector4( -n , -n , nn , n ) ,	// 1:left-bottom-near
            TsVector4( n , -n , nn , n ) ,	// 2:right-bottom-near
            TsVector4( n , n , nn , n ) ,	// 3:right-top-near
            TsVector4( -f , f , ff , f ) ,	// 4:left-top-far
            TsVector4( -f , -f , ff , f ) ,	// 5:left-bottom-far
            TsVector4( f , -f , ff , f ) ,	// 6:right-bottom-far
            TsVector4( f , f , ff , f ) ,	// 7:right-top-far
            TsVector4( 0 , 0 , nn , n ) ,	// 8:vfNearCenter
            TsVector4( 0 , 0 , ff , f ) ,	// 9:vfFarCenter
        };

        // w除算前のクリップ座標系(vt[])から戻り値の座標系への変換を計算する。
        for( TsInt i = 0; i < 10; ++i )
        {
            TsVector4 temp = vt[i] * invViewProj;
            viewFrustum[i] = temp;
        }

        // バウンダリボックスの作成
        m_vtMin = m_vtMax = viewFrustum[0];
        for( TsInt i = 1; i < 8; i++ )
        {
            m_vtMin = TsMin( m_vtMin , viewFrustum[i] );
            m_vtMax = TsMax( m_vtMax , viewFrustum[i] );
        }

        // 視野錘の側面のポリゴンデータを作成
        static const TsInt poli[m_numFace][4] =
        {
            { 0 , 4 , 7 , 3 } ,	// top
            { 3 , 7 , 6 , 2 } ,	// right
            { 2 , 6 , 5 , 1 } ,	// bottom
            { 1 , 5 , 4 , 0 } ,	// left
            { 0 , 3 , 2 , 1 } , // near
            { 4 , 5 , 6 , 7 } , // far
        };

        for( TsInt i = 0; i < m_numFace; i++ )
        {
            for( TsInt j = 0; j < 4; j++ )
            {
                m_vt[i][j] = viewFrustum[poli[i][j]];
            }
            m_numVertices[i] = 4;
        }
    }

    void Transform( const TsMatrix& mtx , TsBool divW )
    {
        m_vtMin = TsVector3( FLT_MAX , FLT_MAX , FLT_MAX );
        m_vtMax = TsVector3( -FLT_MAX , -FLT_MAX , -FLT_MAX );

        if( !divW )
        {
            // w除算不要のケース
            for( TsInt i = 0; i < m_numFace; i++ )
            {
                for( TsInt v = 0; v < m_numVertices[i]; v++ )
                {
                    TsVector3 &vt = m_vt[i][v];

                    vt = mtx.TransformPoint( vt );
                    m_vtMin = TsMin( m_vtMin , vt );
                    m_vtMax = TsMax( m_vtMax , vt );
                }
            }
        }
        else
        {
            // w除算を行う
            for( TsInt i = 0; i < m_numFace; i++ )
            {
                for( TsInt v = 0; v < m_numVertices[i]; v++ )
                {
                    TsVector3 &vt = m_vt[i][v];

                    vt = mtx.TransformCoord( vt );
                    m_vtMin = TsMin( m_vtMin , vt );
                    m_vtMax = TsMax( m_vtMax , vt );
                }
            }
        }
    }

    //=========================================================================
    //! Comment
    //  ボリュームを光源発生面へ投影する
    //  \param lightFace 光源発生面
    //  \par 解説：
    //  　バウンダリボックスは更新されないので注意。
    //=========================================================================
    void ProjectLightSrcFace( const TsLisPSM_LightSrcFace &lightFace )
    {
        for( TsInt i = 0; i < m_numFace; i++ )
        {
            for( TsInt v = 0; v < m_numVertices[i]; v++ )
            {
                m_vt[i][v] = lightFace.Project( m_vt[i][v] );
            }
        }
    }

    // バウンダリボックスの最小値を参照
    const TsVector3 GetBBoxMin( void ) const { return m_vtMin; }

    // バウンダリボックスの最大値を参照
    const TsVector3 GetBBoxMax( void ) const { return m_vtMax; }

    TsBool Clip( const TsVector3& minRange ,
                 const TsVector3& maxRange ,
                 TsUint flag )
    {
        TsBool into = false;

        // 各ポリゴンごとにクリップを計算。
        m_vtMin = TsVector3( FLT_MAX , FLT_MAX , FLT_MAX );
        m_vtMax = TsVector3( -FLT_MAX , -FLT_MAX , -FLT_MAX );
        for( TsInt i = 0; i < m_numFace; i++ )
        {
            TsVector3 vvc[m_maxVertices];
            TsInt nv = ClipPolygon(
                m_numVertices[i] , m_vt[i] , vvc , minRange , maxRange , flag );

            into |= ( 0 < nv );

            // 計算結果をm_vt[]に戻し、バウンダリボックスを更新。
            for( TsInt v = 0; v < nv; v++ )
            {
                TsVector3 &vt = m_vt[i][v];
                vt = vvc[v];
                m_vtMin = TsMin( vt , m_vtMin );
                m_vtMax = TsMax( vt , m_vtMax );
            }
            m_numVertices[i] = nv;
        }

        return into;
    }

protected:


    TsInt ClipPolygon(
        TsInt numVertex , const TsVector3 *polyPos ,
        TsVector3 *clip ,
        const TsVector3 &minRect , const TsVector3 &maxRect ,
        TsUint mask )
    {
        // ポリゴンを±X、±Y、±Zの各面で順に切断していく。
        // ポリゴンの頂点配列は面毎に
        //  polyPos -> workBuffer[0] -> workBuffer[1] -> workBuffer[0] -> ...
        // と代入され、切断による頂点数の変化は numVertex 変数に代入される。
        TsVector3 workBuffer[2][16];
        TsInt destBufferIndex = 0;
        const TsVector3 *src = polyPos;
        TsVector3 *dest = workBuffer[destBufferIndex];

        // minXとmaxXでクリップ。
        if( mask & 0x01 )
        {
            for( TsInt i = 0; i < 2; i++ )
            {
                const TsF32 x = ( i == 0 ) ? minRect.x : maxRect.x;
                const TsF32 w = ( i == 0 ) ? +1.0f : -1.0f;
                TsInt s = 0;
                for( TsInt v = 0; v < numVertex; v++ )
                {
                    // 頂点がクリップラインより中にあれば、その頂点をそのまま採用する。
                    if( 0.0f <= w * ( src[v].x - x ) )
                    {
                        dest[s++] = src[v];
                    }
                    // 辺がクリップラインをまたいでいれば、交点の座標を求めてクリップ
                    // 頂点を挿入する。(中から外、外から中の向きは関係ない。)
                    const TsInt vv = ( v + 1 ) % numVertex;
                    if( ( src[v].x < x ) ^ ( src[vv].x < x ) )
                    {
                        const TsF32 t = ( x - src[v].x ) / ( src[vv].x - src[v].x );
                        dest[s] = TsVector3( TsVector3( src[v] ) * ( 1.0f - t ) );
                        dest[s++] += TsVector3( src[vv] ) * t;
                    }
                }
                // もし代入された頂点がなければ、ポリゴンは完全にクリップアウトされた。
                if( s == 0 ) return 0;
                // バッファを入れ替えて続ける。
                numVertex = s;
                src = dest;
                destBufferIndex = ( destBufferIndex + 1 ) % 2;
                dest = workBuffer[destBufferIndex];
            }
        }

        // minYとmaxYでクリップ。
        if( mask & 0x02 )
        {
            for( TsInt i = 0; i < 2; i++ )
            {
                const TsF32 y = ( i == 0 ) ? minRect.y : maxRect.y;
                const TsF32 w = ( i == 0 ) ? +1.0f : -1.0f;
                TsInt s = 0;
                for( TsInt v = 0; v < numVertex; v++ )
                {
                    // 頂点がクリップラインより中にあれば、その頂点をそのまま採用する。
                    if( 0.0f <= w * ( src[v].y - y ) )
                    {
                        dest[s++] = src[v];
                    }
                    // 辺がクリップラインをまたいでいれば、交点の座標を求めてクリップ
                    // 頂点を挿入する。(中から外、外から中の向きは関係ない。)
                    const TsInt vv = ( v + 1 ) % numVertex;
                    if( ( src[v].y < y ) ^ ( src[vv].y < y ) )
                    {
                        const TsF32 t = ( y - src[v].y ) / ( src[vv].y - src[v].y );
                        dest[s] = TsVector3( ( 1.0f - t ) * TsVector3( src[v] ) );
                        dest[s++] += t * TsVector3( src[vv] );
                    }
                }
                // もし代入された頂点がなければ、ポリゴンは完全にクリップアウトされた。
                if( s == 0 )
                    return 0;
                // バッファを入れ替えて続ける。
                numVertex = s;
                src = dest;
                destBufferIndex = ( destBufferIndex + 1 ) % 2;
                dest = workBuffer[destBufferIndex];
            }
        }

        // minZとmaxZでクリップ。
        if( mask & 0x04 )
        {
            for( TsInt i = 0; i < 2; i++ )
            {
                const TsF32 z = ( i == 0 ) ? minRect.z : maxRect.z;
                const TsF32 w = ( i == 0 ) ? +1.0f : -1.0f;
                TsInt s = 0;
                for( TsInt v = 0; v < numVertex; v++ )
                {
                    // 頂点がクリップラインより中にあれば、その頂点をそのまま採用する。
                    if( 0.0f <= w * ( src[v].z - z ) )
                    {
                        dest[s++] = src[v];
                    }
                    // 辺がクリップラインをまたいでいれば、交点の座標を求めてクリップ
                    // 頂点を挿入する。(中から外、外から中の向きは関係ない。)
                    const TsInt vv = ( v + 1 ) % numVertex;
                    if( ( src[v].z < z ) ^ ( src[vv].z < z ) )
                    {
                        const TsF32 t = ( z - src[v].z ) / ( src[vv].z - src[v].z );
                        dest[s] = TsVector3( ( 1.0f - t ) * TsVector3( src[v] ) );
                        dest[s++] += t * TsVector3( src[vv] );
                    }
                }
                // もし代入された頂点がなければ、ポリゴンは完全にクリップアウトされた。
                if( s == 0 ) return 0;
                // バッファを入れ替えて続ける。
                numVertex = s;
                src = dest;
                destBufferIndex = ( destBufferIndex + 1 ) % 2;
                dest = workBuffer[destBufferIndex];
            }
        }

        // 結果をコピーして終了
        for( TsInt v = 0; v < numVertex; v++ )
        {
            clip[v] = src[v];
        }
        return numVertex;
    }

    // 視野錘の側面のポリゴンで体積を表現
    // 0:top / 1:right / 2:bottom / 3:left / 4:near / 5:far
    static const TsInt m_numFace = 6;

    // 側面毎の角数(頂点数)
    TsInt m_numVertices[m_numFace];

    // 座標バッファの最大サイズ
    static const TsInt m_maxVertices = 16;

    // 角の座標(最大8角形)
    TsVector3 m_vt[m_numFace][m_maxVertices];

    // バウンダリボックス
    TsVector3 m_vtMin;
    TsVector3 m_vtMax;
};