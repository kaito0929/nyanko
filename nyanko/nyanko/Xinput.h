#pragma once

#include <windows.h>
//xinputの前にwindows.hをインクルードすること　
//インクルード順がエラーに関係します
#include "Singleton.h"
#include <Xinput.h>
#pragma comment(lib,"xinput.lib")

//ボタンと配列　添え字の関係
#define GAMEPAD_DPAD_UP			0
#define GAMEPAD_DPAD_DOWN		1
#define GAMEPAD_DPAD_LEFT		2
#define GAMEPAD_DPAD_RIGHT		3
#define GAMEPAD_START			4
#define GAMEPAD_BACK			5
#define GAMEPAD_LEFT_THUMB		6
#define GAMEPAD_RIGHT_THUMB		7
#define GAMEPAD_LEFT_SHOULDER	8
#define GAMEPAD_RIGHT_SHOULDER	9
#define GAMEPAD_A				10
#define GAMEPAD_B				11
#define GAMEPAD_X				12
#define GAMEPAD_Y				13

#define STICK_LEFT		0
#define STICK_RIGHT		1

#define TRIGGER_LEFT	0
#define TRIGGER_RIGHT	1


//一つのスティックの入力状態
//-1～1　未入力で0
struct Stick
{
	float x;
	float y;
};

//4つコントローラの入力をまとめて管理するクラス
class XInput :public Singleton<XInput>
{
private:
	//インナークラス
	//クラスの中のクラス
	//今回はprivateで宣言するため
	//クラス外からは一切使用できない

	//一つのコントローラの入力を管理
	class GamePadState
	{
		//ボタン番号とマスクの関係
		//ボタン数　14
		static int ButtonCode[14];

		//それぞれのボタンの押され続けたフレーム数
		int buttonCount[14];
		int buttonCountPrev[14];

		//コントローラが接続されているか
		bool conected;

		//トリガーボタンの入力 0~1
		float trigger[2];

		//スティックの傾き
		Stick stick[2];

		//入力状態加工前
		//xinputから取得したままのデータ
		XINPUT_STATE state;

	public:
		//コンストラクタ　デストラクタ
		GamePadState();
		~GamePadState();

		//状態の更新
		void Update(int padNum);

		//ボタンが押された瞬間(フレーム)か
		bool ButtonJustPressed(int buttonNum);
		//離されたフレーム
		bool ButtonJustRelease(int buttonNum);
		//押され続けたフレーム数
		int GetButtonCount(int buttonNum);
		//トリガー入力状態
		float GetTrigger(int TriggerNum);
		//スティックの傾き状態
		Stick GetStick(int stickNum);
		//接続されているかどうか
		bool Conected(){ return conected; }
	};

	friend class Singleton<XInput>;

private:

	GamePadState state[4];

public:

	XInput(){}
	~XInput(){}

	//状態の更新
	void Update();

	//ボタンが押された瞬間(フレーム)か
	bool ButtonJustPressed(int padNum, int buttonNum);
	//離されたフレーム
	bool ButtonJustRelease(int padNum, int buttonNum);
	//押され続けたフレーム数
	int GetButtonCount(int padNum, int buttonNum);
	//トリガー入力状態
	float GetTrigger(int padNum, int TriggerNum);
	//スティックの傾き状態
	Stick GetStick(int padNum, int stickNum);



};