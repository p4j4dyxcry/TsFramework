#include "../../../TsAfx.h"
#include "TsLisPSM.h"
#include "TsLisPSM_LightSrcFace.h"
#include "TsLisPSM_ViewVolum.h"

TsComputeLisPSM::TsComputeLisPSM()
{
    m_near = 0.1f;

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

        TsVector3 eyeVector = TsVector3::front;
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
    TsLisPSM_LightSrcFace lightSrcFace;
    {
        // ���[���h���W�n�ł̌������������Ƃ߂�B
        TsVector3 vtLight;

        vtLight = m_lightDir;

        // ���������ʂ��v�Z����B
        lightSrcFace.Init( m_lightDir , m_eyePostion , m_sceneBoundingBox );

        // �������W�n�֕ϊ����Ă����B
        lightSrcFace.Transform( m_viewMatrix * mtxViewLight );
    }

    {

        TsLisPSM_ViewVolume viewVolume;
        {
            // ���쐎�̊p�̍��W�����[���h���W�n�Ŏ擾
            viewVolume.Init( m_viewProjectionMatrix , m_viewMatrix );

            // �N���b�v���v�Z�B
            // ����(Y��)�͈݂͂̂��N���b�v����悤�ɂ���B
            viewVolume.Clip( m_sceneBoundingBox , 0x02 );
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
            TsLisPSM_ViewVolume vv( viewVolume );
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
        TsLisPSM_ViewVolume lsViewVolume( viewVolume );
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
        TsLisPSM_ViewVolume lsExtViewVolume( lsViewVolume );
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
    m_viewDir = TsVector3( m_viewMatrix._31 , m_viewMatrix._32 , m_viewMatrix._33 );

    // Compute Light Space Perspective Shadow Map
    ComputeLisPSM();

    m_viewMatrix = TsMatrix::CreateLookAt( -m_lightDir *20 , TsVector3::zero , TsVector3::front );
    m_lightProjectionMatrix = TsMatrix::CreateOrtho( 1980/32, 1024/32, 3 , 50 );

    m_lVPMatrix = m_viewMatrix * m_lightProjectionMatrix;
    return TS_TRUE;
}

TsBool TsComputeLisPSM::SetEyePos(const TsVector3& value )
{
    m_eyePostion = value;
    return TS_TRUE;
}

TsBool TsComputeLisPSM::SetLightDir(const TsVector3& value )
{
    m_lightDir = value;
    return TS_TRUE;
}

TsBool TsComputeLisPSM::SetProjection(const TsMatrix& value )
{
    m_viewProjectionMatrix = value;

    TsF32 a = m_viewProjectionMatrix._33;
    TsF32 b = m_viewProjectionMatrix._43;

    m_near = -b / fabsf( a );
    m_far  =  b / (1.0f - fabsf( a ));

    return TS_TRUE;
}

TsBool TsComputeLisPSM::SetSceneBoundingBox(const TsAABB3D& aabb)
{
    m_sceneBoundingBox = aabb;
    return TS_TRUE;
}

TsBool TsComputeLisPSM::SetViewMatrix( const TsMatrix& value )
{
    m_viewMatrix = value;
    return TS_TRUE;
}

TsMatrix TsComputeLisPSM::GetLVMatrix()const
{
    return m_lightViewMatrix;
}
TsMatrix TsComputeLisPSM::GetLPMatrix()const
{
    return m_lightProjectionMatrix;
}
TsMatrix TsComputeLisPSM::GetLVPMatrix()const
{
    return m_lVPMatrix;
}