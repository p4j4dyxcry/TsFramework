#include "../../../TsAfx.h"
#include "TsLisPSM.h"

class LightSrcFace
{
public:
    void Init( const TsVector3& lightDir ,
               const TsVector3& eyePos,
               TsF32 sceneSize)
    {
        // 光源方向にある面を選択する。
        // 任意の座標pを面に投影する計算において、投影が無限遠となることを避ける
        // ために、光線と面に1度以上の傾きが無い場合には、その面は光線発生面とし
        // ない。
        TsVector3 vt;
        vt = lightDir.Normalized();
        const TsF32 th = sin( TsRadian( 1.0f ) );

        TsVector3 minScene = TsVector3( -sceneSize , -sceneSize , -sceneSize );
        TsVector3 maxScene = TsVector3( sceneSize , sceneSize , sceneSize );

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

class ViewVolume
{
public:

    ViewVolume(){}

    //=========================================================================
    //! Copy Constructor
    //=========================================================================
    ViewVolume(const ViewVolume& vv )
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
        TsF32 n , f , nn, ff;
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

    void Transform( const TsMatrix& mtx, TsBool divW )
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
                    m_vtMin = TsMin( m_vtMin , vt);
                    m_vtMax = TsMax( m_vtMax , vt);
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
    void ProjectLightSrcFace( const LightSrcFace &lightFace )
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

TsComputeLisPSM::PointList::PointList()
{
    m_list.reserve( 128 );
}

TsInt TsComputeLisPSM::PointList::GetSize()const
{
    return (TsInt)m_list.size();
};
TsBool TsComputeLisPSM::PointList::Add( TsVector3& value )
{
    m_list.push_back( value );
    return TS_TRUE;
}
TsBool TsComputeLisPSM::PointList::Clear()
{
    m_list.clear();
    return TS_TRUE;
}
TsBool TsComputeLisPSM::PointList::Transform( const TsMatrix & matrix )
{
    for each( auto var in  m_list )
        var.TransformCood( matrix );
    return TS_TRUE;
}
TsBool TsComputeLisPSM::PointList::ComputeAABB( TsVector3& min , TsVector3 & max )const
{
    if( GetSize() > 0 )
    {
        for each ( auto var in m_list )
        {
            min = TsMin( min , var );
            max = TsMax( max , var );
        }
    }
    else
    {
        return TS_FALSE;
    }
    return TS_TRUE;
}

TsComputeLisPSM::PointList& TsComputeLisPSM::PointList::operator =( PointList& value )
{
    m_list.clear();
    for( TsInt i = 0; i < value.GetSize(); ++i )
    {
        Add( value[i] );
    }
    return ( *this );
}

TsVector3& TsComputeLisPSM::PointList::operator[]( TsInt index )
{
    return m_list[index];
}
TsComputeLisPSM::TsComputeLisPSM()
{
    m_near = 0.1f;
    m_pointList.Clear();
}
TsVector3 TsComputeLisPSM::ComputeUpVector( TsVector3& viewDir , TsVector3 lightDir )
{
    TsVector3 left = TsVector3::Cross( lightDir , viewDir );
    TsVector3 up = TsVector3::Cross( left , lightDir );
    return up.Normalized();
}
TsBool TsComputeLisPSM::ComputeShadowMap()
{
    TsVector3 max , min;
    TsMatrix lightView = 
        TsMatrix::CreateLookAt( m_eyePostion ,
                                m_eyePostion + m_lightDir ,
                                m_viewDir );
    m_pointList.Transform( lightView );

    m_pointList.ComputeAABB( min , max );

    m_lightProjectionMatrix = GetUnitCubeClipMatrix( min , max );

    return TS_TRUE;
}

TsBool TsComputeLisPSM::ComputeLisPSM()
{
    // Step.0
    // ビュー座標系の座標を光源座標系へ変換する行列 mtxViewLight を計算する。
    // 光源座標系は光源からカメラの視野錘を見る座標系。光源座標系から見た視野
    // 錐をシャドウマップへレンダリングする。光源座標系のX,Y,Z基底ベクトルを定
    // 義し、それからビュー-光源変換行列を求める。この基底ベクトルは各カスケー
    // ド段で共通。
    // X軸：シャドウマップの横方向(leftVector)
    // Y軸：光線方向(upVector)
    // Z軸：シャドウマップの奥行き(縦)方向(viewVector)
    // T要素：ゼロ
    TsF32 sinGamma;
    TsMatrix mtxViewLight , mtxLightView;
    {
        // 光線方向ベクトルをビュー座標系へ変換してupVectorとする。
        // (light.directionは正規化されている。)
        TsVector3 upVector = m_lightDir.Normalized();

        upVector = m_viewMatrix.TransformNormal( upVector );
        upVector.Normalize();

        TsVector3 eyeVector 
//            = TsVector3( m_viewMatrix._31 , m_viewMatrix._32 , m_viewMatrix._33 );
              = TsVector3::front;
        // 視線と光線のなす角のcosを求めておく。
        TsF32 cosGamma = TsVector3::Dot( upVector,eyeVector );

        // 視線と光線が同じ方向を向く場合には直行するベクトルleftVectorが計算
        // できないので、視線方向をごまかして計算を進める。
        static const TsF32 th = cosf( TsRadian( 1.0f ) );
        if( th < fabsf( cosGamma ) )
        {
            eyeVector = TsVector3( 0.0f , sinf( TsRadian( 1.0f ) ) , cosf( TsRadian( 1.0f ) ) );
            cosGamma = TsVector3::Dot( upVector , eyeVector );
        }

        // 視線と光線の角度から計算されるcosGammaとsinGamma。
        sinGamma = sqrtf( 1.0f - cosGamma*cosGamma );

        // 視線と光線に直行するベクトルをleftVectorとする。(単位行列に正規化しておく)
        // ビュー座標系を使っているのでeyeVectorは(0.0, 0.0, 1.0)となる。
        TsVector3 leftVector;
        {
            leftVector = TsVector3::Cross( upVector , eyeVector * -1.0f );
            leftVector.Normalize();
        }

        // 光線とleftVectorに直行するベクトルをviewVectorとする。
        // viewVectorはLisPSM透視変換の奥行き方向を表すベクトル。
        // (通常のカメラからの透視変換ならば視線方向(Z)に相当)
        TsVector3 viewVector;
        {
            viewVector = TsVector3::Cross( upVector , leftVector );
            viewVector.Normalize();
        }

        // mtxViewLightの逆変換を行う、光源座標系からビュー座標系への変換行列も
        // 用意しておく。
        mtxLightView = TsMatrix(
            leftVector.x , leftVector.y , leftVector.z , 0.0f ,
            upVector.x , upVector.y , upVector.z , 0.0f ,
            viewVector.x , viewVector.y , viewVector.z , 0.0f ,
            0.0f , 0.0f , 0.0f , 1.0f
            );

        // ビュー座標系から光源座標系へ変換する行列を計算する。
        // (直交座標系の逆行列=転置行列)という性質を使っている。
        // X軸 : leftVector (視線と光線に直行するベクトル)
        // Y軸 : upVector (光線方向)
        // Z軸 : viewVector (LisPSM透視変換の奥行き方向)
        mtxViewLight = mtxLightView.Transposed();
    }
    LightSrcFace lightSrcFace;
    {
        // ワールド座標系での光線方向をもとめる。
        TsVector3 vtLight;

        vtLight = m_lightDir;

        // 光線発生面を計算する。
        lightSrcFace.Init( m_lightDir , m_eyePostion , m_far );

        // 光源座標系へ変換しておく。
        lightSrcFace.Transform( m_viewMatrix * mtxViewLight );
    }

    {

        ViewVolume viewVolume;
        {
            // 視野錘の角の座標をワールド座標系で取得
            viewVolume.Init( m_viewProjectionMatrix , m_viewMatrix );

            TsVector3 minScene = -TsVector3::one * m_far;
            TsVector3 maxScene =  TsVector3::one * m_far;

            // クリップを計算。
            // 高さ(Y軸)範囲のみをクリップするようにする。
            viewVolume.Clip( minScene , maxScene , 0x02 );
        }

        // Step.3
        // ビュー座標系での視野錘のZ範囲を計算する。
        // これはStep.2においてクリップした視野錘を使う。
        // 注意：
        // viewVolume には座標変換や平面クリップの計算誤差が含まれている。
        // そのためカメラの Near クリップ値が小さいと、ココの transform() で、
        // zn がマイナスの値になる可能性がある。そうなると後の
        // Nopt = (zn + sqrtf(zn * zf)) / sinGamma;
        // において、 sqrtf() が非数を返し、シャドウマップがおかしくなる。
        // それを避けるために、 zn がマイナスにならないように適当なクリップを
        // 行う。これは根本的な解決ではない。
        TsF32 zn , zf;
        {
            ViewVolume vv( viewVolume );
            vv.Transform( m_viewMatrix , TS_FALSE );
            zn = vv.GetBBoxMin().z;
            zf = vv.GetBBoxMax().z;

            if( zn < 0.001f ) zn = 0.001f;
        }

        TsMatrix mtxLightShadow;
        // Step.4
        // 光源座標系での視野錘のZ範囲を計算し、LisPSMの透視変換の原点までの
        // 距離を求める。
        TsF32 projNear , projFar;
        ViewVolume lsViewVolume( viewVolume );
        {
            // 視野錘を光源座標系に変換してZ範囲を取得。
            const TsMatrix mtx( m_viewMatrix * mtxViewLight );
            lsViewVolume.Transform( mtx , TS_FALSE );

            projNear = lsViewVolume.GetBBoxMin().z;
            projFar = lsViewVolume.GetBBoxMax().z;

            // LisPSMの透視変換の原点までの距離を計算
            TsF32 Nopt;
            {
                // 論文でお勧めされていた式。
                const TsF32 zfopt = zn + ( projFar - projNear ) * sinGamma;
                Nopt = ( zn + sqrtf( zn * zfopt ) ) / sinGamma;
            }

            // projNear、projFarを透視変換の原点からの距離に変換
            const TsF32 localOffset = projNear - Nopt;
            projNear -= localOffset;
            projFar -= localOffset;

            // 変換行列を代入
            mtxLightShadow = TsMatrix::CreateTranslate( TsVector3::front * -localOffset );
        }

        // Step.5
        // LisPSMの透視変換を計算して mtxLightShadow に適用。
        // この透視変換ではz/wは(-1.0 - +1.0)の範囲となる。
        {
            TsMatrix mtxProj = TsMatrix::identity;
            mtxProj.m[2][2] = ( projFar + projNear ) / ( projFar - projNear );
            mtxProj.m[3][2] = -2.0f * ( projFar * projNear ) / ( projFar - projNear );

            mtxProj.m[2][3] = 1.0f;
            mtxProj.m[3][3] = 0.0f;
            mtxLightShadow = mtxLightShadow * mtxProj;
        }

        // Step.6
        // 視野錘の外から影を落とす可能性のある空間を考慮するために、視野錘を
        // 光線発生面に投影する。
        ViewVolume lsExtViewVolume( lsViewVolume );
        {
            lsExtViewVolume.ProjectLightSrcFace( lightSrcFace );
        }

        // Step.7
        // 計算した透視変換に対してUniformクリップを行う。
        {
            // 視野錘とそのStep.6で計算した体積をシャドウマップ座標系に変換し、
            // そのの範囲を計算。
            lsViewVolume.Transform( mtxLightShadow , TS_TRUE );
            lsExtViewVolume.Transform( mtxLightShadow , TS_TRUE );

            TsVector3 vtMin;
            vtMin = TsMin( lsViewVolume.GetBBoxMin() , lsExtViewVolume.GetBBoxMin() );

            TsVector3 vtMax;
            vtMax = TsMax( lsViewVolume.GetBBoxMax() , lsExtViewVolume.GetBBoxMax() );

            // XZはシャドウマップテクスチャのUV座標(-1.0 ～ +1.0)に、
            // Yはシャドウマップに書き込まれる深度になる。
            TsMatrix mtxFit = TsMatrix::identity;

            mtxFit.m[0][0] = 2.0f / ( vtMax.x - vtMin.x );
            //			mtxFit.m[1][0] = 0.0f;
            //			mtxFit.m[2][0] = 0.0f;
            mtxFit.m[3][0] = -( vtMax.x + vtMin.x ) / ( vtMax.x - vtMin.x );

            // Y軸のシャドウマップの深度幅はプラットフォームによって異なる。
            // D3D系:(0.0 ～ +1.0)
            // OpenGL系:(-1.0 ～ +1.0)
            {
                // D3D系:
                //				mtxFit.m[0][1] = 0.0f;
                mtxFit.m[1][1] = 1.0f / ( vtMax.y - vtMin.y );
                //				mtxFit.m[2][1] = 0.0f;
                mtxFit.m[3][1] = -vtMin.y / ( vtMax.y - vtMin.y );
            }


            // Z軸については、元々 projNear から projFar の範囲で透視変換を
            // 計算しているため、vtMin.z、vtMax.z は-1.0から+1.0の範囲になって
            // いる。
            //			mtxFit.m[0][2] = 0.0f;
            //			mtxFit.m[1][2] = 0.0f;
            //			mtxFit.m[2][2] = 2.0f / (vtMax.z - vtMin.z);
            //			mtxFit.m[3][2] = -(vtMax.z + vtMin.z) / (vtMax.z - vtMin.z);

            // Uniformクリップを適用。
            mtxLightShadow = mtxLightShadow * mtxFit;
        }

        // 深度(光源からの距離)がZに出力されるように、透視変換後の座標を回転する。
        // シャドウマップはY軸(光線方向)にそってマッピングされる。
        // これは単純にY軸とZ軸の交換によって実現できる。
        for( TsInt j = 0; j < 4; j++ )
        {
            TsF32& y = mtxLightShadow.m[j][1];
            TsF32& z = mtxLightShadow.m[j][2];

            TsF32 sw = y;	
            y = z;	
            z = sw;
        }

        // Step.8
        // ビュー座標系からシャドウマップ座標系への変換行列を計算
        m_lightProjectionMatrix = mtxViewLight * mtxLightShadow;
        m_lightViewMatrix = m_viewMatrix;
        m_lVPMatrix = m_viewMatrix * m_lightProjectionMatrix;
    }

    return TS_TRUE;
}

TsBool TsComputeLisPSM::UpdateShadowMatrix()
{
    TsMatrix viewProj;
    viewProj = m_viewMatrix * m_viewProjectionMatrix;

    //ビューボリュームを求める
    ComputeViewFrustum( viewProj );

    // Light Space Perspective Shadow Map
    ComputeLisPSM();

    //左手座標系に変換
    //TsMatrix scale = TsMatrix::CreateScale( 1.0f , 1.0f , -1.0f );
    //m_lightProjectionMatrix = m_lightProjectionMatrix * scale;
    //m_lVPMatrix = m_lightViewMatrix * m_lightProjectionMatrix;

    return TS_TRUE;
}
TsBool TsComputeLisPSM::ComputeViewFrustum( TsMatrix& viewProj )
{
    m_pointList.Clear();

    //　立方体を作成
    TsVector3 v[8];
    v[0] = TsVector3( -1.0f , +1.0f , -1.0f );
    v[1] = TsVector3( -1.0f , -1.0f , -1.0f );
    v[2] = TsVector3( +1.0f , -1.0f , -1.0f );
    v[3] = TsVector3( +1.0f , +1.0f , -1.0f );
    v[4] = TsVector3( -1.0f , +1.0f , +1.0f );
    v[5] = TsVector3( -1.0f , -1.0f , +1.0f );
    v[6] = TsVector3( +1.0f , -1.0f , +1.0f );
    v[7] = TsVector3( +1.0f , +1.0f , +1.0f );
    for( TsInt i = 0; i<8; i++ )
    {
        m_pointList.Add( v[i] );
    }

    //　ビュー行列→射影行列の逆変換を行う行列を求める
    TsMatrix invViewProj = viewProj.Inversed();

    //　立方体に逆変換する行列をかけ、視錐台を求める
    m_pointList.Transform( invViewProj );

    return TS_TRUE;
}

TsMatrix TsComputeLisPSM::GetUnitCubeClipMatrix( TsVector3& min , TsVector3& max )
{
    TsMatrix result;

    result._11 = 2.0f / ( max.x - min.x );
    result._21 = 0.0f;
    result._31 = 0.0f;
    result._41 = -( max.x + min.x ) / ( max.x - min.x );

    result._12 = 0.0f;
    result._22 = 2.0f / ( max.y - min.y );
    result._32 = 0.0f;
    result._42 = -( max.y + min.y ) / ( max.y - min.y );

    result._13 = 0.0f;
    result._23 = 0.0f;
    result._33 = 1.0f / ( max.z - min.z );
    result._43 = -min.z / ( max.z - min.z );

    result._14 = 0.0f;
    result._24 = 0.0f;
    result._34 = 0.0f;
    result._44 = 1.0f;

    return result;
}
TsMatrix TsComputeLisPSM::GetPerspective( TsF32 nearDist , TsF32 farDist )
{
    TsMatrix result;
    result = TsMatrix::identity;
    result._22 = farDist / ( farDist - nearDist );
    result._24 = 1.0f;
    result._42 = -farDist * nearDist / ( farDist - nearDist );
    result._44 = 0.0f;
    return result;
}

TsBool TsComputeLisPSM::SetEyePos(const TsVector3& value )
{
    m_eyePostion = value;
    return TS_TRUE;
}
TsBool TsComputeLisPSM::SetViewDir(const TsVector3& value )
{
    m_viewDir = value;
    return TS_TRUE;
}
TsBool TsComputeLisPSM::SetLightDir(const TsVector3& value )
{
    m_lightDir = value;
    return TS_TRUE;
}

TsBool TsComputeLisPSM::SetEyeProjection(const TsMatrix& value )
{
    m_viewProjectionMatrix = value;

    TsF32 a = m_viewProjectionMatrix._33;
    TsF32 b = m_viewProjectionMatrix._43;

    m_near = -b / fabsf( a );
    m_far  =  b / (1.0f - fabsf( a ));

    return TS_TRUE;
}

TsBool TsComputeLisPSM::SetEyeViewMatrix( const TsMatrix& value )
{
    m_viewMatrix = value;
    return TS_TRUE;
}

TsMatrix TsComputeLisPSM::GetLVMatrix()
{
    return m_lightViewMatrix;
}
TsMatrix TsComputeLisPSM::GetLPMatrix()
{
    return m_lightProjectionMatrix;
}
TsMatrix TsComputeLisPSM::GetLVPMatrix()
{
    return m_lVPMatrix;
}