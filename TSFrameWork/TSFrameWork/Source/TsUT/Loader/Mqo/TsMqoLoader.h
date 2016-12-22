#pragma once

// mqo Material
struct TsMqoMaterial
{
    TsFloat4 color;
    TsF32 diffuse;
    TsF32 ambient;
    TsF32 emissib;
    TsF32 specular;
    TsF32 specularPower;
    TsString textureName;
};

// mqo vertex
struct TsMqoVertex
{
    TsFloat3 pos;
    TsFloat3 normal;
    TsFloat2 uv;
};

// mqo face
struct TsMqoFace
{
    TsInt3 index;       // triangle indexs
    TsInt  m_matIndex;  // material index
    TsFloat2 uv[3];     // triangle uv
    TsFloat3 normal;    // �@��
    TsF32 angle;        // �X��
    TsF32 area;         // �ʐ�
};

struct TsMqoMesh
{
    TsVector<TsMqoVertex> vertex;
    TsMqoMaterial material;
};

struct TsMqoObject
{
    TsVector<TsMqoMesh> mesh;
};

class TsMqoLoader : public Ts3DModelLoader
{
public :
    TsMqoLoader();
    virtual ~TsMqoLoader();
    TsBool LoadFile( const TsChar* filename )override;
    TsBool CreateCommonData()override;

    //==========================================================
    //  !SetScale
    //  ���b�V���̓ǂݍ��ݎ��ɒP�ʂ����[�g�����W�ɕϊ����܂��B
    //  True	���[�g��(�f�t�H���g)	
    //  False	�Z���`
    //  �����̃��\�b�h�̓��[�h���s���O�ɃR�[������K�v������܂�
    void SetCenti( TsBool flg);

    //==========================================================
    //  !SetSmooth
    //  ���b�V���̓ǂݍ��ݎ��ɖ@�����v�Z���X���[�W���O���܂��B
    //  �����̃��\�b�h�̓��[�h���s���O�ɃR�[������K�v������܂�
    void SetSmooth( TsBool flg );
protected:
    TsVector<TsMqoMaterial> LoadMaterial( std::ifstream& ifs );
    TsVector<TsMqoFace> LoadObject(TsVector<TsFloat3>& pos, std::ifstream & ifs );
    
    //! �}�e���A�������擾
    template <typename T> 
    TsVector<T> PickOutMaterial( TsString file , TsString id ,TsInt num);	

private:
    TsMqoObject* m_mqoObject;
    TsF32  m_loadScale;
    TsBool m_isSmooth;
};

