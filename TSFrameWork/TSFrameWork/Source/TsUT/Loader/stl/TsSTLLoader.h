#pragma once

// stl face
#pragma pack(1)
struct TsStlFace
{
    TsVector3 normal;
    TsVector3 pos[3];
    TsByte    reserve[2]; //データに含まれるダミー領域
};
#pragma pack()

class TsStlLoader : public Ts3DModelLoader
{
public:
    TsStlLoader();
    virtual ~TsStlLoader();
    TsBool LoadFile( const TsChar* filename )override;

    TsBool SaveBinary( const TsChar* filename );
    TsBool SaveAscii( const TsChar* filename );

protected:
    TsStlFace* m_faceList;
    TsUint      m_size;
    
    TsBool LoadFromAscii( const TsChar * filename );
    TsBool LoadFromBinary( const TsChar * filename );

    TsBool IsBinary( const TsChar* filename , TsInt loopNum = 256 );
};