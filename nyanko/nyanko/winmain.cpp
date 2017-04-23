#include <Windows.h>

#include <tchar.h>
#pragma comment(lib,"winmm.lib")

#include "Direct3D.h"
#include "DirectSound.h"
#include "Texture.h"
#include "Sprite.h"
#include "Wave.h"
#include "SoundBuffer.h"
#include "directInput.h"
#include "Xinput.h"

#include "mesh.h"
#include "camera.h"
#include "BoundingBox.h"

#include "Game/GameState.h"
#include "Game/Title.h"


void ObbMove(OrientedBoundingBox& obb,float rotate, D3DXVECTOR3 pos)
{
	
	D3DXVECTOR3 forward;
	D3DXVECTOR3 right;
	D3DXVECTOR3 up;
	
	
	forward.x = cos(rotate + (D3DX_PI / 2));
	forward.z = sin(rotate + (D3DX_PI / 2));
	forward.y = 0;

	right.x = cos(rotate);
	right.z = sin(rotate);
	right.y = 0;

	obb.UpdateInfo(pos, forward, right, up);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//���b�Z�[�W�����ׂ̈̊֐�
	//DispatchMessage�ɂ���ČĂяo�����

	//�E�B���h�E�N���X�̓o�^���Ɋe�E�B���h�E�ɐݒ肳���
	//�A�v���P�[�V���������b�Z�[�W���擾������Ăяo�����悤�ɍ��

	//WM_DESTROY �E��́~�������ꂽ �E�B���h�E���j�����ꂽ�Ƃ�
	//WM_MOVE�@  �E�B���h�E�̈ړ�
	//WM_SIZE�@  �E�B���h�E�T�C�Y�̕ύXWM_SIZE
	//��

	//���ʂȏ�����K�v�Ƃ���ꍇ�i�܂��͎����œƎ��̏������s�������j
	//�͂����Ń��b�Z�[�W�̎�ނɉ������������s��
	//�Ƃ肠�����I����������
	switch (msg)
	{
	case WM_DESTROY: //�E��́~�������ꂽ AltF4�������ꂽ���Ȃ� �E�B���h�E�̔j����

		//���b�Z�[�W�L���[�ɐV����WM_QUIT���b�Z�[�W�𑗂�
		//�����ꃁ�b�Z�[�W�L���[������o���ꃁ�C�����[�v���I����(���C�����[�v�̌p�������ɂ��)	
		PostQuitMessage(0);	

		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);//�f�t�H���g�̃E�B���h�E�v���V�[�W���̎��s
}

static const TCHAR * WC_BASIC = _T("Basic");//��{�E�B���h�E�N���X��

HRESULT RegistClassEx(HINSTANCE hInstance)
{
	//�E�B���h�E�̐ݒ��ۑ�����\����
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(WNDCLASSEX));//wcex�̃A�h���X����@WNDCLASSEX�̗e�ʕ� 0�ŏ�����

	//�\���̂̃T�C�Y
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.cbClsExtra = 0;//0�Œ�
	wcex.cbWndExtra = 0;//0�Œ�

	//Window�̃X�^�C��
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	//WinMain�̃C���X�^���X�n���h��
	wcex.hInstance = hInstance;
	//�E�B���h�E�v���V�[�W���̃A�h���X�@�֐��|�C���^�̐ݒ�
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	//�w�i�F�̐ݒ�
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//���j���[
	wcex.lpszMenuName = NULL;
	//�E�B���h�E�N���X�̖��O
	wcex.lpszClassName = WC_BASIC;
	//�A�C�R��
	wcex.hIcon = (HICON)LoadImage(NULL, MAKEINTRESOURCE(IDI_APPLICATION), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	wcex.hIconSm = NULL;
	//�J�[�\��
	wcex.hCursor = (HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

	//���ݒ肵���E�B���h�E�N���X��o�^�@�o�^���Ȃ��ƃE�B���h�E�����Ȃ�
	return RegisterClassEx(&wcex);
}

HRESULT MakeWindow(HINSTANCE hInstance,HWND & refHWnd,int Width =800,int Height=600)
{
	//�N���C�A���g�̈�̂������������߂�
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right =Width;//��
	rect.bottom = Height;//����

	//�w�肵���N���C�A���g�̈��ێ�����傫���̃E�B���h�E�T�C�Y���v�Z
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);

	HWND hWnd;//�E�B���h�E�̃n���h���@�E�B���h�E�̎��ʂɎg��

	hWnd = CreateWindowEx
		(0,						//�E�B���h�E�g���X�^�C��
		WC_BASIC,				//��ɍ�����E�B���h�E�N���X�̐ݒ�
		_T("�ɂ��WARS"),			//�E�B���h�E�̃^�C�g��
		WS_OVERLAPPEDWINDOW,	//�E�B���h�E�X�^�C��
		CW_USEDEFAULT,			//�ʒux���W
		CW_USEDEFAULT,			//�ʒuy���W
		rect.right - rect.left,	//���̒���
		rect.bottom - rect.top,	//����
		NULL,					//�e�E�B���h�E�̃n���h��
		NULL,					//���j���[�n���h��
		hInstance,				//Winmain�̃n���h��
		NULL
		);

	if (hWnd == NULL)
	{
		MessageBox(NULL, "�E�B���h�E�̍쐬�Ɏ��s\n�v���O�����̏I��", "�~���B", MB_OK); // MB_OK�@���b�Z�[�W�{�b�N�X�̃^�C�v
		return S_FALSE;//�v���O�����̏I��
	}

	ShowWindow(hWnd, SW_SHOWNORMAL);//�E�B���h�E���A�N�e�B�u�ɂ��ĕ\��
	UpdateWindow(hWnd);//�N���C�A���g�̈�̍X�V

	refHWnd = hWnd;

	return S_OK;
}


