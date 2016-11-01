#include "TsFbxAfx.h"

TsFbxBindPose::TsFbxBindPose(TsFbxContext* pFbxContext,
                             TsFbxScene* pFbxScene,
                             FbxPose* pFbxPose) :TsFbxObject( pFbxContext,pFbxScene )
{
    m_pFbxPose = pFbxPose;


    for (TsInt i = 0; i < m_pFbxPose->GetCount(); ++i)
    {
        std::pair<TS_HASH, TsMatrix> pair;
        pair.first = TSUT::StringToHash(m_pFbxPose->GetNode(i)->GetName());
        pair.second = FbxMatrixToTsMatrix(m_pFbxPose->GetMatrix(i));
        m_bindPoseMatrixTable.insert(pair);

       //for debug 
       //FbxMatrix m = m_pFbxPose->GetMatrix(i);
       //printf("NodeName = %s \n", m_pFbxPose->GetNode(i)->GetName());
       //printf("MATRIX \n");
       //printf("%03.4f,%03.4f,%03.4f,%03.4f\n", m.Get(0, 0), m.Get(0, 1), m.Get(0, 2), m.Get(0, 3));
       //printf("%03.4f,%03.4f,%03.4f,%03.4f\n", m.Get(1, 0), m.Get(1, 1), m.Get(1, 2), m.Get(1, 3));
       //printf("%03.4f,%03.4f,%03.4f,%03.4f\n", m.Get(2, 0), m.Get(2, 1), m.Get(2, 2), m.Get(2, 3));
       //printf("%03.4f,%03.4f,%03.4f,%03.4f\n", m.Get(3, 0), m.Get(3, 1), m.Get(3, 2), m.Get(3, 0));
    }
}

TsFbxBindPoseHolder::TsFbxBindPoseHolder(TsFbxContext* pFbxContext, TsFbxScene* pFbxScene)
: TsFbxObject(pFbxContext, pFbxScene)
{

}

TsBool TsFbxBindPoseHolder::ParseBindPose(TsFbxScene* pTsFbxScene )
{
    FbxScene* pFbxScene = pTsFbxScene->GetFbxScene();

    TsInt numPose = pFbxScene->GetPoseCount();

    if (numPose > 0)
    {
        FbxPose* pPose = pFbxScene->GetPose(0);
        m_firstBindPoseHash = TSUT::StringToHash(pPose->GetName());
    }

    for (TsInt i = 0; i < numPose; ++i)
    {
        std::pair<TS_HASH, TsFbxBindPose*> pair;
        FbxPose* pPose = pFbxScene->GetPose(i);
        pair.first = TSUT::StringToHash(pPose->GetName());
        pair.second = TsNew( TsFbxBindPose(m_pFbxContext, m_pFbxScene, pPose));

        m_bindPoseLiblary.insert(pair);
    }

    return TS_TRUE;
}

TsFbxBindPose* TsFbxBindPoseHolder::GetBindPose(const TsString& name)const
{
    TS_HASH hash = TSUT::StringToHash(name);
    return GetBindPose(hash);
}
TsFbxBindPose* TsFbxBindPoseHolder::GetBindPose(TS_HASH hash)const
{
    return m_bindPoseLiblary.find(hash)->second;
}
TsMatrix* TsFbxBindPoseHolder::GetBindPoseMatrix(const TsString& name)const
{
    TS_HASH hash = TSUT::StringToHash(name);
    return GetFirstBindPoseMatrix(hash);
}
TsMatrix* TsFbxBindPoseHolder::GetFirstBindPoseMatrix(TS_HASH hash)const
{
    if (m_bindPoseLiblary.empty())
        return nullptr;
    for( auto p : m_bindPoseLiblary )
    {
        TsMatrix * pResult = p.second->GetBindPoseMatrix( hash );
        if( pResult )
            return pResult;
    }
    return nullptr;
}
