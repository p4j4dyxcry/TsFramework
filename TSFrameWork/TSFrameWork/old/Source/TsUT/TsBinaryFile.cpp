#include "TsUT.h"
#include <fstream>
TsBinary::TsBinary() :
m_bin( nullptr ) ,
m_size( 0 )
{

};

//! constructor
TsBinary::TsBinary( const TsBinary& bin )
{
	*this = bin;
}

//! constructor
TsBinary::TsBinary( TsString filename )
{
	Load( filename );
}

//! destructor
TsBinary::~TsBinary()
{
	TsSafeDelete( m_bin );
	m_size = 0;
}

// load binary
TsBool TsBinary::Load( TsString fileName )
{
	UnLoad();

	std::ifstream ifs( fileName , std::ifstream::in | std::ifstream::binary );

	if( ifs.fail() )
	{
		TsDebugLog( "Load Fail \n\t %s\n" , fileName.c_str() );
		return TS_FALSE;
	}
	TsInt begin = static_cast<TsInt>( ifs.tellg() );
	ifs.seekg( 0 , ifs.end );

	TsInt end = static_cast<TsInt>( ifs.tellg() );
	m_size = (end - begin);
	ifs.clear();
	ifs.seekg( 0 , ifs.beg );
	m_bin = TsNew(TsByte[m_size]);
	//m_bin[m_size - 1] = '\0';  
	ifs.read( (TsS8*)m_bin , m_size);

	return TS_TRUE;
}

void TsBinary::UnLoad()
{
	TsSafeDelete( m_bin );
}
TsByte* TsBinary::GetBinary()const
{
	return m_bin;
}
TsInt	TsBinary::GetSize()const
{
	return m_size;
}

TsBinary TsBinary::operator=( const TsBinary& bin )
{
	m_size = bin.GetSize();
	if( m_size > 0 )
	{
		m_bin = TsNew( TsByte[m_size] );
		memcpy_s( &m_bin , m_size , bin.GetBinary() , m_size );
	}
	else
		m_bin = nullptr;

	return *this;
}