//�G���g���[�|�C���g�@�v���O�����̊J�n�֐�

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	

	if (FAILED(RegistClassEx(hInstance))) //FAILED HRESULT�̒l�����s�������Ă���Ȃ�true
	{
		return 0;
	}

	HWND hWnd=NULL;

	if (FAILED(MakeWindow(hInstance,hWnd, 1200, 1000)))
	{
		return 0;
	}

	//Direct3D�f�o�C�X�̍쐬
	Direct3D* pDirect3d = Direct3D::GetInstance();//�C���X�^���X�ւ̃|�C���^�̎擾
	if (pDirect3d->TryCreate(hWnd))
	{
		
	}

	//DirectSound�f�o�C�X�쐬
	DirectSound * pDirectSound = DirectSound::GetInstance();
	if (pDirectSound->Create(hWnd))
	{
		
	}
	

	//DirectInput
	DirectInput* pDi = DirectInput::GetInstance();

	if (SUCCEEDED(pDi->Init(hWnd)))
	{
		//MessageBox(NULL, "DirectInput����������", "�^�C�g��", MB_OK);
	}

	
	Direct3D::GetInstance()->SetupProjectionMatrix();


	Camera camera;

	SceneManager scenemanager;
	scenemanager.Initialize();
	

	MSG msg = {};

	//msg.message��quit�ɂȂ����烁�C�����[�v�𔲂���
	while (msg.message != WM_QUIT)
	{
		//���b�Z�[�W�L���[�̒��Ƀ��b�Z�[�W�����邩�𒲂ׂāAmsg�ɏ�������true��Ԃ�
		//�������b�Z�[�W���Ȃ����false��Ԃ�
		//PM_REMOVE��ݒ肵�Ă���ƃ��b�Z�[�W�̏������݂̂��ƃ��b�Z�[�W�L���[���珑�����݌��̃f�[�^�͍폜�����B
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�ɕϊ����A��������b�Z�[�W�L���[�Ƀ|�X�g����
			TranslateMessage(&msg);
		
			//���b�Z�[�W�̊��蓖�ā@�E�B���h�E�v���V�[�W���Ń��b�Z�[�W����������
			DispatchMessage(&msg);	
		}
		else
		{

			//���b�Z�[�W�̏������s���Ă��Ȃ��Ƃ��Ɏ��s����
			//�����Ɏ����̂���������������

			//�����ɃQ�[���R�[�h������

			//���͂̍X�V
			
			pDi->Update();

			scenemanager.Update();
			
			//�`�揈��
			//�R�}����������ꍇ�͏������΂����Ƃ�����
			if (SUCCEEDED(pDirect3d->BeginScene()))
			{
				pDirect3d->ClearScreen();

				//�r���[�s��̍X�V
				camera.UpdateViewMatrix();

				scenemanager.Draw();

				pDirect3d->EndScene();
				pDirect3d->Present();
			}


		}	
	}

	Direct3D::DestroyInstance();//�C���X�^���X�̍폜

	return 0;
}