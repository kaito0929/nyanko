#include "GameState.h"
#include "../directInput.h"
#include "../Xinput.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../mesh.h"
#include "../Direct3D.h"
#include <stdio.h>
#include <stdlib.h>	//rand()�֐��p
#include <time.h>	//time()�֐��p

int ax = 100;
int ay = 100;


GameState::GameState()
{
	
}

GameState::~GameState()
{

}

void GameState::Initialize()
{
	mike.x = 4;
	mike.y = 3;
	mike.AttackPosX = 100;
	mike.AttackPosY = 100;

	tyatora.x = 3;
	tyatora.y = 3;
	tyatora.AttackPosX = 100;
	tyatora.AttackPosY = 100;

	kuro.x = 5;
	kuro.y = 3;
	kuro.AttackPosX = 100;
	kuro.AttackPosY = 100;

	shiro.x = 2;
	shiro.y = 2;
	shiro.AttackPosX = 100;
	shiro.AttackPosY = 100;

	SearchX = 0;
	SearchX = 0;
	
	CursorX = 1;
	CursorY = 1;

	MoveCount = 2;

	flag[0] == false;
	flag[1] == false;
	flag[2] == false;

	//�O�єL�̃e�N�X�`���̏�����
	MikenekoTex.Load("Material/mikeneko.png");
	MikenekoSprite.SetPos(mike.x * 200, mike.y * 150);
	MikenekoSprite.SetSize(100, 100);

	//���g���̃e�N�X�`���̏�����
	TyatoraTex.Load("Material/tyatora.png");
	TyatoraSprite.SetPos(tyatora.x * 200, tyatora.y * 150);
	TyatoraSprite.SetSize(100, 100);

	//���L�̃e�N�X�`���̏�����
	KuronekoTex.Load("Material/kuroneko.png");
	KuronekoSprite.SetPos(kuro.x * 200, kuro.y * 150);
	KuronekoSprite.SetSize(100, 100);

	//���L�̃e�N�X�`���̏�����
	ShironekoTex.Load("Material/sironeko.png");
	ShironekoSprite.SetPos(shiro.x * 200, shiro.y * 150);
	ShironekoSprite.SetSize(100, 100);

	//�ړ��\�͈͂̃}�X�̃e�N�X�`���̏�����
	MoveSquaresTex.Load("Material/masu_b.png");
	MoveSquaresSprite.SetSize(100, 100);

	//�U���\�ꏊ�̃}�X�̃e�N�X�`���̏�����
	AttackSquaresTex.Load("Material/masu_r.png");
	AttackSquaresSprite.SetSize(100, 100);

	//�J�[�\���̃e�N�X�`���̏�����
	CursorTex.Load("Material/carsor.png");
	CursorSprite.SetSize(100, 100);

	//�U���R�}���h�̃e�N�X�`���̏�����
	AttackCommandTex.Load("Material/attack.png");
	AttackCommandSprite.SetSize(200, 100);
	AttackCommandSprite.SetPos(800, 450);

	//�ҋ@�R�}���h�̃e�N�X�`���̏�����
	StandbyCommandTex.Load("Material/taiki.png");
	StandbyCommandSprite.SetSize(200, 100);
	StandbyCommandSprite.SetPos(800, 550);

	//�߂�R�}���h�̃e�N�X�`���̏�����
	BackCommandTex.Load("Material/back.png");
	BackCommandSprite.SetSize(200, 100);
	BackCommandSprite.SetPos(800, 650);

	//�R�}���h�I�����̖��̃e�N�X�`��
	TheArrowTex.Load("Material/yazirusi.png");
	TheArrowSprite.SetSize(100, 50);
	TheArrowSprite.SetPos(650, 450);

	playerstate = CHOICE;
	command = ATTACK;
	attack = INITIALSTATE;

	AttackCommandDisplay = false;
	AttackSearchX = 0;
	AttackSearchY = 0;

	AliveFlag = true;

	for (int y = 0; y < MAPSIZE; y++)
	{
		for (int x = 0; x < MAPSIZE; x++)
		{
			if (y == 0 || x == 0 || y == 8 || x == 8)
			{
				Map[y][x] = -2;
			}
			else
			{
				Map[y][x] = -1;
			}
		}
	}

	ChackDirectionX[0] = 1;		//�E�̊m�F
	ChackDirectionY[0] = 0;
	ChackDirectionX[1] = 0;		//���̊m�F
	ChackDirectionY[1] = 1;
	ChackDirectionX[2] = -1;	//���̊m�F
	ChackDirectionY[2] = 0;
	ChackDirectionX[3] = 0;		//��̊m�F
	ChackDirectionY[3] = -1;


	MoveChoiceFlag = false;

	BeforeMapNumX = 0;
	BeforeMapNumY = 0;

	AttackUnitChoiceFlag = false;
	AttackCompleteFlag = false;

}

