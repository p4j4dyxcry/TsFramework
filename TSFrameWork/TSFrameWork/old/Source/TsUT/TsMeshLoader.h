#pragma once

class TsMeshLoader
{
public:
	TsMeshLoader();
	virtual ~TsMeshLoader();

	virtual TsBool LoadFromFile( TsString filename ) = 0;
protected:

private:
};