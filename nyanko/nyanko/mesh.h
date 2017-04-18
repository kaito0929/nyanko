#pragma once

#include "Direct3D.h"


class Mesh
{
	friend class Direct3D;

private:
	//メッシュデータが入るポインタ
	LPD3DXMESH pMesh;
	//ロードしたメッシュに適用されているマテリアルの数
	DWORD numMaterials;

	//メッシュに適用されたマテリアル//配列でメモリ確保します
	D3DMATERIAL9 * pMaterials;
	//マテリアルに対応するテクスチャ//配列でメモリ確保します
	LPDIRECT3DTEXTURE9* ppTextures;

public:
	Mesh();
	~Mesh();

	//表示関数
	void Draw(	D3DXMATRIXA16& mat_transform,	//3D座標　行列
				D3DXMATRIXA16& mat_scale,		//拡大率　行列
				D3DXMATRIXA16& mat_rotate);		//回転	  行列

	//ロード関数
	void Load(TCHAR* path);

};