#include "directInput.h"

DirectInput* DirectInput::pInstance = NULL;

//�R���X�g���N�^
DirectInput::DirectInput()
{
	pDInput = NULL;
	pKeyDevice = NULL;
	pMouseDevice = NULL;

	initialized = false;

	//�z��ϐ��̏�����
	//�S�Ă�0��
	ZeroMemory(&KeyInput, sizeof(int)*INPUT_BUFFER_SIZE);
	ZeroMemory(&KeyInputPreview, sizeof(int)*INPUT_BUFFER_SIZE);
	ZeroMemory(&mouseButtonCounter, sizeof(int) * 8);
	//�\���̂̏�����
	ZeroMemory(&mouseInput, sizeof(DIMOUSESTATE2));
	ZeroMemory(&mouseInputPreview, sizeof(DIMOUSESTATE2));

	//�}�E�X���W�̃f�[�^��������
	mousePos.Set(0, 0);
	mousePosPreview.Set(0, 0);
	mousePosDelta.Set(0, 0);

}

//�f�X�g���N�^
DirectInput::~DirectInput()
{
	Release();
}

//������
HRESULT DirectInput::Init(HWND hwnd)
{
	hWnd = hwnd;

	if (initialized == false)
	{

		//DirectInput�I�u�W�F�N�g�̍쐬
		HRESULT hresult = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, NULL);

		if (FAILED(hresult))
		{
			//���s�Ȃ̂Ŋ֐��I��
			return hresult;
		}

		//�I�u�W�F�N�g���o�����̂ŃL�[�{�[�h�̏�����
		hresult = InitKey();
		if (FAILED(hresult))
		{
			//���s�Ȃ̂Ŋ֐��I��
			return hresult;
		}

		//�}�E�X�̏�����
		hresult = InitMouse();
		if (FAILED(hresult))
		{
			//���s�Ȃ̂Ŋ֐��I��
			return hresult;
		}

		//�S�Ă̏��������I��
		//�������ς݂Ƃ���
		initialized = true;

	}

	return S_OK;
}

//�L�[������
HRESULT DirectInput::InitKey()
{
	HRESULT hresult;

	//�f�o�C�X�I�u�W�F�N�g�̍쐬
	hresult = pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, NULL);
	//GUID_SysKeybord�@�쐬����f�o�C�X�̎��

	if (FAILED(hresult))
	{
		//���s�Ȃ̂Ŋ֐��I��
		return hresult;
	}

	//�f�[�^�t�H�[�}�b�g�ݒ�
	hresult = pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hresult))
	{
		//���s�Ȃ̂Ŋ֐��I��
		return hresult;
	}

	//�������[�h��ݒ�
	hresult = pKeyDevice->SetCooperativeLevel(hWnd,DISCL_NONEXCLUSIVE|DISCL_BACKGROUND);
	if (FAILED(hresult))
	{
		//���s�Ȃ̂Ŋ֐��I��
		return hresult;
	}

	//���̓f�o�C�X�ւ̃A�N�Z�X�����擾
	pKeyDevice->Acquire();

	return S_OK;

}

