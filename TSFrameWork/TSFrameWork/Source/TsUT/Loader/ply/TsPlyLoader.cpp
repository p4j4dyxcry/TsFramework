#include "../../TsUT.h"
#include <fstream>
#include <sstream>
struct PLY_PROPERY
{
    enum Format
    {
        Unknown,

        Signed8 ,
        Signed16,
        Signed32,
        Signed64,

        UnSigned8,
        UnSigned16,
        UnSigned32,
        UnSigned64,

        Float32,
        Float64,
    };

    Format ToFormat(TsString& t)
    {
             if (t == "int8" || t == "char")        return Signed8;
        else if (t == "uint8" || t == "uchar")      return UnSigned8;
        else if (t == "int16" || t == "short")      return Signed16;
        else if (t == "uint16" || t == "ushort")    return UnSigned16;
        else if (t == "int32" || t == "int")        return Signed32;
        else if (t == "uint32" || t == "uint")      return UnSigned32;
        else if (t == "float32" || t == "float")    return Float32;
        else if (t == "float64" || t == "double")   return Float64;
        return Unknown;
    }

    size_t size()const
    {
        switch (propType)
        {
        case Signed8:
        case UnSigned8:
            return sizeof(char);
            break;

        case Signed16:
        case UnSigned16:
            return sizeof(short);
            break;

        case Signed32:
        case UnSigned32:
        case Float32:
            return sizeof(float);
            break;

        case Signed64:
        case UnSigned64:
        case Float64:
            return sizeof(long long);
            break;
        default:
            break;
        }
        return 0;
    }

    TsString name;
    PLY_PROPERY::Format propType;
    PLY_PROPERY::Format listType;
    TsBool isList = TS_FALSE;

    PLY_PROPERY(std::istream & is)
    {
        TsString type;
        is >> type;
        if (type == "list")
        {
            TsString countType;
            is >> countType >> type;
            listType = ToFormat(countType);
            isList = TS_TRUE;
        }
        propType = ToFormat(type);
        is >> name;
    }

    const TsBool operator == (const TsString& name)
    {
        return this->name == name;
    }
};

struct PLY_ELEMENT
{

    enum ElementType
    {
        Unknown,
        Vertex,
        Face,
        Edge
    };

    TsUint   count;
    TsUint   elementSize;
    TsString name;
    std::vector<PLY_PROPERY> m_property;

    PLY_ELEMENT(std::istream & is)
    {
        is >> name >> count;
    }
    ~PLY_ELEMENT()
    {
        TsSafeDelete(pChunk);
    }
    void Analize()
    {
             if (name == "vertex") type = Vertex;
        else if (name == "face") type = Face;
        else if (name == "edge") type = Edge;

        switch (type)
        {
        case Vertex:
            pChunk = TsNew( TsU8[ sizeof(PLY_VERTEX) * count]); break;
        case Face:
            pChunk = TsNew(TsU8[ sizeof(PLY_FACE) * count]); break;
        case Edge:
            pChunk = TsNew(TsU8[sizeof(PLY_EDGE) * count]); break;
        }
    }

    TsBool LoadChunk(std::istream & is)
    {
    }

    TsBool LoadVertexChunk_Ascii(std::istream& is)
    {
        m_property[0].size();
    }

    TsBool LoadFaceChunk(std::istream& is)
    {

    }

    TsBool LoadEdgeChunk(std::istream& is)
    {

    }
    
    ElementType type = Unknown;
    PLY_PROPERY* FindPropery(const TsString& name)
    {
        auto& it = std::find(m_property.begin(), m_property.end(), name);
        if (it == m_property.end())
            return nullptr;
        return &(*it);
    }
    void * pChunk = nullptr;

};

struct PLY_FILE
{
    TsBool isBinary = TS_FALSE;
    TsBool isBigEndian = TS_FALSE;
    std::vector<PLY_ELEMENT> elements;

    PLY_FILE(std::istream& is)
    {
        AnalizeHeader(is);
    }

    TsBool AnalizeHeader(std::istream& is)
    {
        auto ReadFormat=[this](std::istream& is)
        {
            std::string s;
            (is >> s);
            if (s == "binary_little_endian") isBinary = true;
            else if (s == "binary_big_endian") isBinary = isBigEndian = true;
        };

        auto ReadElement = [this](std::istream& is)
        {
            elements.emplace_back(is);
        };

        auto ReadPropery = [this](std::istream& is)
        {
            elements.back().m_property.emplace_back(is);
        };

        std::string line;
        while ( std::getline(is, line) )
        {
            std::istringstream ls(line);
            std::string token;
            ls >> token;
            token = TSUT::StringToLower(token);
            if (token == "ply" || token == "")  continue;
            else if (token == "comment")        continue;
            else if (token == "obj_info")       continue;
            else if (token == "end_header")     break;
            else if (token == "format")         ReadFormat(ls);
            else if (token == "element")        ReadElement(ls);
            else if (token == "property")       ReadPropery(ls);
            else return TS_FALSE;
        }
    }
};

TsBool TsPlyLoader::LoadFile(const TsChar * filename)
{
    std::ifstream ifs(filename);

    if (ifs.fail())
        return TS_FALSE;

    PLY_FILE file(ifs);

}

TsBool TsPlyLoader::AnalizeHeader(std::ofstream& ofs)
{
    return TS_TRUE;
}