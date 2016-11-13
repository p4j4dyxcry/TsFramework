#include "../../../TsAfx.h"
#include "TsLisPSM.h"

class LightSrcFace
{
public:
    void Init( const TsVector3& lightDir ,
               const TsVector3& eyePos,
               TsF32 sceneSize)
    {
        // ���������ɂ���ʂ�I������B
        // �C�ӂ̍��Wp��ʂɓ��e����v�Z�ɂ����āA���e���������ƂȂ邱�Ƃ������
        // ���߂ɁA�����Ɩʂ�1�x�ȏ�̌X���������ꍇ�ɂ́A���̖ʂ͌��������ʂƂ�
        // �Ȃ��B
        TsVector3 vt;
        vt = lightDir.Normalized();
        const TsF32 th = sin( TsRadian( 1.0f ) );

        TsVector3 minScene = TsVector3( -sceneSize , -sceneSize , -sceneSize );
        TsVector3 maxScene = TsVector3( sceneSize , sceneSize , sceneSize );

        // YZ����
        if( th < fabsf( vt.x ) )
        {
            if( 0.0f < vt.x )
            {
                m_vtNormal[m_numFace] = TsVector3( 1.0f , 0.0f , 0.0f );
                m_vtPos[m_numFace] = TsVector3( minScene.x , 0.0f , 0.0f );
            }
            else
            {
                m_vtNormal[m_numFace] = TsVector3( -1.0f , 0.0f , 0.0f );
                m_vtPos[m_numFace] = TsVector3( maxScene.x , 0.0f , 0.0f );
            }
            m_numFace++;
        }

        // XZ����
        if( th < fabsf( vt.y ) )
        {
            if( 0.0f < vt.y )
            {
                m_vtNormal[m_numFace] = TsVector3( 0.0f , 1.0f , 0.0f );
                m_vtPos[m_numFace] = TsVector3( 0.0f , minScene.y , 0.0f );
            }
            else
            {
                m_vtNormal[m_numFace] = TsVector3( 0.0f , -1.0f , 0.0f );
                m_vtPos[m_numFace] = TsVector3( 0.0f , maxScene.y , 0.0f );
            }
            m_numFace++;
        }

        // XY����
        if( th < fabsf( vt.z ) )
        {
            if( 0.0f < vt.z )
            {
                m_vtNormal[m_numFace] = TsVector3( 0.0f , 0.0f , 1.0f );
                m_vtPos[m_numFace] = TsVector3( 0.0f , 0.0f , minScene.z );
            }
            else
            {
                m_vtNormal[m_numFace] = TsVector3( 0.0f , 0.0f , -1.0f );
                m_vtPos[m_numFace] = TsVector3( 0.0f , 0.0f , maxScene.z );
            }
            m_numFace++;
        }
    }

    void Transform( const TsMatrix& mtx )
    {
        TsVector3 temp;
        for( TsInt i = 0; i < m_numFace; i++ )
        {
            m_vtNormal[i] = mtx.TransformNormal( m_vtNormal[i] );
            m_vtPos[i] = mtx.TransformPoint( m_vtPos[i] );
        }
    }

    TsVector3 Project( const TsVector3& vt )const
    {
        // ���W��-Y�����ɓ��e���āA�ł��߂��ʂƂ̌�_���v�Z����B
        TsF32 maxY = -FLT_MAX;
        for( TsInt i = 0; i < m_numFace; i++ )
        {
            const TsVector3 &n = m_vtNormal[i];
            const TsVector3 &o = m_vtPos[i];
            TsF32 a = ( ( o.x - vt.x ) * n.x ) + ( ( o.z - vt.z ) * n.z );
            TsF32 y = ( a / n.y ) + o.y;

            if( maxY < y ) maxY = y;
        }

        return TsVector3( vt.x , maxY , vt.z );
    }

protected:

    // ! �L���Ȍ��������ʂ̐�
    // ! �Œ�1���A�ő�3���ƂȂ�B
    TsInt       m_numFace = 0;

    // ! ���������ʂ̌���
    TsVector3   m_vtNormal[3];

    // ! ���������ʂ̈ʒu
    TsVector3   m_vtPos[3];

    //! ���ʂ̕�������D
    TsF32       m_planeD[3];
};

class ViewVolume
{
public:

    ViewVolume(){}

    //=========================================================================
    //! Copy Constructor
    //=========================================================================
    ViewVolume(const ViewVolume& vv )
    {
        for( TsInt i = 0; i < m_numFace; i++ )
        {
            m_numVertices[i] = vv.m_numVertices[i];
            for( TsInt v = 0; v < m_numVertices[i]; v++ )
            {
                m_vt[i][v] = vv.m_vt[i][v];
            }
        }
        m_vtMin = vv.m_vtMin;
        m_vtMax = vv.m_vtMax;
    }

