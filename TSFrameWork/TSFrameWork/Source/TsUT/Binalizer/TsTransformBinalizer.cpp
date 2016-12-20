#include "../../../TsAfx.h"
#include "TsBinalizerBase.h"
#include "TsTransformBinalizer.h"
#include <fstream>
#include <functional>

TsTransformBinalizer::TsTransformBinalizer()
    :m_pRootTransform(nullptr)
{

}

TsBool TsTransformBinalizer::Binalize(std::ofstream& ofs, TsTransForm* pData)
{
    TsUint transformCount = 0;

    std::function<void(TsTransForm*)> CountOfTransform = [&](TsTransForm* pTrasnform)
    {
        transformCount++;

        if (pTrasnform->GetFirstChild())
            CountOfTransform(pTrasnform->GetFirstChild());

        if (pTrasnform->GetSubling())
            CountOfTransform(pTrasnform->GetSubling());
    };


    std::function<void(TsTransForm*)> WriteTransform = [&](TsTransForm* pTransform)
    {
        TsTransformBinalizer::TransformCommon commonData;
        memset(&commonData, 0, sizeof(TransformCommon));
        strcpy_s(commonData.name, sizeof(TsChar) * 127, pTransform->GetName().c_str());
        commonData.pos = pTransform->m_localPosition;
        commonData.q = pTransform->m_localRotate;
        commonData.scale = pTransform->m_localScale;
        if (pTransform->GetParent())
            commonData.parentPtr = (TsU64)pTransform->GetParent();
        commonData.thisPtr = (TsU64)pTransform;
        transformCount++;

        ofs.write((TsChar*)&commonData, sizeof(commonData));

        if (pTransform->GetFirstChild())
            WriteTransform(pTransform->GetFirstChild());
        if (pTransform->GetSubling())
            WriteTransform(pTransform->GetSubling());
    };

    if (pData == nullptr)
        return TS_FALSE;

    CountOfTransform(pData);

    if (WriteHeader(ofs, typeid(*this).name()) == TS_FALSE)
        return TS_FALSE;
    ofs.write((TsChar*)&transformCount, sizeof(TsUint));
    WriteTransform(pData);

    return TS_TRUE;
}

TsBool TsTransformBinalizer::Decode(std::ifstream& ifs,TsBool readHeader)
{
    if (readHeader )
    {
        if (ReadHeader(ifs, typeid(*this).name()) == TS_FALSE)
            return TS_FALSE;
    }

    TransformCommon * pCommonData;

    TsUint transformCount = 0;
    ifs.read((TsChar*)&transformCount, sizeof(TsUint));

    pCommonData = TsNew(TransformCommon[transformCount]);

    ifs.read((TsChar*)pCommonData, sizeof(TransformCommon) * transformCount);
   
    // nullptr ÇéQè∆Ç≈Ç´ÇÈÇÊÇ§Ç…Ç∑ÇÈ
    m_transformMap.insert(std::pair<TsU64, TsTransForm*>(0, nullptr));

    for (TsUint i = 0; i < transformCount; ++i)
    {
        auto& common = pCommonData[i];
        TsTransForm* pTransform = TsNew(TsTransForm);

        pTransform->SetName(common.name);
        pTransform->m_localPosition = common.pos;
        pTransform->m_localRotate = common.q;
        pTransform->m_localScale = common.scale;

        m_transformMap.insert(std::pair<TsU64, TsTransForm*>(common.thisPtr, pTransform));
    }

    for (TsUint i = 0; i < transformCount; ++i)
    {
        auto& common = pCommonData[i];
        m_transformMap[common.thisPtr]->SetParent(m_transformMap[common.parentPtr]);

        if (common.parentPtr == 0)
        {
            m_pRootTransform = m_transformMap[common.thisPtr];
        }
    }

    TsDelete(pCommonData);

    return TS_TRUE;

}