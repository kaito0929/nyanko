#pragma once

#include "Direct3D.h"

class Texture
{
	//Direct3D�̃N���X�֐�����
	//���̃N���X�̕ϐ��֐���
	//���R�ɃA�N�Z�X�ł���悤�ɂ���
	//�A�N�Z�X�w��̖���
	friend class Direct3D;

private:
	//���[�h�����摜�f�[�^��ۑ�
	IDirect3DTexture9* pTexture;

	//����divU�̐��œ���
	//�c��divV�̐��œ���
	//�������̂Ƃ��Ĉ���

	//UV�̕�����
	//�c���̕�����
	unsigned int divU;//x�����̕���
	unsigned int divV;//y�����̕���

	//UV�ԍ�
	unsigned int numU;//x�ŉ��Ԗ�
	unsigned int numV;//y�ŉ��Ԗ�

public:
	//�R���X�g���N�^	�f�X�g���N�^
	Texture();
	~Texture();

	//�������̐ݒ�
	void SetDivide(unsigned int DivU,unsigned int DivV);

	//�����������Ԗڂ��g�p���邩���߂�֐�
	void SetUVNum(unsigned int NumU, unsigned int NUmV);

	//�摜�̃��[�h
	bool Load(TCHAR*FileName);//�߂�l�͐�����true

	//�������
	void Release();
};