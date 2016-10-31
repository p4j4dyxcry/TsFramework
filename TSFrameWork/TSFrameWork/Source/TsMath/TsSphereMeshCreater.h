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
    TsVector<TsVector3> m_positions;    //頂点位置
    TsVector<TsVector3> m_normal;       //頂点法線
    TsVector<TsInt>     m_index;        //インデックス
    TsInt m_verticalSlice;              //横分割数
    TsInt m_horizontalSlice;            //縦分割数
       
};