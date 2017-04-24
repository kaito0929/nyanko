#pragma once

#include "../mesh.h"
#include "../Direct3D.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../Wave.h"
#include "../DirectSound.h"
#include "../SoundBuffer.h"

class Sound
{
private:

	//�^�C�g���̉��y
	WaveFile TitleMusic;
	SoundBuffer sb_TitleMusic;

	//���C���Q�[���̉��y
	WaveFile MainMusic;
	SoundBuffer sb_MainMusic;

	//���U���g��ʂ̉��y
	WaveFile ResultMusic;
	SoundBuffer sb_ResultMusic;

public:

	Sound();
	~Sound();

	void Initialize();
	void Update();

	//�^�C�g����ʂ̉��y�𗬂��֐�
	void TitleSoundPlay();
	//�^�C�g���̉��y���~�߂�֐�
	void TitleSoundStop();

	//���C���Q�[���̉��y�𗬂��֐�
	void MainSoundPlay();
	//���C���Q�[���̉��y���~�߂�֐�
	void MainSoundStop();

	//�N���A��ʂ̉��y�𗬂��֐�
	void ResultSoundPlay();
	//�N���A��ʂ̉��y���~�߂�֐�
	void ResultSoundStop();

};