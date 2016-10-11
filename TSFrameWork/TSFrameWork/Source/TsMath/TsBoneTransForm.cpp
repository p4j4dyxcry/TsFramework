#include "TsMath.h"

TsMatrix TsBoneTransForm::ToBoneMatrix( const TsMatrix& worldMatrix )
{
#if(1)
    return worldMatrix * m_basePoseInv *  ToWorldMatrix();
#else
    return m_basePoseInv * ToWorldMatrix();
#endif
}

TsBool TsBoneTransForm::IsParentBone()const
{
    if( m_parent == nullptr )
        return TS_FALSE;
    return m_parent->GetClassTypeName() == "TsBoneTransForm";
}
