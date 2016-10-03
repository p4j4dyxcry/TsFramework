#include "TsFbxHeader.h"
#include "TsFbxManager.h"

TsFbxContext::TsFbxContext()
	:m_pFbxManager( nullptr )
{
	m_pFbxManager = FbxManager::Create();
}

TsFbxContext::~TsFbxContext()
{
	FbxSafeRelease( m_pFbxManager );
}

const FbxManager* TsFbxContext::GetFbxManager()const
{
	return m_pFbxManager;
}