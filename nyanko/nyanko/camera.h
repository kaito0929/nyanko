#pragma once

#include "Direct3D.h"

//�����_�̎w��@���@
enum CAMERA_MODE
{
	LOOK_AT_REL,//relative ���΍��W
	LOOK_AT_ABS,//absolute ��΍��W
};

class Camera
{
private:
	D3DXVECTOR3 eyePoint;		//�J�������W
	D3DXVECTOR3 relLookAtPoint;	//�J�����̒����_
								//�J�������W����̑���
	D3DXVECTOR3 absLookAtPoint;	//�J���������_��΍��W

	D3DXVECTOR3 lookAtPoint;	//�J���������_

	D3DXVECTOR3 upVec;			//3D��ԏ����

	CAMERA_MODE cameraMode;		//�����_�w����@

public:
	Camera();
	~Camera();

	//�e��l�擾
	D3DXVECTOR3 GetEyePoint(){ return eyePoint; }
	D3DXVECTOR3 GetLookAtPoint(){ return lookAtPoint; }
	D3DXVECTOR3 GetRelLookAtPoint(){ return relLookAtPoint; }
	D3DXVECTOR3 GetAbsLookAtPoint(){ return absLookAtPoint; }

	D3DXVECTOR3 GetUpVector(){ return upVec; }

	//�����_�̍X�V
	void UpdateLookAtPoint();

	//���W�̐ݒ�֐�
	void SetEyePoint(float x, float y, float z);
	void SetEyePoint(D3DXVECTOR3 pos);

	//�����_�̐ݒ�
	void SetAbsLookAtPoint(float x, float y, float z);
	void SetAbsLookAtPoint(D3DXVECTOR3 pos);
	void SetRelLookAtPoint(float x, float y, float z);
	void SetRelLookAtPoint(D3DXVECTOR3 relPos);

	//�J�����̈ʒu�@�����̍s����v�Z���ăf�o�C�X�ɓn��
	void UpdateViewMatrix();

	void Move(float x, float y);
	void Rotate(float radYaw, float radPitch);

	//�����_�w����@�̐ݒ�
	void SetCameraMode(CAMERA_MODE cmode);

};