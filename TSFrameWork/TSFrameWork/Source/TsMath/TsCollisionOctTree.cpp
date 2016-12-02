#include "../../TsAfx.h"



TsCollisionOctTree::TsCollisionOctTree()
{

}

TsCollisionOctTree::~TsCollisionOctTree()
{
    TsSafeDelete( m_pCells );
}

TsBool TsCollisionOctTree::Initalize( const TsAABB3D& aabb,
                                      TsInt div)
{
    m_range = aabb;
    m_divLevel = TsMin(div, TsCollisionOctTree::MAX_DIV);

    TsInt cellnum = 1;
    for (TsInt i = 0; i < m_divLevel + 1; ++i)
        cellnum *= 8;

    m_cellNum = (cellnum-1)/7;

    m_pCells = TsNew(TsCollisionTreeCell[m_cellNum]);

    return TS_TRUE;
}

// À•W‚©‚ç‹óŠÔ”Ô†‚ðŽZo
TsU64 TsCollisionOctTree::GetMorton(const TsAABB3D& aabb)
{
    static auto ComputePointElement = [this](const TsVector3& p,const TsVector3& unit)
    {
        TsU64 xBitSeparate = (TsByte)((p.x - m_range.GetMin().x) / unit.x);
        xBitSeparate = (xBitSeparate | xBitSeparate << 8) & 0x0000f00f;
        xBitSeparate = (xBitSeparate | xBitSeparate << 4) & 0x000c30c3;
        xBitSeparate = (xBitSeparate | xBitSeparate << 2) & 0x00249249;

        TsU64 yBitSeparate = (TsByte)((p.y - m_range.GetMin().y) / unit.y);
        yBitSeparate = (yBitSeparate | yBitSeparate << 8) & 0x0000f00f;
        yBitSeparate = (yBitSeparate | yBitSeparate << 4) & 0x000c30c3;
        yBitSeparate = (yBitSeparate | yBitSeparate << 2) & 0x00249249;

        TsU64 zBitSeparate = (TsByte)((p.z - m_range.GetMin().z) / unit.z);
        zBitSeparate = (zBitSeparate | zBitSeparate << 8) & 0x0000f00f;
        zBitSeparate = (zBitSeparate | zBitSeparate << 4) & 0x000c30c3;
        zBitSeparate = (zBitSeparate | zBitSeparate << 2) & 0x00249249;

        return xBitSeparate | (yBitSeparate << 1) | (zBitSeparate << 2);
    };

    TsVector3&& unitLength = (m_range.GetMax() - m_range.GetMin()) / (TsF32)(1<<m_divLevel) ;

    // Å¬ƒŒƒxƒ‹‚É‚¨‚¯‚éŠeŽ²ˆÊ’u‚ðŽZo
    TsU64 LT = ComputePointElement(aabb.GetMin(), unitLength);
    TsU64 RB = ComputePointElement(aabb.GetMax(), unitLength);

    // ‹óŠÔ”Ô†‚ðˆø‚«ŽZ‚µ‚Ä
    // ÅãˆÊ‹æØ‚è‚©‚çŠ‘®ƒŒƒxƒ‹‚ðŽZo
    TsU64 Def = RB ^ LT;
    TsUint HiLevel = 1;
    for (TsInt i = 0; i<m_divLevel; i++)
    {
        TsU64 Check = (Def >> (i * 3)) & 0x7;
        if (Check != 0)
            HiLevel = i + 1;
    }

    TsInt pow[MAX_DIV+1];
    pow[0] = 1;
    for (TsInt i = 1; i < TsCollisionOctTree::MAX_DIV+1; ++i)
        pow[i] = pow[i - 1] * 8;

    TsU64 spaceNum = RB >> (HiLevel * 3);
    spaceNum += (pow[m_divLevel - HiLevel] - 1) / 7;;

    if (spaceNum >(unsigned)m_cellNum)
        return 0xffffffff;

    return spaceNum;
}