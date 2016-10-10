#include "TsMath.h"

TsMatrix TsBoneTransForm::ToBoneMatrix( const TsMatrix& worldMatrix )
{
    TsMatrix result = worldMatrix;
    TsBoneTransForm* temp = this;

    TsStack<TsBoneTransForm*> matStack;
    //親を辿ってローカル行列をスタックに格納する
    do
    {
        if( temp )
        {
            if( temp->IsParentBone() )
                temp = dynamic_cast<TsBoneTransForm*>(temp->GetParent());
            else
                break;
        }
        matStack.push( temp );

    } while( temp != nullptr );

    //スタックからローカル行列を取り出しワールド行列を計算する
    while( matStack.empty() == false )
    {
        TsBoneTransForm* pBone = matStack.top();
        result = ( pBone->ToLocalMatrix() ) * result;
        matStack.pop();
    }
     TsMatrix p = ToLocalMatrix() * result;
    return m_basePoseInv * p;
}

TsBool TsBoneTransForm::IsParentBone()const
{
    if( m_parent == nullptr )
        return TS_FALSE;
    return m_parent->GetClassTypeName() == "TsBoneTransForm";
}
