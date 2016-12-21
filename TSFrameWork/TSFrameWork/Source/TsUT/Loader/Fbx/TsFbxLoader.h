#pragma once

class TsFbxContext;
class TsTransformBakeAnimation;
class TsFbxLoader : public Ts3DModelLoader
{
public:
    TsFbxLoader();
    virtual~TsFbxLoader();

    //! Load From File
    virtual TsBool LoadFile(const TsChar* filename)override;

    void SetLoadAnimationFlag(TsBool isLoadAnim);
    void SetLoadGeometoryFlag(TsBool isLoadGeometory);

    TsTransformBakeAnimation* CreateAnimation(TsInt );

    virtual TsBool CreateCommonData()override;
private:
    TsFbxContext* m_pFbxContext;
    TsBool m_isLoadGeometory;
    TsBool m_isLoadAnimation;
    TsString m_filename;
};