    void Init( const TsMatrix& proj ,
               const TsMatrix& view )
    {
        // �t���X�^���̊p�����[���h���W�n��
        TsVector3 viewFrustum[10];

        TsMatrix invViewProj = ( view * proj ).Inversed();

        // near clip �� far clip ���擾����
        TsF32 n , f , nn, ff;
        {
            TsF32 a = proj._33;
            TsF32 b = proj._43;
            n = -b / fabsf( a );
            f = b / ( 1.0f - fabsf( a ) );
            nn = 0;
            ff = f;
        }

        // w���Z�O�̎ˉe���W�n����r���[���W�n�֕ϊ�����s���p���ăt���X�^����
        // �p�̍��W�����߂�B
        // x��y�͕K�����쐍�̕ӏ�ɂ���̂�(x/w)�����(y/w)�͉��s��(z)�ɂ���
        // ��炸+-1.0�ɂȂ�B������(z/w)�� mtxProjection �� near �� far ��
        // ���̂ŁA�ϐ�nn��ff�Ɍv�Z���Ă����B
        TsVector4 vt[] =
        {
            TsVector4( -n , n , nn , n ) ,	// 0:left-top-near
            TsVector4( -n , -n , nn , n ) ,	// 1:left-bottom-near
            TsVector4( n , -n , nn , n ) ,	// 2:right-bottom-near
            TsVector4( n , n , nn , n ) ,	// 3:right-top-near
            TsVector4( -f , f , ff , f ) ,	// 4:left-top-far
            TsVector4( -f , -f , ff , f ) ,	// 5:left-bottom-far
            TsVector4( f , -f , ff , f ) ,	// 6:right-bottom-far
            TsVector4( f , f , ff , f ) ,	// 7:right-top-far
            TsVector4( 0 , 0 , nn , n ) ,	// 8:vfNearCenter
            TsVector4( 0 , 0 , ff , f ) ,	// 9:vfFarCenter
        };

        // w���Z�O�̃N���b�v���W�n(vt[])����߂�l�̍��W�n�ւ̕ϊ����v�Z����B
        for( TsInt i = 0; i < 10; ++i )
        {
            TsVector4 temp = vt[i] * invViewProj;
            viewFrustum[i] = temp;
        }

        // �o�E���_���{�b�N�X�̍쐬
        m_vtMin = m_vtMax = viewFrustum[0];
        for( TsInt i = 1; i < 8; i++ )
        {
            m_vtMin = TsMin( m_vtMin , viewFrustum[i] );
            m_vtMax = TsMax( m_vtMax , viewFrustum[i] );
        }

        // ���쐎�̑��ʂ̃|���S���f�[�^���쐬
        static const TsInt poli[m_numFace][4] =
        {
            { 0 , 4 , 7 , 3 } ,	// top
            { 3 , 7 , 6 , 2 } ,	// right
            { 2 , 6 , 5 , 1 } ,	// bottom
            { 1 , 5 , 4 , 0 } ,	// left
            { 0 , 3 , 2 , 1 } , // near
            { 4 , 5 , 6 , 7 } , // far
        };

        for( TsInt i = 0; i < m_numFace; i++ )
        {
            for( TsInt j = 0; j < 4; j++ )
            {
                m_vt[i][j] = viewFrustum[poli[i][j]];
            }
            m_numVertices[i] = 4;
        }
    }

    void Transform( const TsMatrix& mtx, TsBool divW )
    {
        m_vtMin = TsVector3( FLT_MAX , FLT_MAX , FLT_MAX );
        m_vtMax = TsVector3( -FLT_MAX , -FLT_MAX , -FLT_MAX );

        if( !divW )
        {
            // w���Z�s�v�̃P�[�X
            for( TsInt i = 0; i < m_numFace; i++ )
            {
                for( TsInt v = 0; v < m_numVertices[i]; v++ )
                {
                    TsVector3 &vt = m_vt[i][v];

                    vt = mtx.TransformPoint( vt );
                    m_vtMin = TsMin( m_vtMin , vt);
                    m_vtMax = TsMax( m_vtMax , vt);
                }
            }
        }
        else
        {
            // w���Z���s��
            for( TsInt i = 0; i < m_numFace; i++ )
            {
                for( TsInt v = 0; v < m_numVertices[i]; v++ )
                {
                    TsVector3 &vt = m_vt[i][v];

                    vt = mtx.TransformCoord( vt );
                    m_vtMin = TsMin( m_vtMin , vt );
                    m_vtMax = TsMax( m_vtMax , vt );
                }
            }
        }
    }

