#include "TsFbxAfx.h"

TsFbxSkinAnimation::TsFbxSkinAnimation( TsFbxContext* pFbxContext ,
                                        TsFbxScene*   pFbxScene )
                                        :TsFbxObject( pFbxContext,pFbxScene )
{

}

TsFbxSkinAnimation::~TsFbxSkinAnimation()
{
    FbxTime::eFrames60;

    auto localeTime = m_pFbxContext->GetTimeLocale();
    auto&& boneList = m_pFbxScene->GetBoneList();

    for each( auto it in boneList )
    {
        if( it->GetParent() == nullptr ||
            it->GetParent()->IsSkeleton() == TS_FALSE )
        {
            m_pRootBone = it;
        }
    }

    //todo �Ƃ肠���� 1���C���̂�
    FbxAnimStack* pStack = m_pFbxScene->GetFbxScene()->GetCurrentAnimationStack();
    FbxTakeInfo* pTakeInfo = m_pFbxScene->GetFbxImporter()->GetTakeInfo( 0 );


    TsInt layerCount = pStack->GetMemberCount<FbxAnimLayer>();

    if( layerCount > 0 )
        layerCount = 1;
    else
        return ;
    FbxAnimLayer * pLayer = pStack->GetMember<FbxAnimLayer>();
    FbxTime importOffset = pTakeInfo->mImportOffset;					// �I�t�Z�b�g����
    FbxTime startTime = pTakeInfo->mLocalTimeSpan.GetStart();		// �J�n�������擾
    FbxTime stopTime = pTakeInfo->mLocalTimeSpan.GetStop();			// �I���������擾

    TsS64 animationStartFrame;		
    TsS64 animationEndFrame;

    // �A�j���[�V�����J�n���Ԏ��ԁ^�P�t���[���̎��ԂŊJ�n�t���[�������߂Ă���
    animationStartFrame =
        ( importOffset.Get() + startTime.Get() ) / FbxTime::GetOneFrameValue( FbxTime::eFrames60 );
    // �A�j���[�V�����I�����ԁ^�P�t���[���̎��Ԃ̎��ԂŏI���t���[�������߂Ă���
    animationEndFrame =
        ( importOffset.Get() + stopTime.Get() ) / FbxTime::GetOneFrameValue( FbxTime::eFrames60 );

    for( TsInt i = 0; i < animationEndFrame; ++i )
    {
        for each( auto it in boneList )
        {
            TS_HASH  hash = it->GetHashCode();

            if( i < animationStartFrame )
            {
                TsMatrix matrix = FbxMatrixToTsMatrix( it->GetFbxNode()->EvaluateLocalTransform( 0 ) );
                std::pair<TS_HASH , TsMatrix> pair( hash , matrix );
                m_boneFrameLibrary[i].insert( pair );
                continue;
            }

            FbxTime time;
            time.Set( FbxTime::GetOneFrameValue( FbxTime::eFrames60 ) * (i - animationStartFrame) );
            TsMatrix matrix = FbxMatrixToTsMatrix( it->GetFbxNode()->EvaluateLocalTransform( time ) );
            std::pair<TS_HASH,TsMatrix> pair( hash , matrix );
            m_boneFrameLibrary[i].insert( pair );
        }
    }
}