void GameState::Draw()
{
	Direct3D::SetRenderState(RENDER_ALPHABLEND);

	for (int y = 0; y < MAPSIZE; y++)
	{
		for (int x = 0; x < MAPSIZE; x++)
		{
			MoveSquaresSprite.SetPos((x + 1) * 100, (y + 1) * 100);
			AttackSquaresSprite.SetPos((x + 1) * 100, (y + 1) * 100);
			
			if (Map[y][x]>-1)
			{
				Direct3D::DrawSprite(MoveSquaresSprite, MoveSquaresTex);
			}
			else if (Map[y][x] == -4)
			{
				Direct3D::DrawSprite(AttackSquaresSprite, AttackSquaresTex);
			}

			Direct3D::DrawSprite(CursorSprite, CursorTex);
		}
	}

	Direct3D::DrawSprite(MikenekoSprite, MikenekoTex);
	Direct3D::DrawSprite(TyatoraSprite, TyatoraTex);
	Direct3D::DrawSprite(KuronekoSprite, KuronekoTex);
	Direct3D::DrawSprite(ShironekoSprite, ShironekoTex);

	if (playerstate == COMMAND)
	{
		if (AttackCommandDisplay == true)
		{
			Direct3D::DrawSprite(AttackCommandSprite, AttackCommandTex);
		}
		Direct3D::DrawSprite(StandbyCommandSprite, StandbyCommandTex);
		Direct3D::DrawSprite(BackCommandSprite, BackCommandTex);
		Direct3D::DrawSprite(TheArrowSprite, TheArrowTex);
	}
}

