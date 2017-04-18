#include <windows.h>
#include "Xinput.h"
#include "Singleton.h"

//�X�^�e�B�b�N�ȃ����o�ϐ��̒�`
//�}�X�N�ƃ{�^���ԍ��̑Ή��\
int XInput::GamePadState::ButtonCode[14] =
{
	XINPUT_GAMEPAD_DPAD_UP,		//��{�^��	0x0001
	XINPUT_GAMEPAD_DPAD_DOWN,	//���{�^��	0x0002
	XINPUT_GAMEPAD_DPAD_LEFT,	//���{�^��	0x0004
	XINPUT_GAMEPAD_DPAD_RIGHT,	//�E�{�^��	0x0008
	XINPUT_GAMEPAD_START,		//�X�^�[�g  0x0010
	XINPUT_GAMEPAD_BACK,		//�o�b�N	0x0020
	XINPUT_GAMEPAD_LEFT_THUMB,	//���X�e�B�b�N�̉�������
	XINPUT_GAMEPAD_RIGHT_THUMB,	//�E�X�e�B�b�N�̉�������
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y
};

//�C���i�[�N���X�̃R���X�g���N�^
//��ԊO���̃N���X��::�C���i�[�N���X��::�֐���
XInput::GamePadState::GamePadState()
{
	for (int i = 0; i < 14; i++)
	{
		buttonCount[i] = buttonCountPrev[i] = 0;
	}

	for (int i = 0; i < 2; i++)
	{
		Stick s = { 0.0f, 0.0f };
		stick[i] = s;

		trigger[i] = 0;
	}
}

//�C���i�[�N���X�̃f�X�g���N�^
XInput::GamePadState::~GamePadState()
{
	//���I�ȃ������m�ۂȂǂ͂Ȃ��̂ŏ����Ȃ�
}

