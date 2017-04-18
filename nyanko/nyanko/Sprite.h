#pragma once

#include "Direct3D.h"

//板ポリゴン頂点情報	一つの頂点の情報
//SPRITE_FVFの内容と合わせる必要がある
//MSDN頂点フォーマット参照
struct SpriteVertex
{
	//3次元座標//x,yしか使わないが
	float x, y, z;
	//スクリーン座標に変換済みかどうかを表す
	float rhw;

	DWORD ColorDefuse;

	//テクスチャのUV座標
	float u, v;
};

class Sprite
{
	friend class Direct3D;
public:

	//FVF(柔軟な頂点構造体)
	static const DWORD SPRITE_FVF
		= D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	//XYZRHW	2D座標変換済みかどうかのフラグを持つ
	//TEX1		テクスチャを使用する


private:

	//スプライト位置
	//中心点
	D3DXVECTOR2 pos;

	//スプライトの大木さ
	int width;
	int height;

	//回転	ラジアン
	float rotate;

	float alpha;

public:
	//コンストラクタ
	Sprite();
	//デストラクタ
	~Sprite();

	//中心座標を設定(スクリーン座標)
	void SetPos(float x, float y);
	//サイズの設定
	void SetSize(int Width, int Height);
	//回転の設定
	void SetRotate(float Rotate);

	float GetAlpha(){ return alpha; }
	void SetAlpha(float a);

};