//�}�E�X������
HRESULT DirectInput::InitMouse()
{
	HRESULT hresult;

	//�}�E�X�f�o�C�X�̍쐬�@GUID_SysMouse�����f�o�C�X�̎��
	hresult = pDInput->CreateDevice(GUID_SysMouse, &pMouseDevice, NULL);
	if (FAILED(hresult))
	{
		//���s�Ȃ̂Ŋ֐��I��
		return hresult;
	}

	//�f�[�^�t�H�[�}�b�g
	hresult = pMouseDevice->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hresult))
	{
		//���s�Ȃ̂Ŋ֐��I��
		return hresult;
	}

	//�������[�h�̐ݒ�
	hresult = pMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if (FAILED(hresult))
	{
		//���s�Ȃ̂Ŋ֐��I��
		return hresult;
	}

	//�����[�h�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;//����	//�����[�h

	hresult = pMouseDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(hresult))
	{
		//���s�Ȃ̂Ŋ֐��I��
		return hresult;
	}

	//���̓f�o�C�X�ւ̃A�N�Z�X�����擾
	hresult = pMouseDevice->Acquire();
	if (FAILED(hresult))
	{
		//���s�Ȃ̂Ŋ֐��I��
		return hresult;
	}

	RECT rect;
	GetClientRect(hWnd, &rect);
	int Width = rect.right - rect.left;
	int Height = rect.bottom - rect.top;

	POINT localPos;
	localPos.x = Width / 2;
	localPos.y = Height / 2;

	//�}�E�X�̃|�W�V�����̓N���C�A���g���W�ŊǗ�������
	//mousePos��mousePosPreview�������l�ŃX�^�[�g��������
	mousePos.Set(localPos.x, localPos.y);
	mousePosPreview.Set(localPos.x, localPos.y);
	mousePosDelta.Set(0, 0);

	//�N���C�A���g���W����X�N���[�����W�ւ̕ϊ�
	//hWnd �N���C�A���g�̃E�B���h�E
	ClientToScreen(hWnd, &localPos);
	//localPos���N���C�A���g���W����X�N���[�����W�ɕϊ����ꂽ
	
	//�}�E�X�̃X�^�[�g���E�B���h�E�̒��S�ɂ���
	//�X�N���[�����W�ł̎w��
	SetCursorPos(localPos.x, localPos.y);
	
	return S_OK;
}

//�A�b�v�f�[�g
HRESULT DirectInput::Update()
{
	//�������o���Ă��Ȃ�������
	//�|�C���^�^�ϐ��̏������o���Ă��Ȃ����߃A�N�Z�X�ᔽ���N����
	//�̂Ŋ֐����I����
	if (initialized == false)
	{
		return S_FALSE;
	}

	HRESULT hresult = pKeyDevice->Acquire();

	if ((hresult == DI_OK) || (hresult == S_FALSE))
		//S_FALSE�ȊO�̎��s�͒ʂ��Ȃ�
	{
		//�O��̃L�[����preview�ɑޔ�
		//�������ۂ��ƃR�s�[
		memcpy(KeyInputPreview, KeyInput, sizeof(int)*INPUT_BUFFER_SIZE);

		//��ԁ@���[�N(��Ɨp�̈ꎞ�ϐ�) t=temporaly �ꎞ work=���
		BYTE tKeyInputWork[INPUT_BUFFER_SIZE];

		//�L�[��Ԃ�tKeyInputWork�ɓǂݍ���
		pKeyDevice->GetDeviceState(sizeof(BYTE)*INPUT_BUFFER_SIZE, &tKeyInputWork);

		for (int i = 0; i < INPUT_BUFFER_SIZE; i++)
		{
			if (tKeyInputWork[i])//�L�[��������Ă��邩
			{
				if (KeyInput[i] < 2147483647)//int �ő�l
				{
					//�����ꑱ���Ă���t���[���������Z����
					KeyInput[i]++;
				}
			}
			else
			{
				//�����ꑱ���Ă���t���[������0�ɂ���
				KeyInput[i] = 0;
			}
		}
	}

	//�}�E�X�̃{�^�����擾
	hresult = pMouseDevice->Acquire();
	if ((hresult == DI_OK) || (hresult == S_FALSE))
		//S_FALSE�ȊO�̎��s�͒ʂ��Ȃ�
	{
		//�O��̃}�E�X��Ԃ�ޔ�
		//�������ۂ��ƃR�s�[
		memcpy(&mouseInputPreview, &mouseInput, sizeof(DIMOUSESTATE2));

		//mouseInput�Ƀ}�E�X�̓��͏�Ԃ��擾
		pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseInput);

		for (int i = 0; i < 8; i++)//�S�Ẵ{�^�����`�F�b�N
		{
			if (mouseInput.rgbButtons[i])//�{�^����������Ă���
			{
				if (mouseButtonCounter[i] < 2147483647)
				{
					//�����ꑱ���Ă���t���[���������Z
					mouseButtonCounter[i]++;
				}
			}
			else
			{
				//�����ꑱ���Ă���t���[���������Z�b�g
				mouseButtonCounter[i] = 0;
			}
		}
	}

	//�X�N���[�����̃}�E�X���W
	POINT screenPos;
	//�J�[�\���̈ʒu���X�N���[�����W�Ŏ擾
	GetCursorPos(&screenPos);

	//�X�N���[�����W����N���C�A���g���W�ɕϊ�
	POINT localPos = screenPos;
	ScreenToClient(hWnd, &localPos);

	//1�t���[���O�̈ʒu��ޔ�
	mousePosPreview = mousePos;
	//���݂̍��W���X�V
	mousePos.Set(localPos.x, localPos.y);
	//�ړ������������v�Z
	mousePosDelta = mousePos - mousePosPreview;

	return S_OK;
}

