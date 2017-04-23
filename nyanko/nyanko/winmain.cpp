#include <Windows.h>

#include <tchar.h>
#pragma comment(lib,"winmm.lib")

#include "Direct3D.h"
#include "DirectSound.h"
#include "Texture.h"
#include "Sprite.h"
#include "Wave.h"
#include "SoundBuffer.h"
#include "directInput.h"
#include "Xinput.h"

#include "mesh.h"
#include "camera.h"
#include "BoundingBox.h"

#include "Game/GameState.h"
#include "Game/Title.h"


void ObbMove(OrientedBoundingBox& obb,float rotate, D3DXVECTOR3 pos)
{
	
	D3DXVECTOR3 forward;
	D3DXVECTOR3 right;
	D3DXVECTOR3 up;
	
	
	forward.x = cos(rotate + (D3DX_PI / 2));
	forward.z = sin(rotate + (D3DX_PI / 2));
	forward.y = 0;

	right.x = cos(rotate);
	right.z = sin(rotate);
	right.y = 0;

	obb.UpdateInfo(pos, forward, right, up);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//メッセージ処理の為の関数
	//DispatchMessageによって呼び出される

	//ウィンドウクラスの登録時に各ウィンドウに設定される
	//アプリケーションがメッセージを取得したら呼び出されるように作る

	//WM_DESTROY 右上の×が押された ウィンドウが破棄されたとき
	//WM_MOVE　  ウィンドウの移動
	//WM_SIZE　  ウィンドウサイズの変更WM_SIZE
	//等

	//特別な処理を必要とする場合（または自分で独自の処理を行いたい）
	//はここでメッセージの種類に応じた処理を行う
	//とりあえず終了処理だけ
	switch (msg)
	{
	case WM_DESTROY: //右上の×が押された AltF4が押された時など ウィンドウの破棄時

		//メッセージキューに新しくWM_QUITメッセージを送る
		//いずれメッセージキューから取り出されメインループを終える(メインループの継続条件により)	
		PostQuitMessage(0);	

		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);//デフォルトのウィンドウプロシージャの実行
}

static const TCHAR * WC_BASIC = _T("Basic");//基本ウィンドウクラス名

HRESULT RegistClassEx(HINSTANCE hInstance)
{
	//ウィンドウの設定を保存する構造体
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(WNDCLASSEX));//wcexのアドレスから　WNDCLASSEXの容量分 0で初期化

	//構造体のサイズ
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.cbClsExtra = 0;//0固定
	wcex.cbWndExtra = 0;//0固定

	//Windowのスタイル
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	//WinMainのインスタンスハンドル
	wcex.hInstance = hInstance;
	//ウィンドウプロシージャのアドレス　関数ポインタの設定
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	//背景色の設定
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//メニュー
	wcex.lpszMenuName = NULL;
	//ウィンドウクラスの名前
	wcex.lpszClassName = WC_BASIC;
	//アイコン
	wcex.hIcon = (HICON)LoadImage(NULL, MAKEINTRESOURCE(IDI_APPLICATION), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	wcex.hIconSm = NULL;
	//カーソル
	wcex.hCursor = (HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

	//今設定したウィンドウクラスを登録　登録しないとウィンドウを作れない
	return RegisterClassEx(&wcex);
}

HRESULT MakeWindow(HINSTANCE hInstance,HWND & refHWnd,int Width =800,int Height=600)
{
	//クライアント領域のおおきさを決める
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right =Width;//幅
	rect.bottom = Height;//高さ

	//指定したクライアント領域を保持する大きさのウィンドウサイズを計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);

	HWND hWnd;//ウィンドウのハンドル　ウィンドウの識別に使う

	hWnd = CreateWindowEx
		(0,						//ウィンドウ拡張スタイル
		WC_BASIC,				//先に作ったウィンドウクラスの設定
		_T("にゃんこWARS"),			//ウィンドウのタイトル
		WS_OVERLAPPEDWINDOW,	//ウィンドウスタイル
		CW_USEDEFAULT,			//位置x座標
		CW_USEDEFAULT,			//位置y座標
		rect.right - rect.left,	//幅の長さ
		rect.bottom - rect.top,	//高さ
		NULL,					//親ウィンドウのハンドル
		NULL,					//メニューハンドル
		hInstance,				//Winmainのハンドル
		NULL
		);

	if (hWnd == NULL)
	{
		MessageBox(NULL, "ウィンドウの作成に失敗\nプログラムの終了", "盆娘。", MB_OK); // MB_OK　メッセージボックスのタイプ
		return S_FALSE;//プログラムの終了
	}

	ShowWindow(hWnd, SW_SHOWNORMAL);//ウィンドウをアクティブにして表示
	UpdateWindow(hWnd);//クライアント領域の更新

	refHWnd = hWnd;

	return S_OK;
}


//エントリーポイント　プログラムの開始関数

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	

	if (FAILED(RegistClassEx(hInstance))) //FAILED HRESULTの値が失敗を示しているならtrue
	{
		return 0;
	}

	HWND hWnd=NULL;

	if (FAILED(MakeWindow(hInstance,hWnd, 1200, 1000)))
	{
		return 0;
	}

	//Direct3Dデバイスの作成
	Direct3D* pDirect3d = Direct3D::GetInstance();//インスタンスへのポインタの取得
	if (pDirect3d->TryCreate(hWnd))
	{
		
	}

	//DirectSoundデバイス作成
	DirectSound * pDirectSound = DirectSound::GetInstance();
	if (pDirectSound->Create(hWnd))
	{
		
	}
	

	//DirectInput
	DirectInput* pDi = DirectInput::GetInstance();

	if (SUCCEEDED(pDi->Init(hWnd)))
	{
		//MessageBox(NULL, "DirectInput初期化完了", "タイトル", MB_OK);
	}

	
	Direct3D::GetInstance()->SetupProjectionMatrix();


	Camera camera;

	SceneManager scenemanager;
	scenemanager.Initialize();
	

	MSG msg = {};

	//msg.messageがquitになったらメインループを抜ける
	while (msg.message != WM_QUIT)
	{
		//メッセージキューの中にメッセージがあるかを調べて、msgに書き込みtrueを返す
		//もしメッセージがなければfalseを返す
		//PM_REMOVEを設定しているとメッセージの書き込みのあとメッセージキューから書き込み元のデータは削除される。
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//仮想キーメッセージを文字メッセージに変換し、それをメッセージキューにポストする
			TranslateMessage(&msg);
		
			//メッセージの割り当て　ウィンドウプロシージャでメッセージを処理する
			DispatchMessage(&msg);	
		}
		else
		{

			//メッセージの処理を行っていないときに実行する
			//ここに自分のしたい処理を書く

			//ここにゲームコードを書く

			//入力の更新
			
			pDi->Update();

			scenemanager.Update();
			
			//描画処理
			//コマ落ちさせる場合は処理を飛ばすことがある
			if (SUCCEEDED(pDirect3d->BeginScene()))
			{
				pDirect3d->ClearScreen();

				//ビュー行列の更新
				camera.UpdateViewMatrix();

				scenemanager.Draw();

				pDirect3d->EndScene();
				pDirect3d->Present();
			}


		}	
	}

	Direct3D::DestroyInstance();//インスタンスの削除

	return 0;
}