    //=========================================================================
    //! Comment
    //  �{�����[�������������ʂ֓��e����
    //  \param lightFace ����������
    //  \par ����F
    //  �@�o�E���_���{�b�N�X�͍X�V����Ȃ��̂Œ��ӁB
    //=========================================================================
    void ProjectLightSrcFace( const LightSrcFace &lightFace )
    {
        for( TsInt i = 0; i < m_numFace; i++ )
        {
            for( TsInt v = 0; v < m_numVertices[i]; v++ )
            {
                m_vt[i][v] = lightFace.Project( m_vt[i][v] );
            }
        }
    }

    // �o�E���_���{�b�N�X�̍ŏ��l���Q��
    const TsVector3 GetBBoxMin( void ) const { return m_vtMin; }

    // �o�E���_���{�b�N�X�̍ő�l���Q��
    const TsVector3 GetBBoxMax( void ) const { return m_vtMax; }

    TsBool Clip( const TsVector3& minRange ,
                 const TsVector3& maxRange ,
                 TsUint flag )
    {
        TsBool into = false;

        // �e�|���S�����ƂɃN���b�v���v�Z�B
        m_vtMin = TsVector3( FLT_MAX , FLT_MAX , FLT_MAX );
        m_vtMax = TsVector3( -FLT_MAX , -FLT_MAX , -FLT_MAX );
        for( TsInt i = 0; i < m_numFace; i++ )
        {
            TsVector3 vvc[m_maxVertices];
            TsInt nv = ClipPolygon(
                m_numVertices[i] , m_vt[i] , vvc , minRange , maxRange , flag );

            into |= ( 0 < nv );

            // �v�Z���ʂ�m_vt[]�ɖ߂��A�o�E���_���{�b�N�X���X�V�B
            for( TsInt v = 0; v < nv; v++ )
            {
                TsVector3 &vt = m_vt[i][v];
                vt = vvc[v];
                m_vtMin = TsMin( vt , m_vtMin );
                m_vtMax = TsMax( vt , m_vtMax );
            }
            m_numVertices[i] = nv;
        }

        return into;
    }

protected:


