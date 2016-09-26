//!*******************************************************
//!	TsBinaryFile.h
//!
//!	Binary Loader.
//!
//!	©	2016 Yuki Tsuneyama

class TsBinary : public TsNameObject
{
public:

	//! Constructor
	TsBinary();
	TsBinary( const TsBinary& bin );
	TsBinary( TsString filename );

	//! Destructor
	virtual~TsBinary();

	//=================================================
	// Load
	// バイナリファイルを読み込む
	// @pram filename	読み込み対象のバイナリのファイルパス
	// @return true is Success
	TsBool Load( TsString filename );

	//=================================================
	// UnLoad
	// バイナリファイルを開放する。このメソッドは明示的に呼ぶ
	//　必要はない。
	// @pram filename	読み込み対象のバイナリのファイルパス
	// @return true is Success
	void UnLoad();

	//=================================================
	// GetBinary
	// バイナリソースデータを取得する
	// @return Binary Pointer
	TsByte* GetBinary()const;

	//=================================================
	// GetBinary
	// バイナリソースデータを取得する
	// @return Binary Pointer
	TsInt	GetSize()const;

	//=================================================
	// コピーを行う際は新たなメモリの確保が行われる。
	TsBinary operator = ( const TsBinary& bin );

	//Seek
	TsByte*	SeekString( const TsChar* target, 
						TsInt startIndex = 0 ,
						__out TsInt* outIndex = nullptr);

	TsByte* SeekNumber( TsInt startIndex = 0 ,
						__out TsInt* outIndex = nullptr );
protected:
	TsByte * m_bin;
	TsInt    m_size;
};