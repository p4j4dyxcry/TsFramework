//-------------------------------------------------------------------------
//
// �p�[�e�B�N���̏����t���[�ɂ���
// �ECPU����G�~�b�^�������
// �� ComputeShader�Ńp�[�e�B�N���̃G�~�b�g
//    ->structuredBuffer�ɃG�~�b�^�����i�[
//    ->RWStructuredBuffer�ɗ��q�����o��
// ��ComputeShader�Ńp�[�e�B�N���̃A�b�v�f�[�g
//    ->RWStructuredBuffer���痱�q�������
//    ->RWStructuredBuffer�ɗ��q�����o��
// ��VertexShader��RWstructuredBuffer��structuredBuffer
//  �Ƃ���GeometryShader�Ƀf�[�^�𗬂�
// ��GeometoryShader�œ��͂��ꂽ���_��4�ɕϊ���View��Projection�ϊ����s��
// ��PielShader�ŏo��
//-------------------------------------------------------------------------

//! float1�̃G�~�b�g�p�����[�^
struct float1EmitterParam
{
    float start;        //�J�n
    float randomRange;  //�U�ꕝ
    float end;          //�I��

    // �o�C�g�A���C�����g����
    float dumy;
};

//! float2�̃G�~�b�g�p�����[�^
struct float2EmitterParam
{
    float2 start;       //�J�n
    float2 randomRange; //�U�ꕝ
    float2 end;         //�I��

    // �o�C�g�A���C�����g����
    float2 dumy;
};

//! float3�̃G�~�b�g�p�����[�^
struct float3EmitterParam
{
    float3 start;       //�J�n
    float3 randomRange; //�U�ꕝ
    float3 end;         //�I��

    // �o�C�g�A���C�����g����
    float3 dumy;
};

//! float4�̃G�~�b�g�p�����[�^
struct float4EmitterParam
{
    float4 start;       //�J�n
    float4 randomRange; //�U�ꕝ
    float4 end;         //�I��

    float4 dumy;
};

//---------------------------------------------------------------
//  *ReadMe
//  �p�[�e�B�N���G�~�b�^�ł�EmitterParam�̏�����͂��邱�ƂŃA�j���[�g
//  ���T�|�[�g���܂��B�Ȃ��A�A�j���[�g���s�Ȃ�Ȃ��p�����[�^��EmitterParam
//  ��start��end�𓯒l�ɂ��邱�ƂŃA�j���[�g�𖳌��ɂł��܂��B
//  �A�j���[�g�̌v�Z���͉��L�ł���킳��܂��B
//
//  �yt�z= ���K���c�胉�C�t �y�}��z�J�nlife ��10�Ŏc��life��3 �̂Ƃ���0.3f
//  �ys�z= EmitterParam.start + RandomValue
//�@ strat*t + end(1-t)
//---------------------------------------------------------------
//! �p�[�e�B�N���G�~�b�^�[
//---------------------------------------------------------------
struct ParticleEmitter
{
    //! �p�[�e�B�N���̃G�~�b�^�̈ʒu
    float3EmitterParam m_position;

    //! �p�[�e�B�N���̃G�~�b�^���a
    float1EmitterParam m_emitRadius;

    //! �p�[�e�B�N���̃G�~�b�^�F
    float4EmitterParam m_emitColor;

    //! �p�[�e�B�N���̃G�~�b�^�̏���
    float3EmitterParam m_emitVelocity;

    //! �G�~�b�g��
    float1EmitterParam m_emitNum;

    //! �p�[�e�B�N�����C�t
    float1EmitterParam m_emitLife;

    //! �g�p���郁�����̊J�n�C���f�b�N�X
    uint m_emitMemoryBegin;

    //! ����A�I���C���f�b�N�X
    uint m_emitMemoryEnd;

    //! ���ˊԊu(sec)
    float m_emitInterval;

    //! �o�ߎ���
    float m_elasedTime;

    //! �G�~�b�^�̗����̎�
    float m_emitRandomSeed;

    // �o�C�g�A���C�����g����
    float3 dumy;
};

