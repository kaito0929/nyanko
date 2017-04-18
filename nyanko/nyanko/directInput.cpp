#include "directInput.h"

DirectInput* DirectInput::pInstance = NULL;

//コンストラクタ
DirectInput::DirectInput()
{
	pDInput = NULL;
	pKeyDevice = NULL;
	pMouseDevice = NULL;

	initialized = false;

	//配列変数の初期化
	//全てを0で
	ZeroMemory(&KeyInput, sizeof(int)*INPUT_BUFFER_SIZE);
	ZeroMemory(&KeyInputPreview, sizeof(int)*INPUT_BUFFER_SIZE);
	ZeroMemory(&mouseButtonCounter, sizeof(int) * 8);
	//構造体の初期化
	ZeroMemory(&mouseInput, sizeof(DIMOUSESTATE2));
	ZeroMemory(&mouseInputPreview, sizeof(DIMOUSESTATE2));

	//マウス座標のデータを初期化
	mousePos.Set(0, 0);
	mousePosPreview.Set(0, 0);
	mousePosDelta.Set(0, 0);

}

//デストラクタ
DirectInput::~DirectInput()
{
	Release();
}

//初期化
HRESULT DirectInput::Init(HWND hwnd)
{
	hWnd = hwnd;

	if (initialized == false)
	{

		//DirectInputオブジェクトの作成
		HRESULT hresult = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, NULL);

		if (FAILED(hresult))
		{
			//失敗なので関数終了
			return hresult;
		}

		//オブジェクトが出来たのでキーボードの初期化
		hresult = InitKey();
		if (FAILED(hresult))
		{
			//失敗なので関数終了
			return hresult;
		}

		//マウスの初期化
		hresult = InitMouse();
		if (FAILED(hresult))
		{
			//失敗なので関数終了
			return hresult;
		}

		//全ての初期化が終了
		//初期化済みとする
		initialized = true;

	}

	return S_OK;
}

//キー初期化
HRESULT DirectInput::InitKey()
{
	HRESULT hresult;

	//デバイスオブジェクトの作成
	hresult = pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, NULL);
	//GUID_SysKeybord　作成するデバイスの種類

	if (FAILED(hresult))
	{
		//失敗なので関数終了
		return hresult;
	}

	//データフォーマット設定
	hresult = pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hresult))
	{
		//失敗なので関数終了
		return hresult;
	}

	//強調モードを設定
	hresult = pKeyDevice->SetCooperativeLevel(hWnd,DISCL_NONEXCLUSIVE|DISCL_BACKGROUND);
	if (FAILED(hresult))
	{
		//失敗なので関数終了
		return hresult;
	}

	//入力デバイスへのアクセス権を取得
	pKeyDevice->Acquire();

	return S_OK;

}

//マウス初期化
HRESULT DirectInput::InitMouse()
{
	HRESULT hresult;

	//マウスデバイスの作成　GUID_SysMouseを作るデバイスの種類
	hresult = pDInput->CreateDevice(GUID_SysMouse, &pMouseDevice, NULL);
	if (FAILED(hresult))
	{
		//失敗なので関数終了
		return hresult;
	}

	//データフォーマット
	hresult = pMouseDevice->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hresult))
	{
		//失敗なので関数終了
		return hresult;
	}

	//強調モードの設定
	hresult = pMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if (FAILED(hresult))
	{
		//失敗なので関数終了
		return hresult;
	}

	//軸モードの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;//相対	//軸モード

	hresult = pMouseDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(hresult))
	{
		//失敗なので関数終了
		return hresult;
	}

	//入力デバイスへのアクセス権を取得
	hresult = pMouseDevice->Acquire();
	if (FAILED(hresult))
	{
		//失敗なので関数終了
		return hresult;
	}

	RECT rect;
	GetClientRect(hWnd, &rect);
	int Width = rect.right - rect.left;
	int Height = rect.bottom - rect.top;

	POINT localPos;
	localPos.x = Width / 2;
	localPos.y = Height / 2;

	//マウスのポジションはクライアント座標で管理したい
	//mousePosとmousePosPreviewが同じ値でスタートさせたい
	mousePos.Set(localPos.x, localPos.y);
	mousePosPreview.Set(localPos.x, localPos.y);
	mousePosDelta.Set(0, 0);

	//クライアント座標からスクリーン座標への変換
	//hWnd クライアントのウィンドウ
	ClientToScreen(hWnd, &localPos);
	//localPosがクライアント座標からスクリーン座標に変換された
	
	//マウスのスタートをウィンドウの中心にする
	//スクリーン座標での指定
	SetCursorPos(localPos.x, localPos.y);
	
	return S_OK;
}

