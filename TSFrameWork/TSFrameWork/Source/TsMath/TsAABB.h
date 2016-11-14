//**********************************************************
//! TsAABB.h
//! Bounding Box.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

//=========================================================================
//! TsAABB 
//=========================================================================
class TsAABB
{
public:
    //=========================================================================
    //! public method
    //=========================================================================

    //! Constructor
    TsAABB();
    TsAABB( const TsAABB& );
    TsAABB( const TsVector3& min ,
            const TsVector3& max );


    //! Destructor
    virtual ~TsAABB();

    TsBool SetMin( const TsVector3& );
    TsBool SetMax( const TsVector3& );
    TsAABB Transform( const TsMatrix& );
    const TsVector3& GetMin()const;
    const TsVector3& GetMax()const;

    //! operator
    TsAABB operator * ( const TsMatrix& matrix )const;
    TsAABB operator *= ( const TsMatrix& matrix );
    TsAABB operator = ( const TsAABB& aabb );

    TsBool Collision( const TsAABB& );
    TsBool Collision( const TsVector3& );

private:
    //=========================================================================
    //! propery
    //=========================================================================
    TsVector3 m_min;
    TsVector3 m_max;
};