//�C���i�[�N���X�@�X�V
void XInput::GamePadState::Update(int padNum)
{
	//1�t���[���O��Counter��ۑ�
	for (int i = 0; i < 14; i++)
	{
		buttonCountPrev[i] = buttonCount[i];
	}

	//���͏�Ԃ̎擾����肭�s�������ǂ���
	DWORD funcResult;

	//���͏�Ԃ̎擾
	//padNum���擾�������R���g���[���̔ԍ� 0~4���L��
	//�����������͏�Ԃ�ۑ�����\���̂ւ̃|�C���^(�A�h���X)
	//�擾�ɐ�������΁A�߂�l�� ERROR_SUCCESS ���Ԃ�
	funcResult = XInputGetState(padNum, &state);

	if (funcResult == ERROR_SUCCESS)
	{
		//�擾�ɐ�������
		conected = true;

		//�{�^���̍X�V
		for (int i = 0; i < 14; i++)
		{
			int code = ButtonCode[i];
			//wButtons 14�S�Ẵ{�^���̃I���I�t��S�Ă܂Ƃ߂����̂ł�
			//�ϐ��̂��ꂼ��̃r�b�g���{�^���ɑΉ����Ă���
			//�}�X�N��&���Z���Ă��ꂼ��̃r�b�g�̏������o���K�v������
			if ((state.Gamepad.wButtons& code)!=0)
			{
				//������Ă���̂ŉ����ꑱ���Ă���t���[���������Z
				buttonCount[i]++;
			}
			else
			{
				buttonCount[i] = 0;
			}
		}

		//�X�e�B�b�N�̍X�V
		for (int i = 0; i < 2; i++)
		{
			//i==0�ō��X�e�B�b�N 1�ŉE�X�e�B�b�N�̏��
			int x = (i == 0) ? state.Gamepad.sThumbLX : state.Gamepad.sThumbRX;
			int y = (i == 0) ? state.Gamepad.sThumbLY : state.Gamepad.sThumbRY;
			//�t���ɓ|����
			//-��������-32768���Œ�
			//+�������� 32767���ő�

			//�S�����͂���Ă��Ȃ��Ă������Ȓl�����邱�Ƃ����邽��
			//���錈�܂�����Βl�ȉ��̒l�𒴂��Ȃ���0�Ƃ��Ĉ���
			//�f�b�h�]�[��
			//������ xy�̂ǂ��炩���f�b�h�]�[���̊O�Ȃ�ǂ�����ύX���Ȃ�
			if (x<XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE&&x>-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE&&
				y<XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE&&y>-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{
				x = y = 0;
			}

			//�|����Ă����Ԃ�int�^�̐��l����-1~0^1�͈̔͂̏����ɒ���

			if (x > 0)
			{
				stick[i].x = (float)x / (float)32768;
			}
			else if (x < 0)
			{
				stick[i].x = (float)x / (float)32767;
			}
			else
			{
				stick[i].x = 0;
			}

			if (y > 0)
			{
				stick[i].y = (float)x / (float)32768;
			}
			else if (y < 0)
			{
				stick[i].y = (float)x / (float)32767;
			}
			else
			{
				stick[i].y = 0;
			}
		}

		//�g���K�[�̓��͏��
		for (int i = 0; i < 2; i++)
		{
			//i==0�ō��̃g���K�[ 1�ŉE�̃g���K�[
			int x = (i == 0) ? state.Gamepad.bLeftTrigger : state.Gamepad.bRightTrigger;
			//�S�������Ă��Ȃ���Ԃ�0 �t���ɉ��������255

			//�f�b�h�]�[���̓����ɂ���Ȃ�0�Ƃ��Ĉ���
			if (x > 10)
			{
				trigger[i] = (float)x / 255.0;
			}
			else
			{
				trigger[i] = 0;
			}
		}
	}
	else
	{
		//�R���g���[����Ԃ̎擾�Ɏ��s
		//�擾���悤�Ƃ����R���g���[���ԍ������ڑ��Ȃǂ̏ꍇ
		//xinput�ɔ�Ή��̏ꍇ�Ȃ�
		conected = false;

		//�S�ē��͖����Ƃ��Ĉ���
		for (int i = 0; i < 14; i++)
		{
			buttonCount[i] = 0;
		}
		for (int i = 0; i < 2; i++)
		{
			stick[i].x = stick[i].y = 0;
		}
		for (int i = 0; i < 2; i++)
		{
			trigger[i] = 0;
		}
	}
}

//�C���i�[�N���X
//�����ꂽ�u�Ԃ�
bool XInput::GamePadState::ButtonJustPressed(int buttonNum)
{
	//�Ή�����{�^���͈͓̔���
	if (buttonNum >= 14 || buttonNum < 0)
	{
		//�͈͓��ł͂Ȃ�
		return false;
	}

	if (buttonCount[buttonNum] == 1)
	{
		return true;
	}
	else
	{
		return false;
	}

}


//�C���i�[�N���X
//�����ꂽ�u�Ԃ�
bool XInput::GamePadState::ButtonJustRelease(int buttonNum)
{
	//�Ή�����{�^���͈͓̔���
	if (buttonNum >= 14 || buttonNum < 0)
	{
		//�͈͓��ł͂Ȃ�
		return false;
	}

	//�p���t���[������0��
	//�O��̃t���[���̌p���t���[������1�ȏ�
	//�Ȃ痣���ꂽ�u��
	if (buttonCount[buttonNum] == 0 && buttonCountPrev[buttonNum] >= 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//�C���i�[�N���X
//�����ꑱ�����t���[����
int  XInput::GamePadState::GetButtonCount(int buttonNum)
{
	//�Ή�����{�^���͈͓̔���
	if (buttonNum >= 14 || buttonNum < 0)
	{
		//�͈͊O
		return 0;
	}

	return buttonCount[buttonNum];

}


//�C���i�[�N���X
//�X�e�B�b�N�̌X���擾
Stick XInput::GamePadState::GetStick(int stickNum)
{
	//�Ή�����X�e�B�b�N�͈͓̔���
	if (stickNum >= 0 && stickNum < 2)
	{
		//�͈͓�
		return stick[stickNum];
	}
	else
	{
		//�͈͊O
		//���͂�������Ԃƈꏏ�̃f�[�^��Ԃ�
		Stick s = { 0, 0 };
		return s;
	}
}

//�g���K�[�̏�Ԏ擾�@������Ă��Ȃ���0 ���S�ɉ������܂�Ă����1  ���Ԓn��0~1�̏�����
float XInput::GamePadState::GetTrigger(int triggerNum)
{
	//�Ή�����g���K�[�͈͓̔���
	if (triggerNum >= 0 && triggerNum < 2)
	{
		//�͈͓�
		return trigger[triggerNum];
	}
	else
	{
		//�͈͊O
		return 0;
	}
}

//�����R���g���[���̓��͏�Ԃ̍X�V
void XInput::Update()
{
	for (int i = 0; i < 4; i++)
	{
		state[i].Update(i);
	}
}

bool XInput::ButtonJustPressed(int padNum, int buttonNum)
{
	//padNum���L���Ȕ͈͂�
	if (padNum < 0 || padNum >= 4)
	{
		//�͈͊O
		return false;
	}

	return state[padNum].ButtonJustPressed(buttonNum);
}

bool XInput::ButtonJustRelease(int padNum, int buttonNum)
{
	//padNum���L���Ȕ͈͂�
	if (padNum < 0 || padNum >= 4)
	{
		//�͈͊O
		return false;
	}

	return state[padNum].ButtonJustRelease(buttonNum);
}

int XInput::GetButtonCount(int padNum, int buttonNum)
{
	//padNum���L���Ȕ͈͂�
	if (padNum < 0 || padNum >= 4)
	{
		//�͈͊O
		return false;
	}

	return state[padNum].GetButtonCount(buttonNum);
}

Stick XInput::GetStick(int padNum, int stickNum)
{
	if (padNum < 0 || padNum >= 4)
	{
		//�͈͊O
		Stick s = { 0.0f, 0.0f };
		return s;
	}

	return state[padNum].GetStick(stickNum);
}

float XInput::GetTrigger(int padNum, int triggerNum)
{
	if (padNum < 0 || padNum >= 4)
	{
		return 0;
	}

	return state[padNum].GetTrigger(triggerNum);
}