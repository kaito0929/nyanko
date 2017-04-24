#pragma once
#include "GameState.h"
#include "../directInput.h"
#include "../Xinput.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../mesh.h"
#include "../Direct3D.h"

//�t�F�[�h�A�E�g�p�̐��l
#define CAT_FADE_OUT_CHANGENUM -1 

//�L����ʂ��邽�߂̃X�e�[�^�X
enum CatState
{
	MIKE,		//�O�єL
	KURO,		//���L
	TYATORA,	//���g��
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

	//�L�����N�^�[�̗̑�
	int HitPoint;

	///�U���ΏۂƂ��đI�΂ꂽ���̃t���O
	bool AttackChoiceFlag;

	//�L�̃X�e�[�^�X
	CatState cat;

	//�q�b�g�|�C���g�̃n�[�g�̃e�N�X�`��
	Texture HitPointTex;
	Sprite HitPointSprite[3];
	//�q�b�g�|�C���g�̃n�[�g�̈ʒu
	float HitPointPos[3];

	//���j�b�g���ړ��\���̃t���O
	bool MovePossidleFlag;

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
	Texture MikenekoTex[2];
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

	//���j�b�g���m�̐퓬�̏������s���֐�
	void UnitBattle(CatState Cat, int *hp, bool attackFlag);

	//���j�b�g���t�F�[�h�A�E�g������֐�
	void UnitFade();
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
	Texture KuronekoTex[2];
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

	//���j�b�g���m�̐퓬�̏������s���֐�
	void UnitBattle(CatState Cat, int *hp, bool attackFlag);

	//���j�b�g���t�F�[�h�A�E�g������֐�
	void UnitFade();
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
	Texture TyatoraTex[2];
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

	//���j�b�g���m�̐퓬�̏������s���֐�
	void UnitBattle(CatState Cat, int *hp, bool attackFlag);

	//���j�b�g���t�F�[�h�A�E�g������֐�
	void UnitFade();
};
