#include "TsMath.h"

TsMatrix TsBoneTransForm::ToBoneMatrix( const TsMatrix& worldMatrix )
{
    return m_basePoseInv * ToWorldMatrix();
}

TsBool TsBoneTransForm::IsParentBone()const
{
    if( m_parent == nullptr )
        return TS_FALSE;
    return m_parent->GetClassTypeName() == "TsBoneTransForm";
}
