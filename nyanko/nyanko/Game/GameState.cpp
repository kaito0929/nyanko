#include "GameState.h"
#include "Character.h"
#include "../directInput.h"
#include "../Xinput.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../mesh.h"
#include "../Direct3D.h"
#include <stdio.h>
#include <stdlib.h>	//rand()�֐��p
#include <time.h>	//time()�֐��p

//1P�̔s�k�t���O
bool FirstPlayerLosingFlag = false;
//2P�̔s�k�t���O
bool SecondPlayerLosingFlag = false;


GameState::GameState(ISceneChanger* changer) : BaseScene(changer)
{
	
}

GameState::~GameState()
{

}

void GameState::Initialize()
{
	
	for (int i = 0; i < 2; i++)
	{
		mike[i].Initialize();
		kuro[i].Initialize();
		tyatora[i].Initialize();
	}

	mike[0].x = 1;
	mike[0].y = 7;

	kuro[0].x = 2;
	kuro[0].y = 7;

	tyatora[0].x = 3;
	tyatora[0].y = 7;

	mike[1].x = 1;
	mike[1].y = 1;

	kuro[1].x = 2;
	kuro[1].y = 1;

	tyatora[1].x = 3;
	tyatora[1].y = 1;

	SearchX = 0;
	SearchX = 0;
	
	CursorX = 1;
	CursorY = 1;

	MoveCount = 2;

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

	//�w�i�̃e�N�X�`��
	BackGroundTex.Load("Material/tochi_akichi.png");
	BackGroundSprite.SetSize(1200, 1000);
	BackGroundSprite.SetPos(650, 450);

	//�t�F�[�h�A�E�g�p�̃e�N�X�`��
	FadeTex.Load("Material/fade_b.png");
	FadeSprite.SetPos(600, 500);
	FadeSprite.SetSize(1200, 1100);
	FadeSprite.SetAlpha(1);

	playerstate = CHOICE;
	command = ATTACK;
	attack = INITIALSTATE;
	playerTurn = FirstPlayer_Turn;

	AttackCommandDisplay = false;
	AttackSearchX = 0;
	AttackSearchY = 0;


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

	PlayerActionNum = 3;

	AttackUnitChoiceFlag = false;
	AttackCompleteFlag = false;

	FirstPlayerLosingFlag = false;
	SecondPlayerLosingFlag = false;

}

void GameState::Draw()
{
	Direct3D::SetRenderState(RENDER_ALPHABLEND);

	//�w�i�̕`��
	Direct3D::DrawSprite(BackGroundSprite, BackGroundTex);

	for (int y = 0; y < MAPSIZE; y++)
	{
		for (int x = 0; x < MAPSIZE; x++)
		{
			MoveSquaresSprite.SetPos((x + 1) * 100, (y + 1) * 100);
			AttackSquaresSprite.SetPos((x + 1) * 100, (y + 1) * 100);
			
			if (Map[y][x]>-1)
			{
				//�ړ��\�̐}�X��`��
				Direct3D::DrawSprite(MoveSquaresSprite, MoveSquaresTex);
			}
			else if (Map[y][x] <= -4)
			{
				//�ԃ}�X��`��
				Direct3D::DrawSprite(AttackSquaresSprite, AttackSquaresTex);
			}
			//���j�b�g��I��������ړ������肷��J�[�\���̕`��
			Direct3D::DrawSprite(CursorSprite, CursorTex);
		}
	}

	//Direct3D::DrawSprite(ShironekoSprite, ShironekoTex);

	for (int i = 0; i < 2; i++)
	{
		//�O�єL��`��
		mike[i].Draw();
		//���L��`��
		kuro[i].Draw();
		//���g����`��
		tyatora[i].Draw();
	}

	//�s���R�}���h�̕`��
	//�s���菇��COMMAND�̎��ɏ������s��
	if (playerstate == COMMAND)
	{
		//�l���ɍU���ł���G������Ȃ�΍U���R�}���h��\��
		if (AttackCommandDisplay == true)
		{
			//�U���R�}���h
			Direct3D::DrawSprite(AttackCommandSprite, AttackCommandTex);
		}
		//�ҋ@�R�}���h
		Direct3D::DrawSprite(StandbyCommandSprite, StandbyCommandTex);
		//�߂�R�}���h
		Direct3D::DrawSprite(BackCommandSprite, BackCommandTex);
		//�I��p�̖��
		Direct3D::DrawSprite(TheArrowSprite, TheArrowTex);
	}

	//�t�F�[�h�A�E�g�p�̉摜��`��
	Direct3D::DrawSprite(FadeSprite, FadeTex);
}

