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
	MikenekoTex[0].Load("Material/mikeneko.png");
	MikenekoTex[1].Load("Material/mikeneko2.png");
	//���W�̐ݒ�
	MikenekoSprite.SetPos(x * 200.0f, y * 150.0f);
	//�傫���̐ݒ�
	MikenekoSprite.SetSize(100, 100);

	HitPointTex.Load("Material/heart_gloss.png");
	for (int i = 0; i < 3; i++)
	{
		HitPointSprite[i].SetSize(30, 30);
		HitPointSprite[i].SetPos(x, y);
	}

	HitPointPos[0] = 0.7f;
	HitPointPos[1] = 1.0f;
	HitPointPos[2] = 1.3f;

	//�L�����̗̑͂�3
	HitPoint = 3;

	MovePossidleFlag = true;
}
//�`��֐�
void Mikeneko::Draw()
{
	Direct3D::SetRenderState(RENDER_ALPHABLEND);


	//�ړ������ゾ�����葊��̃^�[���Ȃ�΃��j�b�g�𔒍���
	if (MovePossidleFlag == true)
	{
		Direct3D::DrawSprite(MikenekoSprite, MikenekoTex[0]);
	}
	else if (MovePossidleFlag == false)
	{
		Direct3D::DrawSprite(MikenekoSprite, MikenekoTex[1]);
	}

	for (int i = 0; i < HitPoint; i++)
	{
		Direct3D::DrawSprite(HitPointSprite[i], HitPointTex);
	}
}
//���ۂɓ������֐�
void Mikeneko::Update()
{
	MikenekoSprite.SetPos((x + 1) * AttackPosX, (y + 1) * AttackPosY);

	//�q�b�g�|�C���g�̃n�[�g�̈ʒu������
	for (int i = 0; i < 3; i++)
	{
		HitPointSprite[i].SetPos((x + HitPointPos[i]) * AttackPosX, (y + 1.5f) * AttackPosY);
	}

	//�q�b�g�|�C���g��0�ȉ��ɂȂ�΃��j�b�g�͏���
	if (HitPoint <= 0)
	{
		AliveFlag = false;
	}

	//���j�b�g���t�F�[�h�A�E�g����֐�
	UnitFade();
}

//���j�b�g�����ꂽ���Ƀe�N�X�`�����t�F�[�h�A�E�g������
void Mikeneko::UnitFade()
{
	if (AliveFlag == false)
	{
		MikenekoSprite.SetAlpha(MikenekoSprite.GetAlpha() + (0.1f*CAT_FADE_OUT_CHANGENUM));
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
	KuronekoTex[0].Load("Material/kuroneko.png");
	KuronekoTex[1].Load("Material/kuroneko2.png");
	//���W�̐ݒ�
	KuronekoSprite.SetPos(x * 200, y * 150);
	//�傫���̐ݒ�
	KuronekoSprite.SetSize(100, 100);

	HitPointTex.Load("Material/heart_gloss.png");
	for (int i = 0; i < 3; i++)
	{
		HitPointSprite[i].SetSize(30, 30);
		HitPointSprite[i].SetPos(x, y);
	}

	HitPointPos[0] = 0.7f;
	HitPointPos[1] = 1.0f;
	HitPointPos[2] = 1.3f;

	//�L�����̗̑͂�3
	HitPoint = 3;

	MovePossidleFlag = true;
	
}
//�`��֐�
void Kuroneko::Draw()
{
	Direct3D::SetRenderState(RENDER_ALPHABLEND);

	//�ړ������ゾ�����葊��̃^�[���Ȃ�΃��j�b�g�𔒍���
	if (MovePossidleFlag == true)
	{
		Direct3D::DrawSprite(KuronekoSprite, KuronekoTex[0]);
	}
	else if (MovePossidleFlag == false)
	{
		Direct3D::DrawSprite(KuronekoSprite, KuronekoTex[1]);
	}

	for (int i = 0; i < HitPoint; i++)
	{
		Direct3D::DrawSprite(HitPointSprite[i], HitPointTex);
	}
}
//���ۂɓ������֐�
void Kuroneko::Update()
{
	KuronekoSprite.SetPos((x + 1) * AttackPosX, (y + 1) * AttackPosY);

	//�q�b�g�|�C���g�̃n�[�g�̈ʒu������
	for (int i = 0; i < 3; i++)
	{
		HitPointSprite[i].SetPos((x + HitPointPos[i]) * AttackPosX, (y + 1.5f) * AttackPosY);
	}

	//�q�b�g�|�C���g��0�ȉ��ɂȂ�΃��j�b�g�͏���
	if (HitPoint <= 0)
	{
		AliveFlag = false;
	}
	//���j�b�g���t�F�[�h�A�E�g����֐�S
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
		KuronekoSprite.SetAlpha(KuronekoSprite.GetAlpha() + (0.1f*CAT_FADE_OUT_CHANGENUM));
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
	TyatoraTex[0].Load("Material/tyatora.png");
	TyatoraTex[1].Load("Material/tyatora2.png");
	//���W�̐ݒ�
	TyatoraSprite.SetPos(x * 200, y * 150);
	//�傫���̐ݒ�
	TyatoraSprite.SetSize(100, 100);

	HitPointTex.Load("Material/heart_gloss.png");
	for (int i = 0; i < 3; i++)
	{
		HitPointSprite[i].SetSize(30, 30);
		HitPointSprite[i].SetPos(x, y);
	}

	HitPointPos[0] = 0.7f;
	HitPointPos[1] = 1.0f;
	HitPointPos[2] = 1.3f;

	//�L�����̗̑͂�3
	HitPoint = 3;

	MovePossidleFlag = true;
}
//�`��֐�
void Tyatora::Draw()
{
	Direct3D::SetRenderState(RENDER_ALPHABLEND);

	//�ړ������ゾ�����葊��̃^�[���Ȃ�΃��j�b�g�𔒍���
	if (MovePossidleFlag == true)
	{
		Direct3D::DrawSprite(TyatoraSprite, TyatoraTex[0]);
	}
	else if (MovePossidleFlag == false)
	{
		Direct3D::DrawSprite(TyatoraSprite, TyatoraTex[1]);
	}

	for (int i = 0; i < HitPoint; i++)
	{
		Direct3D::DrawSprite(HitPointSprite[i], HitPointTex);
	}
}
//���ۂɓ������֐�
void Tyatora::Update()
{
	TyatoraSprite.SetPos((x + 1) * AttackPosX, (y + 1) * AttackPosY);

	//�q�b�g�|�C���g�̃n�[�g�̈ʒu������
	for (int i = 0; i < 3; i++)
	{
		HitPointSprite[i].SetPos((x + HitPointPos[i]) * AttackPosX, (y + 1.5f) * AttackPosY);
	}

	//�q�b�g�|�C���g��0�ȉ��ɂȂ�΃��j�b�g�͏���
	if (HitPoint <= 0)
	{
		AliveFlag = false;
	}
	//���j�b�g���t�F�[�h�A�E�g����֐�S
	UnitFade();
}

void Tyatora::UnitBattle(CatState cat, int *hp, bool attackFlag)
{
	if (attackFlag == true && MoveFlag == true)
	{
		//cat1�����L�̏ꍇ�ɏ�������퓬����
		if (cat == KURO)
		{
			*hp -= 2;
		}
		else if (cat == TYATORA)
		{
			*hp -= 1;
		}
	}
}

void Tyatora::UnitFade()
{
	if (AliveFlag == false)
	{
		TyatoraSprite.SetAlpha(TyatoraSprite.GetAlpha() + (0.1f*CAT_FADE_OUT_CHANGENUM));
	}
}