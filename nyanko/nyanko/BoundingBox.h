#pragma once
#include "Direct3D.h"

class OrientedBoundingBox
{
	//OBB

protected:
	D3DXVECTOR3 position;//ボックスの中心
	//向き(単位ベクトル)	
	//正面	右	上
	D3DXVECTOR3 NormDirection[3];

	//各軸の長さ
	//正面	右	上
	FLOAT length[3];

	//直方体	角	座標
	D3DXVECTOR3 vertexPos[8];

public:

	OrientedBoundingBox();
	virtual ~OrientedBoundingBox();

	void UpdateInfo(D3DXVECTOR3 newPos, 
					D3DXVECTOR3 newFor, 
					D3DXVECTOR3 newRight, 
					D3DXVECTOR3 newUp);

	void SetLength(float forward, float right, float up);

	void DrawLine();

private:
	//分離軸に直方体を投影した時の長さ
	//pSep　分離軸(単位ベクトル)
	//e1,e2,e3 
	//あるボックスの方向ベクトル(長さもある)1,2,3
	static float LengthSegmentOnSeparateAxis
		(D3DXVECTOR3* pSep,
		 D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3 = NULL);

public:

	//obb1とobb2が接触していればtrueを返す
	static bool Collision(OrientedBoundingBox& obb1, OrientedBoundingBox& obb2);
};