#pragma once

#include "Direct3D.h"

//注視点の指定　方法
enum CAMERA_MODE
{
	LOOK_AT_REL,//relative 相対座標
	LOOK_AT_ABS,//absolute 絶対座標
};

class Camera
{
private:
	D3DXVECTOR3 eyePoint;		//カメラ座標
	D3DXVECTOR3 relLookAtPoint;	//カメラの注視点
								//カメラ座標からの相対
	D3DXVECTOR3 absLookAtPoint;	//カメラ注視点絶対座標

	D3DXVECTOR3 lookAtPoint;	//カメラ注視点

	D3DXVECTOR3 upVec;			//3D空間上方向

	CAMERA_MODE cameraMode;		//注視点指定方法

public:
	Camera();
	~Camera();

	//各種値取得
	D3DXVECTOR3 GetEyePoint(){ return eyePoint; }
	D3DXVECTOR3 GetLookAtPoint(){ return lookAtPoint; }
	D3DXVECTOR3 GetRelLookAtPoint(){ return relLookAtPoint; }
	D3DXVECTOR3 GetAbsLookAtPoint(){ return absLookAtPoint; }

	D3DXVECTOR3 GetUpVector(){ return upVec; }

	//注視点の更新
	void UpdateLookAtPoint();

	//座標の設定関数
	void SetEyePoint(float x, float y, float z);
	void SetEyePoint(D3DXVECTOR3 pos);

	//注視点の設定
	void SetAbsLookAtPoint(float x, float y, float z);
	void SetAbsLookAtPoint(D3DXVECTOR3 pos);
	void SetRelLookAtPoint(float x, float y, float z);
	void SetRelLookAtPoint(D3DXVECTOR3 relPos);

	//カメラの位置　向きの行列を計算してデバイスに渡す
	void UpdateViewMatrix();

	void Move(float x, float y);
	void Rotate(float radYaw, float radPitch);

	//注視点指定方法の設定
	void SetCameraMode(CAMERA_MODE cmode);

};