#pragma once

struct TsObjTextureOP
{
    TsBool   blend_u = TS_TRUE; //水平テクスチャの混合
    TsBool   blend_v = TS_TRUE; //垂直テクスチャの混合
    TsF32    boost   = 0;       // mip - map　のシャープさを押し上げ
    TsFloat2 mm = TsFloat2(0, 1);

    TsFloat3 offset;
    TsFloat3 scale = TsFloat3(1, 1, 1);
    TsFloat3 turbulence;

    TsFloat2 texResolution;
    TsBool   clamp = TS_FALSE;
    TsF32    bumpParam = 1;

};
// obj material
struct TsObjMaterial
{
    TsString name;

    TsFloat3 ambient;  // Ka ambient
    TsFloat3 diffuse;  // Kd diffuse
    TsFloat3 specluer; // Ks Specluer
    TsF32    power;    // Ns Specluer Power

    TsF32    alpha;    //0 ~ 1 通常は d で定義されるが Tr(反転)も存在する
    TsF32    lum;      // illum 輝度
    TsF32    sharpness;// 0〜1000　デフォルト60 ,100を超えると不正な結果になることがある
                       //値が大きいほどレフレクションマップのオブジェクトが明確に表示される

    TsF32    Ni;        //光の屈折率　0.001~10で定義される 1以下の値は推奨されない
    TsF32    Ns;        //鏡面指数  0〜1000 指数が高いと、タイトで集中したハイライト
    TsFloat3 Tf;        //スペクトル係数


    TsString diffuseMap; //map_Kd ディフューズマップ
    TsObjTextureOP KdTexOP;

    TsString ambientMap; //map_Kd ディフューズマップ
    TsObjTextureOP KaTexOP;

    TsString specluerMap; //map_Ks スペキュラマップ
    TsObjTextureOP KsTexOP;

    TsString hightLightMap; //map_Ns ハイライトマップ
    TsObjTextureOP NsTexOP;

    TsString alphaMap; //map_d  アルファマップ
    TsObjTextureOP AlphaTexOP;

    TsString bumpMap; //map_bump ノーマルマップ
    TsObjTextureOP BumpTexOP;
};

struct TsObjFace
{
    TsInt3 idx_pos;
    TsInt3 idx_normal;
    TsInt3 idx_texcoord;
    TsString  material_name;
    TsObjFace() :
        idx_pos(-1, -1, -1),
        idx_normal(-1, -1, -1),
        idx_texcoord(-1, -1, -1){};
};

class TsOBJLoader : public TsMeshLoader
{
public:
    TsOBJLoader();
    virtual ~TsOBJLoader();
    TsBool LoadFromFile(const TsString& filename, TsLoadOption& option = TsLoadOption())override;
    TsBool LoadFromMemory(void * memory, size_t sz)override;
    virtual TsInt  GetMeshNum() override;
    virtual TsInt  GetVertexSize(TsInt index) override;
    virtual void*  GetVertexBuffer(TsInt index) override;
    virtual size_t GetVertexStride() override;

    TsBool SaveFile(const char* filename);

protected:

    TsBool LoadObj(const char* filename);
    TsBool LoadMtl(const char* filename);

    TsVector<TsObjMaterial> m_materialList;
    TsVector<TsVector3>     m_posList;
    TsVector<TsVector3>     m_normalList;
    TsVector<TsVector2>     m_texcoordList;
    TsVector<TsObjFace>     m_faceList;
};