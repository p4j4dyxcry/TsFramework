#pragma once

class TsSphereMeshCreater
{
public:
    TsBool CreateSphere(TsInt m_devision, TsF32 radius = 1);
    TsVector<TsVector3>& GetPositions()
    {
        return m_positions;
    }
    TsVector<TsInt>& GetIndex()
    {
        return m_index;
    }
    TsVector<TsVector3>& GetNomals()
    {
        return m_normal;
    }

private:
    TsVector<TsVector3> m_positions;    //���_�ʒu
    TsVector<TsVector3> m_normal;       //���_�@��
    TsVector<TsInt>     m_index;        //�C���f�b�N�X
    TsInt m_verticalSlice;              //��������
    TsInt m_horizontalSlice;            //�c������
       
};