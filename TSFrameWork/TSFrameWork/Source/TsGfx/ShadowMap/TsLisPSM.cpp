#include "../../../TsAfx.h"
#include "TsLisPSM.h"
#include "TsLisPSM_LightSrcFace.h"
#include "TsLisPSM_ViewVolum.h"

TsComputeLisPSM::TsComputeLisPSM()
{
    m_near = 0.1f;

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

        TsVector3 eyeVector = TsVector3::front;
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
    TsLisPSM_LightSrcFace lightSrcFace;
    {
        // ワールド座標系での光線方向をもとめる。
        TsVector3 vtLight;

        vtLight = m_lightDir;

        // 光線発生面を計算する。
        lightSrcFace.Init( m_lightDir , m_eyePostion , m_sceneBoundingBox );

        // 光源座標系へ変換しておく。
        lightSrcFace.Transform( m_viewMatrix * mtxViewLight );
    }

    {

        TsLisPSM_ViewVolume viewVolume;
        {
            // 視野錘の角の座標をワールド座標系で取得
            viewVolume.Init( m_viewProjectionMatrix , m_viewMatrix );

            // クリップを計算。
            // 高さ(Y軸)範囲のみをクリップするようにする。
            viewVolume.Clip( m_sceneBoundingBox , 0x02 );
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
            TsLisPSM_ViewVolume vv( viewVolume );
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
        TsLisPSM_ViewVolume lsViewVolume( viewVolume );
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
        TsLisPSM_ViewVolume lsExtViewVolume( lsViewVolume );
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
    m_viewDir = TsVector3( m_viewMatrix._31 , m_viewMatrix._32 , m_viewMatrix._33 );

    // Compute Light Space Perspective Shadow Map
    ComputeLisPSM();

    m_viewMatrix = TsMatrix::CreateLookAt( -m_lightDir *20 , TsVector3::zero , TsVector3::front );
    m_lightProjectionMatrix = TsMatrix::CreateOrtho( 1980/32, 1024/32, 3 , 50 );

    m_lVPMatrix = m_viewMatrix * m_lightProjectionMatrix;
    return TS_TRUE;
}

TsBool TsComputeLisPSM::SetEyePos(const TsVector3& value )
{
    m_eyePostion = value;
    return TS_TRUE;
}

TsBool TsComputeLisPSM::SetLightDir(const TsVector3& value )
{
    m_lightDir = value;
    return TS_TRUE;
}

TsBool TsComputeLisPSM::SetProjection(const TsMatrix& value )
{
    m_viewProjectionMatrix = value;

    TsF32 a = m_viewProjectionMatrix._33;
    TsF32 b = m_viewProjectionMatrix._43;

    m_near = -b / fabsf( a );
    m_far  =  b / (1.0f - fabsf( a ));

    return TS_TRUE;
}

TsBool TsComputeLisPSM::SetSceneBoundingBox(const TsAABB3D& aabb)
{
    m_sceneBoundingBox = aabb;
    return TS_TRUE;
}

TsBool TsComputeLisPSM::SetViewMatrix( const TsMatrix& value )
{
    m_viewMatrix = value;
    return TS_TRUE;
}

TsMatrix TsComputeLisPSM::GetLVMatrix()const
{
    return m_lightViewMatrix;
}
TsMatrix TsComputeLisPSM::GetLPMatrix()const
{
    return m_lightProjectionMatrix;
}
TsMatrix TsComputeLisPSM::GetLVPMatrix()const
{
    return m_lVPMatrix;
}