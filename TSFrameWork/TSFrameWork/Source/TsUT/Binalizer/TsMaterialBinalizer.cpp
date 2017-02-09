#include "../../../TsAfx.h"
#include "TsBinalizerBase.h"
#include "TsMaterialBinalizer.h"
#include <fstream>


//----------------------------------------------------------
//! コンストラクタ
//----------------------------------------------------------
TsMaterialBinalizer::TsMaterialBinalizer()
    :m_pMaterials(nullptr),
    m_materialNum(0),
    m_pCommonMaterials(nullptr)
{

}

//----------------------------------------------------------
//! デストラクタ
//----------------------------------------------------------
TsMaterialBinalizer::~TsMaterialBinalizer()
{
    TsSafeDelete(m_pCommonMaterials);
}

//----------------------------------------------------------
//! マテリアル -> バイナリ
//----------------------------------------------------------
TsBool TsMaterialBinalizer::Binalize(std::ofstream& ofs, TsDefaultMaterial** pData , TsUint count)
{
    if (pData == nullptr)
        return TS_FALSE;

    if (count == 0)
        return TS_FALSE;

    //! ヘッダの書き込み
    if (WriteHeader(ofs, TS_BIN_HEADER) == TS_FALSE)
        return TS_FALSE;

    //! マテリアル数を書き込む
    ofs.write((TsChar*)&count, sizeof(TsUint));

    for (TsUint i = 0; i < count; ++i)
    {
        TsDefaultMaterial& originalMaterial = *pData[i];
        CommonMaterial fileMaterial;
        memset(&fileMaterial, 0, sizeof(fileMaterial));

        //! オリジナルのマテリアルからバイナライズ用の構造体に変換する。
        fileMaterial.diffuse    = originalMaterial.GetColor();
        fileMaterial.ambient    = originalMaterial.GetAmbient();
        fileMaterial.emissive   = originalMaterial.GetEmissive();
        fileMaterial.mataric    = originalMaterial.GetMataric();
        fileMaterial.roughness  = originalMaterial.GetRoughness();
        fileMaterial.specluar   = originalMaterial.GetSpecluer();
        fileMaterial.power      = originalMaterial.GetSpeculerPower();

        //! 名前コピー
        strcpy_s(fileMaterial.name, originalMaterial.GetName().c_str());


        //! Textureを保持する場合ファイルパスを書き込む。
        //  Textureが無い場合のバッファはすべて0である。
        if (originalMaterial.GetAlbedoTexture() != nullptr)
            strcpy_s(fileMaterial.albedoTexture, originalMaterial.GetAlbedoTexture()->GetName().c_str());

        if (originalMaterial.GetSpeculerTexture() != nullptr)
            strcpy_s(fileMaterial.specluerTexture, originalMaterial.GetSpeculerTexture()->GetName().c_str());

        if (originalMaterial.GetNormalTexture() != nullptr)
            strcpy_s(fileMaterial.normalTexture, originalMaterial.GetNormalTexture()->GetName().c_str());

        //! 構造体を書き込む
        ofs.write((TsChar*)&fileMaterial, sizeof(CommonMaterial));
    }
    return TS_TRUE;
}

//----------------------------------------------------------
//! バイナリ -> マテリアル
//----------------------------------------------------------
TsBool TsMaterialBinalizer::Decode(std::ifstream& ifs)
{
    //! マテリアル数を読み込む
    ifs.read((TsChar*)&m_materialNum, sizeof(TsUint));

    //! オリジナルのマテリアルのメモリを確保
    m_pMaterials = TsNew(TsDefaultMaterial[m_materialNum]);

    //! バイナリを読み込むメモリの確保
    m_pCommonMaterials = TsNew(CommonMaterial[m_materialNum]);

    //! 全てのマテリアルバイナリを読み込む
    ifs.read((TsChar*)m_pCommonMaterials, sizeof(CommonMaterial) * m_materialNum);

    return TS_TRUE;
}

TsBool TsMaterialBinalizer::BuildMaterial( TsDevice* pDev )
{
    //! バイナリ -> マテリアルのコンバート
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