//アップデート
HRESULT DirectInput::Update()
{
	//初期化出来ていなかったら
	//ポインタ型変数の準備が出来ていないためアクセス違反が起こる
	//ので関数を終える
	if (initialized == false)
	{
		return S_FALSE;
	}

	HRESULT hresult = pKeyDevice->Acquire();

	if ((hresult == DI_OK) || (hresult == S_FALSE))
		//S_FALSE以外の失敗は通さない
	{
		//前回のキー情報をpreviewに退避
		//メモリ丸ごとコピー
		memcpy(KeyInputPreview, KeyInput, sizeof(int)*INPUT_BUFFER_SIZE);

		//状態　ワーク(作業用の一時変数) t=temporaly 一時 work=作業
		BYTE tKeyInputWork[INPUT_BUFFER_SIZE];

		//キー状態をtKeyInputWorkに読み込む
		pKeyDevice->GetDeviceState(sizeof(BYTE)*INPUT_BUFFER_SIZE, &tKeyInputWork);

		for (int i = 0; i < INPUT_BUFFER_SIZE; i++)
		{
			if (tKeyInputWork[i])//キーが押されているか
			{
				if (KeyInput[i] < 2147483647)//int 最大値
				{
					//押され続けているフレーム数を加算する
					KeyInput[i]++;
				}
			}
			else
			{
				//押され続けているフレーム数を0にする
				KeyInput[i] = 0;
			}
		}
	}

	//マウスのボタンを取得
	hresult = pMouseDevice->Acquire();
	if ((hresult == DI_OK) || (hresult == S_FALSE))
		//S_FALSE以外の失敗は通さない
	{
		//前回のマウス状態を退避
		//メモリ丸ごとコピー
		memcpy(&mouseInputPreview, &mouseInput, sizeof(DIMOUSESTATE2));

		//mouseInputにマウスの入力状態を取得
		pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseInput);

		for (int i = 0; i < 8; i++)//全てのボタンをチェック
		{
			if (mouseInput.rgbButtons[i])//ボタンが押されている
			{
				if (mouseButtonCounter[i] < 2147483647)
				{
					//押され続けているフレーム数を加算
					mouseButtonCounter[i]++;
				}
			}
			else
			{
				//押され続けているフレーム数をリセット
				mouseButtonCounter[i] = 0;
			}
		}
	}

	//スクリーン中のマウス座標
	POINT screenPos;
	//カーソルの位置をスクリーン座標で取得
	GetCursorPos(&screenPos);

	//スクリーン座標からクライアント座標に変換
	POINT localPos = screenPos;
	ScreenToClient(hWnd, &localPos);

	//1フレーム前の位置を退避
	mousePosPreview = mousePos;
	//現在の座標を更新
	mousePos.Set(localPos.x, localPos.y);
	//移動した差分を計算
	mousePosDelta = mousePos - mousePosPreview;

	return S_OK;
}

//解放処理
HRESULT DirectInput::Release()
{
	if (pMouseDevice)
	{
		pMouseDevice->Unacquire();
		pMouseDevice->Release();
		pMouseDevice = NULL;
	}

	if (pKeyDevice)
	{
		pKeyDevice->Unacquire();
		pKeyDevice->Release();
		pKeyDevice = NULL;
	}

	if (pDInput)
	{
		pDInput->Release();
		pDInput = NULL;
	}
	initialized = false;

	return S_OK;
}

//キーが押されているならtrue
bool DirectInput::KeyState(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		//入力されたボタンの番号が範囲内

		if (KeyInput[code] > 0)
		{
			//フレーム数が1以上なら
			//押されている
			return true;
		}
		else
		{
			return false;
		}
	}

	//範囲外の場合は問答無用でfalse
	return false;
}

//1フレーム前
//キーが押されているならtrue
bool DirectInput::KeyStatePreview(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		//入力されたボタンの番号が範囲内

		if (KeyInputPreview[code] > 0)
		{
			//フレーム数が1以上なら
			//押されている
			return true;
		}
		else
		{
			return false;
		}
	}

	//範囲外の場合は問答無用でfalse
	return false;
}

//キーが押された瞬間か
bool DirectInput::KeyJustPressed(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		//継続フレーム数が1なら押された瞬間
		return  (KeyInput[code] == 1);
	}
	return false;
}

//キーが離された瞬間か
bool DirectInput::KeyJustReleased(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		//継続フレーム数が1なら押された瞬間
		return  (KeyInput[code] == 0) && (KeyInputPreview[code] >= 1);
	}
	return false;
}

//キーが押され続けているフレーム数
int DirectInput::KeyCount(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		return KeyInput[code];
	}
	//範囲外は0
	return 0;
}


//マウスのボタン状態
bool DirectInput::MouseButton(int num)
{
	if (num >= 0 && num < 8)
	{
		return (mouseInput.rgbButtons[num] >= 1);
	}
	else
	{
		//範囲外なので必ずfalse
		return false;
	}
}

//マウスのボタンが押された瞬間か
bool DirectInput::MouseButtonJustPressed(int num)
{
	if (num >= 0 && num < 8)
	{
		//前回が押されていなくて　今回が押されている
		//押された瞬間
		return (mouseInput.rgbButtons[num] >= 1) && (mouseInputPreview.rgbButtons[num] == 0);
	}
	else
	{
		//範囲外なので必ずfalse
		return false;
	}
}

//マウスボタンが離された瞬間か
bool DirectInput::MouseButtonJustReleased(int num)
{
	if (num >= 0 && num < 8)
	{
		//前回が押されていて　今回が押されていない
		//離された瞬間
		return (mouseInput.rgbButtons[num] == 0) && (mouseInputPreview.rgbButtons[num] >= 1);
	}
	else
	{
		//範囲外なので必ずfalse
		return false;
	}
}

//マウスボタンが押され続けているフレーム数
int DirectInput::MouseButtonCount(int num)
{
	if (num >= 0 && num < 8)
	{
		return mouseButtonCounter[num];
	}
	else
	{
		//範囲外はゼロ
		return 0;
	}
}