void GameState::Update()
{
	DirectInput* pDi = DirectInput::GetInstance();

	CursorSprite.SetPos((CursorX + 1) * 100, (CursorY + 1) * 100);
	MikenekoSprite.SetPos((mike.x + 1) * mike.AttackPosX, (mike.y + 1) * mike.AttackPosY);
	TyatoraSprite.SetPos((tyatora.x + 1) * tyatora.AttackPosX, (tyatora.y + 1) * tyatora.AttackPosY);
	KuronekoSprite.SetPos((kuro.x + 1) * kuro.AttackPosX, (kuro.y + 1) * kuro.AttackPosY);
	ShironekoSprite.SetPos((shiro.x + 1) * 100, (shiro.y + 1) * 100);
	
	//=���ړ��\�͈͂̕\���֐���================================

	Search();

	//===========================================================

	switch (playerstate)
	{
	case CHOICE://�ǂ̔L�𓮂��������߂�

		//�U���R�}���h�̕\���t���O��false�ɂ��Ă���
		AttackCommandDisplay = false;
		//�I��p�̃J�[�\���𓮂����֐�
		MoveCursor();

		//�}�b�v�����Z�b�g
		for (int y = 0; y < MAPSIZE; y++)
		{
			for (int x = 0; x < MAPSIZE; x++)
			{
				//�ړ��s�\�ȏꏊ��-2�ɂ��Ă���
				//����ȊO�̏ꏊ�͈ړ��\�ꏊ�Ƃ���-1��
				if (y == 0 || x == 0 || y == 8 || x == 8)
				{
					Map[y][x] = -2;
				}
				else
				{
					Map[y][x] = -1;
				}
			}
		}

		for (int i = 0; i < 3; i++)
		{
			flag[i] = false;
		}

		if (AliveFlag == true)
		{
			//���j�b�g�i�L�j������ꏊ��-3��
			Map[mike.y][mike.x] = -3;
			Map[shiro.y][shiro.x] = -3;
			Map[tyatora.y][tyatora.x] = -3;
			Map[kuro.y][kuro.x] = -3;
		}

		//���j�b�g��I��
		UnitChoice(mike.x, mike.y,&flag[0]);
		UnitChoice(tyatora.x, tyatora.y, &flag[1]);
		UnitChoice(kuro.x, kuro.y, &flag[2]);
		
		break;
	case MOVE://�L���ړ�������

		//�I��p�̃J�[�\�����ړ�������֐�
		MoveCursor();

		UnitMove(&mike.x, &mike.y,flag[0]);
		UnitMove(&tyatora.x, &tyatora.y,flag[1]);
		UnitMove(&kuro.x, &kuro.y, flag[2]);

		break;
	case COMMAND://�ҋ@�A�U���A�߂�Ƃ������s�������߂�R�}���h�I��

		//�ړ��������̏ꂩ��l���𒲂ׂčU���\�ȃ}�X�����邩�𒲂ׂ�
		for (int i = 0; i < CHACKNUM; i++)
		{
			AttackSearchX = CursorX + ChackDirectionX[i];
			AttackSearchY = CursorY + ChackDirectionY[i];

			//��A���A�E�A���̉������ɍU���\�}�X�������
			//�U���R�}���h��\��������
			if (Map[AttackSearchY][AttackSearchX] == -4)
			{
				AttackCommandDisplay = true;
			}
			else if (Map[AttackSearchY][AttackSearchX] == -3)
			{
				Map[AttackSearchY][AttackSearchX] = -4;
			}

		}

		//�R�}���h��I��������̈ړ�
		CommandChoice();

		//�I������R�}���h�ɂ���ĕω�
		switch (command)
		{
		case ATTACK://�U���t�F�[�Y�ɑJ��

			//���̈ʒu��ύX
			TheArrowSprite.SetPos(650, 450);
			if (pDi->KeyJustPressed(DIK_RETURN))
			{
				playerstate = ATTACKCHOICE;
			}
			break;
		case STANDBY://���̏�ɑҋ@����
			//���̈ʒu��ύX
			TheArrowSprite.SetPos(650, 550);
			if (pDi->KeyJustPressed(DIK_RETURN))
			{
				playerstate = CHOICE;
			}
			break;
		case BACK://���̏ꏊ�֖߂�
			//���̈ʒu��ύX
			TheArrowSprite.SetPos(650, 650);
			if (pDi->KeyJustPressed(DIK_RETURN))
			{
				mike.x = BeforeMapNumX;
				mike.y = BeforeMapNumY;
				playerstate = CHOICE;
			}
			break;
		}

		break;

	case ATTACKCHOICE://�U������G���j�b�g������
		
		if (AttackUnitChoiceFlag == false)
		{
			//�U���Ώۂ�I������J�[�\�����ړ�������֐�
			MoveCursor();
		}

		if (pDi->KeyJustPressed(DIK_RETURN))
		{
			if (Map[CursorY][CursorX] == -4)
			{
				//�t���O��true�ɂ��Ĉړ��J�n
				AttackUnitChoiceFlag = true;
			}
		}

		AttackMotion(mike.x, mike.y, &mike.AttackPosX, &mike.AttackPosY, &flag[0]);
		AttackMotion(tyatora.x, tyatora.y, &tyatora.AttackPosX, &tyatora.AttackPosY, &flag[1]);
		AttackMotion(kuro.x, kuro.y, &kuro.AttackPosX, &kuro.AttackPosY, &flag[2]);
		break;
	}
	
	UnitFade();
	
}

//�ړ��\�͈͂̕\���֐�
void GameState::Search()
{

	for (int x = 0; x < MAPSIZE; x++)
	{
		for (int y = 0; y < MAPSIZE; y++)
		{
			//Map�̐��l��1�ȏ�Ȃ炻�̏ꏊ����J�n
			if (Map[y][x]>=1)
			{
				for (int i = 0; i < CHACKNUM; i++)
				{
					//�O�єL�ׂ̗̃}�X�𒲂ׂ�
					SearchX = x + ChackDirectionX[i];
					SearchY = y + ChackDirectionY[i];

					//�c��ړ��񐔂ɂ�Map�̐��l����
					MoveCount = Map[y][x];

					//�t���O��true�Ȃ�ΌJ��Ԃ�
					while (MoveChoiceFlag == true)
					{
						//Map�̐��l��-1�Ȃ珈��
						//����ȊO�Ȃ�΃��[�v���甲����
						if (Map[SearchY][SearchX] == -1)
						{
							//�ړ��񐔂��}�C�i�X
							MoveCount--;

							//�c��ړ��񐔂�Map�ւƑ��
							Map[SearchY][SearchX] = MoveCount;

							//���l�����ꂼ��̕����։��Z
							SearchX += ChackDirectionX[i];
							SearchY += ChackDirectionY[i];

						}
						else if (Map[SearchY][SearchX] == -3)
						{
							Map[SearchY][SearchX] = -4;
							break;
						}
						else
						{
							break;
						}

						//�ړ��񐔂�0�ɂȂ����烋�[�v���甲����
						if (MoveCount == 0)
						{
							break;
						}
					}
				}
			}
		}
	}

}

