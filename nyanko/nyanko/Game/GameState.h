#pragma once

#include "../mesh.h"
#include "../Direct3D.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../Wave.h"
#include "../DirectSound.h"
#include "../SoundBuffer.h"

#include "Character.h"

#define CHACKNUM 4
#define MAPSIZE 9

//�E�����Ə�����ւ̍U������ʒu�̏��
#define MINUS_ATTACK_NUM 85
//�������Ɖ������ւ̍U������ʒu�̏��
#define PLUS_ATTACK_NUM 120

//�t�F�[�h�A�E�g�p�̐��l
#define FADE_OUT_CHANGENUM -1 

struct Cat
{
	//���j�b�g�̍��W
	int x;
	int y;
	//�U�����Ă���悤�Ɍ����邽�߂̃��j�b�g�̕\���ꏊ�̕ϐ�
	int AttackPosX;
	int AttackPosY;
};

//��{�I�ȃv���C���[�̑���
enum PlayerState
{
	CHOICE,			//�ǂ̃��j�b�g�𑀍삷�邩���߂�
	MOVE,			//�I���������j�b�g���ړ�
	COMMAND,		//�U���A�ҋ@�A�߂�̎O�̑I���������߂�
	ATTACKCHOICE,	//�U���Ώۂ�I��
};

//���j�b�g���ړ�������ɕ\�������R�}���h�̑I��
enum Command
{
	ATTACK,			//�U���R�}���h
	STANDBY,		//�ҋ@�R�}���h
	BACK,			//�߂�R�}���h
};

//�U������ۂ̓�������
enum AttackMotion
{
	UP,				//�����
	DOWN,			//������
	RIGHT,			//�E����
	LEFT,			//������
	RETURN,			//���̈ʒu��
	INITIALSTATE,	//���������܂��Ă��Ȃ������̏��
};

class GameState
{
private:

	//�O�єL�̃e�N�X�`��
	Texture MikenekoTex;
	Sprite MikenekoSprite;

	//���g���̃e�N�X�`��
	Texture TyatoraTex;
	Sprite TyatoraSprite;

	//���L�̃e�N�X�`��
	Texture KuronekoTex;
	Sprite KuronekoSprite;

	//���L�̃e�N�X�`��
	Texture ShironekoTex;
	Sprite ShironekoSprite;

	//�ړ��ł���͈͂̃}�X
	Texture MoveSquaresTex;
	Sprite MoveSquaresSprite;

	//�U���\�������}�X
	Texture AttackSquaresTex;
	Sprite AttackSquaresSprite;

	//�I�������肷��J�[�\���̃e�N�X�`��
	Texture CursorTex;
	Sprite CursorSprite;

	//�s�������߂�R�}���h�i�U���j
	Texture AttackCommandTex;
	Sprite AttackCommandSprite;

	//�s�������߂�R�}���h�i�ҋ@�j
	Texture StandbyCommandTex;
	Sprite StandbyCommandSprite;

	//�s�������߂�R�}���h�i�߂�j
	Texture BackCommandTex;
	Sprite BackCommandSprite;

	//�R�}���h��I��������
	Texture TheArrowTex;
	Sprite TheArrowSprite;

	//�O�єL�̍��W
	Cat mike;
	//���g���̍��W
	Cat tyatora;
	//���L�̍��W
	Cat kuro;
	//���L�̍��W
	Cat shiro;

	//�v���C���[�̍s���菇
	PlayerState playerstate;

	//�U���Ƒҋ@�R�}���h�̑I��
	Command command;

	//�U�������ۂ̓���
	AttackMotion attack;

	//�}�b�v�f�[�^:9*9
	int Map[MAPSIZE][MAPSIZE];

	//�ړ��͈͂𒲂ׂ�ۂ̕����ϊ��ׂ̈̕ϐ�
	int ChackDirectionX[CHACKNUM];
	int ChackDirectionY[CHACKNUM];

	//�ړ��͈͂𒲂ׂ邽�߂̕ϐ�
	int SearchX;
	int SearchY;

	//�J�[�\���̕\���ʒu�̕ϐ�
	int CursorX;
	int CursorY;

	//�ړ��ʒu�����߂�ۂ̃t���O
	bool MoveChoiceFlag;

	//�c��ړ���
	//���j�b�g�͎O��ړ����邱�Ƃ��o����
	int MoveCount;

	//�U���R�}���h��\�����邩�̃t���O
	bool AttackCommandDisplay;

	//�U���ł��郆�j�b�g���l���ɂ��邩���ׂ�ϐ�
	int AttackSearchX;
	int AttackSearchY;

	//�ړ�����O�̕ϐ���������
	int BeforeMapNumX;
	int BeforeMapNumY;

	//�����t���O
	bool AliveFlag;	

	//�U�����郆�j�b�g�����߂����̃t���O
	bool AttackUnitChoiceFlag;

	//�U�������̃t���O
	bool AttackCompleteFlag;

	bool flag[3];

public:
	//�R���X�g���N�^
	GameState();			
	//�f�X�g���N�^
	~GameState();			

	//������
	void Initialize();	
	//���ۂ̓���
	void Update();			
	//�`��
	void Draw();			

	//�ړ��ł���ʒu��������֐�
	void Search();

	//�J�[�\�����ړ�������
	void MoveCursor();

	//�R�}���h��I��������𓮂���
	void CommandChoice();

	//�U�����Ă���悤�ɓ������֐�
	void AttackMotion(int x,int y,int *attackPosX,int *attackPosY,bool *flag);

	//���j�b�g���t�F�[�h�A�E�g������֐�
	void UnitFade();

	//���������j�b�g������
	void UnitChoice(int x, int y,bool *flag);

	//���j�b�g�����ۂɈړ�������֐�
	void UnitMove(int *x, int *y,bool flag);
};