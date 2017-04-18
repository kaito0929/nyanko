#pragma once

#define D3D_DEBUG_INFO	//Direct3D�f�o�b�O�t���O

#define D3DFVF_LINE_VERTEX D3DFVF_XYZ|D3DFVF_DIFFUSE

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment (lib,"comctl32.lib")
#pragma comment (lib,"winmm.lib")

#include <comdef.h>
#include <d3d9.h>//direct3d
#include <d3dx9.h>//direct3d
#include <mmsystem.h>

#include <Windows.h>

#include <tchar.h>
#include <windowsx.h>
#include <commctrl.h>
#include <WinUser.h>

#include "Singleton.h"

//#include <memory>//�X�}�[�g�|�C���^�̂�@

struct LINE_VERTEX
{
	D3DXVECTOR3 pos;
	DWORD color;
};



enum RENDER_STATE
{
	RENDER_DEFAULT,		//�f�t�H���g
	RENDER_ALPHATEST,	//���e�X�g
	RENDER_HALFADD,		//�����Z����
	RENDER_ADD,		//���Z����

	RENDER_MESH,
	RENDER_ALPHABLEND,
};

//�N���X�̖��O�����錾���Ă���
//�w�b�_�[���ŃN���X�������g�p�\�ɂ���
//���̂̕���cpp�ŃC���N���[�h
class Texture;
class Sprite;
class Mesh;

class Direct3D
{
	friend class Singleton<Direct3D>;
public:
	typedef void(*FuncPointer)();//�֐��|�C���^�̌^��`
private:
	//Direct3D�f�o�C�X�����p�I�u�W�F�N�g
	IDirect3D9* pD3D9 = NULL;

	//Direct3D�f�o�C�X
	//�e�N�X�`���̃��[�h��|���S���̕\���ɕK�v
	IDirect3DDevice9 *pDevice3D = NULL;

	bool isDeviceCreated;
	bool IsDeviceCreated(){ return isDeviceCreated; };//�f�o�C�X���쐬�ς݂��ǂ���

	bool Create(HWND hWnd);//�f�o�C�X�̍쐬
	void Release();//�f�o�C�X�̔j��	

	//�V���O���g���ɂ���
	static Direct3D* pInstance ;

	//�v���C�x�[�g�ɂ��ăN���X�O������s�ł��Ȃ�����
	//�N���X�O����V�����I�u�W�F�N�g������邱�Ƃ�h����
	//Direct3D�̃C���X�^���X��������������݂ł��Ȃ���Ԃɂ���
	Direct3D();//�R���X�g���N�^
	~Direct3D();//�f�X�g���N�^
	void operator=(const Direct3D& obj){}	//������Z
	Direct3D(const Direct3D& obj){};		//�R�s�[�R���X�g���N�^

	static RENDER_STATE currentState;
	
public :
	//�I�u�W�F�N�g�̍쐬
	static Direct3D* GetInstance()
	{
		if (pInstance == NULL)
		{
			pInstance = new Direct3D;
		}
		return pInstance;
	}
	//�I�u�W�F�N�g�̔j��
	static void DestroyInstance()
	{
		if (pInstance != NULL)
		{
			delete pInstance;
			pInstance = NULL;
		}
	}

	static void SetRenderState(RENDER_STATE renderState);

	bool TryCreate(HWND hWnd);//�f�o�C�X�쐬�����݂�

	HRESULT BeginScene();//�`��J�n�̍��}
	HRESULT ClearScreen();//�o�b�N�o�b�t�@�̃N���A
	HRESULT EndScene();//�`��I���̍��}
	HRESULT Present();//�o�b�N�o�b�t�@���t�����g�ɔ��f

	//�e�N�X�`���̓ǂݍ���
	//Texture�N���X��Load�֐�������s
	//���H��true��Ԃ�
	bool LoadTexture(Texture& texture,TCHAR*FileName);

	//�X�v���C�g�̕`��
	static void DrawSprite(Sprite& sprite, Texture&texture, bool isTurn = false);
	
	//���b�V���̃��[�h
	void LoadMesh(Mesh& mesh, TCHAR* path);
	//���b�V���̕\��
	void DrawMesh(Mesh& mesh, D3DXMATRIXA16& worldMat);

	//�r���[�s��̐ݒ�
	void SetViewMatrix(D3DXMATRIXA16& mat);

	//�ˉe�s��̐ݒ�
	void SetupProjectionMatrix();

	//�����̕`��	count ���̖{�� 
	//pVertex[0]-[1]�ň�{[2][3]�ň�{
	//pVertex�̗v�f���͏��count��2�{
	void DrawLine(LINE_VERTEX* pVertex, int count);

	
};

