#include "../../TsAfx.h"


TsAABB::TsAABB()
{

}
TsAABB::TsAABB( const TsVector3& min ,
                const TsVector3& max )
{
    SetMin( min );
    SetMax( max );
}

TsAABB::TsAABB( const TsAABB & aabb)
{
    *this = aabb;
}

TsAABB::~TsAABB()
{

}

TsBool TsAABB::SetMin( const TsVector3& v )
{
    m_min = v;
    return TS_TRUE;
}

TsBool TsAABB::SetMax( const TsVector3& v )
{
    m_max = v;
    return TS_TRUE;
}

TsAABB TsAABB::Transform( const TsMatrix& matrix )
{
    m_min *= matrix;
    m_max *= matrix;

    return *this;
}

const TsVector3& TsAABB::GetMin()const
{
    return m_min;
}
const TsVector3& TsAABB::GetMax()const
{
    return m_max;
}

TsAABB TsAABB::operator * ( const TsMatrix& matrix )const
{
    TsAABB aabb = *this;
    return aabb.Transform( matrix );
}
TsAABB TsAABB::operator *= ( const TsMatrix& matrix )
{
    return Transform(matrix);
}

TsAABB TsAABB::operator = ( const TsAABB& aabb )
{
    SetMin( aabb.m_min );
    SetMax( aabb.m_max );
    return *this;
}
