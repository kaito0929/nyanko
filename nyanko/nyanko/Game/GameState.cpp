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
//�������֐�
void GameState::Initialize()
{
	//1P�̔L�̏�����
	mike_1P.Initialize();
	kuro_1P.Initialize();
	tyatora_1P.Initialize();

	//2P�̔L�̏�����
	mike_2P.Initialize();
	kuro_2P.Initialize();
	tyatora_2P.Initialize();

	mike_1P.x = 1;
	mike_1P.y = 7;

	kuro_1P.x = 2;
	kuro_1P.y = 7;

	tyatora_1P.x = 3;
	tyatora_1P.y = 7;

	mike_2P.x = 1;
	mike_2P.y = 1;

	kuro_2P.x = 2;
	kuro_2P.y = 1;

	tyatora_2P.x = 3;
	tyatora_2P.y = 1;

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
	BackGroundSprite.SetSize(1200, 1100);
	BackGroundSprite.SetPos(600, 500);

	//�J�b�g�C���p�̃e�N�X�`���i1P�j
	CutIn_FirstPlayerTex.Load("Material/turn1.png");
	CutIn_FirstPlayerSprite.SetSize(1200, 600);
	CutIn_FirstPlayerSprite.SetPos(600, 500);
	CutIn_FirstPlayerSprite.SetAlpha(1);

	//�J�b�g�C���p�̃e�N�X�`���i2P�j
	CutIn_SecondPlayerTex.Load("Material/turn2.png");
	CutIn_SecondPlayerSprite.SetSize(1200, 600);
	CutIn_SecondPlayerSprite.SetPos(600, 500);
	CutIn_SecondPlayerSprite.SetAlpha(0);

	//�t�F�[�h�A�E�g�p�̃e�N�X�`��
	FadeTex.Load("Material/fade_b.png");
	FadeSprite.SetPos(600, 500);
	FadeSprite.SetSize(1200, 1100);
	FadeSprite.SetAlpha(1);

	playerstate = CHOICE;
	command = ATTACK;
	attack = INITIALSTATE;
	playerTurn = FirstPlayer_Turn;
	fade = FADE_IN;

	AttackCommandDisplay = false;
	AttackSearchX = 0;
	AttackSearchY = 0;

	sound.Initialize();
	se.Initialize();

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
//�`��֐�
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


	//1P�̎O�єL��`��
	mike_1P.Draw();
	//1P�̍��L��`��
	kuro_1P.Draw();
	//1P�̒��g����`��
	tyatora_1P.Draw();

	//2P�̎O�єL��`��
	mike_2P.Draw();
	//2P�̍��L��`��
	kuro_2P.Draw();
	//2P�̒��g����`��
	tyatora_2P.Draw();

	//1P�̃J�b�g�C���`��
	Direct3D::DrawSprite(CutIn_FirstPlayerSprite, CutIn_FirstPlayerTex);
	//2P�̃J�b�g�C���`��
	Direct3D::DrawSprite(CutIn_SecondPlayerSprite, CutIn_SecondPlayerTex);

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
//���C���̓���
void GameState::Update()
{
	sound.MainSoundPlay();

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
			sound.MainSoundStop();
			mSceneChanger->ChangeScene(STATE_RESULT);
		}
	}

	//�J�[�\���̈ʒu��ύX�ł���悤�ɂ����ŏ���
	CursorSprite.SetPos((CursorX + 1) * 100, (CursorY + 1) * 100);


	//�ړ��\�͈͂̕\���֐�
	Search();

	//1P�̃��j�b�g�̏������s��
	mike_1P.Update();
	kuro_1P.Update();
	tyatora_1P.Update();

	//2P�̃��j�b�g�̏������s��
	mike_2P.Update();
	kuro_2P.Update();
	tyatora_2P.Update();

	//���v���C���[���ʂ̏����Ȃ̂ŕ�����K�v�͂Ȃ��̂ł����ŏ���
	if (playerstate == CHOICE)
	{
		//==��1P�p�̏�����=================================================================
		//�ړ������ǂ����̃t���O��false�ɂ��Ă���
		mike_1P.MoveFlag = false;
		kuro_1P.MoveFlag = false;
		tyatora_1P.MoveFlag = false;
		//�U���ΏۂƂ��đI�΂ꂽ���̃t���O��false�ɂ��Ă���
		mike_1P.AttackChoiceFlag = false;
		kuro_1P.AttackChoiceFlag = false;
		tyatora_1P.AttackChoiceFlag = false;
		//=================================================================================

		//==��2P�p�̏�����=================================================================
		//�ړ������ǂ����̃t���O��false�ɂ��Ă���
		mike_2P.MoveFlag = false;
		kuro_2P.MoveFlag = false;
		tyatora_2P.MoveFlag = false;
		//�U���ΏۂƂ��đI�΂ꂽ���̃t���O��false�ɂ��Ă���
		mike_2P.AttackChoiceFlag = false;
		kuro_2P.AttackChoiceFlag = false;
		tyatora_2P.AttackChoiceFlag = false;
		//=================================================================================
	}

	CutInFade();

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
	if (mike_1P.AliveFlag == false && kuro_1P.AliveFlag == false && tyatora_1P.AliveFlag == false)
	{
		FirstPlayerLosingFlag = true;
	}
	//2P���S�ł����ꍇ�ɕ����t���O��true�ɂ���
	//���̌�Ń��U���g��ʂ֑J��
	else if (mike_2P.AliveFlag == false && kuro_2P.AliveFlag == false && tyatora_2P.AliveFlag == false)
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

	//================================================================
	//�J�[�\���̈ړ��𐧌䂷��֐�
	//================================================================

	//�J�[�\���̏�ړ�
	if (pDi->KeyJustPressed(DIK_UP))
	{
		if (CursorY != 1)
		{
			CursorY--;
			//�ړ��������Ɍ��ʉ����Đ�
			se.ChoiceSEPlay();
		}
	}

	//�J�[�\���̉��ړ�
	if (pDi->KeyJustPressed(DIK_DOWN))
	{
		if (CursorY != 7)
		{
			CursorY++;
			//�ړ��������Ɍ��ʉ����Đ�
			se.ChoiceSEPlay();
		}
	}

	//�J�[�\���̍��ړ�
	if (pDi->KeyJustPressed(DIK_LEFT))
	{
		if (CursorX != 1)
		{
			CursorX--;
			//�ړ��������Ɍ��ʉ����Đ�
			se.ChoiceSEPlay();
		}
	}

	//�J�[�\���̉E�ړ�
	if (pDi->KeyJustPressed(DIK_RIGHT))
	{
		if (CursorX != 7)
		{
			CursorX++;
			//�ړ��������Ɍ��ʉ����Đ�
			se.ChoiceSEPlay();
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
			se.ChoiceSEPlay();
		}
		if (pDi->KeyJustPressed(DIK_DOWN))
		{
			command = STANDBY;
			se.ChoiceSEPlay();
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
				se.ChoiceSEPlay();
			}
			else
			{
				command = BACK;
				se.ChoiceSEPlay();
			}
		}
		if (pDi->KeyJustPressed(DIK_DOWN))
		{
			command = BACK;
			se.ChoiceSEPlay();
		}
		break;
	case BACK:

		//��L�[�A���L�[�������āA�ҋ@�R�}���h���U���R�}���h��I��
		//�ׂɓG���j�b�g�����Ȃ��ꍇ�A�U���R�}���h�Ɉړ����Ȃ��悤�ɂ���
		if (pDi->KeyJustPressed(DIK_UP))
		{
			command = STANDBY;
			se.ChoiceSEPlay();
		}
		if (pDi->KeyJustPressed(DIK_DOWN))
		{
			if (AttackCommandDisplay == true)
			{
				command = ATTACK;
				se.ChoiceSEPlay();
			}
			else
			{
				command = STANDBY;
				se.ChoiceSEPlay();
			}
		}
		break;
	}

}