//�ړ�����L��ꏊ��I������J�[�\���𑀍삷��֐�
void GameState::MoveCursor()
{
	DirectInput* pDi = DirectInput::GetInstance();

	//=���J�[�\���̈ړ���===================================================================
	if (pDi->KeyJustPressed(DIK_UP))
	{
		if (CursorY != 1)
		{
			CursorY--;
		}
	}

	if (pDi->KeyJustPressed(DIK_DOWN))
	{
		if (CursorY != 7)
		{
			CursorY++;
		}
	}

	if (pDi->KeyJustPressed(DIK_LEFT))
	{
		if (CursorX != 1)
		{
			CursorX--;
		}
	}

	if (pDi->KeyJustPressed(DIK_RIGHT))
	{
		if (CursorX != 7)
		{
			CursorX++;
		}
	}
}

//�U�����ҋ@����I�Ԗ��𑀍삷��֐�
void GameState::CommandChoice()
{
	DirectInput* pDi = DirectInput::GetInstance();

	//�R�}���h��I������J�[�\�����ړ�����

	//======================================================
	//�@ATTACK:�ׂɓG���j�b�g������Ȃ�ΑI���\
	//�@STANDBY:���������ɑҋ@
	//�@BACK:�s������O�ɖ߂�
	//======================================================

	switch(command)
	{
	case ATTACK:

		//�ׂɓG���j�b�g�����Ȃ��ꍇ�͑ҋ@�R�}���h�ɂȂ�悤�ɕύX
		if (AttackCommandDisplay == false)
		{
			command = STANDBY;
		}

		//��L�[�A���L�[�������āA�߂�R�}���h���ҋ@�R�}���h��I��
		if (pDi->KeyJustPressed(DIK_UP))
		{
			command = BACK;
		}
		if (pDi->KeyJustPressed(DIK_DOWN))
		{
			command = STANDBY;
		}
		break;
	case STANDBY:

		//��L�[�A���L�[�������āA�߂�R�}���h���U���R�}���h��I��
		//�ׂɓG���j�b�g�����Ȃ��ꍇ�A�U���R�}���h�Ɉړ����Ȃ��悤�ɂ���
		if (pDi->KeyJustPressed(DIK_UP))
		{
			if (AttackCommandDisplay == true)
			{
				command = ATTACK;
			}
			else
			{
				command = BACK;
			}
		}
		if (pDi->KeyJustPressed(DIK_DOWN))
		{
			command = BACK;
		}
		break;
	case BACK:

		//��L�[�A���L�[�������āA�ҋ@�R�}���h���U���R�}���h��I��
		//�ׂɓG���j�b�g�����Ȃ��ꍇ�A�U���R�}���h�Ɉړ����Ȃ��悤�ɂ���
		if (pDi->KeyJustPressed(DIK_UP))
		{
			command = STANDBY;
		}
		if (pDi->KeyJustPressed(DIK_DOWN))
		{
			if (AttackCommandDisplay == true)
			{
				command = ATTACK;
			}
			else
			{
				command = STANDBY;
			}
		}
		break;
	}

}

