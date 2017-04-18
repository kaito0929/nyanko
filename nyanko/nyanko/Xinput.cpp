#include <windows.h>
#include "Xinput.h"
#include "Singleton.h"

//スタティックなメンバ変数の定義
//マスクとボタン番号の対応表
int XInput::GamePadState::ButtonCode[14] =
{
	XINPUT_GAMEPAD_DPAD_UP,		//上ボタン	0x0001
	XINPUT_GAMEPAD_DPAD_DOWN,	//下ボタン	0x0002
	XINPUT_GAMEPAD_DPAD_LEFT,	//左ボタン	0x0004
	XINPUT_GAMEPAD_DPAD_RIGHT,	//右ボタン	0x0008
	XINPUT_GAMEPAD_START,		//スタート  0x0010
	XINPUT_GAMEPAD_BACK,		//バック	0x0020
	XINPUT_GAMEPAD_LEFT_THUMB,	//左スティックの押し込み
	XINPUT_GAMEPAD_RIGHT_THUMB,	//右スティックの押し込み
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y
};

//インナークラスのコンストラクタ
//一番外側のクラス名::インナークラス名::関数名
XInput::GamePadState::GamePadState()
{
	for (int i = 0; i < 14; i++)
	{
		buttonCount[i] = buttonCountPrev[i] = 0;
	}

	for (int i = 0; i < 2; i++)
	{
		Stick s = { 0.0f, 0.0f };
		stick[i] = s;

		trigger[i] = 0;
	}
}

//インナークラスのデストラクタ
XInput::GamePadState::~GamePadState()
{
	//動的なメモリ確保などはないので処理なし
}

