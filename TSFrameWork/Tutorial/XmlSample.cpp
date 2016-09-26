/*
	�`���[�g���A���@XML ���[�_�[�̎g����

*/

#include "../TSFrameWork/Source/TsUT/TsUT.h"
#include "../TSFrameWork/Source/TsGfx/TsGfx.h"
#include "../TSFrameWork/Source/TsUT/TsXML.h"
#include "../TSFrameWork/Source/TsOS/TsWindowsUtility.h"
/*
	<!----test.xml---->


	<?xml version="1.0" encoding="utf-8"?>
	<material>
	  <dif value="21 1 71 3"/>
	  <spc value="1 6 1 71"/>
	  <emi value="3 1 10 91"/>
	</material>
*/

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE 	hPrevInstance, LPSTR lpszArgs, TsInt nWinMode)
{
	// windows �̏�����
	TsWINSetWindowTitle("Tutorial Xml Sample");
	TsWINCreateMainWindow(hInstance, nWinMode,TsInt2(800,600));

	//�R���\�[���̏�����
	TSUT::TsLoggerInit();

	// xml �̓ǂݍ���
	TsXML xml("test.xml");

	// xml�@����ŏ��Ɍ�������material�G�������g���擾
	TsXMLElement* material = xml.FindFirst("material");

	// material�G�������g���� dif �G�������g���擾
	TsXMLElement* dif = material->FindChild("dif");
	
	// dif�G�������g����A�g���r���[�g���擾
	TsXMLAttribute* at = dif->GetAttribute("value");

	// attribute����float4���擾
	TsFloat4 data = at->GetFloat4Value();

	// dif = 21.00f 1.00f 71.00f 3.00f
	printf("%s = %2.2f,%2.2f,%2.2f,%2.2f", dif->GetName().c_str(), data[0], data[1], data[2], data[3]);

	return 0;
}