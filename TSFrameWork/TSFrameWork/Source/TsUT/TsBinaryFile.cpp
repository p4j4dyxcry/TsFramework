#include "TsUT.h"
#include <fstream>

//----------------------------------------------------------
//! Constructor
//----------------------------------------------------------
TsBinary::TsBinary() :
m_bin( nullptr ) ,
m_size( 0 )
{

};

//----------------------------------------------------------
//! Copy Constructor
//----------------------------------------------------------
TsBinary::TsBinary( const TsBinary& bin )
{
    *this = bin;
}

//----------------------------------------------------------
//! Constructor of Load
//----------------------------------------------------------
TsBinary::TsBinary( TsString filename )
{
    Load( filename );
}

//----------------------------------------------------------
//! Destructor
//----------------------------------------------------------
TsBinary::~TsBinary()
{
    TsSafeDelete( m_bin );
    m_size = 0;
}

//----------------------------------------------------------
//! Load Binary
//----------------------------------------------------------
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

//----------------------------------------------------------
//! Release Binary
//----------------------------------------------------------
void TsBinary::UnLoad()
{
    TsSafeDelete( m_bin );
}

//----------------------------------------------------------
//! Get Binary
//----------------------------------------------------------
TsByte* TsBinary::GetBinary()const
{
    return m_bin;
}

//----------------------------------------------------------
//! Get Binary Size
//----------------------------------------------------------
TsInt	TsBinary::GetSize()const
{
    return m_size;
}

//----------------------------------------------------------
//! Deep Copy
//----------------------------------------------------------
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

//----------------------------------------------------------
//! Seek To String
//----------------------------------------------------------
TsByte*	TsBinary::SeekString(	const TsChar* target,
                                TsInt startIndex ,		/*= 0 */
                                __out TsInt* outIndex	/*= nullptr */)
{
    if (startIndex > m_size)
        return nullptr;

    TsInt index = startIndex;

    auto Cmp = [&]()
    {
        TsInt i = index;
        TsInt j = 0;
        while (target[j] != '\0' &&
                m_bin[i] != '\0')
        {
            if (target[j] != m_bin[i])
                return TS_FALSE;
            j++;
            i++;
        }

        return TS_TRUE;
    };

    while ( m_bin[index] != '\0')
    {
        if ( Cmp() )
        {
            if ( outIndex )
                *outIndex = index;
            return &m_bin[index];
        }

        ++index;
    }
    return nullptr;
}

//----------------------------------------------------------
//! Seek To Number Value
//----------------------------------------------------------
TsByte* TsBinary::SeekNumber(	TsInt startIndex ,		/*= 0 */
                                __out TsInt* outIndex	/*= nullptr */	 )
{
    if (startIndex > m_size)
        return nullptr;

    TsInt index = startIndex;

    for (;  m_bin[index] < '0' ||
            m_bin[index] > '9' ||
            m_bin[index] != '-'; ++index) 
    {};

    if (outIndex)
        *outIndex = index;
    return &m_bin[index];

}