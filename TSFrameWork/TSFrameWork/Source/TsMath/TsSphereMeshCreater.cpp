#include "../../TsAfx.h" 

TsBool TsSphereMeshCreater::CreateSphere(TsInt horizontalSlice,
                                         TsF32 rad)
{
    //----------------------------------------------------------
    //※ todo
    //
    // ! 現状だと半球の面が裏返ってしまっている。
    //  Zを反転させ、Indexを再度割り当て直せばいけそう・・・？
    //
    if (horizontalSlice <= 2 || rad <= 0)
        return TS_FALSE;

    m_positions.clear();
    m_normal.clear();
    m_index.clear();

    m_horizontalSlice = horizontalSlice + 1;
    m_verticalSlice   = horizontalSlice * 2;

    TsF32 pitchAngle    = TsRadian(180.0f / (TsF32)m_horizontalSlice);
    TsF32 rotAngle      = TsRadian(360.0f / (TsF32)m_verticalSlice);

    //! 北極点と南極点の生成
    m_positions.push_back(TsVector3::up * rad);
    m_positions.push_back(TsVector3::down * rad);
    m_normal.push_back(TsVector3::up);
    m_normal.push_back(TsVector3::down);

    m_positions.reserve( m_verticalSlice * m_horizontalSlice );
    m_normal.reserve(m_verticalSlice * m_horizontalSlice);
    //! 頂点位置を計算する
    for (TsInt i = 1; i < m_verticalSlice; ++i)
    {
        TsF32 xSin = 
            sin( pitchAngle * i );

        for (TsInt j = 0; j < m_horizontalSlice; ++j)
        {
            TsVector3 vertex;
            
            vertex.x = cos(j * rotAngle)   * xSin *rad;
            vertex.y = cos(i * pitchAngle) * rad;
            vertex.z = sin(j * rotAngle)   * xSin * rad;

            // 頂点 位置&法線 を格納
            m_positions.push_back( vertex );

            // 法線は　位置 - 球の中心　を正規化して求めるが
            // 中心は原点であるため位置を正規化すれば求まる。
            m_normal.push_back(vertex.Normalized());
        }
    }

    // 南極点と北極点を除いた面のインデックスを作成
    TsInt fVert = 2;        //南極点と北極点の分Indexを進めるために。
    for (TsInt i = 1; i<m_horizontalSlice - 1; ++i)
    {
        for (TsInt j = 0; j<m_verticalSlice; ++j)
        {
            TsInt p0 = i * m_verticalSlice + j;
            TsInt p1 = (j == m_verticalSlice - 1) ? p0 - m_verticalSlice : p0;

            //! 面を構成する４点を二つの三角形面に分割する

            // face 1
            {
                m_index.push_back((p0 + 1 - m_verticalSlice) + fVert);
                m_index.push_back((p1 + 2 - m_verticalSlice) + fVert);
                m_index.push_back((p1 + 2) + fVert);
            }

            // face 2
            {
                m_index.push_back((p1 + 2) + fVert);
                m_index.push_back((p0 + 1) + fVert);
                m_index.push_back((p0 + 1 - m_verticalSlice) + fVert);
            }

        }
    }


    //球の北極点と南極点の対応

    //! 南極点周辺のポリゴンインデックスを取得するためのオフセット
    TsInt offLastVerts = fVert + (m_verticalSlice * (horizontalSlice - 1));
    for (TsInt i = 0; i<m_verticalSlice; ++i )
    {
        TsInt j = (i == m_verticalSlice - 1) ? -1 : i;

        //北極点ポリゴン
        {
            m_index.push_back(fVert - 1);
            m_index.push_back((j + 2) + fVert);
            m_index.push_back((i + 1) + fVert);
        }

        //南極点ポリゴン
        {
            m_index.push_back(fVert);
            m_index.push_back((i + 1) + offLastVerts);
            m_index.push_back((j + 2) + offLastVerts);
        }
    }

    return TS_TRUE;
}