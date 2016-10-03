#pragma once

class TsFbxContext
{
public:
	TsFbxContext();
	virtual ~TsFbxContext();
	const FbxManager* GetFbxManager()const;
private:
	FbxManager* m_pFbxManager;
};

class TsFbxObject
{
public:
	TsFbxObject( TsFbxContext* pFbxContext )
	{
		m_pFbxContext = pFbxContext;
	}
	virtual ~TsFbxObject();
protected:
	TsFbxContext* m_pFbxContext;
};