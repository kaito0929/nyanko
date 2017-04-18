#include "Sprite.h"

//�R���X�g���N�^
Sprite::Sprite()
{
	pos.x = pos.y = 0.0f;
	width = height = 0;
	rotate = 0.0f;
	alpha = 1.0f;
}
//�f�X�g���N�^
Sprite::~Sprite()
{
	//�������̊J���Ȃǂ̕K�v�Ȏ�
}

//���S���W��ݒ�(�X�N���[�����W)
void Sprite::SetPos(float x, float y)
{
	pos.x = x;
	pos.y = y;
}
//�T�C�Y�̐ݒ�
void Sprite::SetSize(int Width, int Height)
{
	width = Width;
	height = Height;
}
//��]�̐ݒ�
void Sprite::SetRotate(float Rotate)
{
	rotate = Rotate;
}

void  Sprite::SetAlpha(float a)
{
	if (a < 0.0f)
	{
		alpha = 0.0f;
	}
	else if (a>1.0f)
	{
		alpha = 1;
	}
	else
	{
		alpha = a;
	}

}