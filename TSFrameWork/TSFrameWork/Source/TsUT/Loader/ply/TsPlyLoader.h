
#pragma once

struct PLY_VERTEX
{
    TsVector3 pos;
    TsFloat3  normal;
    TsFloat4  color;
};

struct PLY_EDGE
{
    TsInt     vertexID[2];
    TsFloat4  color;
};


struct PLY_FACE
{
    TsInt       vertexID[8];
    TsVector2   uv[8];

    PLY_FACE()
    {
        memset(vertexID, -1, sizeof(TsInt) * 8);
    }

};

class TsPlyLoader : public Ts3DModelLoader
{
public:
    TsPlyLoader();
    virtual ~TsPlyLoader();
    TsBool LoadFile(const TsChar * filename)override;
    TsBool SaveFile(const TsChar* filename)override;

    virtual TsBool CreateCommonData()override;

protected:

    TsBool AnalizeHeader(std::ofstream& ofs);
    TsBool SaveHeader( const TsChar* filename );

    TsVector<PLY_VERTEX> m_vertex;
    TsVector<PLY_FACE>   m_face;
};