void GameState::Update()
{
	if (FirstPlayerLosingFlag == false && SecondPlayerLosingFlag == false)
	{
		//�t�F�[�h�C�����s��
		FadeSprite.SetAlpha(FadeSprite.GetAlpha() + (0.01f*FADE_IN_CHANGENUM));
	}
	else if (FirstPlayerLosingFlag == true || SecondPlayerLosingFlag == true)
	{
		//�t�F�[�h�A�E�g���s��
		FadeSprite.SetAlpha(FadeSprite.GetAlpha() + (0.01f*FADE_OUT_CHANGENUM));

		if (FadeSprite.GetAlpha() == 1)
		{
			mSceneChanger->ChangeScene(STATE_RESULT);
		}
	}

	//�J�[�\���̈ʒu��ύX�ł���悤�ɂ����ŏ���
	CursorSprite.SetPos((CursorX + 1) * 100, (CursorY + 1) * 100);


	//�ړ��\�͈͂̕\���֐�
	Search();

	for (int i = 0; i < 2; i++)
	{
		mike[i].Update();
		kuro[i].Update();
		tyatora[i].Update();

		//���v���C���[���ʂ̏����Ȃ̂ŕ�����K�v�͂Ȃ��̂ł����ŏ���
		if (playerstate == CHOICE)
		{
			//�ړ������ǂ����̃t���O��false�ɂ��Ă���
			mike[i].MoveFlag = false;
			kuro[i].MoveFlag = false;
			tyatora[i].MoveFlag = false;

			mike[i].AttackChoiceFlag = false;
			kuro[i].AttackChoiceFlag = false;
			tyatora[i].AttackChoiceFlag = false;
		}
	}
	

	switch (playerTurn)
	{
	case FirstPlayer_Turn:
		//1P�̍s��
		FirstPlayer_Update();
		break;
	case SecondPlayer_Turn:
		//2P�̍s��
		SecondPlayer_Update();
		break;
	}

	//1P���S�ł����ꍇ�ɕ����t���O��true�ɂ���
	//���̌�Ń��U���g��ʂ֑J��
	if (mike[0].AliveFlag == false && kuro[0].AliveFlag == false && tyatora[0].AliveFlag == false)
	{
		FirstPlayerLosingFlag = true;
	}
	//2P���S�ł����ꍇ�ɕ����t���O��true�ɂ���
	//���̌�Ń��U���g��ʂ֑J��
	else if (mike[1].AliveFlag == false && kuro[1].AliveFlag == false && tyatora[1].AliveFlag == false)
	{
		SecondPlayerLosingFlag = true;
	}

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
					//�L�ׂ̗̃}�X�𒲂ׂ�
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

//�U�����ҋ@���߂邩��I�Ԗ��𑀍삷��֐�
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
void GameState::AttackMotion(int x, int y, int *attackPosX, int *attackPosY, bool *moveFlag)
{
	if (*moveFlag == true)
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
				*attackPosY -= 1;
			}
			else
			{
				attack = RETURN;
			}
			break;
		case DOWN://�������̓G�Ɍ������ē���
			if (*attackPosY <= PLUS_ATTACK_NUM)
			{
				*attackPosY += 1;
			}
			else
			{
				attack = RETURN;
			}
			break;
		case RIGHT://�E�����̓G�Ɍ������ē���
			if (*attackPosX <= PLUS_ATTACK_NUM)
			{
				*attackPosX += 1;
			}
			else
			{
				attack = RETURN;
			}
			break;
		case LEFT://�������̓G�Ɍ������ē���
			if (*attackPosX >= MINUS_ATTACK_NUM)
			{
				*attackPosX -= 1;
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
				*attackPosY += 1;
			}
			else if (*attackPosY > 100)
			{
				*attackPosY -= 1;
			}
			else if (*attackPosX < 100)
			{
				*attackPosX += 1;
			}
			else if (*attackPosX > 100)
			{
				*attackPosX -= 1;
			}
			else
			{
				attack = INITIALSTATE;
				playerstate = CHOICE;
				PlayerActionNum--;
				
				//�O�єL�̐퓬���e�̏���
				mike[0].UnitBattle(tyatora[1].cat, &tyatora[1].HitPoint, tyatora[1].AttackChoiceFlag);
				mike[0].UnitBattle(mike[1].cat, &mike[1].HitPoint, mike[1].AttackChoiceFlag);
				//�O�єL�̐퓬���e�̏���
				mike[1].UnitBattle(tyatora[0].cat, &tyatora[0].HitPoint, tyatora[0].AttackChoiceFlag);
				mike[1].UnitBattle(mike[0].cat, &mike[0].HitPoint, mike[0].AttackChoiceFlag);
				
				//���L�̐퓬���e�̏���
				kuro[0].UnitBattle(mike[1].cat, &mike[1].HitPoint, mike[1].AttackChoiceFlag);
				kuro[0].UnitBattle(kuro[1].cat, &kuro[1].HitPoint, kuro[1].AttackChoiceFlag);
				//���L�̐퓬���e�̏���
				kuro[1].UnitBattle(mike[0].cat, &mike[0].HitPoint, mike[0].AttackChoiceFlag);
				kuro[1].UnitBattle(kuro[0].cat, &kuro[0].HitPoint, kuro[0].AttackChoiceFlag);

				//���g���̐퓬���e�̏���
				tyatora[0].UnitBattle(kuro[1].cat, &kuro[1].HitPoint, kuro[1].AttackChoiceFlag);
				tyatora[0].UnitBattle(tyatora[1].cat, &tyatora[1].HitPoint, tyatora[1].AttackChoiceFlag);
				//���g���̐퓬���e�̏���
				tyatora[1].UnitBattle(kuro[0].cat, &kuro[0].HitPoint, kuro[0].AttackChoiceFlag);
				tyatora[1].UnitBattle(tyatora[0].cat, &tyatora[0].HitPoint, tyatora[0].AttackChoiceFlag);
			}
			break;
		}
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
		//�J�[�\���̈ʒu��-1�ȏ�Ȃ�Ώ���
		if (Map[CursorY][CursorX] > -1 && flag == true)
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

