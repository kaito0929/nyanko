#pragma once

#include "Direct3D.h"

class Texture
{
	//Direct3Dのクラス関数から
	//このクラスの変数関数に
	//自由にアクセスできるようにする
	//アクセス指定の無視
	friend class Direct3D;

private:
	//ロードした画像データを保存
	IDirect3DTexture9* pTexture;

	//横をdivUの数で等分
	//縦をdivVの数で等分
	//したものとして扱う

	//UVの分割数
	//縦横の分割数
	unsigned int divU;//x方向の分割
	unsigned int divV;//y方向の分割

	//UV番号
	unsigned int numU;//xで何番目
	unsigned int numV;//yで何番目

public:
	//コンストラクタ	デストラクタ
	Texture();
	~Texture();

	//分割数の設定
	void SetDivide(unsigned int DivU,unsigned int DivV);

	//分割した何番目を使用するか決める関数
	void SetUVNum(unsigned int NumU, unsigned int NUmV);

	//画像のロード
	bool Load(TCHAR*FileName);//戻り値は成功でtrue

	//解放処理
	void Release();
};