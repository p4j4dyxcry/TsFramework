#include "../../../../TsAfx.h"

Ts3DModelLoader::Ts3DModelLoader()
{

}

Ts3DModelLoader::~Ts3DModelLoader()
{

}

TsBool Ts3DModelLoader::LoadFile( const TsChar * filename )
{
    SetName( filename );

    return TS_TRUE;
}

TsBool Ts3DModelLoader::SaveFile( const TsChar * filename )
{
    return TS_FALSE;
}