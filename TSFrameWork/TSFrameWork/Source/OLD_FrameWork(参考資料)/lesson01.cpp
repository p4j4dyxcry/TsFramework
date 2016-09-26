//////////////////////////////////////////////////////////
//														//
// 	lesson01.cpp										//
// 		DirectX11�O���t�B�b�N�X������					//
//														//
//////////////////////////////////////////////////////////
#include <windows.h>

#include <stdio.h>
#include <process.h>	

#include "CDirectXGraphics.h" 

#include "Common.h"

// �}�N���̒�`
#define NAME	"win32A"
#define TITLE	"SP42 lesson01 DirectX11 ������ "

#define		SCREEN_X		((int)(1600.f * 0.75f))
#define		SCREEN_Y		((int)(900.f  * 0.75f))
#define		FULLSCREEN		0				// �t���X�N���[���t���O

// �����N���C�u�����̐ݒ�
#pragma comment(lib, "dxgi.lib")			// �����N�Ώۃ��C�u������dxgi.lib��ǉ�
#pragma comment(lib, "d3d11.lib")			// �����N�Ώۃ��C�u������d3d11.lib��ǉ�
#pragma comment(lib, "winmm.lib")			// �����N�Ώۃ��C�u������winmm.lib��ǉ�

// �v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);			// �E�C���h�E���b�Z�[�W�֐�
unsigned int   WINAPI GameMain(void  *);					// �Q�[�����C���֐��֐�
bool	GameInit(HINSTANCE ,HWND ,int ,int,bool );				// �Q�[���̏�������
void	GameExit();											// �Q�[���̏I������
void	CALLBACK TimerProc(UINT ,UINT,DWORD,DWORD,DWORD);	// �^�C�}����

// �O���[�o���ϐ�
CDirectXGraphics	g_pD3D11;			// �c�h�q�d�b�s�w�@�f�q�`�o�g�h�b�r�@�I�u�W�F�N�g		

// �Q�[�����C���X���b�h�֘A
HANDLE			g_hEventHandle;		// �C�x���g�n���h��
int				g_timerid=0;		// �^�C�}�h�c
bool			g_EndFlag=false;	// �I���t���O
HANDLE			g_hThread;			// �X���b�h�̃n���h���l
unsigned int	g_dwThreadId;		// �X���b�h�h�c

#include "SampleMesh.h"

//////////////////////////
// �v�h�m�l�`�h�m�֐�	//
//////////////////////////
int APIENTRY WinMain(HINSTANCE 	hInstance, 		// �A�v���P�[�V�����C���X�^���X�l
					 HINSTANCE 	hPrevInstance,	// �Ӗ��Ȃ�
					 LPSTR 		lpszArgs, 		// �N�����̈���������
					 int 		nWinMode)		// �E�C���h�E�\�����[�h
{
	HWND			hwnd;						// �E�C���h�E�n���h��
	MSG				msg;						// ���b�Z�[�W�\����
	WNDCLASSEX		wcex;						// �E�C���h�E�N���X�\����

	int				width;						// �E�C���h�E�̕�
	int				height;						// �E�C���h�E�̍���
	CoInitializeEx( nullptr , COINIT_MULTITHREADED );

	// �E�C���h�E�N���X���̃Z�b�g
	wcex.hInstance		= hInstance;			// �C���X�^���X�l�̃Z�b�g
	wcex.lpszClassName	= NAME;					// �N���X��
	wcex.lpfnWndProc	= (WNDPROC)WndProc;		// �E�C���h�E���b�Z�[�W�֐�
	wcex.style			= 0;					// �E�C���h�E�X�^�C��
	wcex.cbSize 		= sizeof(WNDCLASSEX);	// �\���̂̃T�C�Y
	wcex.hIcon			= LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);	// ���[�W�A�C�R��
	wcex.hIconSm		= LoadIcon((HINSTANCE)NULL, IDI_WINLOGO);		// �X���[���A�C�R��
	wcex.hCursor		= LoadCursor((HINSTANCE)NULL, IDC_ARROW);		// �J�[�\���X�^�C��
	wcex.lpszMenuName	= 0; 					// ���j���[�Ȃ�
	wcex.cbClsExtra		= 0;					// �G�L�X�g���Ȃ�
	wcex.cbWndExtra		= 0;					
	wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);		// �w�i�F��

	if (!RegisterClassEx(&wcex)) return FALSE;	// �E�C���h�E�N���X�̓o�^

	timeBeginPeriod(1);			// �^�C�}�̕���\�͂��P�����ɂ���
	g_timerid = timeSetEvent(16,1,TimerProc,1,TIME_PERIODIC);

	// �E�C���h�E�T�C�Y���v�Z
	if(FULLSCREEN){
		width = SCREEN_X;
		height = SCREEN_Y;
	}
	else{
		width = SCREEN_X+GetSystemMetrics(SM_CXDLGFRAME)*2;
		height = SCREEN_Y+GetSystemMetrics(SM_CYDLGFRAME)*2+GetSystemMetrics(SM_CYCAPTION);						// �E�C���h�E�T�C�Y
	}

	hwnd = CreateWindow(
				NAME, 							// �E�C���h�E�N���X�̖��O
				TITLE, 							// �^�C�g��
				WS_CAPTION | WS_SYSMENU,		// �E�C���h�E�X�^�C��
				100,100,						// X���W,Y���W
				width,height,					// �E�C���h�E�T�C�Y
				HWND_DESKTOP, 					// �e�E�C���h�E�Ȃ�
				(HMENU)NULL, 					// ���j���[�Ȃ�
				hInstance, 						// �C���X�^���X�n���h��
				(LPVOID)NULL);					// �ǉ������Ȃ�

	if (!hwnd) return FALSE;


	// �Q�[���̏�������
	if (!GameInit(hInstance, hwnd, width, height,FULLSCREEN)){
		GameExit();
		MessageBox(hwnd, "ERROR!", "GameInit Error", MB_OK);
		return false;
	}

	// �E�C���h�E��\������
	ShowWindow(hwnd, nWinMode);
	UpdateWindow(hwnd);

	// ���b�Z�[�W����[�v
	while(1){
		// ���b�Z�[�W���擾
		if( !GetMessage( &msg, NULL, 0, 0 ) ) 
		{ 
			break; 
		} 
		else{ 
			TranslateMessage(&msg); 		// �L�[�{�[�h�g�p���\�ɂ���
			DispatchMessage(&msg); 			// �R���g���[�����v�h�m�c�n�v�r�ɖ߂�
		} 
	}

	g_EndFlag=true;									// �I���t���O���I���ɂ���

	if(g_timerid) timeKillEvent(g_timerid);			// �}���`���f�B�A�^�C�}�̏I��
	timeEndPeriod(1);								// �^�C�}�̕���\�͂��Ƃɖ߂�

	// �Q�[���̏I������
	GameExit();

	return (int)msg.wParam;
}
//////////////////////////
// �Q�[�����C���֐�		//
//////////////////////////
unsigned int	WINAPI GameMain(void* p1)
{
	SampleLoad();
	while(1){
		// �C�x���g�t���O���Z�b�g�����̂�҂@�^�C���A�E�g�P�O�O�O����
		DWORD sts=WaitForSingleObject(g_hEventHandle,1000);
		if(sts==WAIT_FAILED){
			break;
		}else if(sts == WAIT_TIMEOUT){
			if(g_EndFlag){
				break;
			}
			continue;
		}

		float ClearColor[4] = { 0, 0, 1.0f, 1.0f }; //red,green,blue,alpha

		// �^�[�Q�b�g�o�b�t�@�N���A
	    g_pD3D11.GetImmediateContext()->ClearRenderTargetView( g_pD3D11.GetRenderTargetView(), ClearColor );

		SampleDraw();

		// SWAP
		g_pD3D11.GetSwapChain()->Present( 0, 0 );

	}
	return 1;
}

