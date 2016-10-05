#include "../../TsUT.h"
#include "TsFbxHeader.h"

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
	//todo loop animation frame count
	m_boneFrameLibrary.resize( 100 );
	for( TsInt i = 0; i < 100; ++i )
	{
		for each( auto it in boneList )
		{
			TS_HASH  hash = it->GetHashCode();
			TsMatrix matrix = FbxMatrixToTsMatrix( it->GetFbxNode()->EvaluateLocalTransform( i ));
			std::pair<TS_HASH,TsMatrix> pair( hash , matrix );
			m_boneFrameLibrary[i].insert( pair );
		}
	}
}