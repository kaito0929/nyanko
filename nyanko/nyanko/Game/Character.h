#pragma once
#include "GameState.h"
#include "../directInput.h"
#include "../Xinput.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../mesh.h"
#include "../Direct3D.h"

enum Direction
{
	MOVE_UP,				//�����
	MOVE_DOWN,			//������
	MOVE_RIGHT,			//�E����
	MOVE_LEFT,			//������
};

//================================================================
//�N���X��  �FCharacter�N���X
//���N���X�F
//
//���e�F���̃N���X�����j�b�g�̊��N���X�Ƃ��Ĕh�������Ă���
//================================================================

class Character
{
protected:

public:
	//�R���X�g���N�^
	Character();
	//�f�X�g���N�^
	~Character();

	//�L�����N�^�[�̍��W
	int x;
	int y;

	//�L�����N�^�[�̐����t���O
	bool AliveFlag;

	//�U�����Ă���悤�Ɍ����邽�߂̃��j�b�g�̕\���ꏊ�̕ϐ�
	int AttackPosX;
	int AttackPosY;

	//�ړ������̃t���O
	bool MoveFlag;
};


//================================================================
//�N���X��  �FMikeneko�N���X
//���N���X�FCharacter�N���X
//
//���e�FCharacter����h���������N���X�B�O�єL�̃N���X
//================================================================
class Mikeneko :public Character
{
private:
	//�O�єL�̃e�N�X�`��
	Texture MikenekoTex;
	Sprite MikenekoSprite;

public:
	//�R���X�g���N�^
	Mikeneko();
	//�f�X�g���N�^
	~Mikeneko();

	//������
	void Initialize();
	//���ۂ̓���
	void Update();
	//�`��
	void Draw();
};

//================================================================
//�N���X��  �FKuroneko�N���X
//���N���X�FCharacter�N���X
//
//���e�FCharacter����h���������N���X�B���L�̃N���X
//================================================================
class Kuroneko :public Character
{
private:
	//���L�̃e�N�X�`��
	Texture KuronekoTex;
	Sprite KuronekoSprite;

public:
	//�R���X�g���N�^
	Kuroneko();
	//�f�X�g���N�^
	~Kuroneko();

	//������
	void Initialize();
	//���ۂ̓���
	void Update();
	//�`��
	void Draw();
};

//================================================================
//�N���X��  �FTyatora�N���X
//���N���X�FCharacter�N���X
//
//���e�FCharacter����h���������N���X�B���g���̃N���X
//================================================================
class Tyatora :public Character
{
private:
	//���g���̃e�N�X�`��
	Texture TyatoraTex;
	Sprite TyatoraSprite;

public:
	//�R���X�g���N�^
	Tyatora();
	//�f�X�g���N�^
	~Tyatora();

	//������
	void Initialize();
	//���ۂ̓���
	void Update();
	//�`��
	void Draw();
};
