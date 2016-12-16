#include"../../TsAfx.h"

TsSkeleton::TsSkeleton()
{
    m_boneList.reserve( 512 );
    m_isUpdate = TS_FALSE;
}
TsSkeleton::~TsSkeleton()
{
    for each( auto it in m_boneList )
        TsSafeDelete( it );
}

TsBool TsSkeleton::AddBone( TsTransForm* pTransform ,
                            TsInt boneID ,
                            const TsMatrix& bindPoseMatrix ,
                            TsTransForm* pRootBone )
{
    TsBone* pBone = TsNew( TsBone );
    pBone->SetBindPoseMatrix( bindPoseMatrix );
    pBone->m_pTransform = pTransform;
    pBone->SetBoneID( boneID );
    pBone->m_RootBone = pRootBone;
    pBone->ComputeBoneMatrix();
    m_boneList.push_back( pBone );
    return TS_TRUE;
}


TsBool TsSkeleton::UpdateSkeleton()
{
    if (m_isUpdate == TS_FALSE)
    {
        for (TsUint i = 0; i < m_boneList.size(); ++i)
            m_boneList[i]->ComputeBoneMatrix();
    }
    m_isUpdate = TS_TRUE;
    return TS_TRUE;
}

TsVector<TsBone*>& TsSkeleton::GetBoneList()
{
    return m_boneList;
}