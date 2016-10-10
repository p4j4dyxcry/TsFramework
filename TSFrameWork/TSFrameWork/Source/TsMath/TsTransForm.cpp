#include "../TsUT/TsUT.h"
#include <functional>

#include "TsTransForm.h"
#include "TsMatrix.h"


using namespace std;

TsTransForm::TsTransForm(const TsMatrix& m)
{
    this->TsTransForm::TsTransForm();
    *this = m;
}
TsTransForm::~TsTransForm()
{
    if (m_parent)
    {
        RemoveOfParantChild();
    }
    auto p = m_firstChild;
    while (p != nullptr)
    {
        auto temp = p;
        p = p->m_subling;
        TsDelete(temp);
    }
}
TsTransForm& TsTransForm::operator=( const TsMatrix& m )
{
    //! 行列データを軸データ・位置データ・スケールデータに分解

    //軸データ
    TsVector3 xA( m.m[0][0] , m.m[0][1] , m.m[0][2] );
    TsVector3 yA( m.m[1][0] , m.m[1][1] , m.m[1][2] );
    TsVector3 zA( m.m[2][0] , m.m[2][1] , m.m[2][2] );

    //スケールデータ
    float xScale = xA.Length();
    float yScale = yA.Length();
    float zScale = zA.Length();

    //位置データ
    TsVector3 pos = TsVector3(m.m[3][0], m.m[3][1], m.m[3][2]);

    //回転データの計算は拡縮成分・平行移動成分を除いた回転行列に落としてからクォータニオン化する。
    TsMatrix rotate;

    //軸をスケールで割ることで回転行列の各成分を求める。
    rotate.m[0][0] = xA.x / xScale;
    rotate.m[1][0] = xA.y / xScale;
    rotate.m[2][0] = xA.z / xScale;

    rotate.m[0][1] = yA.x / yScale;
    rotate.m[1][1] = yA.y / yScale;
    rotate.m[2][1] = yA.z / yScale;

    rotate.m[0][2] = zA.x / zScale;
    rotate.m[1][2] = zA.y / zScale;
    rotate.m[2][2] = zA.z / zScale;

    //値の設定（現状は全てローカル値と解釈することにする)
    m_localScale = TsVector3(xScale, yScale, zScale);
    m_localPosition = pos;
    m_localRotate = rotate.ToQuaternion();

    return *this;
}

TsTransForm TsTransForm::operator*( const TsMatrix& m )
{
    TsMatrix local = ToLocalMatrix();
    TsTransForm result( ( local * m ) );
    result.SetName( GetName() );
    result.m_parent = m_parent;
    return result;
}

TsTransForm& TsTransForm::operator*=( const TsMatrix& m )
{
    return *this = (*this * m);
}
//!*******************************************************
//! [メソッド] ローカル行列を取得する
TsMatrix TsTransForm::ToLocalMatrix()
{
    return TsMatrix( m_localPosition , m_localRotate ,m_localScale);
}

TsMatrix TsTransForm::ToLocalTRSMatrix()
{
    return  TsMatrix::CreateTranslate(m_localPosition) *
            TsMatrix::CreateRotate( m_localRotate)*
            TsMatrix::CreateScale( m_localScale);
}

//!*******************************************************
//! [メソッド] ワールド行列を取得する
TsMatrix TsTransForm::ToWorldMatrix()
{
    TsMatrix result = ToLocalMatrix();
    TsTransForm* temp = this;
    
    //親を辿る
    while( temp->m_parent != nullptr )
    {
        result *= temp->m_parent->ToLocalMatrix();
        temp = temp->m_parent;
    } 


    return result;
}

TsTransForm* TsTransForm::GetParent()const
{
    return m_parent;
}

TsTransForm* TsTransForm::GetFirstChild()const
{
    return m_firstChild;
}

TsTransForm* TsTransForm::GetSubling()const
{
    return m_subling;
}

TsVector3 TsTransForm::GetWorldPos()
{
    TsMatrix m = ToWorldMatrix();
    return TsVector3(m._42, m._42, m._43);
}

TsVector3 TsTransForm::GetWorldScale()
{
    TsTransForm t = ToWorldMatrix();
    return t.m_localScale;
}

TsQuaternion TsTransForm::GetWorldRotate()
{
    TsTransForm t = ToWorldMatrix();
    return t.m_localRotate;
}

TsBool TsTransForm::SetParent(__inout TsTransForm* parent)
{
    //既に親がいる場合はその親との関係を清算し、新しい親と関係を構築する必要がある。
    if( m_parent )
    {		
        if (RemoveOfParantChild() == TS_FALSE)
            return TS_FALSE;
    }

    //親に子供がいない場合第一子を自分にする
    if( parent->m_firstChild == nullptr )
        parent->m_firstChild = this;

    //親にすでに子供がいる場合は兄弟の末尾に自分を追加する
    else
    {
        TsTransForm ** subling = &( parent->m_firstChild->m_subling );
        while( *subling != nullptr )
        {
            subling = &( ( *subling )->m_subling );
        }
        *subling = this;
    }

    m_parent = parent;
    return TS_TRUE;
}

TsBool TsTransForm::AddChild(TsTransForm* pTransform)
{
    return pTransform->SetParent(this);
}



TsBool	TsTransForm::RemoveOfParantChild()
{
    // 親の第一子が自分ならそのままポインタの繋ぎ変え
    if (m_parent->m_firstChild == this)
    {
        m_parent->m_firstChild = m_parent->m_firstChild->m_subling;
    }
    //親の第二子以降が自分なら兄弟を辿ってポインタの繋ぎ変え
    else if (m_subling)
    {
        auto s = m_parent->m_firstChild->m_subling;
        for (; s != nullptr; s = s->m_subling)
        {
            if (s->m_subling == this)
            {
                s->m_subling = m_subling;
            }
        }
    }
    else
    {
        //木構造が正常にできていない。エラー
        return TS_FALSE;
    }

    return TS_TRUE;
}

TsTransForm* TsTransForm::FindChildByhash(TS_HASH hash)
{
    std::function<TsTransForm*(TsTransForm*)> TreeSearch = [&](TsTransForm* p)
    {
        TsTransForm* ptr = nullptr;
        if( p->GetHashCode() == hash )
        {
            return p;
        }
        if( p->m_firstChild )
        {
            ptr = TreeSearch( p->m_firstChild );
            if( ptr )
                return ptr;
        }
        if( p->m_subling )
        {
            ptr = TreeSearch( p->m_subling );
            if( ptr )
                return ptr;
        }

        return ptr;
    };

    return TreeSearch(this);
}

TsTransForm* TsTransForm::FindChildByClassName( const TsString& name )
{
    std::function<TsTransForm*( TsTransForm* )> TreeSearch = [&]( TsTransForm* p )
    {
        TsTransForm* ptr = nullptr;
        if( p->GetClassTypeName() == name )
        {
            return p;
        }
        if( p->m_firstChild )
        {
            ptr = TreeSearch( p->m_firstChild );
            if( ptr )
                return ptr;
        }
        if( p->m_subling )
        {
            ptr = TreeSearch( p->m_subling );
            if( ptr )
                return ptr;
        }

        return ptr;
    };

    return TreeSearch( this );
}

TsTransForm* TsTransForm::FindChildByName(const TsString& name)
{
    TS_HASH hash = TSUT::StringToHash(name);

    return FindChildByhash(hash);
}