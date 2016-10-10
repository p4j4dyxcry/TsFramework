#pragma once

class TsBoneTransForm : public TsTransForm
{
public:
    virtual TsMatrix ToBoneMatrix( const TsMatrix& worldMatrix );
    TsBool SetBasePoseInv( const TsMatrix& inv)
    {
        m_basePoseInv = inv;
        return TS_TRUE;
    }
    //! ƒNƒ‰ƒX–¼‚ðŽæ“¾‚·‚é
    virtual TsString GetClassTypeName()const
    {
        return "TsBoneTransForm";
    }
    TsBool SetID( TsInt id )
    {
        m_id = id;
        return TS_TRUE;
    };
    TsInt GetID()const
    {
        return m_id;
    }

protected:
    TsMatrix m_basePoseInv;
    TsBool IsParentBone()const;
    TsInt m_id;
};