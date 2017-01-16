#include "../../TsUT.h"
#include <fstream>
#include <sstream>
struct PLY_PROPERY
{
    enum Format
    {
        Signed8 = 8,
        Signed16 = 16,
        Signed32 = 32,
        Signed64 = 64,

        UnSigned8 = 8,
        UnSigned16 = 16,
        UnSigned32 = 32,
        UnSigned64 = 64,

        Float32 = 32,
        Float64 = 64,

        Unknown,
    };

    TsString name;
    PLY_PROPERY::Format format;

    const TsBool operator == (const TsString& name)
    {
        return this->name == name;
    }
};

struct PLY_ELEMENT
{
    TsUint   count;
    TsUint   elementSize;
    TsString name;
    std::vector<PLY_PROPERY> m_property;

    PLY_PROPERY FindPropery(const TsString& name)
    {
        return *std::find(m_property.begin(), m_property.end(), name);
    }

};

auto ConvertPLYFormat = [](const TsString& str)
{
    if (str == "float")
        return PLY_PROPERY::Float32;
    if (str == "double")
        return PLY_PROPERY::Float64;
    if (str == "int")
        return PLY_PROPERY::Signed32;
    if (str == "char")
        return PLY_PROPERY::Signed8;
    if (str == "uchar")
        return PLY_PROPERY::UnSigned8;

    return PLY_PROPERY::Unknown;
};

TsBool TsPlyLoader::LoadFile(const TsChar * filename)
{
    std::ifstream ifs(filename);

    if (ifs.fail())
        return TS_FALSE;

    
    TsVector<PLY_ELEMENT> elements;

    //------------------------------------------------
    //! Analize Header
    //------------------------------------------------
    while (TS_TRUE)
    {
        TsString temp;
        std::getline(ifs, temp);

        if (temp.find("element ") == 0)
        {
            PLY_ELEMENT elm;
            TsChar name[64] = "";
            sscanf_s(temp.c_str(),
                "element %s %d",
                name,
                &elm.count);
            elm.name = name;
            elements.push_back(elm);
        }

        if (temp.find("property ") == 0)
        {
            if (elements.size() == 0)
                return TS_FALSE;

            PLY_ELEMENT& elm = elements[elements.size() - 1];

            PLY_PROPERY prop;

            TsChar format[64] = "";
            TsChar name[64] = "";
            sscanf_s(temp.c_str(),
                "property %s %s",
                format,
                name);

            prop.name = name;
            prop.format = ConvertPLYFormat( format );

            elm.m_property.push_back(prop);
        }
    }
}

TsBool TsPlyLoader::AnalizeHeader(std::ofstream& ofs)
{
    return TS_TRUE;
}