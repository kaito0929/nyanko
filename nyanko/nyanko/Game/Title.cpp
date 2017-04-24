#include "Title.h"
#include "../directInput.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../Direct3D.h"
#include "../Wave.h"
#include "../DirectSound.h"
#include "../SoundBuffer.h"



Title::Title(ISceneChanger* changer) : BaseScene(changer)
{

}

Title::~Title()
{

}

void Title::Initialize()
{
	//�^�C�g�����S
	TitleTex.Load("Material/title.png");
	TitleSprite.SetPos(600, 500);
	TitleSprite.SetSize(1200, 1100);

	//�G���^�[�������悤�Ɏw������e�N�X�`��
	EnterTex.Load("Material/enter.png");
	EnterSprite.SetPos(600, 700);
	EnterSprite.SetSize(600, 400);

	//�t�F�[�h�A�E�g�p�̃e�N�X�`��
	FadeTex.Load("Material/fade_b.png");
	FadeSprite.SetPos(600, 500);
	FadeSprite.SetSize(1200, 1100);
	FadeSprite.SetAlpha(0);

	sound.Initialize();
	se.Initialize();
}


void Title::Draw()
{
	Direct3D::SetRenderState(RENDER_ALPHABLEND);

	//�^�C�g����ʂ̕`��
	Direct3D::DrawSprite(TitleSprite, TitleTex);

	if (EnterFlashingNum == 0)
	{
		//�G���^�[�L�[�������悤�Ɏw��
		Direct3D::DrawSprite(EnterSprite, EnterTex);
	}

	//�t�F�[�h�A�E�g�p�̉摜��`��
	Direct3D::DrawSprite(FadeSprite, FadeTex);
}

void Title::Update()
{
	DirectInput* pDi = DirectInput::GetInstance();

	sound.TitleSoundPlay();

	//==���G���^�[�L�[�������悤�Ɏw�����o���e�N�X�`����_�ł����遡====================================
	//�J�E���g���v���X
	EnterDrawCount++;
	//�\����\����switch�Ő؂�ւ���
	switch (EnterFlashingNum)
	{
	case 0://�e�N�X�`���̕\��
		if (EnterDrawCount % 30 == 0)
		{
			EnterFlashingNum = 1;
		}
		break;
	case 1://�e�N�X�`���̔�\��
		if (EnterDrawCount % 30 == 0)
		{
			EnterFlashingNum = 0;
		}
		break;
	}
	//==================================================================================================


	//�G���^�[�L�[���������Ȃ�t���O��true�ɂ��ăt�F�[�h�A�E�g�J�n
	if (pDi->KeyJustPressed(DIK_RETURN))
	{
		if (FadeFlag == false)
		{
			se.DecisionSEPlay();
		}
		FadeFlag = true;
	}
	//FadeFlag��true�Ȃ���s����悤��
	if (FadeFlag == true)
	{
		FadeSprite.SetAlpha(FadeSprite.GetAlpha() + (0.01f*FADE_OUT_CHANGENUM));
	}

	//���S�ɉ�ʂ��Â��Ȃ����Ȃ�V�[�������C���Q�[���ɕύX
	if (FadeSprite.GetAlpha() == 1)
	{
		sound.TitleSoundStop();
		mSceneChanger->ChangeScene(STATE_MAIN);
	}
}


