#include "TsMath.h"

TsMatrix TsBoneTransForm::ToBoneMatrix( const TsMatrix& worldMatrix )
{
    TsMatrix result = worldMatrix;
    TsBoneTransForm* temp = this;

    TsStack<TsBoneTransForm*> matStack;
    //�e��H���ă��[�J���s����X�^�b�N�Ɋi�[����
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

    //�X�^�b�N���烍�[�J���s������o�����[���h�s����v�Z����
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