//インナークラス　更新
void XInput::GamePadState::Update(int padNum)
{
	//1フレーム前のCounterを保存
	for (int i = 0; i < 14; i++)
	{
		buttonCountPrev[i] = buttonCount[i];
	}

	//入力状態の取得が上手く行ったかどうか
	DWORD funcResult;

	//入力状態の取得
	//padNumが取得したいコントローラの番号 0~4が有効
	//第二引数が入力状態を保存する構造体へのポインタ(アドレス)
	//取得に成功すれば、戻り値が ERROR_SUCCESS が返る
	funcResult = XInputGetState(padNum, &state);

	if (funcResult == ERROR_SUCCESS)
	{
		//取得に成功した
		conected = true;

		//ボタンの更新
		for (int i = 0; i < 14; i++)
		{
			int code = ButtonCode[i];
			//wButtons 14個全てのボタンのオンオフを全てまとめたものです
			//変数のそれぞれのビットがボタンに対応している
			//マスクと&演算してそれぞれのビットの情報を取り出す必要がある
			if ((state.Gamepad.wButtons& code)!=0)
			{
				//押されているので押され続けているフレーム数を加算
				buttonCount[i]++;
			}
			else
			{
				buttonCount[i] = 0;
			}
		}

		//スティックの更新
		for (int i = 0; i < 2; i++)
		{
			//i==0で左スティック 1で右スティックの情報
			int x = (i == 0) ? state.Gamepad.sThumbLX : state.Gamepad.sThumbRX;
			int y = (i == 0) ? state.Gamepad.sThumbLY : state.Gamepad.sThumbRY;
			//フルに倒して
			//-方向だと-32768が最低
			//+方向だと 32767が最大

			//全く入力されていなくても小さな値が入ることがあるため
			//ある決まった絶対値以下の値を超えないと0として扱う
			//デッドゾーン
			//ただし xyのどちらかがデッドゾーンの外ならどちらも変更しない
			if (x<XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE&&x>-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE&&
				y<XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE&&y>-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{
				x = y = 0;
			}

			//倒されている状態をint型の数値から-1~0^1の範囲の少数に直す

			if (x > 0)
			{
				stick[i].x = (float)x / (float)32768;
			}
			else if (x < 0)
			{
				stick[i].x = (float)x / (float)32767;
			}
			else
			{
				stick[i].x = 0;
			}

			if (y > 0)
			{
				stick[i].y = (float)x / (float)32768;
			}
			else if (y < 0)
			{
				stick[i].y = (float)x / (float)32767;
			}
			else
			{
				stick[i].y = 0;
			}
		}

		//トリガーの入力状態
		for (int i = 0; i < 2; i++)
		{
			//i==0で左のトリガー 1で右のトリガー
			int x = (i == 0) ? state.Gamepad.bLeftTrigger : state.Gamepad.bRightTrigger;
			//全く押していない状態で0 フルに押し込んで255

			//デッドゾーンの内側にいるなら0として扱う
			if (x > 10)
			{
				trigger[i] = (float)x / 255.0;
			}
			else
			{
				trigger[i] = 0;
			}
		}
	}
	else
	{
		//コントローラ状態の取得に失敗
		//取得しようとしたコントローラ番号が未接続などの場合
		//xinputに非対応の場合など
		conected = false;

		//全て入力無しとして扱う
		for (int i = 0; i < 14; i++)
		{
			buttonCount[i] = 0;
		}
		for (int i = 0; i < 2; i++)
		{
			stick[i].x = stick[i].y = 0;
		}
		for (int i = 0; i < 2; i++)
		{
			trigger[i] = 0;
		}
	}
}

//インナークラス
//押された瞬間か
bool XInput::GamePadState::ButtonJustPressed(int buttonNum)
{
	//対応するボタンの範囲内か
	if (buttonNum >= 14 || buttonNum < 0)
	{
		//範囲内ではない
		return false;
	}

	if (buttonCount[buttonNum] == 1)
	{
		return true;
	}
	else
	{
		return false;
	}

}


//インナークラス
//離された瞬間か
bool XInput::GamePadState::ButtonJustRelease(int buttonNum)
{
	//対応するボタンの範囲内か
	if (buttonNum >= 14 || buttonNum < 0)
	{
		//範囲内ではない
		return false;
	}

	//継続フレーム数が0で
	//前回のフレームの継続フレーム数が1以上
	//なら離された瞬間
	if (buttonCount[buttonNum] == 0 && buttonCountPrev[buttonNum] >= 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//インナークラス
//押され続けたフレーム数
int  XInput::GamePadState::GetButtonCount(int buttonNum)
{
	//対応するボタンの範囲内か
	if (buttonNum >= 14 || buttonNum < 0)
	{
		//範囲外
		return 0;
	}

	return buttonCount[buttonNum];

}


//インナークラス
//スティックの傾き取得
Stick XInput::GamePadState::GetStick(int stickNum)
{
	//対応するスティックの範囲内か
	if (stickNum >= 0 && stickNum < 2)
	{
		//範囲内
		return stick[stickNum];
	}
	else
	{
		//範囲外
		//入力が無い状態と一緒のデータを返す
		Stick s = { 0, 0 };
		return s;
	}
}

//トリガーの状態取得　押されていないと0 完全に押し込まれていると1  中間地を0~1の小数で
float XInput::GamePadState::GetTrigger(int triggerNum)
{
	//対応するトリガーの範囲内か
	if (triggerNum >= 0 && triggerNum < 2)
	{
		//範囲内
		return trigger[triggerNum];
	}
	else
	{
		//範囲外
		return 0;
	}
}

//書くコントローラの入力状態の更新
void XInput::Update()
{
	for (int i = 0; i < 4; i++)
	{
		state[i].Update(i);
	}
}

bool XInput::ButtonJustPressed(int padNum, int buttonNum)
{
	//padNumが有効な範囲か
	if (padNum < 0 || padNum >= 4)
	{
		//範囲外
		return false;
	}

	return state[padNum].ButtonJustPressed(buttonNum);
}

bool XInput::ButtonJustRelease(int padNum, int buttonNum)
{
	//padNumが有効な範囲か
	if (padNum < 0 || padNum >= 4)
	{
		//範囲外
		return false;
	}

	return state[padNum].ButtonJustRelease(buttonNum);
}

int XInput::GetButtonCount(int padNum, int buttonNum)
{
	//padNumが有効な範囲か
	if (padNum < 0 || padNum >= 4)
	{
		//範囲外
		return false;
	}

	return state[padNum].GetButtonCount(buttonNum);
}

Stick XInput::GetStick(int padNum, int stickNum)
{
	if (padNum < 0 || padNum >= 4)
	{
		//範囲外
		Stick s = { 0.0f, 0.0f };
		return s;
	}

	return state[padNum].GetStick(stickNum);
}

float XInput::GetTrigger(int padNum, int triggerNum)
{
	if (padNum < 0 || padNum >= 4)
	{
		return 0;
	}

	return state[padNum].GetTrigger(triggerNum);
}