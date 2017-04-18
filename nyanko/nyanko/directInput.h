#pragma once

//DirectInputデバッグフラグ
#define D3D_DEBUG_INF0

//ダイレクトインプットバージョン
#define DIRECT_INPUT_VERSION 0x0800

//マウスボタン番号に名前を付ける
#define MOUSE_BUTTON_LEFT 0
#define MOUSE_BUTTON_RIGHT 1
#define MOUSE_BUTTON_MIDDLE 2

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"d3dxof.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")

#include <comdef.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>
#include <dinput.h>//directInput

#include "MyVector.h"

#define INPUT_BUFFER_SIZE 256

class DirectInput
{
public:

	//インスタンスの取得
	//インスタンスが無い場合は動的に作成
	static DirectInput* GetInstance()
	{
		if (pInstance == NULL)
		{
			pInstance = new DirectInput();
		}

		return pInstance;
	}

private:
	//privateなメンバ変数

	//インスタンスへのアドレス　これの指す先以外に
	//インスタンスが出来てはいけない
	static DirectInput* pInstance;

	//directinput オブジェクト
	LPDIRECTINPUT8 pDInput;

	//キーボード用のデバイス
	LPDIRECTINPUTDEVICE8 pKeyDevice;
	//マウス用デバイス
	LPDIRECTINPUTDEVICE8 pMouseDevice;

	//キーボード情報
	//何フレーム押され続けたか
	int KeyInput[INPUT_BUFFER_SIZE];
	//1フレーム前のKeyInputと一致
	int KeyInputPreview[INPUT_BUFFER_SIZE];

	//マウス情報
	//現在のマウス座標
	Vector2 mousePos;
	//1フレーム前の座標
	Vector2 mousePosPreview;
	//1フレーム前と現在のマウス座標の差分
	Vector2 mousePosDelta;

	//マウスのボタン
	DIMOUSESTATE2 mouseInput;
	//1フレーム前のマウスボタン
	DIMOUSESTATE2 mouseInputPreview;
	//何フレーム押され続けているか
	int mouseButtonCounter[8];//8 ボタン数

	//ウィンドウのハンドル
	HWND hWnd;

	//初期化済みか
	bool initialized;

public:
	//publicなメンバ変数


private:
	//privateなメンバ関数

	//-----------------------------------------------------------------
	//シングルトンにするということで
	//二つ目のインスタンスが発生するような処理は
	//privateにして公開しない
	DirectInput();
	~DirectInput();

	DirectInput(DirectInput& refDi);//コピーコンストラクタ
	DirectInput& operator =(DirectInput&refDi);//代入演算子のover load
	//------------------------------------------------------------------

	HRESULT InitKey();//キーボードの準備
	HRESULT InitMouse();//マウスの準備

public:
	//publicなメンバ関数

	//初期化の処理
	HRESULT Init(HWND hWnd);//この関数からInitKeyとInitMouse

	//入力状態の更新
	HRESULT Update();

	//解放処理
	HRESULT Release();

	//初期化済みか
	bool Initialized(){ return initialized; }

	//入力状態の取得関数

	//key
	bool KeyState(int code);//単純に押されているかどうか
	bool KeyStatePreview(int code);//前フレームで押されていたかどうか
	bool KeyJustPressed(int code);//押された瞬間か
	bool KeyJustReleased(int code);//離された瞬間
	int KeyCount(int code);//押されているフレーム数

	//mouse
	//マウス状態
	DIMOUSESTATE2 MouseState(){ return mouseInput; }
	//マウス状態　1フレーム前
	DIMOUSESTATE2 MouseStatePreview(){ return mouseInputPreview; }

	//現在の座標
	Vector2 MousePosition(){ return mousePos; }
	//1フレーム前の座標
	Vector2 MousePositionPreview(){ return mousePosPreview; }
	//今回のフレームで移動したマウス座標差分
	Vector2 MousePositionDelta(){ return mousePosDelta; }

	//マウスボタン　押されているか
	bool MouseButton(int num);
	//押された瞬間か　押されたフレームか
	bool MouseButtonJustPressed(int num);
	//離されたフレームか
	bool MouseButtonJustReleased(int num);
	//押され続けているフレーム数
	int MouseButtonCount(int num);

};