    TsInt ClipPolygon(
        TsInt numVertex , const TsVector3 *polyPos ,
        TsVector3 *clip ,
        const TsVector3 &minRect , const TsVector3 &maxRect ,
        TsUint mask )
    {
        // �|���S�����}X�A�}Y�A�}Z�̊e�ʂŏ��ɐؒf���Ă����B
        // �|���S���̒��_�z��͖ʖ���
        //  polyPos -> workBuffer[0] -> workBuffer[1] -> workBuffer[0] -> ...
        // �Ƒ������A�ؒf�ɂ�钸�_���̕ω��� numVertex �ϐ��ɑ�������B
        TsVector3 workBuffer[2][16];
        TsInt destBufferIndex = 0;
        const TsVector3 *src = polyPos;
        TsVector3 *dest = workBuffer[destBufferIndex];

        // minX��maxX�ŃN���b�v�B
        if( mask & 0x01 )
        {
            for( TsInt i = 0; i < 2; i++ )
            {
                const TsF32 x = ( i == 0 ) ? minRect.x : maxRect.x;
                const TsF32 w = ( i == 0 ) ? +1.0f : -1.0f;
                TsInt s = 0;
                for( TsInt v = 0; v < numVertex; v++ )
                {
                    // ���_���N���b�v���C����蒆�ɂ���΁A���̒��_�����̂܂܍̗p����B
                    if( 0.0f <= w * ( src[v].x - x ) )
                    {
                        dest[s++] = src[v];
                    }
                    // �ӂ��N���b�v���C�����܂����ł���΁A��_�̍��W�����߂ăN���b�v
                    // ���_��}������B(������O�A�O���璆�̌����͊֌W�Ȃ��B)
                    const TsInt vv = ( v + 1 ) % numVertex;
                    if( ( src[v].x < x ) ^ ( src[vv].x < x ) )
                    {
                        const TsF32 t = ( x - src[v].x ) / ( src[vv].x - src[v].x );
                        dest[s] = TsVector3( TsVector3( src[v] ) * ( 1.0f - t ) );
                        dest[s++] += TsVector3( src[vv] ) * t;
                    }
                }
                // ����������ꂽ���_���Ȃ���΁A�|���S���͊��S�ɃN���b�v�A�E�g���ꂽ�B
                if( s == 0 ) return 0;
                // �o�b�t�@�����ւ��đ�����B
                numVertex = s;
                src = dest;
                destBufferIndex = ( destBufferIndex + 1 ) % 2;
                dest = workBuffer[destBufferIndex];
            }
        }

        // minY��maxY�ŃN���b�v�B
        if( mask & 0x02 )
        {
            for( TsInt i = 0; i < 2; i++ )
            {
                const TsF32 y = ( i == 0 ) ? minRect.y : maxRect.y;
                const TsF32 w = ( i == 0 ) ? +1.0f : -1.0f;
                TsInt s = 0;
                for( TsInt v = 0; v < numVertex; v++ )
                {
                    // ���_���N���b�v���C����蒆�ɂ���΁A���̒��_�����̂܂܍̗p����B
                    if( 0.0f <= w * ( src[v].y - y ) )
                    {
                        dest[s++] = src[v];
                    }
                    // �ӂ��N���b�v���C�����܂����ł���΁A��_�̍��W�����߂ăN���b�v
                    // ���_��}������B(������O�A�O���璆�̌����͊֌W�Ȃ��B)
                    const TsInt vv = ( v + 1 ) % numVertex;
                    if( ( src[v].y < y ) ^ ( src[vv].y < y ) )
                    {
                        const TsF32 t = ( y - src[v].y ) / ( src[vv].y - src[v].y );
                        dest[s] = TsVector3( ( 1.0f - t ) * TsVector3( src[v] ) );
                        dest[s++] += t * TsVector3( src[vv] );
                    }
                }
                // ����������ꂽ���_���Ȃ���΁A�|���S���͊��S�ɃN���b�v�A�E�g���ꂽ�B
                if( s == 0 )
                    return 0;
                // �o�b�t�@�����ւ��đ�����B
                numVertex = s;
                src = dest;
                destBufferIndex = ( destBufferIndex + 1 ) % 2;
                dest = workBuffer[destBufferIndex];
            }
        }

        // minZ��maxZ�ŃN���b�v�B
        if( mask & 0x04 )
        {
            for( TsInt i = 0; i < 2; i++ )
            {
                const TsF32 z = ( i == 0 ) ? minRect.z : maxRect.z;
                const TsF32 w = ( i == 0 ) ? +1.0f : -1.0f;
                TsInt s = 0;
                for( TsInt v = 0; v < numVertex; v++ )
                {
                    // ���_���N���b�v���C����蒆�ɂ���΁A���̒��_�����̂܂܍̗p����B
                    if( 0.0f <= w * ( src[v].z - z ) )
                    {
                        dest[s++] = src[v];
                    }
                    // �ӂ��N���b�v���C�����܂����ł���΁A��_�̍��W�����߂ăN���b�v
                    // ���_��}������B(������O�A�O���璆�̌����͊֌W�Ȃ��B)
                    const TsInt vv = ( v + 1 ) % numVertex;
                    if( ( src[v].z < z ) ^ ( src[vv].z < z ) )
                    {
                        const TsF32 t = ( z - src[v].z ) / ( src[vv].z - src[v].z );
                        dest[s] = TsVector3( ( 1.0f - t ) * TsVector3( src[v] ) );
                        dest[s++] += t * TsVector3( src[vv] );
                    }
                }
                // ����������ꂽ���_���Ȃ���΁A�|���S���͊��S�ɃN���b�v�A�E�g���ꂽ�B
                if( s == 0 ) return 0;
                // �o�b�t�@�����ւ��đ�����B
                numVertex = s;
                src = dest;
                destBufferIndex = ( destBufferIndex + 1 ) % 2;
                dest = workBuffer[destBufferIndex];
            }
        }

        // ���ʂ��R�s�[���ďI��
        for( TsInt v = 0; v < numVertex; v++ )
        {
            clip[v] = src[v];
        }
        return numVertex;
    }

    // ���쐎�̑��ʂ̃|���S���ő̐ς�\��
    // 0:top / 1:right / 2:bottom / 3:left / 4:near / 5:far
    static const TsInt m_numFace = 6;

    // ���ʖ��̊p��(���_��)
    TsInt m_numVertices[m_numFace];

    // ���W�o�b�t�@�̍ő�T�C�Y
    static const TsInt m_maxVertices = 16;

    // �p�̍��W(�ő�8�p�`)
    TsVector3 m_vt[m_numFace][m_maxVertices];

    // �o�E���_���{�b�N�X
    TsVector3 m_vtMin;
    TsVector3 m_vtMax;
};

TsComputeLisPSM::PointList::PointList()
{
    m_list.reserve( 128 );
}

