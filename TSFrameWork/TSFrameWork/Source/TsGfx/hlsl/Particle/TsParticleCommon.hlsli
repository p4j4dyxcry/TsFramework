
//! float1�̃G�~�b�g�p�����[�^
struct float1EmitterParam
{
    float start;        //�J�n
    float randomRange;  //�U�ꕝ
    float end;          //�I��

    float dumy;
};

//! float2�̃G�~�b�g�p�����[�^
struct float2EmitterParam
{
    float2 start;       //�J�n
    float2 randomRange; //�U�ꕝ
    float2 end;         //�I��

    float2 dumy;
};

//! float3�̃G�~�b�g�p�����[�^
struct float3EmitterParam
{
    float3 start;       //�J�n
    float3 randomRange; //�U�ꕝ
    float3 end;         //�I��

    float3 dumy;
};

//! float4�̃G�~�b�g�p�����[�^
struct float4EmitterParam
{
    float4 start;       //�J�n
    float4 randomRange; //�U�ꕝ
    float4 end;         //�I��
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

    float3 dumy;
};

//---------------------------------------------------------------
//! �p�[�e�B�N���\����
//---------------------------------------------------------------
struct Particle
{
    float3  position;
    float   radius;
    float3  velocity;
    float   life;
    float   maxLife;
    float3  gravity;
    float4  color;
    float4  addtionalcolor;
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
float3 ComputeGravity(Particle particle)
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

float ToAdditional(float1EmitterParam value , float life)
{
    return lerp(value.start, value.end, ComputeTFunc(1.0f / life));
}

float2 ToAdditional(float2EmitterParam value, float life)
{
    return lerp(value.start, value.end, ComputeTFunc(1.0f / life));
}

float3 ToAdditional(float3EmitterParam value, float life)
{
    return lerp(value.start, value.end, ComputeTFunc(1.0f / life));
}

float4 ToAdditional(float4EmitterParam value, float life)
{
    return lerp(value.start, value.end, ComputeTFunc(1.0f / life));
}