//�������
HRESULT DirectInput::Release()
{
	if (pMouseDevice)
	{
		pMouseDevice->Unacquire();
		pMouseDevice->Release();
		pMouseDevice = NULL;
	}

	if (pKeyDevice)
	{
		pKeyDevice->Unacquire();
		pKeyDevice->Release();
		pKeyDevice = NULL;
	}

	if (pDInput)
	{
		pDInput->Release();
		pDInput = NULL;
	}
	initialized = false;

	return S_OK;
}

//�L�[��������Ă���Ȃ�true
bool DirectInput::KeyState(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		//���͂��ꂽ�{�^���̔ԍ����͈͓�

		if (KeyInput[code] > 0)
		{
			//�t���[������1�ȏ�Ȃ�
			//������Ă���
			return true;
		}
		else
		{
			return false;
		}
	}

	//�͈͊O�̏ꍇ�͖ⓚ���p��false
	return false;
}

//1�t���[���O
//�L�[��������Ă���Ȃ�true
bool DirectInput::KeyStatePreview(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		//���͂��ꂽ�{�^���̔ԍ����͈͓�

		if (KeyInputPreview[code] > 0)
		{
			//�t���[������1�ȏ�Ȃ�
			//������Ă���
			return true;
		}
		else
		{
			return false;
		}
	}

	//�͈͊O�̏ꍇ�͖ⓚ���p��false
	return false;
}

//�L�[�������ꂽ�u�Ԃ�
bool DirectInput::KeyJustPressed(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		//�p���t���[������1�Ȃ牟���ꂽ�u��
		return  (KeyInput[code] == 1);
	}
	return false;
}

//�L�[�������ꂽ�u�Ԃ�
bool DirectInput::KeyJustReleased(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		//�p���t���[������1�Ȃ牟���ꂽ�u��
		return  (KeyInput[code] == 0) && (KeyInputPreview[code] >= 1);
	}
	return false;
}

//�L�[�������ꑱ���Ă���t���[����
int DirectInput::KeyCount(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		return KeyInput[code];
	}
	//�͈͊O��0
	return 0;
}


//�}�E�X�̃{�^�����
bool DirectInput::MouseButton(int num)
{
	if (num >= 0 && num < 8)
	{
		return (mouseInput.rgbButtons[num] >= 1);
	}
	else
	{
		//�͈͊O�Ȃ̂ŕK��false
		return false;
	}
}

//�}�E�X�̃{�^���������ꂽ�u�Ԃ�
bool DirectInput::MouseButtonJustPressed(int num)
{
	if (num >= 0 && num < 8)
	{
		//�O�񂪉�����Ă��Ȃ��ā@���񂪉�����Ă���
		//�����ꂽ�u��
		return (mouseInput.rgbButtons[num] >= 1) && (mouseInputPreview.rgbButtons[num] == 0);
	}
	else
	{
		//�͈͊O�Ȃ̂ŕK��false
		return false;
	}
}

//�}�E�X�{�^���������ꂽ�u�Ԃ�
bool DirectInput::MouseButtonJustReleased(int num)
{
	if (num >= 0 && num < 8)
	{
		//�O�񂪉�����Ă��ā@���񂪉�����Ă��Ȃ�
		//�����ꂽ�u��
		return (mouseInput.rgbButtons[num] == 0) && (mouseInputPreview.rgbButtons[num] >= 1);
	}
	else
	{
		//�͈͊O�Ȃ̂ŕK��false
		return false;
	}
}

//�}�E�X�{�^���������ꑱ���Ă���t���[����
int DirectInput::MouseButtonCount(int num)
{
	if (num >= 0 && num < 8)
	{
		return mouseButtonCounter[num];
	}
	else
	{
		//�͈͊O�̓[��
		return 0;
	}
}