//�}�b�v�̐��l��ς���֐�
void GameState::MapChange(int x,int y,bool flag)
{
	if (flag == true)
	{
		Map[y][x] = -3;
	}
}

//1P�̍s���֐�
void GameState::FirstPlayer_Update()
{
	DirectInput* pDi = DirectInput::GetInstance();

	switch (playerstate)
	{
	case CHOICE://�ǂ̔L�𓮂��������߂�

		//�U���R�}���h�̕\���t���O��false�ɂ��Ă���
		AttackCommandDisplay = false;
		//�I��p�̃J�[�\���𓮂����֐�
		MoveCursor();
		//�}�b�v�̐��l����x���Z�b�g���Ă���
		MapReset();
		
		//�G���j�b�g�̂���ꏊ��-3�ɂȂ�悤�ɂ���
		MapChange(mike[1].x, mike[1].y, mike[1].AliveFlag);
		MapChange(kuro[1].x, kuro[1].y, kuro[1].AliveFlag);
		MapChange(tyatora[1].x, tyatora[1].y, tyatora[1].AliveFlag);

		//���j�b�g��I��
		UnitChoice(mike[0].x, mike[0].y, &mike[0].MoveFlag);
		UnitChoice(kuro[0].x, kuro[0].y, &kuro[0].MoveFlag);
		UnitChoice(tyatora[0].x, tyatora[0].y, &tyatora[0].MoveFlag);

		//�ő�s���񐔂�0�ɂȂ�΃^�[�����؂�ւ��
		if (PlayerActionNum == 0)
		{
			playerTurn = SecondPlayer_Turn;
			PlayerActionNum = 3;
		}

		break;
	case MOVE://�L���ړ�������

		//�I��p�̃J�[�\�����ړ�������֐�
		MoveCursor();

		//�I���������j�b�g���ړ�
		UnitMove(&mike[0].x, &mike[0].y, mike[0].MoveFlag);
		UnitMove(&kuro[0].x, &kuro[0].y, kuro[0].MoveFlag);
		UnitMove(&tyatora[0].x, &tyatora[0].y, tyatora[0].MoveFlag);

		break;
	case COMMAND://�ҋ@�A�U���A�߂�Ƃ������s�������߂�R�}���h�I��

		//�ړ��������̏ꂩ��l���𒲂ׂčU���\�ȃ}�X�����邩�𒲂ׂ�
		for (int i = 0; i < CHACKNUM; i++)
		{
			AttackSearchX = CursorX + ChackDirectionX[i];
			AttackSearchY = CursorY + ChackDirectionY[i];

			//��A���A�E�A���̉������ɍU���\�}�X�������
			//�U���R�}���h��\��������
			if (Map[AttackSearchY][AttackSearchX] == -5)
			{
				AttackCommandDisplay = true;
			}
			else if (Map[AttackSearchY][AttackSearchX] == -3 || Map[AttackSearchY][AttackSearchX] == -4)
			{
				Map[AttackSearchY][AttackSearchX] = -5;
			}

		}

		//�R�}���h��I��������̈ړ�
		CommandChoice();

		//�I�������R�}���h�̌��ʂ���������֐�
		CommandPlay(&mike[0].x, &mike[0].y,mike[0].MoveFlag);
		CommandPlay(&kuro[0].x, &kuro[0].y, kuro[0].MoveFlag);
		CommandPlay(&tyatora[0].x, &tyatora[0].y, tyatora[0].MoveFlag);

		break;

	case ATTACKCHOICE://�U������G���j�b�g������

		if (AttackUnitChoiceFlag == false)
		{
			//�U���Ώۂ�I������J�[�\�����ړ�������֐�
			MoveCursor();
		}

		if (pDi->KeyJustPressed(DIK_RETURN))
		{
			if (Map[CursorY][CursorX] == -5)
			{
				//�t���O��true�ɂ��Ĉړ��J�n
				AttackUnitChoiceFlag = true;

				AttackFlag(mike[1].x, mike[1].y, CursorX, CursorY, &mike[1].AttackChoiceFlag);
				AttackFlag(kuro[1].x, kuro[1].y, CursorX, CursorY, &kuro[1].AttackChoiceFlag);
				AttackFlag(tyatora[1].x, tyatora[1].y, CursorX, CursorY, &tyatora[1].AttackChoiceFlag);
			}
		}

		//���j�b�g�����߂�ꂽ�����֍U��
		AttackMotion(mike[0].x, mike[0].y, &mike[0].AttackPosX, &mike[0].AttackPosY, &mike[0].MoveFlag);
		AttackMotion(kuro[0].x, kuro[0].y, &kuro[0].AttackPosX, &kuro[0].AttackPosY, &kuro[0].MoveFlag);
		AttackMotion(tyatora[0].x, tyatora[0].y, &tyatora[0].AttackPosX, &tyatora[0].AttackPosY, &tyatora[0].MoveFlag);
		break;
	}
}

