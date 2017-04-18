#pragma once

//DirectInput�f�o�b�O�t���O
#define D3D_DEBUG_INF0

//�_�C���N�g�C���v�b�g�o�[�W����
#define DIRECT_INPUT_VERSION 0x0800

//�}�E�X�{�^���ԍ��ɖ��O��t����
#define MOUSE_BUTTON_LEFT 0
#define MOUSE_BUTTON_RIGHT 1
#define MOUSE_BUTTON_MIDDLE 2

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"d3dxof.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")

#include <comdef.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>
#include <dinput.h>//directInput

#include "MyVector.h"

#define INPUT_BUFFER_SIZE 256

class DirectInput
{
public:

	//�C���X�^���X�̎擾
	//�C���X�^���X�������ꍇ�͓��I�ɍ쐬
	static DirectInput* GetInstance()
	{
		if (pInstance == NULL)
		{
			pInstance = new DirectInput();
		}

		return pInstance;
	}

private:
	//private�ȃ����o�ϐ�

	//�C���X�^���X�ւ̃A�h���X�@����̎w����ȊO��
	//�C���X�^���X���o���Ă͂����Ȃ�
	static DirectInput* pInstance;

	//directinput �I�u�W�F�N�g
	LPDIRECTINPUT8 pDInput;

	//�L�[�{�[�h�p�̃f�o�C�X
	LPDIRECTINPUTDEVICE8 pKeyDevice;
	//�}�E�X�p�f�o�C�X
	LPDIRECTINPUTDEVICE8 pMouseDevice;

	//�L�[�{�[�h���
	//���t���[�������ꑱ������
	int KeyInput[INPUT_BUFFER_SIZE];
	//1�t���[���O��KeyInput�ƈ�v
	int KeyInputPreview[INPUT_BUFFER_SIZE];

	//�}�E�X���
	//���݂̃}�E�X���W
	Vector2 mousePos;
	//1�t���[���O�̍��W
	Vector2 mousePosPreview;
	//1�t���[���O�ƌ��݂̃}�E�X���W�̍���
	Vector2 mousePosDelta;

	//�}�E�X�̃{�^��
	DIMOUSESTATE2 mouseInput;
	//1�t���[���O�̃}�E�X�{�^��
	DIMOUSESTATE2 mouseInputPreview;
	//���t���[�������ꑱ���Ă��邩
	int mouseButtonCounter[8];//8 �{�^����

	//�E�B���h�E�̃n���h��
	HWND hWnd;

	//�������ς݂�
	bool initialized;

public:
	//public�ȃ����o�ϐ�


private:
	//private�ȃ����o�֐�

	//-----------------------------------------------------------------
	//�V���O���g���ɂ���Ƃ������Ƃ�
	//��ڂ̃C���X�^���X����������悤�ȏ�����
	//private�ɂ��Č��J���Ȃ�
	DirectInput();
	~DirectInput();

	DirectInput(DirectInput& refDi);//�R�s�[�R���X�g���N�^
	DirectInput& operator =(DirectInput&refDi);//������Z�q��over load
	//------------------------------------------------------------------

	HRESULT InitKey();//�L�[�{�[�h�̏���
	HRESULT InitMouse();//�}�E�X�̏���

public:
	//public�ȃ����o�֐�

	//�������̏���
	HRESULT Init(HWND hWnd);//���̊֐�����InitKey��InitMouse

	//���͏�Ԃ̍X�V
	HRESULT Update();

	//�������
	HRESULT Release();

	//�������ς݂�
	bool Initialized(){ return initialized; }

	//���͏�Ԃ̎擾�֐�

	//key
	bool KeyState(int code);//�P���ɉ�����Ă��邩�ǂ���
	bool KeyStatePreview(int code);//�O�t���[���ŉ�����Ă������ǂ���
	bool KeyJustPressed(int code);//�����ꂽ�u�Ԃ�
	bool KeyJustReleased(int code);//�����ꂽ�u��
	int KeyCount(int code);//������Ă���t���[����

	//mouse
	//�}�E�X���
	DIMOUSESTATE2 MouseState(){ return mouseInput; }
	//�}�E�X��ԁ@1�t���[���O
	DIMOUSESTATE2 MouseStatePreview(){ return mouseInputPreview; }

	//���݂̍��W
	Vector2 MousePosition(){ return mousePos; }
	//1�t���[���O�̍��W
	Vector2 MousePositionPreview(){ return mousePosPreview; }
	//����̃t���[���ňړ������}�E�X���W����
	Vector2 MousePositionDelta(){ return mousePosDelta; }

	//�}�E�X�{�^���@������Ă��邩
	bool MouseButton(int num);
	//�����ꂽ�u�Ԃ��@�����ꂽ�t���[����
	bool MouseButtonJustPressed(int num);
	//�����ꂽ�t���[����
	bool MouseButtonJustReleased(int num);
	//�����ꑱ���Ă���t���[����
	int MouseButtonCount(int num);

};