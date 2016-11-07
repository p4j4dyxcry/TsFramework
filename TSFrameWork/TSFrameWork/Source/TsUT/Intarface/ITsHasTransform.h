//!*******************************************************
//! IHasTransform.h
//!
//! Transform を保持するインターフェース
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsTransForm;

class IHasTransform
{
public:
    //! コンストラクタ
    IHasTransform();
    //! デストラクタ
    virtual ~IHasTransform();

    //! ローカル位置を取得
    TsVector3 GetLocalPosition()const;

    //! ローカルの回転を取得
    TsQuaternion GetLocalRotate()const;

    //! ローカルスケールの取得
    TsVector3 GetLocalScale()const;

    //! ワールド位置の取得
    TsVector3 GetWorldPosition()const;

    //! ワールド行列の取得
    TsMatrix GetWorldMatrix()const;

    //! ワールド座標上でのオブジェクトの拡縮を取得
    TsVector3 GetWorldScale()const;

    //! ワールド座標上での回転を取得
    TsQuaternion GetWorldRotate()const;

    //! Transformへのポインタを取得
    const TsTransForm*	GetTransformPtr()const;
    
    //! 親のインターフェースを設定する
    TsBool SetParent( IHasTransform* );

    //! 親を取得
    TsTransForm* GetParent()const;

    //! 子を取得
    TsTransForm* FindChild( const TsString& )const;

    //! 子のインターフェースを設定する
    TsBool AddChild( IHasTransform* );

    //! ワールド位置を設定する
    TsBool SetWorldPosition(const TsVector3&);

    //! ローカル位置を設定する
    TsBool SetLocalPosition(const TsVector3&);

    //! 回転軸と角度からローカルの回転を設定する
    TsBool SetLocalRotateAxis(const TsVector3&, TsF32 angle);

    //! クォータニオンからローカルの回転を設定する
    TsBool SetLocalRotate(const TsQuaternion&);

    //! ワールド空間でのスケールを設定する
    TsBool SetWorldScale(const TsVector3&);

    //! ローカル空間でのスケールを設定する
    TsBool SetLocalScale(const TsVector3&);

    //! 指定した座標を注視する
    TsBool LookAt(const TsVector3& eye,
                  const TsVector3& target,
                  const TsVector3& up = TsVector3::up);

protected:
    TsTransForm* m_pTransform;	//Transform
};