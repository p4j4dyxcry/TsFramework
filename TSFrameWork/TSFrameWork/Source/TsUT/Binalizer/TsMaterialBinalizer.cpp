#include "../../../TsAfx.h"
#include "TsBinalizerBase.h"
#include "TsMaterialBinalizer.h"
#include <fstream>


//----------------------------------------------------------
//! �R���X�g���N�^
//----------------------------------------------------------
TsMaterialBinalizer::TsMaterialBinalizer()
    :m_pMaterials(nullptr),
    m_materialNum(0),
    m_pCommonMaterials(nullptr)
{

}

//----------------------------------------------------------
//! �f�X�g���N�^
//----------------------------------------------------------
TsMaterialBinalizer::~TsMaterialBinalizer()
{
    TsSafeDelete(m_pCommonMaterials);
}

//----------------------------------------------------------
//! �}�e���A�� -> �o�C�i��
//----------------------------------------------------------
TsBool TsMaterialBinalizer::Binalize(std::ofstream& ofs, TsDefaultMaterial** pData , TsUint count)
{
    if (pData == nullptr)
        return TS_FALSE;

    if (count == 0)
        return TS_FALSE;

    //! �w�b�_�̏�������
    if (WriteHeader(ofs, TS_BIN_HEADER) == TS_FALSE)
        return TS_FALSE;

    //! �}�e���A��������������
    ofs.write((TsChar*)&count, sizeof(TsUint));

    for (TsUint i = 0; i < count; ++i)
    {
        TsDefaultMaterial& originalMaterial = *pData[i];
        CommonMaterial fileMaterial;
        memset(&fileMaterial, 0, sizeof(fileMaterial));

        //! �I���W�i���̃}�e���A������o�C�i���C�Y�p�̍\���̂ɕϊ�����B
        fileMaterial.diffuse    = originalMaterial.GetColor();
        fileMaterial.ambient    = originalMaterial.GetAmbient();
        fileMaterial.emissive   = originalMaterial.GetEmissive();
        fileMaterial.mataric    = originalMaterial.GetMataric();
        fileMaterial.roughness  = originalMaterial.GetRoughness();
        fileMaterial.specluar   = originalMaterial.GetSpecluer();
        fileMaterial.power      = originalMaterial.GetSpeculerPower();

        //! ���O�R�s�[
        strcpy_s(fileMaterial.name, originalMaterial.GetName().c_str());


        //! Texture��ێ�����ꍇ�t�@�C���p�X���������ށB
        //  Texture�������ꍇ�̃o�b�t�@�͂��ׂ�0�ł���B
        if (originalMaterial.GetAlbedoTexture() != nullptr)
            strcpy_s(fileMaterial.albedoTexture, originalMaterial.GetAlbedoTexture()->GetName().c_str());

        if (originalMaterial.GetSpeculerTexture() != nullptr)
            strcpy_s(fileMaterial.specluerTexture, originalMaterial.GetSpeculerTexture()->GetName().c_str());

        if (originalMaterial.GetNormalTexture() != nullptr)
            strcpy_s(fileMaterial.normalTexture, originalMaterial.GetNormalTexture()->GetName().c_str());

        //! �\���̂���������
        ofs.write((TsChar*)&fileMaterial, sizeof(CommonMaterial));
    }
    return TS_TRUE;
}

//----------------------------------------------------------
//! �o�C�i�� -> �}�e���A��
//----------------------------------------------------------
TsBool TsMaterialBinalizer::Decode(std::ifstream& ifs)
{
    //! �}�e���A������ǂݍ���
    ifs.read((TsChar*)&m_materialNum, sizeof(TsUint));

    //! �I���W�i���̃}�e���A���̃��������m��
    m_pMaterials = TsNew(TsDefaultMaterial[m_materialNum]);

    //! �o�C�i����ǂݍ��ރ������̊m��
    m_pCommonMaterials = TsNew(CommonMaterial[m_materialNum]);

    //! �S�Ẵ}�e���A���o�C�i����ǂݍ���
    ifs.read((TsChar*)m_pCommonMaterials, sizeof(CommonMaterial) * m_materialNum);

    return TS_TRUE;
}

TsBool TsMaterialBinalizer::BuildMaterial( TsDevice* pDev )
{
    //! �o�C�i�� -> �}�e���A���̃R���o�[�g
    for (TsUint i = 0; i < m_materialNum; ++i)
    {
        TsDefaultMaterial& mat = m_pMaterials[i];
        CommonMaterial& common = m_pCommonMaterials[i];
        mat.CreateMaterial(pDev);

        mat.SetName(common.name);
        mat.SetColor(common.diffuse);
        mat.SetAmbient(common.ambient);
        mat.SetEmissive(common.emissive);
        mat.SetMataric(common.mataric);
        mat.SetSpeculer(common.specluar);
        mat.SetSpeculerPower(common.power);
        mat.SetRoughness(common.roughness);

        if (common.albedoTexture[0] != 0)
            mat.LoadAlbedoTextureFromFile(pDev, common.albedoTexture);
        if (common.specluerTexture[0] != 0)
            mat.LoadSpeculerTextureFromFile(pDev, common.specluerTexture);
        if (common.normalTexture[0] != 0)
            mat.LoadNormalTextureFromFile(pDev, common.normalTexture);
    }

    return TS_TRUE;
}