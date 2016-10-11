#include"../../TsAfx.h"

TsSkeleton::TsSkeleton()
    :m_rootBone(nullptr)
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
                            const TsMatrix& bindPoseMatrix )
{
    TsBone* pBone = TsNew( TsBone );
    pBone->SetBindPoseMatrix( bindPoseMatrix );
    pBone->m_pTransform = pTransform;
    pBone->SetBoneID( boneID );
    pBone->m_RootBone = m_rootBone;
    m_boneList.push_back( pBone );
    return TS_TRUE;
}
TsBool TsSkeleton::SetRootBone( TsTransForm* pTransform ,
                                TsInt boneID ,
                                const TsMatrix& bindPoseMatrix )
{
    if( m_rootBone )
    {
        for( auto it = m_boneList.begin(); it != m_boneList.end(); ++it )
        {
            if( ( *it )->GetHashCode() == m_rootBone->GetHashCode() )
            {
                m_boneList.erase( it );
                break;
            }
        }
        TsSafeDelete( m_rootBone );
    }
    
    m_rootBone = TsNew( TsBone );
    m_boneList.push_back( m_rootBone );
    for each( auto p in m_boneList )
        p->m_RootBone = m_rootBone;
        
    return TS_TRUE;
}

TsBool TsSkeleton::UpdateSkeleton()
{
    for( TsInt i = 0; i < m_boneList.size(); ++i )
        m_boneList[i]->ComputeBoneMatrix();
    return TS_TRUE;
}

TsVector<TsBone*>& TsSkeleton::GetBoneList()
{
    return m_boneList;
}