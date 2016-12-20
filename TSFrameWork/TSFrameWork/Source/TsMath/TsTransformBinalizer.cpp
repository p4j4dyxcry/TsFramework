#include "../../TsAfx.h"
#include <fstream>
#include <functional>

TsBool TsTransformBinalizer::WriteTransform(std::ofstream& ofs, TsTransForm* pRoot)
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

    if (pRoot == nullptr)
        return TS_FALSE;

    CountOfTransform(pRoot);
    const TsChar header[20] = "WriteTransform";
    ofs.write(header, sizeof(header));
    ofs.write((TsChar*)&transformCount, sizeof(TsUint));
    WriteTransform( pRoot );

    return TS_TRUE;
}

TsTransForm* TsTransformBinalizer::ReadTransform(std::ifstream& ifs,TsBool readHeader)
{
    if (readHeader)
    {
        TsChar header[20];
        ifs.read(header, sizeof(header));

        if ( strcmp(header,"WriteTransform" ) != 0)
        {
            return TS_FALSE;
        }

    }

    TransformCommon * pCommonData;

    TsUint transformCount = 0;
    ifs.read((TsChar*)&transformCount, sizeof(TsUint));

    pCommonData = TsNew(TransformCommon[transformCount]);

    ifs.read((TsChar*)pCommonData, sizeof(TransformCommon) * transformCount);
    TsMap<TsU64, TsTransForm*> map;

    // nullptr ÇéQè∆Ç≈Ç´ÇÈÇÊÇ§Ç…Ç∑ÇÈ
    map.insert(std::pair<TsU64, TsTransForm*>(0, nullptr));

    for (TsUint i = 0; i < transformCount; ++i)
    {
        auto& common = pCommonData[i];
        TsTransForm* pTransform = TsNew(TsTransForm);

        pTransform->SetName(common.name);
        pTransform->m_localPosition = common.pos;
        pTransform->m_localRotate = common.q;
        pTransform->m_localScale = common.scale;

        map.insert(std::pair<TsU64, TsTransForm*>(common.thisPtr, pTransform));
    }

    TsTransForm* pRootTransform = nullptr;

    for (TsUint i = 0; i < transformCount; ++i)
    {
        auto& common = pCommonData[i];
        map[common.thisPtr]->SetParent(map[common.parentPtr]);

        if (common.parentPtr == 0)
        {
            pRootTransform = map[common.thisPtr];
        }

    }

    TsDelete(pCommonData);

    return pRootTransform;

}