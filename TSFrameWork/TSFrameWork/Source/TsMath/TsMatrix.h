//!*******************************************************
//! TsMatrix.h
//!
//! 行列
//!
//! © 2016 Yuki Tsuneyama

#pragma once

#include "../../Extern/xna/Xna.h"

#define Matrix TsMatrix

class TsVector3;
class TsQuaternion;

class TsMatrix : public XMFLOAT4X4
{
public:
    using oprator = XMFLOAT4X4;
    TsMatrix( XMFLOAT4X4& float4x4 ) :XMFLOAT4X4( float4x4 ){};
    TsMatrix( TsF32 , TsF32 , TsF32 , TsF32 ,
              TsF32 , TsF32 , TsF32 , TsF32 ,
              TsF32 , TsF32 , TsF32 , TsF32 ,
              TsF32 , TsF32 , TsF32 , TsF32 );
    TsMatrix(){ *this = TsMatrix::identity; };
    TsMatrix( XMMATRIX& matrix ){ *this = matrix; }
    TsMatrix( TsVector3& pos , TsQuaternion& rotate );
    TsMatrix( TsVector3& pos , TsQuaternion& rotate , TsVector3& scale);

    TsMatrix& operator = ( const XMMATRIX& matrix );
    TsMatrix  operator *  (const TsMatrix& matrix  )const;
    TsMatrix& operator *= (const TsMatrix& matrix );
    XMMATRIX  ToXMMATRIX()const;
    TsQuaternion ToQuaternion()const;
    TsMatrix& Transpose();
    TsMatrix& Inverse();

    TsMatrix Transposed()const;
    TsMatrix Inversed();

    // 計算結果を w = 1.0f
    TsVector3 TransformCoord(const TsVector3&)const;
    TsVector3 TransformVector(const TsVector3&)const;

    //位置をTransform　するためのMethod
    TsVector3 TransformPoint(const TsVector3&)const;

    //法線をTransform するためのMethod
    TsVector3 TransformNormal( const TsVector3& )const;

    TsVector4 TransformVector(const TsVector4&)const;

    TsBool operator == (const TsMatrix&);
    TsBool operator != (const TsMatrix&);

    //! Vector3 形式で行(横)を設定する
    TsVector3 GetVec3Row(TsInt collum)const;

    //! Vector3 形式で列(縦)を設定する
    TsVector3 GetVec3Collum(TsInt row)const;

    TsVector3 GetVec4Row(TsInt collum)const;
    TsVector3 GetVec4Collum(TsInt row)const;

    void SetVec3Row(TsInt collum, const TsVector3&);
    void SetVec3Collum(TsInt row, const TsVector3&);

    void SetVec4Row(TsInt collum, const TsVector4&);
    void SetVec4Collum(TsInt row, const TsVector4&);

    static TsMatrix CreateTranslate( FLOAT x, FLOAT y , FLOAT z );
    static TsMatrix CreateTranslate( const TsVector3& translate );
    static TsMatrix CreateRotate( const TsQuaternion& q );
    static TsMatrix CreateScale( FLOAT x , FLOAT y , FLOAT z );
    static TsMatrix CreateScale( const TsVector3& scale );
    static TsMatrix CreateLookAt( const TsVector3& eye , const TsVector3& at , const TsVector3& up  );
    static TsMatrix CreateOrtho( TsF32 width ,TsF32 height,TsF32 _near,TsF32 _far)
    {
       return TsMatrix( XMMatrixOrthographicLH( width , height , _near , _far ));
    }

    static const TsMatrix identity;

protected:
};