//�U�����Ă���悤�ɓ������֐�
void GameState::AttackMotion(int x, int y, int *attackPosX, int *attackPosY, bool *moveFlag, bool *actionFlag)
{
	if (*moveFlag == true)
	{
		//�t���O��true�Ȃ�΍U���̕��������肷��
		if (AttackUnitChoiceFlag == true)
		{
			se.CatSEPlay();
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
				//1P��2P�ɍU�������ۂ̐퓬���e�̏���
				mike_1P.UnitBattle(tyatora_2P.cat, &tyatora_2P.HitPoint, tyatora_2P.AttackChoiceFlag);
				mike_1P.UnitBattle(mike_2P.cat, &mike_2P.HitPoint, mike_2P.AttackChoiceFlag);
				//2P��1P�ɍU�������ۂ̐퓬���e�̏���
				mike_2P.UnitBattle(tyatora_1P.cat, &tyatora_1P.HitPoint, tyatora_1P.AttackChoiceFlag);
				mike_2P.UnitBattle(mike_1P.cat, &mike_1P.HitPoint, mike_1P.AttackChoiceFlag);

				//1P��2P�ɍU�������ۂ̐퓬���e�̏���
				kuro_1P.UnitBattle(mike_2P.cat, &mike_2P.HitPoint, mike_2P.AttackChoiceFlag);
				kuro_1P.UnitBattle(kuro_2P.cat, &kuro_2P.HitPoint, kuro_2P.AttackChoiceFlag);
				//2P��1P�ɍU�������ۂ̐퓬���e�̏���
				kuro_2P.UnitBattle(mike_1P.cat, &mike_1P.HitPoint, mike_1P.AttackChoiceFlag);
				kuro_2P.UnitBattle(kuro_1P.cat, &kuro_1P.HitPoint, kuro_1P.AttackChoiceFlag);

				//1P��2P�ɍU�������ۂ̐퓬���e�̏���
				tyatora_1P.UnitBattle(kuro_2P.cat, &kuro_2P.HitPoint, kuro_2P.AttackChoiceFlag);
				tyatora_1P.UnitBattle(tyatora_2P.cat, &tyatora_2P.HitPoint, tyatora_2P.AttackChoiceFlag);
				//2P��1P�ɍU�������ۂ̐퓬���e�̏���
				tyatora_2P.UnitBattle(kuro_1P.cat, &kuro_1P.HitPoint, kuro_1P.AttackChoiceFlag);
				tyatora_2P.UnitBattle(tyatora_1P.cat, &tyatora_1P.HitPoint, tyatora_1P.AttackChoiceFlag);

				*actionFlag = false;

				attack = INITIALSTATE;
				playerstate = CHOICE;
				PlayerActionNum--;
			}
			break;
		}
	}
}