////////////////////////////////////////////////////////////////
// ���b�Z�[�W�����֐��i�E�C���h�E�֐��j                       //
//															  //
//	���̊֐��́A�n�r����Ăяo����ă��b�Z�[�W�L���[����      //
//	���b�Z�[�W�̈��n�����󂯂�								  //
//															  //
////////////////////////////////////////////////////////////////
LRESULT WINAPI WndProc(HWND hwnd, 				// �E�C���h�E�n���h���l
						UINT message,			// ���b�Z�[�W���ʎq
						WPARAM wParam,			// �t�я��P
						LPARAM lParam)			// �t�я��Q
{
	switch (message){
		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

//////////////////////////
// �Q�[���̏���������	//
//////////////////////////
bool GameInit(HINSTANCE hinst,HWND hwnd,int width,int height,bool fullscreen){

	bool		sts;		

	// �c�����������w�@�f���������������I�u�W�F�N�g������
	sts = g_pD3D11.Init(hwnd,FULLSCREEN,width,height,fullscreen);
	if(!sts){		
		MessageBox(hwnd,"ERROR!!","DirectX �������G���[",MB_OK);
		return false;
	}

	// �C�x���g�n���h������
	g_hEventHandle=CreateEvent(NULL,false,false,NULL);
	if(g_hEventHandle==NULL){
		MessageBox(hwnd,"ERROR!!","CreateEvent �G���[",MB_OK);
		return false;
	}

	// �X���b�h����(�Q�[�����C��)
	g_hThread = (HANDLE)_beginthreadex(NULL,0,GameMain,0,0,&g_dwThreadId);
	if(g_hThread==0){
		MessageBox(hwnd,"ERROR!!","beginthreadex �G���[",MB_OK);
		return false;
	}

	return	true;
}

//////////////////////////
// �Q�[���̏I������		//
//////////////////////////
void GameExit(){

	SampleExit();
	// �Q�[�����C���X���b�h�̏I����҂�
	WaitForSingleObject(g_hThread,INFINITE);

	// �X���b�h�n���h�����N���[�Y
	CloseHandle(g_hThread);

	// �C�x���g�n���h���N���[�Y
	CloseHandle(g_hEventHandle);

	g_pD3D11.Exit();

}

//////////////////////////
//	�^�C�}�[�o�q�n�b	//
//////////////////////////
void CALLBACK TimerProc(UINT ,UINT,DWORD,DWORD,DWORD){
	// �I���t���O���Z�b�g����Ă��Ȃ���΃C�x���g���Z�b�g����
	if(!g_EndFlag){
		SetEvent(g_hEventHandle);
	}
}
