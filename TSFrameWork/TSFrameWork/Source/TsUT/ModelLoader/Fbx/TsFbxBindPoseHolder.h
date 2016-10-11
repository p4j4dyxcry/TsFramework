#pragma once

class TsFbxBindPose : public TsFbxObject,
                      public TsNameObject
{
public:
    TsFbxBindPose(TsFbxContext* pContext, 
                  TsFbxScene* pScene,
                  FbxPose* pFbxPose);

    FbxPose* GetFbxPose()const
    {
        return m_pFbxPose;
    }
    TsMatrix* GetBindPoseMatrix( TS_HASH hash )
    {
        auto it = m_bindPoseMatrixTable.find(hash);
        if ( it == m_bindPoseMatrixTable.end())
            return nullptr;

        return &it->second;
    }
protected:

    FbxPose * m_pFbxPose;
    TsMap<TS_HASH,TsMatrix> m_bindPoseMatrixTable;
};
class TsFbxBindPoseHolder : public TsFbxObject
{
public:
    TsFbxBindPoseHolder(TsFbxContext* pFbxContext, TsFbxScene* pFbxScene);
    TsBool ParseBindPose( TsFbxScene* pFbxScene );
    TsFbxBindPose* GetBindPose(const TsString& name)const;
    TsFbxBindPose* GetBindPose(TS_HASH hash)const;
    TsMatrix* GetFirstBindPoseMatrix( const TsString& )const;
    TsMatrix* GetFirstBindPoseMatrix(TS_HASH hash)const;
protected:
    TsMap<TS_HASH, TsFbxBindPose*> m_bindPoseLiblary;
    TS_HASH m_firstBindPoseHash;
};