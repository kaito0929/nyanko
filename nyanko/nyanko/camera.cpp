#include "camera.h"

Camera::Camera()
{
	//���_����z�̃v���X�����������悤�ɏ�����
	eyePoint = D3DXVECTOR3(0.0f, 10.0f, -5.0f);

	lookAtPoint = D3DXVECTOR3(0.0f, -2.0f, 1.0f);
	relLookAtPoint = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	absLookAtPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	upVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//�����_�̍X�V 
	UpdateLookAtPoint();
}

Camera::~Camera()
{

}

void Camera::UpdateLookAtPoint()
{
	//�����_�̎w����@�ɂ����
	//�����_�̌v�Z���@��ς���

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

//���W�̐ݒ�֐�
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

//�����_�̐ݒ�
void Camera::SetAbsLookAtPoint(float x, float y, float z)
{
	absLookAtPoint = D3DXVECTOR3(x, y, z);
	//UpdateLookAtPoint();//��΃��[�h���ǉ������܂ł͖��Ӗ�
}

void Camera::SetAbsLookAtPoint(D3DXVECTOR3 pos)
{
	absLookAtPoint = pos;
	//UpdateLookAtPoint();//��΃��[�h���ǉ������܂ł͖��Ӗ�
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

//�J�����̈ʒu�@�����̍s����v�Z���ăf�o�C�X�ɓn��
void Camera::UpdateViewMatrix()
{
	//�r���[�s��̍쐬
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &eyePoint, &lookAtPoint, &upVec);

	Direct3D::GetInstance()->SetViewMatrix(matView);
}

void Camera::SetCameraMode(CAMERA_MODE cmode)
{
	cameraMode = cmode;

	//���[�h�ύX�ɏ]����
	//�����_���ς��ꍇ������
	//�Ƃ������w�ǂ̃P�[�X�ŕς��
	UpdateLookAtPoint();
}

void Camera::Move(float front, float right)
{
	//�����̍��̉�]�ɍ��킹�Ĉړ������͕ς��
	//front + ���� - ���
	//right + �E   - ��

	//�����_�����Ύw��̎��̂�
	if (cameraMode == LOOK_AT_REL)
	{
		//���݌����Ă�������̊p�x���v�Z
		float rad = atan2(relLookAtPoint.z, relLookAtPoint.x);

		//���ʂւ̃x�N�g���i�W�����j��
		//x���W������z���W�����ŕ\��
		float frontZ = sin(rad);
		float frontX = cos(rad);
		D3DXVECTOR3 frontVec(frontX, 0, frontZ);

		//�p�x�����v����4����1����]
		//rad�������Ă�������ɑ΂��Đ^�E�̊p�x������
		rad -= 3.141592 / 2;

		//���ʂւ̃x�N�g���i�W�����j��
		//x���W������z���W�����ŕ\��
		float rightZ = sin(rad);
		float rightX = cos(rad);
		D3DXVECTOR3 rightVec(rightX, 0, rightZ);

		//�V�����J�������W���v�Z
		D3DXVECTOR3 newPos = eyePoint + (frontVec*front) + (rightVec*right);

		//�V�J�������W��K�p
		SetEyePoint(newPos.x, newPos.y, newPos.z);
	}
}

void Camera::Rotate(float radYaw, float radPitch)
{
	//�J�����̉�]

	//radYaw	y���𒆐S�ɂ�����]�̒l
	//radPitch  ���������𒆐S�ɂ�����]�̒l

	//roll�͎g�p���Ȃ�

	//�����_�����Ύw��̎��̂�
	if (cameraMode == LOOK_AT_REL)
	{
		double x = relLookAtPoint.x;
		double y = relLookAtPoint.y;
		double z = relLookAtPoint.z;

		//z��x���烈�[��]�̊p�x�����߂�	(rad)
		double firstRadYaw = atan2(relLookAtPoint.z, relLookAtPoint.x);

		//�O�p�֐��̉��@�藝����
		//��]��̃��[��]�̃T�C���A�R�T�C�������߂�
		float resultZ = sin(firstRadYaw)*cos(radYaw) + cos(firstRadYaw)*sin(radYaw);
		float resultX = cos(firstRadYaw)*cos(radYaw) - sin(firstRadYaw)*sin(radYaw);
		//�����܂ł����[��]�̏���

		//�s�b�`��]

		//���݂̃s�b�`�̃R�T�C��
		//x,z���ʂł̋���
		//sqrt ������
		double pitchX = (sqrt(x*x + z*z));

		//�s�b�`�p�����߂�
		double firstRadPitch = atan2(y, pitchX);

		//��]�̐��� +- ��36�x
		double addRadPitch = radPitch;//�p�x�̉��Z��
		if (addRadPitch + firstRadPitch >= 3.1415f*(1.0f / 5))
		{
			addRadPitch = (3.1415f*(1.0f / 5)) - firstRadPitch;
		}

		if (addRadPitch + firstRadPitch <= -3.1415f*(1.0f / 5))
		{
			addRadPitch = -(3.1415f*(1.0f / 5)) - firstRadPitch;
		}

		//�O�p�֐��̉��@�藝�ŉ�]
		float resultY = sin(firstRadPitch)*cos(addRadPitch) + cos(firstRadPitch)*sin(addRadPitch);
		float resultPitchX = cos(firstRadPitch)*cos(addRadPitch) - sin(firstRadPitch)*sin(addRadPitch);

		//�s�b�`��]�̃R�T�C�����k��
		resultX *= resultPitchX;
		resultZ *= resultPitchX;

		//�����_�̍X�V�ŉ�]�̓K�p
		relLookAtPoint = D3DXVECTOR3(resultX, resultY, resultZ);
	}
}