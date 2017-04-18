#pragma once

#include "Direct3D.h"

//�|���S�����_���	��̒��_�̏��
//SPRITE_FVF�̓��e�ƍ��킹��K�v������
//MSDN���_�t�H�[�}�b�g�Q��
struct SpriteVertex
{
	//3�������W//x,y�����g��Ȃ���
	float x, y, z;
	//�X�N���[�����W�ɕϊ��ς݂��ǂ�����\��
	float rhw;

	DWORD ColorDefuse;

	//�e�N�X�`����UV���W
	float u, v;
};

class Sprite
{
	friend class Direct3D;
public:

	//FVF(�_��Ȓ��_�\����)
	static const DWORD SPRITE_FVF
		= D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	//XYZRHW	2D���W�ϊ��ς݂��ǂ����̃t���O������
	//TEX1		�e�N�X�`�����g�p����


private:

	//�X�v���C�g�ʒu
	//���S�_
	D3DXVECTOR2 pos;

	//�X�v���C�g�̑�؂�
	int width;
	int height;

	//��]	���W�A��
	float rotate;

	float alpha;

public:
	//�R���X�g���N�^
	Sprite();
	//�f�X�g���N�^
	~Sprite();

	//���S���W��ݒ�(�X�N���[�����W)
	void SetPos(float x, float y);
	//�T�C�Y�̐ݒ�
	void SetSize(int Width, int Height);
	//��]�̐ݒ�
	void SetRotate(float Rotate);

	float GetAlpha(){ return alpha; }
	void SetAlpha(float a);

};