#pragma once

#include "../mesh.h"
#include "../Direct3D.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../Wave.h"
#include "../DirectSound.h"
#include "../SoundBuffer.h"

#include "../Scene/SceneManager.h"
#include "../Scene/ISceneChanger.h"
#include "../Scene/BaseScene.h"
#include "Character.h"

#include "Sound.h"
#include "SoundEffect.h"
#include "Fade.h"

#define CHACKNUM 4
#define MAPSIZE 9

//�E�����Ə�����ւ̍U������ʒu�̏��
#define MINUS_ATTACK_NUM 85
//�������Ɖ������ւ̍U������ʒu�̏��
#define PLUS_ATTACK_NUM 120


//�}�b�v���̈ړ��\�͈͂̍ő�ƍŏ��l
#define MAP_SIZE_MIN 1
#define MAP_SIZE_MAX 7

//�}�b�v�̒[�����̍ő�ƍŏ��l
#define MAPEND_MIN 0
#define MAPEND_MAX 8

//�}�b�v�̒[�����̐��l
#define MAPEND_NUM -2
//�}�b�v�̕W���̐��l
#define MAPSTANDARD_NUM -1
//�U���Ώۂ�����}�b�v�̐��l
#define MAP_ATTACK_OK -4

extern bool FirstPlayerLosingFlag;
extern bool SecondPlayerLosingFlag;


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

//�v���C���[�̃^�[����؂�ւ��邽�߂̕ϐ�
enum PlayerTurn
{
	FirstPlayer_Turn,
	SecondPlayer_Turn,
};

//�^�[���؂�ւ��̃J�b�g�C���̂��߂̏��
enum FADE
{
	FADE_IN,
	FADE_OUT,
};

class GameState : public BaseScene
{
private:

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

	//�w�i�̃e�N�X�`��
	Texture BackGroundTex;
	Sprite BackGroundSprite;

	//�����̑��֐}�̃e�N�X�`��
	Texture CompatibilityTex;
	Sprite CompatibilitySprite;

	//�v���C���[�̍s���菇
	PlayerState playerstate;

	//�U���Ƒҋ@�R�}���h�̑I��
	Command command;

	//�U�������ۂ̓���
	AttackMotion attack;

	//�v���C���[�̃^�[����؂�ւ��邽�߂̕ϐ�
	PlayerTurn playerTurn;

	//�J�b�g�C���̃t�F�[�h�C���t�F�[�h�A�E�g
	int hoge;

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

	//�c��ړ��͈�
	//���j�b�g�͎O�}�X�ړ����邱�Ƃ��o����
	int MoveCount;

	//�U���R�}���h��\�����邩�̃t���O
	bool AttackCommandDisplay;

	//�U���ł��郆�j�b�g���l���ɂ��邩���ׂ�ϐ�
	int AttackSearchX;
	int AttackSearchY;

	//�ړ�����O�̕ϐ���������
	int BeforeMapNumX;
	int BeforeMapNumY;


	//�U�����郆�j�b�g�����߂����̃t���O
	bool AttackUnitChoiceFlag;

	//�U�������̃t���O
	bool AttackCompleteFlag;

	//�v���C���[�̍s����
	int PlayerActionNum;

	bool TurnChangeFlag;

	Mikeneko mike_1P[2];
	Kuroneko kuro_1P[2];
	Tyatora tyatora_1P[2];

	Mikeneko mike_2P[2];
	Kuroneko kuro_2P[2];
	Tyatora tyatora_2P[2];


	Sound sound;
	SoundEffect se;

public:
	//�R���X�g���N�^
	GameState::GameState(ISceneChanger* changer);
	//�f�X�g���N�^
	~GameState();			

	//������
	void Initialize() override;
	//���ۂ̓���
	void Update() override;
	//�`��
	void Draw() override;

	//�ړ��ł���ʒu��������֐�
	void Search();

	//�J�[�\�����ړ�������
	void MoveCursor();

	//�R�}���h��I��������𓮂���
	void CommandChoice();

	//�U�����Ă���悤�ɓ������֐�
	void AttackMotion(int x,int y,int *attackPosX,int *attackPosY,bool *moveFlag,bool *actionFlag);

	//���������j�b�g������
	void UnitChoice(int x, int y,bool *flag,bool moveFlag);

	//���j�b�g�����ۂɈړ�������֐�
	void UnitMove(int *x, int *y,bool flag);

	//�}�b�v�̐��l��ς��邽�߂̊֐�
	void MapChange(int myX, int myY, bool myFlag, int enemyX, int enemyY, bool enemyFlag);

	//�ړ��\���̃t���O��؂�ւ���֐�
	void UnitMovePossidleChange(bool aliveFlag, bool *moveFlag);

	//1P�̍s���֐�
	void FirstPlayer_Update();

	//2P�̍s���֐�
	void SecondPlayer_Update();

	//�}�b�v�̐��l�����Z�b�g����֐�
	void MapReset();

	//�R�}���h�𑀍삷��֐�
	void CommandPlay(int *x,int *y,bool flag,bool* moveFlag);

	//���j�b�g���m�̐퓬�̏������s���֐�
	void UnitBattle();

	//�U���ΏۂƂ��đI�΂ꂽ���̃t���O�𑀍삷��֐�
	void AttackFlag(int x, int y, bool*flag);

	//�ړ���ɍU���\�����ׂ�֐�
	void CheckAttack();

	void TurnChange();
};