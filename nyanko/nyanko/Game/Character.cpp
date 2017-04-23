#include "GameState.h"
#include "Character.h"
#include "../directInput.h"
#include "../Xinput.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../mesh.h"
#include "../Direct3D.h"

int h = 3;

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

	AttackPosX = 100;
	AttackPosY = 100;

	AliveFlag = true;
	MoveFlag = false;
	AttackChoiceFlag = false;

	cat = MIKE;

	//�O�єL�̃e�N�X�`���̏�����
	MikenekoTex.Load("Material/mikeneko.png");
	//���W�̐ݒ�
	MikenekoSprite.SetPos(x * 200, y * 150);
	//�傫���̐ݒ�
	MikenekoSprite.SetSize(100, 100);

	//�L�����̗̑͂�3
	HitPoint = 3;
}
//�`��֐�
void Mikeneko::Draw()
{
	Direct3D::SetRenderState(RENDER_ALPHABLEND);
	Direct3D::DrawSprite(MikenekoSprite, MikenekoTex);
}
//���ۂɓ������֐�
void Mikeneko::Update()
{
	MikenekoSprite.SetPos((x + 1) * AttackPosX, (y + 1) * AttackPosY);

	if (HitPoint <= 0)
	{
		AliveFlag = false;
	}

	UnitFade();
}

//���j�b�g�����ꂽ���Ƀe�N�X�`�����t�F�[�h�A�E�g������
void Mikeneko::UnitFade()
{
	if (AliveFlag == false)
	{
		MikenekoSprite.SetAlpha(MikenekoSprite.GetAlpha() + (0.1f*FADE_OUT_CHANGENUM));
	}
}

//�U�����鑊��ɂ���ď�����ς���֐�
void Mikeneko::UnitBattle(CatState cat, int *hp, bool attackFlag)
{
	if (attackFlag == true && MoveFlag == true)
	{
		//cat1���O�єL�̏ꍇ�ɏ�������퓬����
		if (cat == TYATORA)
		{
			//h - 2;
			*hp -= 2;
		}
		else if (cat == MIKE)
		{
			*hp -= 1;
		}
	}
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

	AttackPosX = 100;
	AttackPosY = 100;

	AliveFlag = true;
	MoveFlag = false;
	AttackChoiceFlag = false;


	cat = KURO;

	//���L�̃e�N�X�`���̏�����
	KuronekoTex.Load("Material/kuroneko.png");
	//���W�̐ݒ�
	KuronekoSprite.SetPos(x * 200, y * 150);
	//�傫���̐ݒ�
	KuronekoSprite.SetSize(100, 100);

	//�L�����̗̑͂�3
	HitPoint = 3;
	
}
//�`��֐�
void Kuroneko::Draw()
{
	Direct3D::SetRenderState(RENDER_ALPHABLEND);
	Direct3D::DrawSprite(KuronekoSprite, KuronekoTex);
}
//���ۂɓ������֐�
void Kuroneko::Update()
{
	KuronekoSprite.SetPos((x + 1) * AttackPosX, (y + 1) * AttackPosY);

	if (HitPoint <= 0)
	{
		AliveFlag = false;
	}

	UnitFade();
}

void Kuroneko::UnitBattle(CatState cat, int *hp, bool attackFlag)
{
	if (attackFlag == true && MoveFlag == true)
	{
		//cat1�����L�̏ꍇ�ɏ�������퓬����
		if (cat == MIKE)
		{
			*hp -= 2;
		}
		else if (cat == KURO)
		{
			*hp -= 1;
		}
	}
}

void Kuroneko::UnitFade()
{
	if (AliveFlag == false)
	{
		KuronekoSprite.SetAlpha(KuronekoSprite.GetAlpha() + (0.1f*FADE_OUT_CHANGENUM));
	}
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

	AttackPosX = 100;
	AttackPosY = 100;

	AliveFlag = true;
	MoveFlag = false;
	AttackChoiceFlag = false;


	cat = TYATORA;

	//���g���̃e�N�X�`���̏�����
	TyatoraTex.Load("Material/tyatora.png");
	//���W�̐ݒ�
	TyatoraSprite.SetPos(x * 200, y * 150);
	//�傫���̐ݒ�
	TyatoraSprite.SetSize(100, 100);

	//�L�����̗̑͂�3
	HitPoint = 3;
}
//�`��֐�
void Tyatora::Draw()
{
	Direct3D::SetRenderState(RENDER_ALPHABLEND);
	Direct3D::DrawSprite(TyatoraSprite, TyatoraTex);
}
//���ۂɓ������֐�
void Tyatora::Update()
{
	TyatoraSprite.SetPos((x + 1) * AttackPosX, (y + 1) * AttackPosY);

	if (HitPoint <= 0)
	{
		AliveFlag = false;
	}

	UnitFade();
}

void Tyatora::UnitBattle(CatState cat, int *hp, bool attackFlag)
{
	if (attackFlag == true && MoveFlag == true)
	{
		//cat1�����g���̏ꍇ�ɏ�������퓬����
		if (cat == KURO)
		{
			*hp - 2;
		}
		else if (cat == TYATORA)
		{
			*hp - 1;
		}
	}
}

void Tyatora::UnitFade()
{
	if (AliveFlag == false)
	{
		TyatoraSprite.SetAlpha(TyatoraSprite.GetAlpha() + (0.1f*FADE_OUT_CHANGENUM));
	}
}