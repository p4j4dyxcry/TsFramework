#include"../../TsAfx.h"

TsSkeleton::TsSkeleton()
{
    m_boneList.reserve( 512 );
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
    m_boneList.push_back( pBone );
    return TS_TRUE;
}


TsBool TsSkeleton::UpdateSkeleton()
{
    for( TsUint i = 0; i < m_boneList.size(); ++i )
        m_boneList[i]->ComputeBoneMatrix();
    return TS_TRUE;
}

TsVector<TsBone*>& TsSkeleton::GetBoneList()
{
    return m_boneList;
}