TsInt TsComputeLisPSM::PointList::GetSize()const
{
    return (TsInt)m_list.size();
};
TsBool TsComputeLisPSM::PointList::Add( TsVector3& value )
{
    m_list.push_back( value );
    return TS_TRUE;
}
TsBool TsComputeLisPSM::PointList::Clear()
{
    m_list.clear();
    return TS_TRUE;
}
TsBool TsComputeLisPSM::PointList::Transform( const TsMatrix & matrix )
{
    for each( auto var in  m_list )
        var.TransformCood( matrix );
    return TS_TRUE;
}
TsBool TsComputeLisPSM::PointList::ComputeAABB( TsVector3& min , TsVector3 & max )const
{
    if( GetSize() > 0 )
    {
        for each ( auto var in m_list )
        {
            min = TsMin( min , var );
            max = TsMax( max , var );
        }
    }
    else
    {
        return TS_FALSE;
    }
    return TS_TRUE;
}

TsComputeLisPSM::PointList& TsComputeLisPSM::PointList::operator =( PointList& value )
{
    m_list.clear();
    for( TsInt i = 0; i < value.GetSize(); ++i )
    {
        Add( value[i] );
    }
    return ( *this );
}

TsVector3& TsComputeLisPSM::PointList::operator[]( TsInt index )
{
    return m_list[index];
}
TsComputeLisPSM::TsComputeLisPSM()
{
    m_near = 0.1f;
    m_pointList.Clear();
}
TsVector3 TsComputeLisPSM::ComputeUpVector( TsVector3& viewDir , TsVector3 lightDir )
{
    TsVector3 left = TsVector3::Cross( lightDir , viewDir );
    TsVector3 up = TsVector3::Cross( left , lightDir );
    return up.Normalized();
}
TsBool TsComputeLisPSM::ComputeShadowMap()
{
    TsVector3 max , min;
    TsMatrix lightView = 
        TsMatrix::CreateLookAt( m_eyePostion ,
                                m_eyePostion + m_lightDir ,
                                m_viewDir );
    m_pointList.Transform( lightView );

    m_pointList.ComputeAABB( min , max );

    m_lightProjectionMatrix = GetUnitCubeClipMatrix( min , max );

    return TS_TRUE;
}

