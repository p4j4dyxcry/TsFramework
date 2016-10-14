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

    //todo とりあえず 1レイヤのみ
    FbxAnimStack* pStack = m_pFbxScene->GetFbxScene()->GetCurrentAnimationStack();
    FbxTakeInfo* pTakeInfo = m_pFbxScene->GetFbxImporter()->GetTakeInfo( 0 );


    TsInt layerCount = pStack->GetMemberCount<FbxAnimLayer>();

    if( layerCount > 0 )
        layerCount = 1;
    else
        return ;
    FbxAnimLayer * pLayer = pStack->GetMember<FbxAnimLayer>();
    FbxTime importOffset = pTakeInfo->mImportOffset;					// オフセット時間
    FbxTime startTime = pTakeInfo->mLocalTimeSpan.GetStart();		// 開始時刻を取得
    FbxTime stopTime = pTakeInfo->mLocalTimeSpan.GetStop();			// 終了時刻を取得

    TsS64 animationStartFrame;		
    TsS64 animationEndFrame;

    // アニメーション開始時間時間／１フレームの時間で開始フレームを求めている
    animationStartFrame =
        ( importOffset.Get() + startTime.Get() ) / FbxTime::GetOneFrameValue( FbxTime::eFrames60 );
    // アニメーション終了時間／１フレームの時間の時間で終了フレームを求めている
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