//---------------------------------------------------------------
//! �p�[�e�B�N���\����
//---------------------------------------------------------------
struct Particle
{
    //! �ʒu�����肷��v�f
    float3  position;
    float3  velocity;
    float   gravity;

    //! �傫�������肷��v�f
    float   radius;
    float   addtionalradius;

    //! ���������肷��v�f
    float   life;
    float   maxLife;

    //! �F�����肷��v�f
    float4  color;
    float4  addtionalcolor;

    // �o�C�g�A���C�����g����
    float   dumy;
};

//---------------------------------------------------------------
//! ������ύX����ۂɂ͂��̊֐������������܂��B
//---------------------------------------------------------------
float ParticleRand(float3 value ,float seed)
{
    return frac(cos(dot(value ,float3(428.1529f,582.952f,94.00425f)) + seed) * 5343.3414f);
}



//---------------------------------------------------------------
//! �d�͌v�Z��ύX����ۂɂ͂��̊֐������������܂��B
//---------------------------------------------------------------
float ComputeGravity(Particle particle)
{
    //todo : test
    return particle.gravity * 1.08f;
    //return t * t * (3.0f - 2.0f + t);
}

//---------------------------------------------------------------
//! �⊮����ύX����ۂɂ͂��̊֐������������܂��B
//---------------------------------------------------------------
float ComputeTFunc( float t )
{
    //todo : test
    return t;
    //return t * t * (3.0f - 2.0f + t);
}

//---------------------------------------------------------------
//! 1�t���[���ł̕ω��ʂ��v�Z���܂��B
//---------------------------------------------------------------
float ToAdditional(float start, float1EmitterParam value, float life)
{
    return lerp(start, value.end, ComputeTFunc(1.0f / life));
}

float2 ToAdditional(float2 start, float2EmitterParam value, float life)
{
    return lerp(start, value.end, ComputeTFunc(1.0f / life));
}

float3 ToAdditional(float3 start, float3EmitterParam value, float life)
{
    return lerp(start, value.end, ComputeTFunc(1.0f / life));
}

float4 ToAdditional(float4 start , float4EmitterParam value, float life)
{
    return lerp(start, value.end, ComputeTFunc(1.0f / life));
}

float RandomInitialize(float3 value, float1EmitterParam emitterParam, float seed)
{
    return emitterParam.start +
           emitterParam.randomRange *
           ParticleRand(value, seed);
}

float2 RandomInitialize(float3 value, float2EmitterParam emitterParam, float seed)
{
    return emitterParam.start +
           emitterParam.randomRange *
           ParticleRand(value, seed);
}

float3 RandomInitialize(float3 value, float3EmitterParam emitterParam, float seed)
{
    return emitterParam.start +
           emitterParam.randomRange *
           ParticleRand(value, seed);
}

float4 RandomInitialize(float3 value, float4EmitterParam emitterParam, float seed)
{
    return emitterParam.start +
           emitterParam.randomRange *
           ParticleRand(value, seed);
}

//---------------------------------------------------------------
//! ���_�V�F�[�_�̓���
//---------------------------------------------------------------
struct PARTICLE_VS_IN
{
    uint InstanceID : SV_InstanceID;
};

//---------------------------------------------------------------
//! ���_�V�F�[�_�̏o�� & �W�I���g���V�F�[�_�̓���
//---------------------------------------------------------------
struct PARTICLE_VS_OUT
{
    float4 position : SV_Position;
    float4 color    : COLOR0;
    float  size     : TEXCOORD0;
};
#define PARTICLE_GS_IN PARTICLE_VS_OUT

//---------------------------------------------------------------
//! �W�I���g���V�F�[�_�̏o�� & �s�N�Z���V�F�[�_�̓��� 
//---------------------------------------------------------------
struct PARTICLE_GS_OUT
{
    float4 position : SV_Position;
    float4 color    : COLOR0;
    float2 uv       : TEXCOORD0;
};
#define PARTICLE_PS_IN PARTICLE_GS_OUT

//---------------------------------------------------------------
//! �s�N�Z���V�F�[�_�̏o��
//---------------------------------------------------------------
struct PARTICLE_PS_OUT
{
    float4 color : SV_Target0;
};