//���j�b�g��I���������̏���
void GameState::UnitChoice(int x,int y,bool *flag,bool moveFlag)
{
	DirectInput* pDi = DirectInput::GetInstance();

	//�J�[�\���̈ʒu�ƔL�̈ʒu����v���Ă���
	//�G���^�[�L�[���������珈��
	if (CursorX == x && CursorY == y&&moveFlag == true)
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
			se.DecisionSEPlay();
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
			se.DecisionSEPlay();
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

//���^�[���̎��Ƀ}�b�v�̐��l��؂�ւ���
void GameState::MyTurnMapChange(int x, int y, bool flag)
{
	if (flag == true)
	{
		Map[y][x] = -2;
	}
}

void GameState::UnitMovePossidleChange(bool aliveFlag, bool *moveFlag)
{
	if (aliveFlag == true)
	{
		*moveFlag = true;
	}
	else
	{
		*moveFlag = false;
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
		//���̊֐���1P�̃^�[���̎��Ɏ��s�����̂ł����ł̓G��2P�ɂȂ�
		MapChange(mike_2P.x, mike_2P.y, mike_2P.AliveFlag);
		MapChange(kuro_2P.x, kuro_2P.y, kuro_2P.AliveFlag);
		MapChange(tyatora_2P.x, tyatora_2P.y, tyatora_2P.AliveFlag);

		//�����̃��j�b�g�̈ʒu��-2�ɂ��Ă����Ė����Ɩ������d�Ȃ�Ȃ��悤�ɂ���
		//-2�Ȃ��Search�֐��̃��[�v��������̂�
		MyTurnMapChange(mike_1P.x, mike_1P.y, mike_1P.AliveFlag);
		MyTurnMapChange(kuro_1P.x, kuro_1P.y, kuro_1P.AliveFlag);
		MyTurnMapChange(tyatora_1P.x, tyatora_1P.y, tyatora_1P.AliveFlag);

		//�����̃��j�b�g��I��
		//���̊֐��͂��ꂩ��ړ����郆�j�b�g��MoveFlag�𑀍삷��
		//���Ɉړ��ς݂ł���Ȃ�ΑI���ł��Ȃ��悤�ɂȂ��Ă���
		UnitChoice(mike_1P.x, mike_1P.y, &mike_1P.MoveFlag, mike_1P.MovePossidleFlag);
		UnitChoice(kuro_1P.x, kuro_1P.y, &kuro_1P.MoveFlag, kuro_1P.MovePossidleFlag);
		UnitChoice(tyatora_1P.x, tyatora_1P.y, &tyatora_1P.MoveFlag, tyatora_1P.MovePossidleFlag);

		//�ő�s���񐔂�0�ɂȂ�΃^�[�����؂�ւ��
		//���j�b�g���ړ����A�ҋ@���U�����s�����ۂ�PlayerActionNum�̓}�C�i�X�����
		//�J�b�g�C�����t�F�[�h�C��������̂͂��̏�����
		if (PlayerActionNum == 0)
		{
			playerTurn = SecondPlayer_Turn;
			fade = FADE_OUT;
			PlayerActionNum = 3;

			//�G���j�b�g�A2P�̈ړ��\���̃t���O��true�ɂ��čs���\�ɂ���
			UnitMovePossidleChange(mike_2P.AliveFlag, &mike_2P.MovePossidleFlag);
			UnitMovePossidleChange(kuro_2P.AliveFlag, &kuro_2P.MovePossidleFlag);
			UnitMovePossidleChange(tyatora_2P.AliveFlag, &tyatora_2P.MovePossidleFlag);
		}

		//�S�Ẵ��j�b�g���s���s�\�Ȃ�΃^�[����؂�ւ���
		if (mike_1P.MovePossidleFlag == false && kuro_1P.MovePossidleFlag == false && tyatora_1P.MovePossidleFlag == false)
		{
			playerTurn = SecondPlayer_Turn;
			fade = FADE_OUT;
			PlayerActionNum = 3;

			//�G���j�b�g�A2P�̈ړ��\���̃t���O��true�ɂ��čs���\�ɂ���
			UnitMovePossidleChange(mike_2P.AliveFlag, &mike_2P.MovePossidleFlag);
			UnitMovePossidleChange(kuro_2P.AliveFlag, &kuro_2P.MovePossidleFlag);
			UnitMovePossidleChange(tyatora_2P.AliveFlag, &tyatora_2P.MovePossidleFlag);
		}
		

		break;
	case MOVE://�L���ړ�������

		//�I��p�̃J�[�\�����ړ�������֐�
		//���̏ꍇ�͈ړ�������肷�邽�߂Ɉړ�
		MoveCursor();

		//�I���������j�b�g���ړ�
		//�ړ����ォ�̃t���O��true�������ꍇ�ɂ݈̂ړ�
		//�J�[�\���̍��W��I������Ă��郆�j�b�g�̍��W�ɑ������
		UnitMove(&mike_1P.x, &mike_1P.y, mike_1P.MoveFlag);
		UnitMove(&kuro_1P.x, &kuro_1P.y, kuro_1P.MoveFlag);
		UnitMove(&tyatora_1P.x, &tyatora_1P.y, tyatora_1P.MoveFlag);

		break;
	case COMMAND://�ҋ@�A�U���A�߂�Ƃ������s�������߂�R�}���h�I��

		//�ړ��������̏ꂩ��l���𒲂ׂčU���\�ȃ}�X�����邩�𒲂ׂ�
		for (int i = 0; i < CHACKNUM; i++)
		{
			//�ړ�������̎l���𒲂ׂ�
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
				//�ׂ̐��l��-3��-4�Ȃ�΃}�b�v�̐��l��-5�ɂ��āA�U���\�Ƃ���
				Map[AttackSearchY][AttackSearchX] = -5;
			}

		}

		//�R�}���h��I��������̈ړ�
		CommandChoice();

		//�I�������R�}���h�̌��ʂ���������֐�
		//�ړ��������j�b�g�̕������������s��
		CommandPlay(&mike_1P.x, &mike_1P.y, mike_1P.MoveFlag, &mike_1P.MovePossidleFlag);
		CommandPlay(&kuro_1P.x, &kuro_1P.y, kuro_1P.MoveFlag, &kuro_1P.MovePossidleFlag);
		CommandPlay(&tyatora_1P.x, &tyatora_1P.y, tyatora_1P.MoveFlag, &tyatora_1P.MovePossidleFlag);

		break;

	case ATTACKCHOICE://�U������G���j�b�g������

		//�U�����łȂ���΃J�[�\���𓮂�����悤�ɂ��Ă���
		if (AttackUnitChoiceFlag == false)
		{
			//�U���Ώۂ�I������J�[�\�����ړ�������֐�
			MoveCursor();
		}

		//�G���^�[�L�[������������
		if (pDi->KeyJustPressed(DIK_RETURN))
		{
			//���̏�̃}�b�v�̐��l��-5�ł���Ώ���
			if (Map[CursorY][CursorX] == -5)
			{
				//�t���O��true�ɂ��Ĉړ��J�n
				AttackUnitChoiceFlag = true;

				//�U���ΏۂƂ��đI�΂ꂽ2P�L�����̃t���O��true�ɂ���
				//��������Ύl���ɂȂ��U���\�}�X��I�񂾏ꍇ�ł��U���͍s���Ȃ�
				AttackFlag(mike_2P.x, mike_2P.y, CursorX, CursorY, &mike_2P.AttackChoiceFlag);
				AttackFlag(kuro_2P.x, kuro_2P.y, CursorX, CursorY, &kuro_2P.AttackChoiceFlag);
				AttackFlag(tyatora_2P.x, tyatora_2P.y, CursorX, CursorY, &tyatora_2P.AttackChoiceFlag);
			}
		}

		//���j�b�g�����߂�ꂽ�����֍U�����Ă��镗�Ɍ�����
		//�J�[�\���ƃ��j�b�g�̈ʒu���r���ĕ���������
		//���̃��[�V�������I�������퓬�̌㏈�����s����HP�̌��Z��s���I���ɂ����肷��
		AttackMotion(mike_1P.x, mike_1P.y, &mike_1P.AttackPosX, &mike_1P.AttackPosY, &mike_1P.MoveFlag, &mike_1P.MovePossidleFlag);
		AttackMotion(kuro_1P.x, kuro_1P.y, &kuro_1P.AttackPosX, &kuro_1P.AttackPosY, &kuro_1P.MoveFlag, &kuro_1P.MovePossidleFlag);
		AttackMotion(tyatora_1P.x, tyatora_1P.y, &tyatora_1P.AttackPosX, &tyatora_1P.AttackPosY, &tyatora_1P.MoveFlag, &tyatora_1P.MovePossidleFlag);
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
		//���̊֐���2P�̃^�[���̎��Ɏ��s�����̂ł����ł̓G��1P�ɂȂ�
		MapChange(mike_1P.x, mike_1P.y, mike_1P.AliveFlag);
		MapChange(kuro_1P.x, kuro_1P.y, kuro_1P.AliveFlag);
		MapChange(tyatora_1P.x, tyatora_1P.y, tyatora_1P.AliveFlag);

		//�����̃��j�b�g�̈ʒu��-2�ɂ��Ă����Ė����Ɩ������d�Ȃ�Ȃ��悤�ɂ���
		//-2�Ȃ��Search�֐��̃��[�v��������̂�
		MyTurnMapChange(mike_2P.x, mike_2P.y, mike_2P.AliveFlag);
		MyTurnMapChange(kuro_2P.x, kuro_2P.y, kuro_2P.AliveFlag);
		MyTurnMapChange(tyatora_2P.x, tyatora_2P.y, tyatora_2P.AliveFlag);

		//�����̃��j�b�g��I��
		//���̊֐��͂��ꂩ��ړ����郆�j�b�g��MoveFlag�𑀍삷��
		//���Ɉړ��ς݂ł���Ȃ�ΑI���ł��Ȃ��悤�ɂȂ��Ă���
		UnitChoice(mike_2P.x, mike_2P.y, &mike_2P.MoveFlag, mike_2P.MovePossidleFlag);
		UnitChoice(kuro_2P.x, kuro_2P.y, &kuro_2P.MoveFlag, kuro_2P.MovePossidleFlag);
		UnitChoice(tyatora_2P.x, tyatora_2P.y, &tyatora_2P.MoveFlag, tyatora_2P.MovePossidleFlag);

		//�ő�s���񐔂�0�ɂȂ�΃^�[�����؂�ւ��
		//���j�b�g���ړ����A�ҋ@���U�����s�����ۂ�PlayerActionNum�̓}�C�i�X�����
		//�J�b�g�C�����t�F�[�h�C��������̂͂��̏�����
		if (PlayerActionNum == 0)
		{
			playerTurn = FirstPlayer_Turn;
			fade = FADE_OUT;
			PlayerActionNum = 3;

			//�G���j�b�g�A1P�̈ړ��\���̃t���O��true�ɂ��čs���\�ɂ���
			UnitMovePossidleChange(mike_1P.AliveFlag, &mike_1P.MovePossidleFlag);
			UnitMovePossidleChange(kuro_1P.AliveFlag, &kuro_1P.MovePossidleFlag);
			UnitMovePossidleChange(tyatora_1P.AliveFlag, &tyatora_1P.MovePossidleFlag);
		}

		//�S�Ẵ��j�b�g���s���s�\�Ȃ�΃^�[����؂�ւ���
		if (mike_2P.MovePossidleFlag == false && kuro_2P.MovePossidleFlag == false && tyatora_2P.MovePossidleFlag == false)
		{
			playerTurn = FirstPlayer_Turn;
			fade = FADE_OUT;
			PlayerActionNum = 3;

			//�G���j�b�g�A2P�̈ړ��\���̃t���O��true�ɂ��čs���\�ɂ���
			UnitMovePossidleChange(mike_1P.AliveFlag, &mike_1P.MovePossidleFlag);
			UnitMovePossidleChange(kuro_1P.AliveFlag, &kuro_1P.MovePossidleFlag);
			UnitMovePossidleChange(tyatora_1P.AliveFlag, &tyatora_1P.MovePossidleFlag);
		}

		break;
	case MOVE://�L���ړ�������

		//�I��p�̃J�[�\�����ړ�������֐�
		//���̏ꍇ�͈ړ�������肷�邽�߂Ɉړ�
		MoveCursor();

		//�I���������j�b�g���ړ�
		//�ړ����̃t���O��true�������ꍇ�ɂ݈̂ړ�
		//�J�[�\���̍��W��I������Ă��郆�j�b�g�̍��W�ɑ������
		UnitMove(&mike_2P.x, &mike_2P.y, mike_2P.MoveFlag);
		UnitMove(&kuro_2P.x, &kuro_2P.y, kuro_2P.MoveFlag);
		UnitMove(&tyatora_2P.x, &tyatora_2P.y, tyatora_2P.MoveFlag);

		break;
	case COMMAND://�ҋ@�A�U���A�߂�Ƃ������s�������߂�R�}���h�I��

		//�ړ��������̏ꂩ��l���𒲂ׂčU���\�ȃ}�X�����邩�𒲂ׂ�
		for (int i = 0; i < CHACKNUM; i++)
		{
			//�ړ�������̎l���𒲂ׂ�
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
				//�ׂ̐��l��-3��-4�Ȃ�΃}�b�v�̐��l��-5�ɂ��āA�U���\�Ƃ���
				Map[AttackSearchY][AttackSearchX] = -5;
			}

		}

		//�R�}���h��I��������̈ړ�
		CommandChoice();

		//�I�������R�}���h�̌��ʂ���������֐�
		//�ړ��������j�b�g�̕������������s��
		CommandPlay(&mike_2P.x, &mike_2P.y, mike_2P.MoveFlag, &mike_2P.MovePossidleFlag);
		CommandPlay(&kuro_2P.x, &kuro_2P.y, kuro_2P.MoveFlag, &kuro_2P.MovePossidleFlag);
		CommandPlay(&tyatora_2P.x, &tyatora_2P.y, tyatora_2P.MoveFlag, &tyatora_2P.MovePossidleFlag);

		break;

	case ATTACKCHOICE://�U������G���j�b�g������

		//�U�����łȂ���΃J�[�\���𓮂�����悤�ɂ��Ă���
		if (AttackUnitChoiceFlag == false)
		{
			//�U���Ώۂ�I������J�[�\�����ړ�������֐�
			MoveCursor();
		}

		//�G���^�[�L�[������������
		if (pDi->KeyJustPressed(DIK_RETURN))
		{
			//���̏�̃}�b�v�̐��l��-5�ł���Ώ���
			if (Map[CursorY][CursorX] == -5)
			{
				//�t���O��true�ɂ��Ĉړ��J�n
				AttackUnitChoiceFlag = true;

				//�U���ΏۂƂ��đI�΂ꂽ2P�L�����̃t���O��true�ɂ���
				//��������Ύl���ɂȂ��U���\�}�X��I�񂾏ꍇ�ł��U���͍s���Ȃ�
				AttackFlag(mike_1P.x, mike_1P.y, CursorX, CursorY, &mike_1P.AttackChoiceFlag);
				AttackFlag(kuro_1P.x, kuro_1P.y, CursorX, CursorY, &kuro_1P.AttackChoiceFlag);
				AttackFlag(tyatora_1P.x, tyatora_1P.y, CursorX, CursorY, &tyatora_1P.AttackChoiceFlag);
			}
		}

		//���j�b�g�����߂�ꂽ�����֍U�����Ă��镗�Ɍ�����
		//�J�[�\���ƃ��j�b�g�̈ʒu���r���ĕ���������
		//���̃��[�V�������I�������퓬�̌㏈�����s����HP�̌��Z��s���I���ɂ����肷��
		AttackMotion(mike_2P.x, mike_2P.y, &mike_2P.AttackPosX, &mike_2P.AttackPosY, &mike_2P.MoveFlag, &mike_2P.MovePossidleFlag);
		AttackMotion(kuro_2P.x, kuro_2P.y, &kuro_2P.AttackPosX, &kuro_2P.AttackPosY, &kuro_2P.MoveFlag, &kuro_2P.MovePossidleFlag);
		AttackMotion(tyatora_2P.x, tyatora_2P.y, &tyatora_2P.AttackPosX, &tyatora_2P.AttackPosY, &tyatora_2P.MoveFlag, &tyatora_2P.MovePossidleFlag);
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
void GameState::CommandPlay(int *x, int *y, bool flag,bool *moveFlag)
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
				se.DecisionSEPlay();
				playerstate = ATTACKCHOICE;
			}
			break;
		case STANDBY://���̏�ɑҋ@����
			//���̈ʒu��ύX
			TheArrowSprite.SetPos(650, 550);
			if (pDi->KeyJustPressed(DIK_RETURN))
			{
				se.DecisionSEPlay();
				//�v���C���[�̍s���񐔂��}�C�i�X
				PlayerActionNum--;
				//�s���ς݂Ƃ���
				*moveFlag = false;
				//�s�����j�b�g�����߂鏈���ɖ߂�
				playerstate = CHOICE;
			}
			break;
		case BACK://���̏ꏊ�֖߂�
			//���̈ʒu��ύX
			TheArrowSprite.SetPos(650, 650);
			if (pDi->KeyJustPressed(DIK_RETURN))
			{
				se.DecisionSEPlay();
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
	//���j�b�g�̍��W�ƃJ�[�\���̍��W����v�����Ȃ��
	if (x == cursorX&&y == cursorY)
	{
		//���̃��j�b�g��flag�𑀍삷��
		*flag = true;
	}
}

