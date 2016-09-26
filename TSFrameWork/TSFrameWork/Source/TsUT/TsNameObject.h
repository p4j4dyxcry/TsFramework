#pragma once

class TsNameObject
{
public:
	TsString GetName()const{ return m_name; }
	void SetName( TsString str ){
		m_name = str;
		m_hashCode = TSUT::StringToHash( str );
	}
	TS_HASH GetHashCode()const{ return m_hashCode; }
	TS_HASH GetTypeIDHash();
private:
	TsString m_name;
	TS_HASH m_hashCode;
};