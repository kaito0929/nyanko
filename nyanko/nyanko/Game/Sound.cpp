#include "../directInput.h"
#include "GameState.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../Direct3D.h"
#include "../Wave.h"
#include "../DirectSound.h"
#include "../SoundBuffer.h"
#include "Sound.h"

Sound::Sound()
{

}

Sound::~Sound()
{

}

void Sound::Initialize()
{
	//�^�C�g���̉��֌W
	TitleMusic.Load("Sound/title.wav");
	sb_TitleMusic.Create(TitleMusic);

	//���C���Q�[���̉��֌W
	MainMusic.Load("Sound/main.wav");
	sb_MainMusic.Create(MainMusic);

	//�Q�[���N���A��ʂ̉��֌W
	ResultMusic.Load("Sound/result.wav");
	sb_ResultMusic.Create(ResultMusic);

}

void Sound::Update()
{

}

//�^�C�g���̉��y�𗬂��֐�
void Sound::TitleSoundPlay()
{
	sb_TitleMusic.Play(true);
}

//�^�C�g���̉��y���~�߂�֐�
void Sound::TitleSoundStop()
{
	sb_TitleMusic.Stop();
}

//���C���Q�[���̉��y�𗬂��֐�
void Sound::MainSoundPlay()
{
	sb_MainMusic.Play(true);
}

//���C���Q�[���̉��y���~�߂�֐�
void Sound::MainSoundStop()
{
	sb_MainMusic.Stop();
}

//���U���g��ʂ̉��y�𗬂��֐�
void Sound::ResultSoundPlay()
{
	sb_ResultMusic.Play(true);
}

//���U���g��ʂ̉��y���~�߂�֐�
void Sound::ResultSoundStop()
{
	sb_ResultMusic.Stop();
}