//�J�b�g�C���̃t�F�[�h�C���t�F�[�h�A�E�g
void GameState::CutInFade()
{
	//�S�̓I�ȃt�F�[�h�C���t�F�[�h�A�E�g�悤�̉摜�̃��l��0�Ȃ�Ώ��������悤��
	if (FadeSprite.GetAlpha() == 0)
	{
		//fade�ɂ���ăt�F�[�h�C���A�t�F�[�h�A�E�g��؂�ւ���
		switch (fade)
		{
		case FADE_IN://�t�F�[�h�C�����s��

			//playerTurn�ɂ���ăt�F�[�h�C�����s���摜��؂�ւ���
			if (playerTurn == FirstPlayer_Turn)
			{
				CutIn_FirstPlayerSprite.SetAlpha(CutIn_FirstPlayerSprite.GetAlpha() + (0.03f* FADE_IN_CHANGENUM));
			}
			else if (playerTurn == SecondPlayer_Turn)
			{
				CutIn_SecondPlayerSprite.SetAlpha(CutIn_SecondPlayerSprite.GetAlpha() + (0.03f* FADE_IN_CHANGENUM));
			}

			break;
		case FADE_OUT://�t�F�[�h�A�E�g���s��

			//playerTurn�ɂ���ăt�F�[�h�C�����s���摜��؂�ւ���
			if (playerTurn == FirstPlayer_Turn)
			{
				CutIn_FirstPlayerSprite.SetAlpha(CutIn_FirstPlayerSprite.GetAlpha() + (0.03f* FADE_OUT_CHANGENUM));
				//���l��1�ɂ܂ŉ��Z���ꂽ��PlayerTurn��FADE_IN�ɐ؂�ւ��ăt�F�[�h�C�����s���悤��
				if (CutIn_FirstPlayerSprite.GetAlpha() == 1)
				{
					//�J�b�g�C�����̌��ʉ��Đ�
					se.CutInSEPlay();
					fade = FADE_IN;
				}
			}
			//�������̓��e����L�Ɠ����Ȃ̂ŃR�����g�͏�L���Q�Ƃ��Ă�������
			else if (playerTurn == SecondPlayer_Turn)
			{
				CutIn_SecondPlayerSprite.SetAlpha(CutIn_SecondPlayerSprite.GetAlpha() + (0.03f* FADE_OUT_CHANGENUM));

				if (CutIn_SecondPlayerSprite.GetAlpha() == 1)
				{
					se.CutInSEPlay();
					fade = FADE_IN;
				}
			}
			break;
		}
	}
}