//�U�����Ă���悤�ɓ������֐�
void GameState::AttackMotion(int x, int y, int *attackPosX, int *attackPosY, bool *flag)
{
	if (*flag == true)
	{
		//�t���O��true�Ȃ�΍U���̕��������肷��
		if (AttackUnitChoiceFlag == true)
		{
			//�J�[�\���̈ʒu�ƃ��j�b�g�̈ʒu���r����
			//�ړ����������ύX����
			if (CursorY < y)
			{
				//�J�[�\���̈ʒu�����j�b�g��艺�������ꍇ�i���̏ꍇ��y���Ȃ̂Ő��l�����Ȃ�������j
				//���j�b�g�̐^��ֈړ�����悤�ɕύX
				//�ȉ������悤�ɂ���
				attack = UP;
			}
			else if (CursorY > y)
			{
				attack = DOWN;
			}
			else if (CursorX < x)
			{
				attack = LEFT;
			}
			else if (CursorX > x)
			{
				attack = RIGHT;
			}
		}

		//�ړ���Ԃ�RETURN�ȊO�Ȃ�΃t���O��false�ɂ��Ă���
		if (attack != RETURN)
		{
			AttackUnitChoiceFlag = false;
		}


		//��Ō��肵�������Ƀ��j�b�g�̍��W��ύX����
		//�ړ�����ɒB�����ꍇ�ɂ͌��̈ʒu�ւƖ߂�悤��
		//�ϐ������Z�����茸�Z�����肷��

		switch (attack)
		{
		case UP://������̓G�Ɍ������ē���
			if (*attackPosY >= MINUS_ATTACK_NUM)
			{
				ay--;
				*attackPosY = ay;
			}
			else
			{
				attack = RETURN;
			}
			break;
		case DOWN://�������̓G�Ɍ������ē���
			if (*attackPosY <= PLUS_ATTACK_NUM)
			{
				ay++;
				*attackPosY = ay;
			}
			else
			{
				attack = RETURN;
			}
			break;
		case RIGHT://�E�����̓G�Ɍ������ē���
			if (*attackPosX <= PLUS_ATTACK_NUM)
			{
				ax++;
				*attackPosX = ax;
			}
			else
			{
				attack = RETURN;
			}
			break;
		case LEFT://�������̓G�Ɍ������ē���
			if (*attackPosX >= MINUS_ATTACK_NUM)
			{
				ax--;
				*attackPosX = ax;
			}
			else
			{
				attack = RETURN;
			}
			break;
		case RETURN://���̈ʒu�֖߂�

			//�ϐ���100�ȉ���������100�ȏ�ł���Ȃ��
			//���Ɉʒu�ɖ߂�悤�ɕϐ������Z�����Z����
			if (*attackPosY < 100)
			{
				ay++;
				*attackPosY = ay;
			}
			else if (*attackPosY > 100)
			{
				ay--;
				*attackPosY = ay;
			}
			else if (*attackPosX < 100)
			{
				ax++;
				*attackPosX = ax;
			}
			else if (*attackPosX > 100)
			{
				ax--;
				*attackPosX = ax;
			}
			else
			{
				AliveFlag = false;
				attack = INITIALSTATE;
				playerstate = CHOICE;
				*flag = false;
			}

			break;
		}
	}

	

}

//���j�b�g�����ꂽ���Ƀe�N�X�`�����t�F�[�h�A�E�g������
void GameState::UnitFade()
{
	if (AliveFlag == false)
	{
		ShironekoSprite.SetAlpha(ShironekoSprite.GetAlpha() + (0.1f*FADE_OUT_CHANGENUM));
	}
}

//���j�b�g��I���������̏���
void GameState::UnitChoice(int x,int y,bool *flag)
{
	DirectInput* pDi = DirectInput::GetInstance();

	//�J�[�\���̈ʒu�ƔL�̈ʒu����v���Ă���
	//�G���^�[�L�[���������珈��
	if (CursorX == x && CursorY == y)
	{
		if (pDi->KeyJustPressed(DIK_RETURN))
		{
			//�ړ��\�ꏊ��T���t���O��true�ɂ���
			MoveChoiceFlag = true;
			//playerstate��Move�ɕύX
			//����ňړ��\��
			playerstate = MOVE;
			//�I�������L�̈ʒu��3��
			Map[CursorY][CursorX] = 3;

			//���j�b�g�̌��Ɉʒu�̕ϐ�����
			BeforeMapNumX = x;
			BeforeMapNumY = y;
			*flag = true;
		}
	}
}

//���j�b�g���ړ�������
void GameState::UnitMove(int *x, int *y,bool flag)
{
	DirectInput* pDi = DirectInput::GetInstance();

	if (pDi->KeyJustPressed(DIK_RETURN))
	{
		//�J�[�\���̈ʒu��-1�ȏ�3�����Ȃ�Ώ���
		if (Map[CursorY][CursorX] > -1 && Map[CursorY][CursorX] != 3 && flag == true)
		{
			//�L���J�[�\���̈ʒu�ֈړ�
			*x = CursorX;
			*y = CursorY;
			//�ړ��\�}�X�𒲂ׂ�t���O��false��
			MoveChoiceFlag = false;
			//�R�}���h�I���֑J��
			playerstate = COMMAND;
		}
	}
}