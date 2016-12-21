#pragma once
class TsFbxAnimation : public TsFbxObject
                         , public TsNameObject
{
public: 
    TsFbxAnimation( TsFbxContext* pFbxContext,
                        TsFbxScene*   pFbxScene);
    virtual ~TsFbxAnimation();
    TsVector<TsMap<TS_HASH , TsMatrix>> GetBoneFrameLibrary()const
    {
        return m_boneFrameLibrary;
    }
protected:
    TsInt m_startFrame;
    TsInt m_endFrame;
    TsFbxNode * m_pRootBone;
    TsVector<TsMap<TS_HASH , TsMatrix>> m_boneFrameLibrary;
};
/*

class CBone
{
friend class CBoneManager;
private:
int id = 0;

CMatrix		m_init;
CMatrix		m_inv;
CTransForm* m_now;

CBoneManager * m_boneManager = nullptr;

public:
CBone() :m_parent( nullptr ) ,
m_firstChild( nullptr ) ,
m_subling( nullptr ){};

std::string m_name;

CBone* m_parent;
CBone* m_firstChild;
CBone* m_subling;

void InitializeBone( CTransForm* pTransform );

void SetParent( CBone* bone );
};

class CBoneManager
{
friend class CBone;
static const int MAX_BONE = 256;

public:

CBoneManager() :m_rootBone( nullptr ){};

std::list<CBone*>  m_boneList;
CBone* m_rootBone;
CMatrix m_boneTransFormMatrix[MAX_BONE];

//! ƒ{[ƒ“‚ð’Ç‰Á‚·‚é
void AddBone( CBone* bone , int id );

//ƒAƒjƒ[ƒVƒ‡ƒ“î•ñ‚©‚çƒ{[ƒ“s—ñ‚ðÝ’è‚·‚é
void SetSkinInfo( const CSkinAnimation& skin , int frame );

//ƒ{[ƒ“‚ðXV‚·‚é
void UpdateBones( const CMatrix& world );

//ƒ{[ƒ“‚ðŽæ“¾‚·‚é
CBone* FindBone( const std::string& target );

void CreateRootNode( const std::string& name );

private:
//’P‘Ìƒ{[ƒ“‚ÌXV
void UpdateBone( CBone* bone , const CMatrix& world );

//ƒ‹[ƒgƒ{[ƒ“‚ðŽZo
void ComputeRootBone();

};

void CBoneManager::UpdateBone( CBone* bone , const CMatrix& world )
{
int id = bone->id;
( *bone->m_now ) *= world;

m_boneTransFormMatrix[id] =
bone->m_inv *
bone->m_now->ToLocalMatrix();

m_boneTransFormMatrix[id].Transpose();

if( bone->m_firstChild )
UpdateBone( bone->m_firstChild , bone->m_now->ToLocalMatrix() );
if( bone->m_subling )
UpdateBone( bone->m_subling , world );
}
*/