TsBool TsComputeLisPSM::ComputeLisPSM()
{
    // Step.0
    // �r���[���W�n�̍��W���������W�n�֕ϊ�����s�� mtxViewLight ���v�Z����B
    // �������W�n�͌�������J�����̎��쐎��������W�n�B�������W�n���猩������
    // �����V���h�E�}�b�v�փ����_�����O����B�������W�n��X,Y,Z���x�N�g�����
    // �`���A���ꂩ��r���[-�����ϊ��s������߂�B���̊��x�N�g���͊e�J�X�P�[
    // �h�i�ŋ��ʁB
    // X���F�V���h�E�}�b�v�̉�����(leftVector)
    // Y���F��������(upVector)
    // Z���F�V���h�E�}�b�v�̉��s��(�c)����(viewVector)
    // T�v�f�F�[��
    TsF32 sinGamma;
    TsMatrix mtxViewLight , mtxLightView;
    {
        // ���������x�N�g�����r���[���W�n�֕ϊ�����upVector�Ƃ���B
        // (light.direction�͐��K������Ă���B)
        TsVector3 upVector = m_lightDir.Normalized();

        upVector = m_viewMatrix.TransformNormal( upVector );
        upVector.Normalize();

        TsVector3 eyeVector 
//            = TsVector3( m_viewMatrix._31 , m_viewMatrix._32 , m_viewMatrix._33 );
              = TsVector3::front;
        // �����ƌ����̂Ȃ��p��cos�����߂Ă����B
        TsF32 cosGamma = TsVector3::Dot( upVector,eyeVector );

        // �����ƌ��������������������ꍇ�ɂ͒��s����x�N�g��leftVector���v�Z
        // �ł��Ȃ��̂ŁA�������������܂����Čv�Z��i�߂�B
        static const TsF32 th = cosf( TsRadian( 1.0f ) );
        if( th < fabsf( cosGamma ) )
        {
            eyeVector = TsVector3( 0.0f , sinf( TsRadian( 1.0f ) ) , cosf( TsRadian( 1.0f ) ) );
            cosGamma = TsVector3::Dot( upVector , eyeVector );
        }

        // �����ƌ����̊p�x����v�Z�����cosGamma��sinGamma�B
        sinGamma = sqrtf( 1.0f - cosGamma*cosGamma );

        // �����ƌ����ɒ��s����x�N�g����leftVector�Ƃ���B(�P�ʍs��ɐ��K�����Ă���)
        // �r���[���W�n���g���Ă���̂�eyeVector��(0.0, 0.0, 1.0)�ƂȂ�B
        TsVector3 leftVector;
        {
            leftVector = TsVector3::Cross( upVector , eyeVector * -1.0f );
            leftVector.Normalize();
        }

        // ������leftVector�ɒ��s����x�N�g����viewVector�Ƃ���B
        // viewVector��LisPSM�����ϊ��̉��s��������\���x�N�g���B
        // (�ʏ�̃J��������̓����ϊ��Ȃ�Ύ�������(Z)�ɑ���)
        TsVector3 viewVector;
        {
            viewVector = TsVector3::Cross( upVector , leftVector );
            viewVector.Normalize();
        }

        // mtxViewLight�̋t�ϊ����s���A�������W�n����r���[���W�n�ւ̕ϊ��s���
        // �p�ӂ��Ă����B
        mtxLightView = TsMatrix(
            leftVector.x , leftVector.y , leftVector.z , 0.0f ,
            upVector.x , upVector.y , upVector.z , 0.0f ,
            viewVector.x , viewVector.y , viewVector.z , 0.0f ,
            0.0f , 0.0f , 0.0f , 1.0f
            );

        // �r���[���W�n����������W�n�֕ϊ�����s����v�Z����B
        // (�������W�n�̋t�s��=�]�u�s��)�Ƃ����������g���Ă���B
        // X�� : leftVector (�����ƌ����ɒ��s����x�N�g��)
        // Y�� : upVector (��������)
        // Z�� : viewVector (LisPSM�����ϊ��̉��s������)
        mtxViewLight = mtxLightView.Transposed();
    }
    LightSrcFace lightSrcFace;
    {
        // ���[���h���W�n�ł̌������������Ƃ߂�B
        TsVector3 vtLight;

        vtLight = m_lightDir;

        // ���������ʂ��v�Z����B
        lightSrcFace.Init( m_lightDir , m_eyePostion , m_far );

        // �������W�n�֕ϊ����Ă����B
        lightSrcFace.Transform( m_viewMatrix * mtxViewLight );
    }

    {

        ViewVolume viewVolume;
        {
            // ���쐎�̊p�̍��W�����[���h���W�n�Ŏ擾
            viewVolume.Init( m_viewProjectionMatrix , m_viewMatrix );

            TsVector3 minScene = -TsVector3::one * m_far;
            TsVector3 maxScene =  TsVector3::one * m_far;

            // �N���b�v���v�Z�B
            // ����(Y��)�͈݂͂̂��N���b�v����悤�ɂ���B
            viewVolume.Clip( minScene , maxScene , 0x02 );
        }

        // Step.3
        // �r���[���W�n�ł̎��쐎��Z�͈͂��v�Z����B
        // �����Step.2�ɂ����ăN���b�v�������쐎���g���B
        // ���ӁF
        // viewVolume �ɂ͍��W�ϊ��╽�ʃN���b�v�̌v�Z�덷���܂܂�Ă���B
        // ���̂��߃J������ Near �N���b�v�l���������ƁA�R�R�� transform() �ŁA
        // zn ���}�C�i�X�̒l�ɂȂ�\��������B�����Ȃ�ƌ��
        // Nopt = (zn + sqrtf(zn * zf)) / sinGamma;
        // �ɂ����āA sqrtf() ���񐔂�Ԃ��A�V���h�E�}�b�v�����������Ȃ�B
        // ���������邽�߂ɁA zn ���}�C�i�X�ɂȂ�Ȃ��悤�ɓK���ȃN���b�v��
        // �s���B����͍��{�I�ȉ����ł͂Ȃ��B
        TsF32 zn , zf;
        {
            ViewVolume vv( viewVolume );
            vv.Transform( m_viewMatrix , TS_FALSE );
            zn = vv.GetBBoxMin().z;
            zf = vv.GetBBoxMax().z;

            if( zn < 0.001f ) zn = 0.001f;
        }

        TsMatrix mtxLightShadow;
        // Step.4
        // �������W�n�ł̎��쐎��Z�͈͂��v�Z���ALisPSM�̓����ϊ��̌��_�܂ł�
        // ���������߂�B
        TsF32 projNear , projFar;
        ViewVolume lsViewVolume( viewVolume );
        {
            // ���쐎���������W�n�ɕϊ�����Z�͈͂��擾�B
            const TsMatrix mtx( m_viewMatrix * mtxViewLight );
            lsViewVolume.Transform( mtx , TS_FALSE );

            projNear = lsViewVolume.GetBBoxMin().z;
            projFar = lsViewVolume.GetBBoxMax().z;

            // LisPSM�̓����ϊ��̌��_�܂ł̋������v�Z
            TsF32 Nopt;
            {
                // �_���ł����߂���Ă������B
                const TsF32 zfopt = zn + ( projFar - projNear ) * sinGamma;
                Nopt = ( zn + sqrtf( zn * zfopt ) ) / sinGamma;
            }

            // projNear�AprojFar�𓧎��ϊ��̌��_����̋����ɕϊ�
            const TsF32 localOffset = projNear - Nopt;
            projNear -= localOffset;
            projFar -= localOffset;

            // �ϊ��s�����
            mtxLightShadow = TsMatrix::CreateTranslate( TsVector3::front * -localOffset );
        }

        // Step.5
        // LisPSM�̓����ϊ����v�Z���� mtxLightShadow �ɓK�p�B
        // ���̓����ϊ��ł�z/w��(-1.0 - +1.0)�͈̔͂ƂȂ�B
        {
            TsMatrix mtxProj = TsMatrix::identity;
            mtxProj.m[2][2] = ( projFar + projNear ) / ( projFar - projNear );
            mtxProj.m[3][2] = -2.0f * ( projFar * projNear ) / ( projFar - projNear );

            mtxProj.m[2][3] = 1.0f;
            mtxProj.m[3][3] = 0.0f;
            mtxLightShadow = mtxLightShadow * mtxProj;
        }

        // Step.6
        // ���쐎�̊O����e�𗎂Ƃ��\���̂����Ԃ��l�����邽�߂ɁA���쐎��
        // ���������ʂɓ��e����B
        ViewVolume lsExtViewVolume( lsViewVolume );
        {
            lsExtViewVolume.ProjectLightSrcFace( lightSrcFace );
        }

        // Step.7
        // �v�Z���������ϊ��ɑ΂���Uniform�N���b�v���s���B
        {
            // ���쐎�Ƃ���Step.6�Ōv�Z�����̐ς��V���h�E�}�b�v���W�n�ɕϊ����A
            // ���͈̂̔͂��v�Z�B
            lsViewVolume.Transform( mtxLightShadow , TS_TRUE );
            lsExtViewVolume.Transform( mtxLightShadow , TS_TRUE );

            TsVector3 vtMin;
            vtMin = TsMin( lsViewVolume.GetBBoxMin() , lsExtViewVolume.GetBBoxMin() );

            TsVector3 vtMax;
            vtMax = TsMax( lsViewVolume.GetBBoxMax() , lsExtViewVolume.GetBBoxMax() );

            // XZ�̓V���h�E�}�b�v�e�N�X�`����UV���W(-1.0 �` +1.0)�ɁA
            // Y�̓V���h�E�}�b�v�ɏ������܂��[�x�ɂȂ�B
            TsMatrix mtxFit = TsMatrix::identity;

            mtxFit.m[0][0] = 2.0f / ( vtMax.x - vtMin.x );
            //			mtxFit.m[1][0] = 0.0f;
            //			mtxFit.m[2][0] = 0.0f;
            mtxFit.m[3][0] = -( vtMax.x + vtMin.x ) / ( vtMax.x - vtMin.x );

            // Y���̃V���h�E�}�b�v�̐[�x���̓v���b�g�t�H�[���ɂ���ĈقȂ�B
            // D3D�n:(0.0 �` +1.0)
            // OpenGL�n:(-1.0 �` +1.0)
            {
                // D3D�n:
                //				mtxFit.m[0][1] = 0.0f;
                mtxFit.m[1][1] = 1.0f / ( vtMax.y - vtMin.y );
                //				mtxFit.m[2][1] = 0.0f;
                mtxFit.m[3][1] = -vtMin.y / ( vtMax.y - vtMin.y );
            }


            // Z���ɂ��ẮA���X projNear ���� projFar �͈̔͂œ����ϊ���
            // �v�Z���Ă��邽�߁AvtMin.z�AvtMax.z ��-1.0����+1.0�͈̔͂ɂȂ���
            // ����B
            //			mtxFit.m[0][2] = 0.0f;
            //			mtxFit.m[1][2] = 0.0f;
            //			mtxFit.m[2][2] = 2.0f / (vtMax.z - vtMin.z);
            //			mtxFit.m[3][2] = -(vtMax.z + vtMin.z) / (vtMax.z - vtMin.z);

            // Uniform�N���b�v��K�p�B
            mtxLightShadow = mtxLightShadow * mtxFit;
        }

        // �[�x(��������̋���)��Z�ɏo�͂����悤�ɁA�����ϊ���̍��W����]����B
        // �V���h�E�}�b�v��Y��(��������)�ɂ����ă}�b�s���O�����B
        // ����͒P����Y����Z���̌����ɂ���Ď����ł���B
        for( TsInt j = 0; j < 4; j++ )
        {
            TsF32& y = mtxLightShadow.m[j][1];
            TsF32& z = mtxLightShadow.m[j][2];

            TsF32 sw = y;	
            y = z;	
            z = sw;
        }

        // Step.8
        // �r���[���W�n����V���h�E�}�b�v���W�n�ւ̕ϊ��s����v�Z
        m_lightProjectionMatrix = mtxViewLight * mtxLightShadow;
        m_lightViewMatrix = m_viewMatrix;
        m_lVPMatrix = m_viewMatrix * m_lightProjectionMatrix;
    }

    return TS_TRUE;
}