//2P�̍s���֐�
void GameState::SecondPlayer_Update()
{
	DirectInput* pDi = DirectInput::GetInstance();

	switch (playerstate)
	{
	case CHOICE://�ǂ̔L�𓮂��������߂�

		//�U���R�}���h�̕\���t���O��false�ɂ��Ă���
		AttackCommandDisplay = false;
		//�I��p�̃J�[�\���𓮂����֐�
		MoveCursor();
		//�}�b�v�̐��l����x���Z�b�g���Ă���
		MapReset();

		//�G���j�b�g�̂���ꏊ��-3�ɂȂ�悤�ɂ���
		MapChange(mike[0].x, mike[0].y, mike[0].AliveFlag);
		MapChange(kuro[0].x, kuro[0].y, kuro[0].AliveFlag);
		MapChange(tyatora[0].x, tyatora[0].y, tyatora[0].AliveFlag);

		//���j�b�g��I��
		UnitChoice(mike[1].x, mike[1].y, &mike[1].MoveFlag);
		UnitChoice(kuro[1].x, kuro[1].y, &kuro[1].MoveFlag);
		UnitChoice(tyatora[1].x, tyatora[1].y, &tyatora[1].MoveFlag);

		//�ő�s���񐔂�0�ɂȂ�΃^�[�����؂�ւ��
		if (PlayerActionNum == 0)
		{
			playerTurn = FirstPlayer_Turn;
			PlayerActionNum = 3;
		}

		break;
	case MOVE://�L���ړ�������

		//�I��p�̃J�[�\�����ړ�������֐�
		MoveCursor();

		UnitMove(&mike[1].x, &mike[1].y, mike[1].MoveFlag);
		UnitMove(&kuro[1].x, &kuro[1].y, kuro[1].MoveFlag);
		UnitMove(&tyatora[1].x, &tyatora[1].y, tyatora[1].MoveFlag);

		break;
	case COMMAND://�ҋ@�A�U���A�߂�Ƃ������s�������߂�R�}���h�I��

		//�ړ��������̏ꂩ��l���𒲂ׂčU���\�ȃ}�X�����邩�𒲂ׂ�
		for (int i = 0; i < CHACKNUM; i++)
		{
			AttackSearchX = CursorX + ChackDirectionX[i];
			AttackSearchY = CursorY + ChackDirectionY[i];

			//��A���A�E�A���̉������ɍU���\�}�X�������
			//�U���R�}���h��\��������
			if (Map[AttackSearchY][AttackSearchX] == -5)
			{
				AttackCommandDisplay = true;
			}
			else if (Map[AttackSearchY][AttackSearchX] == -3 || Map[AttackSearchY][AttackSearchX] == -4)
			{
				Map[AttackSearchY][AttackSearchX] = -5;
			}

		}

		//�R�}���h��I��������̈ړ�
		CommandChoice();

		//�I�������R�}���h�̌��ʂ���������֐�
		CommandPlay(&mike[1].x, &mike[1].y, mike[1].MoveFlag);
		CommandPlay(&kuro[1].x, &kuro[1].y, kuro[1].MoveFlag);
		CommandPlay(&tyatora[1].x, &tyatora[1].y, tyatora[1].MoveFlag);

		break;

	case ATTACKCHOICE://�U������G���j�b�g������

		if (AttackUnitChoiceFlag == false)
		{
			//�U���Ώۂ�I������J�[�\�����ړ�������֐�
			MoveCursor();
		}

		if (pDi->KeyJustPressed(DIK_RETURN))
		{
			if (Map[CursorY][CursorX] == -5)
			{
				//�t���O��true�ɂ��Ĉړ��J�n
				AttackUnitChoiceFlag = true;

				AttackFlag(mike[0].x, mike[0].y, CursorX, CursorY, &mike[0].AttackChoiceFlag);
				AttackFlag(kuro[0].x, kuro[0].y, CursorX, CursorY, &kuro[0].AttackChoiceFlag);
				AttackFlag(tyatora[0].x, tyatora[0].y, CursorX, CursorY, &tyatora[0].AttackChoiceFlag);
			}
		}

		AttackMotion(mike[1].x, mike[1].y, &mike[1].AttackPosX, &mike[1].AttackPosY, &mike[1].MoveFlag);
		AttackMotion(kuro[1].x, kuro[1].y, &kuro[1].AttackPosX, &kuro[1].AttackPosY, &kuro[1].MoveFlag);
		AttackMotion(tyatora[1].x, tyatora[1].y, &tyatora[1].AttackPosX, &tyatora[1].AttackPosY, &tyatora[1].MoveFlag);
		break;
	}
}

//�}�b�v�̐��l�����Z�b�g����֐�
void GameState::MapReset()
{
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

}

//�R�}���h�̓��e����������֐�
void GameState::CommandPlay(int *x, int *y, bool flag)
{
	DirectInput* pDi = DirectInput::GetInstance();

	if (flag == true)
	{
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
				//�v���C���[�̍s���񐔂��}�C�i�X
				PlayerActionNum--;
				//�s�����j�b�g�����߂鏈���ɖ߂�
				playerstate = CHOICE;
			}
			break;
		case BACK://���̏ꏊ�֖߂�
			//���̈ʒu��ύX
			TheArrowSprite.SetPos(650, 650);
			if (pDi->KeyJustPressed(DIK_RETURN))
			{
				*x = BeforeMapNumX;
				*y = BeforeMapNumY;
				playerstate = CHOICE;
				break;
			}
		}
	}
}

//�U���ΏۂƂ��đI�΂ꂽ���̃t���O�𑀍삷��֐�
void GameState::AttackFlag(int x, int y, int cursorX, int cursorY, bool*flag)
{
	if (x == cursorX&&y == cursorY)
	{
		*flag = true;
	}
}