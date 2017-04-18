#include "camera.h"

Camera::Camera()
{
	//原点からzのプラス方向を向くように初期化
	eyePoint = D3DXVECTOR3(0.0f, 10.0f, -5.0f);

	lookAtPoint = D3DXVECTOR3(0.0f, -2.0f, 1.0f);
	relLookAtPoint = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	absLookAtPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	upVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//注視点の更新 
	UpdateLookAtPoint();
}

Camera::~Camera()
{

}

void Camera::UpdateLookAtPoint()
{
	//注視点の指定方法によって
	//注視点の計算方法を変える

	switch (cameraMode)
	{
	case LOOK_AT_REL:
		lookAtPoint = eyePoint + relLookAtPoint;
		break;
	case LOOK_AT_ABS:
		lookAtPoint = absLookAtPoint;
		break;
	}


}

//座標の設定関数
void Camera::SetEyePoint(float x, float y, float z)
{
	eyePoint = D3DXVECTOR3(x, y, z);
	UpdateLookAtPoint();
}

void Camera::SetEyePoint(D3DXVECTOR3 pos)
{
	eyePoint = pos;
	UpdateLookAtPoint();
}

//注視点の設定
void Camera::SetAbsLookAtPoint(float x, float y, float z)
{
	absLookAtPoint = D3DXVECTOR3(x, y, z);
	//UpdateLookAtPoint();//絶対モードが追加されるまでは無意味
}

void Camera::SetAbsLookAtPoint(D3DXVECTOR3 pos)
{
	absLookAtPoint = pos;
	//UpdateLookAtPoint();//絶対モードが追加されるまでは無意味
}

void Camera::SetRelLookAtPoint(float x, float y, float z)
{
	relLookAtPoint = D3DXVECTOR3(x, y, z);
	UpdateLookAtPoint();
}

void Camera::SetRelLookAtPoint(D3DXVECTOR3 relPos)
{
	relLookAtPoint = relPos;
	UpdateLookAtPoint();
}

//カメラの位置　向きの行列を計算してデバイスに渡す
void Camera::UpdateViewMatrix()
{
	//ビュー行列の作成
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &eyePoint, &lookAtPoint, &upVec);

	Direct3D::GetInstance()->SetViewMatrix(matView);
}

void Camera::SetCameraMode(CAMERA_MODE cmode)
{
	cameraMode = cmode;

	//モード変更に従って
	//注視点が変わる場合がある
	//というか殆どのケースで変わる
	UpdateLookAtPoint();
}

void Camera::Move(float front, float right)
{
	//自分の今の回転に合わせて移動方向は変わる
	//front + 正面 - 後ろ
	//right + 右   - 左

	//注視点が相対指定の時のみ
	if (cameraMode == LOOK_AT_REL)
	{
		//現在向いている方向の角度を計算
		float rad = atan2(relLookAtPoint.z, relLookAtPoint.x);

		//正面へのベクトル（標準化）を
		//x座標方向とz座標方向で表す
		float frontZ = sin(rad);
		float frontX = cos(rad);
		D3DXVECTOR3 frontVec(frontX, 0, frontZ);

		//角度を時計回りに4分の1周回転
		//radが向いている方向に対して真右の角度を示す
		rad -= 3.141592 / 2;

		//正面へのベクトル（標準化）を
		//x座標方向とz座標方向で表す
		float rightZ = sin(rad);
		float rightX = cos(rad);
		D3DXVECTOR3 rightVec(rightX, 0, rightZ);

		//新しいカメラ座標を計算
		D3DXVECTOR3 newPos = eyePoint + (frontVec*front) + (rightVec*right);

		//新カメラ座標を適用
		SetEyePoint(newPos.x, newPos.y, newPos.z);
	}
}

void Camera::Rotate(float radYaw, float radPitch)
{
	//カメラの回転

	//radYaw	y軸を中心にした回転の値
	//radPitch  水平方向を中心にした回転の値

	//rollは使用しない

	//注視点が相対指定の時のみ
	if (cameraMode == LOOK_AT_REL)
	{
		double x = relLookAtPoint.x;
		double y = relLookAtPoint.y;
		double z = relLookAtPoint.z;

		//zとxからヨー回転の角度を求める	(rad)
		double firstRadYaw = atan2(relLookAtPoint.z, relLookAtPoint.x);

		//三角関数の加法定理から
		//回転後のヨー回転のサイン、コサインを求める
		float resultZ = sin(firstRadYaw)*cos(radYaw) + cos(firstRadYaw)*sin(radYaw);
		float resultX = cos(firstRadYaw)*cos(radYaw) - sin(firstRadYaw)*sin(radYaw);
		//ここまでがヨー回転の処理

		//ピッチ回転

		//現在のピッチのコサイン
		//x,z平面での距離
		//sqrt 平方根
		double pitchX = (sqrt(x*x + z*z));

		//ピッチ角を求める
		double firstRadPitch = atan2(y, pitchX);

		//回転の制限 +- 約36度
		double addRadPitch = radPitch;//角度の加算量
		if (addRadPitch + firstRadPitch >= 3.1415f*(1.0f / 5))
		{
			addRadPitch = (3.1415f*(1.0f / 5)) - firstRadPitch;
		}

		if (addRadPitch + firstRadPitch <= -3.1415f*(1.0f / 5))
		{
			addRadPitch = -(3.1415f*(1.0f / 5)) - firstRadPitch;
		}

		//三角関数の加法定理で回転
		float resultY = sin(firstRadPitch)*cos(addRadPitch) + cos(firstRadPitch)*sin(addRadPitch);
		float resultPitchX = cos(firstRadPitch)*cos(addRadPitch) - sin(firstRadPitch)*sin(addRadPitch);

		//ピッチ回転のコサイン分縮小
		resultX *= resultPitchX;
		resultZ *= resultPitchX;

		//注視点の更新で回転の適用
		relLookAtPoint = D3DXVECTOR3(resultX, resultY, resultZ);
	}
}