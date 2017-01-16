#pragma once

namespace TSUT
{
    //! N角形の図形を三角形単位に分割する
    // pOut     　出力　( indexCount - 2 ) * 3 の領域が必要
    // indexList  対象の面データ
    // indexCount 面を構成する頂点の数　(etc 四角形 = 4 , 五角形 = 5 ...
    inline void ConvertGeometoryToTriangleList(TsUint* pOut,
        const TsUint* indexList,
        const TsUint  indexCount)
    {
        TsUint* p = &pOut[0];
        const TsUint* h = &indexList[0];
        const TsUint* m = &indexList[0] + 1;
        for (TsInt i = 0; i < indexCount - 2; ++i)
        {
            *(p++) = *h;
            *(p++) = *(m++);
            *(p++) = *m;
        }
    }

    //! 右手系の面を左手系もしくは左手系の面を右手系の変換する
    inline void ReverseFace(TsUint* pInOut, TsUint TriangleCount)
    {
        for (TsInt i = 0; i < TriangleCount; ++i)
        {
            TsUint temp = pInOut[i * 3 + 0];
            pInOut[i * 3 + 0] = pInOut[i * 3 + 2];
            pInOut[i * 3 + 2] = temp;
        };
    }
}
