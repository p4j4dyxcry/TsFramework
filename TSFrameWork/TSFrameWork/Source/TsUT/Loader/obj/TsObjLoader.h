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

struct TsObjMaterialUse
{
    TsBool useAmbient  = TS_FALSE;
    TsBool useDiffuse  = TS_FALSE;
    TsBool useSpecluer = TS_FALSE;
    TsBool usePower = TS_FALSE;
    TsBool useAlpha = TS_FALSE;
    TsBool useLum = TS_FALSE;
    TsBool useSharpness = TS_FALSE;
    TsBool useNi = TS_FALSE;
    TsBool useNs = TS_FALSE;
    TsBool useTf = TS_FALSE;
};

// obj material
struct TsObjMaterial
{
    TsString name;
    TsString dir;
    TsObjMaterialUse usingMaterialParam;

    TsFloat3 ambient;  // Ka ambient
    TsFloat3 diffuse;  // Kd diffuse
    TsFloat3 specluer; // Ks Specluer
    TsF32    power = 0;    // Ns Specluer Power

    TsF32    alpha = 1;    //0 ~ 1 通常は d で定義されるが Tr(反転)も存在する
    TsF32    lum = 0;      // illum 輝度
    TsF32    sharpness = 60;// 0〜1000　デフォルト60 ,100を超えると不正な結果になることがある
                       //値が大きいほどレフレクションマップのオブジェクトが明確に表示される

    TsF32    Ni = 0;        //光の屈折率　0.001~10で定義される 1以下の値は推奨されない
    TsF32    Ns = 0;        //鏡面指数  0〜1000 指数が高いと、タイトで集中したハイライト
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
    
    TsObjFace() :
        idx_pos(-1, -1, -1),
        idx_normal(-1, -1, -1),
        idx_texcoord(-1, -1, -1){};
};

struct TsObjMesh
{
    std::vector<TsObjFace> m_face;
    TsString  material_name;
    TsString  name;
};

class TsOBJLoader : public Ts3DModelLoader
{
public:
    TsOBJLoader();
    virtual ~TsOBJLoader(); 
    TsBool LoadFile( const TsChar * filename )override;
    TsBool SaveFile( const TsChar* filename )override;

    virtual TsBool CreateCommonData()override;

protected:

    TsBool LoadObj(const char* filename);
    TsBool LoadMtl(const char* filename);

    TsBool SaveMaterial(const char* filename);

    TsVector<TsObjMaterial> m_materialList;
    TsVector<TsVector3>     m_posList;
    TsVector<TsVector3>     m_normalList;
    TsVector<TsVector2>     m_texcoordList;
    TsVector<TsObjMesh>     m_objMesh;
};