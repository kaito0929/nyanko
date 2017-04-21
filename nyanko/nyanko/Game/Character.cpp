#include "GameState.h"
#include "Character.h"
#include "../directInput.h"
#include "../Xinput.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../mesh.h"
#include "../Direct3D.h"

//=====================================================
//��Character�N���X��
//=====================================================
//�R���X�g���N�^
Character::Character()
{

}
//�f�X�g���N�^
Character::~Character()
{

}

//�}�b�v�̐��l��ς���֐�
void Character::MapChange(int *map[9][9],int x,int y,bool flag)
{
	if (flag == true)
	{
		*map[y][x] = -3;
	}
}

//=====================================================
//��Mikeneko�N���X��
//=====================================================
//�R���X�g���N�^
Mikeneko::Mikeneko()
{

}
//�f�X�g���N�^
Mikeneko::~Mikeneko()
{

}
//�������֐�
void Mikeneko::Initialize()
{
	x = 4;
	y = 3;

	AttackPosX = 100;
	AttackPosY = 100;

	AliveFlag = true;

	//�O�єL�̃e�N�X�`���̏�����
	MikenekoTex.Load("Material/mikeneko.png");
	//���W�̐ݒ�
	MikenekoSprite.SetPos(x * 200, y * 150);
	//�傫���̐ݒ�
	MikenekoSprite.SetSize(100, 100);
}
//�`��֐�
void Mikeneko::Draw()
{
	if (AliveFlag == true)
	{
		Direct3D::DrawSprite(MikenekoSprite, MikenekoTex);
	}
}
//���ۂɓ������֐�
void Mikeneko::Update()
{
	MikenekoSprite.SetPos((x + 1) * AttackPosX, (y + 1) * AttackPosY);
}

//=====================================================
//��Kuroneko�N���X��
//=====================================================
//�R���X�g���N�^
Kuroneko::Kuroneko()
{

}
//�f�X�g���N�^
Kuroneko::~Kuroneko()
{

}
//�������֐�
void Kuroneko::Initialize()
{
	x = 5;
	y = 3;

	AttackPosX = 100;
	AttackPosY = 100;

	AliveFlag = true;

	//���L�̃e�N�X�`���̏�����
	KuronekoTex.Load("Material/kuroneko.png");
	//���W�̐ݒ�
	KuronekoSprite.SetPos(x * 200, y * 150);
	//�傫���̐ݒ�
	KuronekoSprite.SetSize(100, 100);
}
//�`��֐�
void Kuroneko::Draw()
{
	if (AliveFlag == true)
	{
		Direct3D::DrawSprite(KuronekoSprite, KuronekoTex);
	}
}
//���ۂɓ������֐�
void Kuroneko::Update()
{
	KuronekoSprite.SetPos((x + 1) * AttackPosX, (y + 1) * AttackPosY);
}

//=====================================================
//��Tyatora�N���X��
//=====================================================
//�R���X�g���N�^
Tyatora::Tyatora()
{

}
//�f�X�g���N�^
Tyatora::~Tyatora()
{

}
//�������֐�
void Tyatora::Initialize()
{
	x = 3;
	y = 3;

	AttackPosX = 100;
	AttackPosY = 100;

	AliveFlag = true;

	//���g���̃e�N�X�`���̏�����
	TyatoraTex.Load("Material/tyatora.png");
	//���W�̐ݒ�
	TyatoraSprite.SetPos(x * 200, y * 150);
	//�傫���̐ݒ�
	TyatoraSprite.SetSize(100, 100);
}
//�`��֐�
void Tyatora::Draw()
{
	if (AliveFlag == true)
	{
		Direct3D::DrawSprite(TyatoraSprite, TyatoraTex);
	}
}
//���ۂɓ������֐�
void Tyatora::Update()
{
	TyatoraSprite.SetPos((x + 1) * AttackPosX, (y + 1) * AttackPosY);
}