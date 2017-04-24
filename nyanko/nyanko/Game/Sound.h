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

	//タイトルの音楽
	WaveFile TitleMusic;
	SoundBuffer sb_TitleMusic;

	//メインゲームの音楽
	WaveFile MainMusic;
	SoundBuffer sb_MainMusic;

	//リザルト画面の音楽
	WaveFile ResultMusic;
	SoundBuffer sb_ResultMusic;

public:

	Sound();
	~Sound();

	void Initialize();
	void Update();

	//タイトル画面の音楽を流す関数
	void TitleSoundPlay();
	//タイトルの音楽を止める関数
	void TitleSoundStop();

	//メインゲームの音楽を流す関数
	void MainSoundPlay();
	//メインゲームの音楽を止める関数
	void MainSoundStop();

	//クリア画面の音楽を流す関数
	void ResultSoundPlay();
	//クリア画面の音楽を止める関数
	void ResultSoundStop();

};