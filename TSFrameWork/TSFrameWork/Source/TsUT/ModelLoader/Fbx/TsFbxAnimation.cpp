#include "TsFbxAfx.h"

TsFbxAnimation::TsFbxAnimation( TsFbxContext* pFbxContext ,
                                        TsFbxScene*   pFbxScene )
                                        :TsFbxObject( pFbxContext,pFbxScene )
{
    FbxTime::eFrames60;

    auto localeTime = m_pFbxContext->GetTimeLocale();
    auto&& nodeList = m_pFbxScene->GetNodeList();

    //todo �Ƃ肠���� 1���C���̂�
    FbxAnimStack* pStack = m_pFbxScene->GetFbxScene()->GetCurrentAnimationStack();

    if( pStack == nullptr )
        return;

    FbxTakeInfo* pTakeInfo = m_pFbxScene->GetFbxImporter()->GetTakeInfo( 0 );


    TsInt layerCount = pStack->GetMemberCount<FbxAnimLayer>();

    if( layerCount > 0 )
        layerCount = 1;
    else
        return;
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
    m_boneFrameLibrary.resize( (TsInt)animationEndFrame );
    for( TsInt i = 0; i < animationEndFrame; ++i )
    {
        for each( auto it in nodeList )
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
            time.Set( FbxTime::GetOneFrameValue( FbxTime::eFrames60 ) * ( i - animationStartFrame ) );
            TsTransForm transform = FbxMatrixToTsMatrix( it->GetFbxNode()->EvaluateLocalTransform( time ) );
            transform.m_localPosition.x *= -1;
            transform.m_localRotate.x *= -1;
            transform.m_localRotate.w *= -1;
            std::pair<TS_HASH , TsMatrix> pair( hash , transform.ToLocalMatrix() );
            m_boneFrameLibrary[i].insert( pair );
        }
    }
}

TsFbxAnimation::~TsFbxAnimation()
{
 
}