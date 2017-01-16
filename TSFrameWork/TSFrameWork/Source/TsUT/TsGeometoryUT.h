#pragma once

namespace TSUT
{
    //! N�p�`�̐}�`���O�p�`�P�ʂɕ�������
    // pOut     �@�o�́@( indexCount - 2 ) * 3 �̗̈悪�K�v
    // indexList  �Ώۂ̖ʃf�[�^
    // indexCount �ʂ��\�����钸�_�̐��@(etc �l�p�` = 4 , �܊p�` = 5 ...
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

    //! �E��n�̖ʂ�����n�������͍���n�̖ʂ��E��n�̕ϊ�����
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
