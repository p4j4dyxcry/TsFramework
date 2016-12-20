#include "../../../TsAfx.h"
#include "TsBinalizerBase.h"
#include "TsMaterialBinalizer.h"
#include <fstream>

TsMaterialBinalizer::TsMaterialBinalizer()
    :m_pMaterials(nullptr),
    m_materialNum(0)
{

}

TsBool TsMaterialBinalizer::Binalize(std::ofstream& ofs, TsDefaultMaterial** pData , TsUint count)
{
    if (pData == nullptr)
        return TS_FALSE;

    if (count == 0)
        return TS_FALSE;


    if (WriteHeader(ofs, typeid(*this).name()) == TS_FALSE)
        return TS_FALSE;

    ofs.write((TsChar*)&count, sizeof(TsUint));

    for (TsUint i = 0; i < count; ++i)
    {
        TsDefaultMaterial& om = *pData[i];
        MaterialCommon cm;
        memset(&cm, 0, sizeof(cm));
        cm.diffuse = om.GetColor();
        cm.ambient = om.GetAmbient();
        cm.emissive = om.GetEmissive();
        cm.mataric = om.GetMataric();
        cm.roughness = om.GetRoughness();
        cm.specluar = om.GetSpecluer();
        cm.power = om.GetSpeculerPower();

        strcpy_s(cm.name, om.GetName().c_str());
        if (om.GetAlbedoTexture() != nullptr)
            strcpy_s(cm.albedoTexture, om.GetAlbedoTexture()->GetName().c_str());

        if (om.GetSpeculerTexture() != nullptr)
            strcpy_s(cm.specluerTexture, om.GetSpeculerTexture()->GetName().c_str());

        if (om.GetNormalTexture() != nullptr)
            strcpy_s(cm.normalTexture, om.GetNormalTexture()->GetName().c_str());
        ofs.write((TsChar*)&cm, sizeof(MaterialCommon));
    }
    return TS_TRUE;
}
TsBool TsMaterialBinalizer::Decode(TsDevice* pDev, std::ifstream& ifs, TsBool readHeader)
{
    if (readHeader)
    {
        if (ReadHeader(ifs, typeid(*this).name()) == TS_FALSE)
            return TS_FALSE;
    }
    ifs.read((TsChar*)&m_materialNum, sizeof(TsUint));

    m_pMaterials = TsNew(TsDefaultMaterial[m_materialNum]);
    MaterialCommon* pCommon = TsNew(MaterialCommon[m_materialNum]);

    ifs.read((TsChar*)pCommon, sizeof( MaterialCommon ) * m_materialNum);

    for (TsUint i = 0; i < m_materialNum; ++i)
    {
        TsDefaultMaterial& mat = m_pMaterials[i];
        MaterialCommon& common = pCommon[i];
        mat.CreateMaterial(pDev);

        mat.SetName(common.name);
        mat.SetColor( common.diffuse );
        mat.SetAmbient(common.ambient);
        mat.SetEmissive(common.emissive);
        mat.SetMataric(common.mataric);
        mat.SetSpeculer(common.specluar);
        mat.SetSpeculerPower(common.power);
        mat.SetRoughness(common.roughness);

        if (common.albedoTexture[0] != 0)
            mat.LoadAlbedoTextureFromFile(pDev, common.albedoTexture);
        if ( common.specluerTexture[0]  != 0)
            mat.LoadSpeculerTextureFromFile(pDev, common.specluerTexture);
        if ( common.normalTexture[0]    != 0)
            mat.LoadNormalTextureFromFile(pDev, common.normalTexture);
    }

    return TS_TRUE;
}