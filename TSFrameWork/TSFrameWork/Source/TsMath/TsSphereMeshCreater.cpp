#include "../../TsAfx.h" 

TsBool TsSphereMeshCreater::CreateSphere(TsInt horizontalSlice,
                                         TsF32 rad)
{
    //----------------------------------------------------------
    //�� todo
    //
    // ! ���󂾂Ɣ����̖ʂ����Ԃ��Ă��܂��Ă���B
    //  Z�𔽓]�����AIndex���ēx���蓖�Ē����΂��������E�E�E�H
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

    //! �k�ɓ_�Ɠ�ɓ_�̐���
    m_positions.push_back(TsVector3::up * rad);
    m_positions.push_back(TsVector3::down * rad);
    m_normal.push_back(TsVector3::up);
    m_normal.push_back(TsVector3::down);

    m_positions.reserve( m_verticalSlice * m_horizontalSlice );
    m_normal.reserve(m_verticalSlice * m_horizontalSlice);
    //! ���_�ʒu���v�Z����
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

            // ���_ �ʒu&�@�� ���i�[
            m_positions.push_back( vertex );

            // �@���́@�ʒu - ���̒��S�@�𐳋K�����ċ��߂邪
            // ���S�͌��_�ł��邽�߈ʒu�𐳋K������΋��܂�B
            m_normal.push_back(vertex.Normalized());
        }
    }

    // ��ɓ_�Ɩk�ɓ_���������ʂ̃C���f�b�N�X���쐬
    TsInt fVert = 2;        //��ɓ_�Ɩk�ɓ_�̕�Index��i�߂邽�߂ɁB
    for (TsInt i = 1; i<m_horizontalSlice - 1; ++i)
    {
        for (TsInt j = 0; j<m_verticalSlice; ++j)
        {
            TsInt p0 = i * m_verticalSlice + j;
            TsInt p1 = (j == m_verticalSlice - 1) ? p0 - m_verticalSlice : p0;

            //! �ʂ��\������S�_���̎O�p�`�ʂɕ�������

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


    //���̖k�ɓ_�Ɠ�ɓ_�̑Ή�

    //! ��ɓ_���ӂ̃|���S���C���f�b�N�X���擾���邽�߂̃I�t�Z�b�g
    TsInt offLastVerts = fVert + (m_verticalSlice * (horizontalSlice - 1));
    for (TsInt i = 0; i<m_verticalSlice; ++i )
    {
        TsInt j = (i == m_verticalSlice - 1) ? -1 : i;

        //�k�ɓ_�|���S��
        {
            m_index.push_back(fVert - 1);
            m_index.push_back((j + 2) + fVert);
            m_index.push_back((i + 1) + fVert);
        }

        //��ɓ_�|���S��
        {
            m_index.push_back(fVert);
            m_index.push_back((i + 1) + offLastVerts);
            m_index.push_back((j + 2) + offLastVerts);
        }
    }

    return TS_TRUE;
}