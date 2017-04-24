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

#include "Sound.h"
#include "SoundEffect.h"

//�t�F�[�h�A�E�g�p�̐��l
#define FADE_OUT_CHANGENUM 1 

class Title : public BaseScene
{
private:

	//�^�C�g���̃e�N�X�`��
	Texture TitleTex;
	Sprite TitleSprite;

	//�G���^�[�L�[�������Ă��炤�w���̃e�N�X�`��
	Texture EnterTex;
	Sprite EnterSprite;

	//�t�F�[�h�A�E�g�p�̃e�N�X�`��
	Texture FadeTex;
	Sprite FadeSprite;

	//�t�F�[�h�A�E�g�����s���邩�̃t���O
	//�G���^�[�L�[���������Ȃ�true�ɂ���
	bool FadeFlag;

	//�G���^�[�L�[�̑���𑣂��t�H���g��_�ł����邽�߂̕ϐ�
	int EnterDrawCount;
	int EnterFlashingNum;

	Sound sound;
	SoundEffect se;

public:
	//�R���X�g���N�^
	Title::Title(ISceneChanger* changer);
	//�f�X�g���N�^
	~Title();

	//������
	void Initialize() override;
	//���ۂ̓���
	void Update() override;
	//�`��
	void Draw() override;

};