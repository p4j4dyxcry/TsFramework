#pragma once

// mqo Material
struct TsMqoMaterial
{
	TsFloat4 color;
	TsF32 diffuse;
	TsF32 ambient;
	TsF32 emissib;
	TsF32 specular;
	TsF32 specularPower;
	TsString textureName;
};

// mqo vertex
struct TsMqoVertex
{
	TsFloat3 pos;
	TsFloat3 normal;
	TsFloat2 uv;
};

// mqo face
struct TsMqoFace
{
	TsInt3 index;		// triangle indexs
	TsInt  m_matIndex;	// material index
	TsFloat2 uv[3];		// triangle uv
	TsFloat3 normal;	// 法線
	TsF32 angle;		// 傾き
	TsF32 area;			// 面積
};

struct TsMqoMesh
{
	TsVector<TsMqoVertex> vertex;
	TsMqoMaterial material;
};

struct TsMqoObject
{
	TsVector<TsMqoMesh> mesh;
};

class TsMqoLoader : public TsMeshLoader
{
public :
	TsMqoLoader();
	virtual ~TsMqoLoader();
	TsBool LoadFromFile( TsString filename )override;
	TsBool LoadFromMemory( void * memory , size_t sz )override;
	virtual TsInt  GetMeshNum() override;
	virtual TsInt  GetVertexSize( TsInt index ) override;
	virtual void*  GetVertexBuffer( TsInt index ) override;
	virtual size_t GetVertexStride() override;
	
	TsInt GetMaterialNum()const;
	TsString GetTexturePass(TsInt index)const;
		

	//==========================================================
	//	!SetScale
	//	メッシュの読み込み時に単位をメートル座標に変換します。
	//	True	メートル(デフォルト)	
	//	False	センチ
	//	※このメソッドはロードを行う前にコールする必要があります
	void SetCenti( TsBool flg);

	//==========================================================
	//	!SetSmooth
	//	メッシュの読み込み時に法線を計算しスムージングします。
	//	※このメソッドはロードを行う前にコールする必要があります
	void SetSmooth( TsBool flg );
protected:
	TsVector<TsMqoMaterial> LoadMaterial( std::ifstream& ifs );
	TsVector<TsMqoFace> LoadObject(TsVector<TsFloat3>& pos, std::ifstream & ifs );
	
	//! マテリアル情報を取得
	template <typename T> 
	TsVector<T> PickOutMaterial( TsString file , TsString id ,TsInt num);	

private:
	TsMqoObject* m_mqoObject;
	TsF32  m_loadScale;
	TsBool m_isSmooth;
};

