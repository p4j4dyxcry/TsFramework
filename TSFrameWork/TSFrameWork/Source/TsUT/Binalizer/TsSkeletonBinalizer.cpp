#include "../../../TsAfx.h"
#include "TsBinalizerBase.h"
#include "TsTransformBinalizer.h"
#include "TsSkeletonBinalizer.h"
#include <fstream>
TsSkeletonBinalizer::TsSkeletonBinalizer()
    :m_pSkeleton(nullptr)
{

}

TsBool TsSkeletonBinalizer::Binalize(std::ofstream& ofs, TsSkeleton* pData)
{
    if (pData == nullptr)
        return TS_FALSE;

    auto boneList = pData->GetBoneList();

    if (boneList.empty())
        return TS_FALSE;

    if (WriteHeader(ofs, typeid(*this).name()) == TS_FALSE)
        return TS_FALSE;  

    TsUint boneNum = boneList.size();
    ofs.write((TsChar*)&boneNum, sizeof(TsUint));

    for (auto p : boneList)
    {
        CommonBone bone;
        memset(&bone, 0, sizeof(CommonBone));
        bone.id = p->GetBoneID();
        bone.bindPoseMatrix = p->GetBindPoseMatrix();
        bone.transformPtr = (TsU64)p->GetTransform();
        if (p->GetName().size() > 0)
            strcpy_s(bone.name, p->GetName().c_str());

        ofs.write((TsChar*)&bone, sizeof(CommonBone));
    }

    return TS_TRUE;
}
TsBool TsSkeletonBinalizer::Decode(std::ifstream& ifs,
                                   TsTransformBinalizer* pTransformBinalizer,
                                   TsBool readHeader )
{
    if (readHeader)
    {
        if (ReadHeader(ifs, typeid(*this).name()) == TS_FALSE)
            return TS_FALSE;
    }

    TsUint boneNum = 0;
    m_pSkeleton = TsNew(TsSkeleton);

    ifs.read((TsChar*)&boneNum, sizeof(TsUint));

    CommonBone* pBones = TsNew(CommonBone[boneNum]);
    ifs.read((TsChar*)pBones, sizeof(CommonBone)*boneNum);
    for (TsUint i = 0; i < boneNum; ++i)
    {
        CommonBone& bone = pBones[i];
        TsTransForm* pTransform = pTransformBinalizer->FindByBinalyPtr(bone.transformPtr);

        m_pSkeleton->AddBone(pTransform,bone.id,bone.bindPoseMatrix);
    }

    return TS_TRUE;
}