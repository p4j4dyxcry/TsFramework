#include "../../../TsAfx.h"
#include "TsBinalizerBase.h"
#include "TsTransformBinalizer.h"
#include "TsSkeletonBinalizer.h"
#include <fstream>
TsSkeletonBinalizer::TsSkeletonBinalizer()
    :m_pSkeleton(nullptr)
    , m_boneNum(0)
    , m_pCommonBones(nullptr)
{

}

TsSkeletonBinalizer::~TsSkeletonBinalizer()
{
    TsSafeDelete(m_pCommonBones);
}

TsBool TsSkeletonBinalizer::Binalize(std::ofstream& ofs, TsSkeleton* pData)
{
    if (pData == nullptr)
        return TS_FALSE;

    auto boneList = pData->GetBoneList();

    if (boneList.empty())
        return TS_FALSE;

    if (WriteHeader(ofs, TS_BIN_HEADER) == TS_FALSE)
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
TsBool TsSkeletonBinalizer::Decode(std::ifstream& ifs)
{
    ifs.read((TsChar*)&m_boneNum, sizeof(TsUint));

    m_pCommonBones = TsNew(CommonBone[m_boneNum]);
    ifs.read((TsChar*)m_pCommonBones, sizeof(CommonBone)*m_boneNum);

    return TS_TRUE;
}

TsBool TsSkeletonBinalizer::BuildSkeleton( TsTransformBinalizer* pTransformBinalizer )
{
	m_pSkeleton = TsNew( TsSkeleton );
    for (TsUint i = 0; i < m_boneNum; ++i)
    {
        CommonBone& bone = m_pCommonBones[i];
        TsTransForm* pTransform = pTransformBinalizer->FindByBinalyPtr(bone.transformPtr);

        m_pSkeleton->AddBone(pTransform, bone.id, bone.bindPoseMatrix);
    }
    return TS_TRUE;
}