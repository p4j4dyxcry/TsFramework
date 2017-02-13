


/*
hlsl �p�� Utility �֐��Q

�ꕔ�֐���Define�}�N���ɂ��Ă���̂́Afloat4 float3 ���̊֐���
�I�[�o�[���C�h�Ŏ������Ȃ��Ă��悭�Ȃ邽�߁B

2016 Yuki Tsuneyama
*/

// ��
#define TS_PI (3.14159265358979323846f)

//! -1�`1��ԁ@�� 0�`1��Ԃɕϊ�
#define PackUnsigned( data ) \
    (data * 0.5f + 0.5f)

//! 0�`1��ԁ@�� -1�`1��Ԃɕϊ�
#define UnPackUnsigned( data )  \
    ((data - 0.5f) * 2)

//! 2���⊮
#define EaseIn( t ) \
    (t*t)

//! 2���⊮
#define EaseOut( t ) \
    (t*(2 - t))

//! 3���⊮
#define Cubic( t ) \
    (t * t * (3.0f - 2.0f + t))

//! 5���⊮
#define HexLerp \
    (t * t * t * (t * (6 * t - 15) + 10))

// ���j�A��Ԃɕϊ�
#define ToLinear( gamma ) \
    (pow(gamma, 1.0f / 2.2f))

// �K���}��Ԃɕϊ�
#define ToGamma( lin ) \
    (pow(lin, 2.2f))

// �g�[���}�b�s���O
#define Tone( color , exposure) \
    (1.0 - exp(color * exposure))

// uv ���烉���_���ȃX�J�����쐬����
float Rand(float2 uv)
{
    float a = frac(dot(uv, float2(2.067390879775102, 12.451168662908249))) - 0.5;
    float s = a * (6.182785114200511 + a * a * (-38.026512460676566 + a * a * 53.392573080032137));
    float t = frac(s * 43758.5453);
    return UnPackUnsigned(t);
}


// �P�x���o
float Luminance(float3 color)
{
    return dot(color, float3(0.298912f, 0.586611f, 0.114478f));
}

//! �e�N�X�`���T�C�Y�̎擾
float2 TexSize(Texture2D tex)
{
    float2 size;
    float  numMipMapLevel;
    tex.GetDimensions(0, size.x, size.y, numMipMapLevel);

    return size;
}

//! 4�_�T���v�����O
float4 Sample4(Texture2D tex,
    SamplerState samp,
    float2 texcoord)
{
    float2 size = 1.0f / TexSize(tex);
        float x = size.x;
    float y = size.y;

    float4 result = 0;

        result += tex.Sample(samp, texcoord + float2(x, 0));
    result += tex.Sample(samp, texcoord + float2(-x, 0));
    result += tex.Sample(samp, texcoord + float2(0, y));
    result += tex.Sample(samp, texcoord + float2(0, -y));

    const float invTotal = 1.0f / 5.0f;

    return result / invTotal;
}

//! 8�_�T���v�����O
float4 Sample8(Texture2D tex,
    SamplerState samp,
    float2 texcoord)
{
    float2 size = 1.0f / TexSize(tex);
        float x = size.x;
    float y = size.y;
    float4 result = 0;
        result += tex.Sample(samp, texcoord);

    result += tex.Sample(samp, texcoord + float2(x, 0));
    result += tex.Sample(samp, texcoord + float2(-x, 0));
    result += tex.Sample(samp, texcoord + float2(0, y));
    result += tex.Sample(samp, texcoord + float2(0, -y));

    result += tex.Sample(samp, texcoord + float2(x, y));
    result += tex.Sample(samp, texcoord + float2(-x, y));
    result += tex.Sample(samp, texcoord + float2(x, y));
    result += tex.Sample(samp, texcoord + float2(-x, -y));

    const float invTotal = 1.0f / 9.0f;

    return result / invTotal;
}