TsBool TsComputeLisPSM::UpdateShadowMatrix()
{
    TsMatrix viewProj;
    viewProj = m_viewMatrix * m_viewProjectionMatrix;

    //�r���[�{�����[�������߂�
    ComputeViewFrustum( viewProj );

    // Light Space Perspective Shadow Map
    ComputeLisPSM();

    //������W�n�ɕϊ�
    //TsMatrix scale = TsMatrix::CreateScale( 1.0f , 1.0f , -1.0f );
    //m_lightProjectionMatrix = m_lightProjectionMatrix * scale;
    //m_lVPMatrix = m_lightViewMatrix * m_lightProjectionMatrix;

    return TS_TRUE;
}
TsBool TsComputeLisPSM::ComputeViewFrustum( TsMatrix& viewProj )
{
    m_pointList.Clear();

    //�@�����̂��쐬
    TsVector3 v[8];
    v[0] = TsVector3( -1.0f , +1.0f , -1.0f );
    v[1] = TsVector3( -1.0f , -1.0f , -1.0f );
    v[2] = TsVector3( +1.0f , -1.0f , -1.0f );
    v[3] = TsVector3( +1.0f , +1.0f , -1.0f );
    v[4] = TsVector3( -1.0f , +1.0f , +1.0f );
    v[5] = TsVector3( -1.0f , -1.0f , +1.0f );
    v[6] = TsVector3( +1.0f , -1.0f , +1.0f );
    v[7] = TsVector3( +1.0f , +1.0f , +1.0f );
    for( TsInt i = 0; i<8; i++ )
    {
        m_pointList.Add( v[i] );
    }

    //�@�r���[�s�񁨎ˉe�s��̋t�ϊ����s���s������߂�
    TsMatrix invViewProj = viewProj.Inversed();

    //�@�����̂ɋt�ϊ�����s��������A����������߂�
    m_pointList.Transform( invViewProj );

    return TS_TRUE;
}

