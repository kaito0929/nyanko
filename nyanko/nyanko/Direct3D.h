#pragma once

#define D3D_DEBUG_INFO	//Direct3Dデバッグフラグ

#define D3DFVF_LINE_VERTEX D3DFVF_XYZ|D3DFVF_DIFFUSE

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment (lib,"comctl32.lib")
#pragma comment (lib,"winmm.lib")

#include <comdef.h>
#include <d3d9.h>//direct3d
#include <d3dx9.h>//direct3d
#include <mmsystem.h>

#include <Windows.h>

#include <tchar.h>
#include <windowsx.h>
#include <commctrl.h>
#include <WinUser.h>

#include "Singleton.h"

//#include <memory>//スマートポインタのやつ　

struct LINE_VERTEX
{
	D3DXVECTOR3 pos;
	DWORD color;
};



enum RENDER_STATE
{
	RENDER_DEFAULT,		//デフォルト
	RENDER_ALPHATEST,	//αテスト
	RENDER_HALFADD,		//半加算合成
	RENDER_ADD,		//加算合成

	RENDER_MESH,
	RENDER_ALPHABLEND,
};

//クラスの名前だけ宣言しておく
//ヘッダー内でクラス名だけ使用可能にする
//実体の方はcppでインクルード
class Texture;
class Sprite;
class Mesh;

class Direct3D
{
	friend class Singleton<Direct3D>;
public:
	typedef void(*FuncPointer)();//関数ポインタの型定義
private:
	//Direct3Dデバイス生成用オブジェクト
	IDirect3D9* pD3D9 = NULL;

	//Direct3Dデバイス
	//テクスチャのロードやポリゴンの表示に必要
	IDirect3DDevice9 *pDevice3D = NULL;

	bool isDeviceCreated;
	bool IsDeviceCreated(){ return isDeviceCreated; };//デバイスが作成済みかどうか

	bool Create(HWND hWnd);//デバイスの作成
	void Release();//デバイスの破棄	

	//シングルトンにする
	static Direct3D* pInstance ;

	//プライベートにしてクラス外から実行できなくする
	//クラス外から新しくオブジェクトが作られることを防いで
	//Direct3Dのインスタンスが一つだけしか存在できない状態にする
	Direct3D();//コンストラクタ
	~Direct3D();//デストラクタ
	void operator=(const Direct3D& obj){}	//代入演算
	Direct3D(const Direct3D& obj){};		//コピーコンストラクタ

	static RENDER_STATE currentState;
	
public :
	//オブジェクトの作成
	static Direct3D* GetInstance()
	{
		if (pInstance == NULL)
		{
			pInstance = new Direct3D;
		}
		return pInstance;
	}
	//オブジェクトの破棄
	static void DestroyInstance()
	{
		if (pInstance != NULL)
		{
			delete pInstance;
			pInstance = NULL;
		}
	}

	static void SetRenderState(RENDER_STATE renderState);

	bool TryCreate(HWND hWnd);//デバイス作成を試みる

	HRESULT BeginScene();//描画開始の合図
	HRESULT ClearScreen();//バックバッファのクリア
	HRESULT EndScene();//描画終了の合図
	HRESULT Present();//バックバッファをフロントに反映

	//テクスチャの読み込み
	//TextureクラスのLoad関数から実行
	//精工をtrueを返す
	bool LoadTexture(Texture& texture,TCHAR*FileName);

	//スプライトの描画
	static void DrawSprite(Sprite& sprite, Texture&texture, bool isTurn = false);
	
	//メッシュのロード
	void LoadMesh(Mesh& mesh, TCHAR* path);
	//メッシュの表示
	void DrawMesh(Mesh& mesh, D3DXMATRIXA16& worldMat);

	//ビュー行列の設定
	void SetViewMatrix(D3DXMATRIXA16& mat);

	//射影行列の設定
	void SetupProjectionMatrix();

	//直線の描画	count 線の本数 
	//pVertex[0]-[1]で一本[2][3]で一本
	//pVertexの要素数は常にcountの2倍
	void DrawLine(LINE_VERTEX* pVertex, int count);

	
};

