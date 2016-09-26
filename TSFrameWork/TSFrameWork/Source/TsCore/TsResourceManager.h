//todo

#pragma once
class TsResourceManager
{
public:
	TsBool LoadShaders( TsDevice* pDev )
	{
		TSUT::TsFileDirectory shaderDir( TSUT::Resource::GetCSODirectory() );
		shaderDir.SetSearchSubFolderFlag( TS_TRUE );
		shaderDir.AddExtension( TsString(".cso") );

		TsList<TsString>&& fileList = shaderDir.GetFileList();
		TsBinary bin;
		for each( auto var in fileList )
		{
			if( var.find( "VS" ) )
			{
				auto vs = TsNew( TsVertexShader );
				vs->LoadFromCSO( pDev->GetDevD3D() , var.c_str() );
			}

			if( var.find( "PS" ) )
			{
				auto ps = TsNew( TsPixelShader );
				ps->LoadFromCSO( pDev->GetDevD3D() , var.c_str() );
			}
		}
		return TS_TRUE;
	}
private:

};