TsMatrix TsComputeLisPSM::GetUnitCubeClipMatrix( TsVector3& min , TsVector3& max )
{
    TsMatrix result;

    result._11 = 2.0f / ( max.x - min.x );
    result._21 = 0.0f;
    result._31 = 0.0f;
    result._41 = -( max.x + min.x ) / ( max.x - min.x );

    result._12 = 0.0f;
    result._22 = 2.0f / ( max.y - min.y );
    result._32 = 0.0f;
    result._42 = -( max.y + min.y ) / ( max.y - min.y );

    result._13 = 0.0f;
    result._23 = 0.0f;
    result._33 = 1.0f / ( max.z - min.z );
    result._43 = -min.z / ( max.z - min.z );

    result._14 = 0.0f;
    result._24 = 0.0f;
    result._34 = 0.0f;
    result._44 = 1.0f;

    return result;
}
TsMatrix TsComputeLisPSM::GetPerspective( TsF32 nearDist , TsF32 farDist )
{
    TsMatrix result;
    result = TsMatrix::identity;
    result._22 = farDist / ( farDist - nearDist );
    result._24 = 1.0f;
    result._42 = -farDist * nearDist / ( farDist - nearDist );
    result._44 = 0.0f;
    return result;
}

TsBool TsComputeLisPSM::SetEyePos(const TsVector3& value )
{
    m_eyePostion = value;
    return TS_TRUE;
}
TsBool TsComputeLisPSM::SetViewDir(const TsVector3& value )
{
    m_viewDir = value;
    return TS_TRUE;
}
TsBool TsComputeLisPSM::SetLightDir(const TsVector3& value )
{
    m_lightDir = value;
    return TS_TRUE;
}

TsBool TsComputeLisPSM::SetEyeProjection(const TsMatrix& value )
{
    m_viewProjectionMatrix = value;

    TsF32 a = m_viewProjectionMatrix._33;
    TsF32 b = m_viewProjectionMatrix._43;

    m_near = -b / fabsf( a );
    m_far  =  b / (1.0f - fabsf( a ));

    return TS_TRUE;
}

TsBool TsComputeLisPSM::SetEyeViewMatrix( const TsMatrix& value )
{
    m_viewMatrix = value;
    return TS_TRUE;
}

TsMatrix TsComputeLisPSM::GetLVMatrix()
{
    return m_lightViewMatrix;
}
TsMatrix TsComputeLisPSM::GetLPMatrix()
{
    return m_lightProjectionMatrix;
}
TsMatrix TsComputeLisPSM::GetLVPMatrix()
{
    return m_lVPMatrix;
}