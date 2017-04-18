#pragma once

#include <windows.h>
//xinput�̑O��windows.h���C���N���[�h���邱�Ɓ@
//�C���N���[�h�����G���[�Ɋ֌W���܂�
#include "Singleton.h"
#include <Xinput.h>
#pragma comment(lib,"xinput.lib")

//�{�^���Ɣz��@�Y�����̊֌W
#define GAMEPAD_DPAD_UP			0
#define GAMEPAD_DPAD_DOWN		1
#define GAMEPAD_DPAD_LEFT		2
#define GAMEPAD_DPAD_RIGHT		3
#define GAMEPAD_START			4
#define GAMEPAD_BACK			5
#define GAMEPAD_LEFT_THUMB		6
#define GAMEPAD_RIGHT_THUMB		7
#define GAMEPAD_LEFT_SHOULDER	8
#define GAMEPAD_RIGHT_SHOULDER	9
#define GAMEPAD_A				10
#define GAMEPAD_B				11
#define GAMEPAD_X				12
#define GAMEPAD_Y				13

#define STICK_LEFT		0
#define STICK_RIGHT		1

#define TRIGGER_LEFT	0
#define TRIGGER_RIGHT	1


//��̃X�e�B�b�N�̓��͏��
//-1�`1�@�����͂�0
struct Stick
{
	float x;
	float y;
};

//4�R���g���[���̓��͂��܂Ƃ߂ĊǗ�����N���X
class XInput :public Singleton<XInput>
{
private:
	//�C���i�[�N���X
	//�N���X�̒��̃N���X
	//�����private�Ő錾���邽��
	//�N���X�O����͈�؎g�p�ł��Ȃ�

	//��̃R���g���[���̓��͂��Ǘ�
	class GamePadState
	{
		//�{�^���ԍ��ƃ}�X�N�̊֌W
		//�{�^�����@14
		static int ButtonCode[14];

		//���ꂼ��̃{�^���̉����ꑱ�����t���[����
		int buttonCount[14];
		int buttonCountPrev[14];

		//�R���g���[�����ڑ�����Ă��邩
		bool conected;

		//�g���K�[�{�^���̓��� 0~1
		float trigger[2];

		//�X�e�B�b�N�̌X��
		Stick stick[2];

		//���͏�ԉ��H�O
		//xinput����擾�����܂܂̃f�[�^
		XINPUT_STATE state;

	public:
		//�R���X�g���N�^�@�f�X�g���N�^
		GamePadState();
		~GamePadState();

		//��Ԃ̍X�V
		void Update(int padNum);

		//�{�^���������ꂽ�u��(�t���[��)��
		bool ButtonJustPressed(int buttonNum);
		//�����ꂽ�t���[��
		bool ButtonJustRelease(int buttonNum);
		//�����ꑱ�����t���[����
		int GetButtonCount(int buttonNum);
		//�g���K�[���͏��
		float GetTrigger(int TriggerNum);
		//�X�e�B�b�N�̌X�����
		Stick GetStick(int stickNum);
		//�ڑ�����Ă��邩�ǂ���
		bool Conected(){ return conected; }
	};

	friend class Singleton<XInput>;

private:

	GamePadState state[4];

public:

	XInput(){}
	~XInput(){}

	//��Ԃ̍X�V
	void Update();

	//�{�^���������ꂽ�u��(�t���[��)��
	bool ButtonJustPressed(int padNum, int buttonNum);
	//�����ꂽ�t���[��
	bool ButtonJustRelease(int padNum, int buttonNum);
	//�����ꑱ�����t���[����
	int GetButtonCount(int padNum, int buttonNum);
	//�g���K�[���͏��
	float GetTrigger(int padNum, int TriggerNum);
	//�X�e�B�b�N�̌X�����
	Stick GetStick(int padNum, int stickNum);



};