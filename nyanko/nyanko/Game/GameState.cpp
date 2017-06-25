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

Fade fade_;


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
	for (int i = 0; i < 2; i++)
	{
		//1P�̔L�̏�����
		mike_1P[i].Initialize();
		kuro_1P[i].Initialize();
		tyatora_1P[i].Initialize();

		//2P�̔L�̏�����
		mike_2P[i].Initialize();
		kuro_2P[i].Initialize();
		tyatora_2P[i].Initialize();
	}

	mike_1P[0].x = 1;
	mike_1P[0].y = 7;
	mike_1P[1].x = 4;
	mike_1P[1].y = 7;

	kuro_1P[0].x = 2;
	kuro_1P[0].y = 7;
	kuro_1P[1].x = 5;
	kuro_1P[1].y = 7;

	tyatora_1P[0].x = 3;
	tyatora_1P[0].y = 7;
	tyatora_1P[1].x = 6;
	tyatora_1P[1].y = 7;

	mike_2P[0].x = 1;
	mike_2P[0].y = 1;
	mike_2P[1].x = 4;
	mike_2P[1].y = 1;

	kuro_2P[0].x = 2;
	kuro_2P[0].y = 1;
	kuro_2P[1].x = 5;
	kuro_2P[1].y = 1;

	tyatora_2P[0].x = 3;
	tyatora_2P[0].y = 1;
	tyatora_2P[1].x = 6;
	tyatora_2P[1].y = 1;

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


	//�����}�̃e�N�X�`��
	CompatibilityTex.Load("Material/soukan.png");
	CompatibilitySprite.SetPos(900, 250);
	CompatibilitySprite.SetSize(350, 200);

	playerstate = CHOICE;
	command = ATTACK;
	attack = INITIALSTATE;
	playerTurn = FirstPlayer_Turn;
	hoge = FADE_IN;

	AttackCommandDisplay = false;
	AttackSearchX = 0;
	AttackSearchY = 0;

	sound.Initialize();
	se.Initialize();

	for (int y = 0; y < MAPSIZE; y++)
	{
		for (int x = 0; x < MAPSIZE; x++)
		{
			if (y == MAPEND_MIN || x == MAPEND_MIN || y == MAPEND_MAX || x == MAPEND_MAX)
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

	TurnChangeFlag = false;

	fade_.Initialize();

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
			MoveSquaresSprite.SetPos((x + 1.0f) * 100.0f, (y + 1.0f) * 100.0f);
			AttackSquaresSprite.SetPos((x + 1.0f) * 100.0f, (y + 1.0f) * 100.0f);

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

	Direct3D::DrawSprite(CompatibilitySprite, CompatibilityTex);

	for (int i = 0; i < 2; i++)
	{
		//1P�̎O�єL��`��
		mike_1P[i].Draw();
		//1P�̍��L��`��
		kuro_1P[i].Draw();
		//1P�̒��g����`��
		tyatora_1P[i].Draw();

		//2P�̎O�єL��`��
		mike_2P[i].Draw();
		//2P�̍��L��`��
		kuro_2P[i].Draw();
		//2P�̒��g����`��
		tyatora_2P[i].Draw();
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

	fade_.Draw();

}

//���C���̓���
void GameState::Update()
{
	sound.MainSoundPlay();

	if (FirstPlayerLosingFlag == false && SecondPlayerLosingFlag == false)
	{
		fade_.FadeIn();
	}
	else if (FirstPlayerLosingFlag == true || SecondPlayerLosingFlag == true)
	{
		if (fade_.FadeOut() == true)
		{
			sound.MainSoundStop();
			mSceneChanger->ChangeScene(STATE_RESULT);
		}
	}

	//�J�[�\���̈ʒu��ύX�ł���悤�ɂ����ŏ���
	CursorSprite.SetPos((CursorX + 1.0f) * 100.0f, (CursorY + 1) * 100.0f);

	for (int i = 0; i < 2; i++)
	{
		//1P�̃��j�b�g�̏������s��
		mike_1P[i].Update();
		kuro_1P[i].Update();
		tyatora_1P[i].Update();

		//2P�̃��j�b�g�̏������s��
		mike_2P[i].Update();
		kuro_2P[i].Update();
		tyatora_2P[i].Update();
	}

	switch (playerstate)
	{
	case CHOICE://�ǂ̔L�𓮂��������߂�

		for (int i = 0; i < 2; i++)
		{
			//==��1P�p�̏�����=================================================================

			//�ړ������ǂ����̃t���O��false�ɂ��Ă���
			mike_1P[i].MoveFlag = false;
			kuro_1P[i].MoveFlag = false;
			tyatora_1P[i].MoveFlag = false;
			//�U���ΏۂƂ��đI�΂ꂽ���̃t���O��false�ɂ��Ă���
			mike_1P[i].AttackChoiceFlag = false;
			kuro_1P[i].AttackChoiceFlag = false;
			tyatora_1P[i].AttackChoiceFlag = false;


			//==��2P�p�̏�����=================================================================

			//�ړ������ǂ����̃t���O��false�ɂ��Ă���
			mike_2P[i].MoveFlag = false;
			kuro_2P[i].MoveFlag = false;
			tyatora_2P[i].MoveFlag = false;
			//�U���ΏۂƂ��đI�΂ꂽ���̃t���O��false�ɂ��Ă���
			mike_2P[i].AttackChoiceFlag = false;
			kuro_2P[i].AttackChoiceFlag = false;
			tyatora_2P[i].AttackChoiceFlag = false;
		}

		//�U���R�}���h�̕\���t���O��false�ɂ��Ă���
		AttackCommandDisplay = false;
		//�I��p�̃J�[�\���𓮂����֐�
		MoveCursor();
		//�}�b�v�̐��l����x���Z�b�g���Ă���
		MapReset();
		//�v���C���[�̑���̐؂�ւ�
		TurnChange();
		break;
	case MOVE://�L���ړ�������
		//�I��p�̃J�[�\�����ړ�������֐�
		//���̏ꍇ�͈ړ�������肷�邽�߂Ɉړ�
		MoveCursor();
		break;
	case COMMAND://�ҋ@�A�U���A�߂�Ƃ������s�������߂�R�}���h�I��
		//�ړ��悪�U���\���𒲂ׂ�
		CheckAttack();
		//�R�}���h��I��������̈ړ�
		CommandChoice();
		break;
	case ATTACKCHOICE://�U������G���j�b�g������

		//�U�����łȂ���΃J�[�\���𓮂�����悤�ɂ��Ă���
		if (AttackUnitChoiceFlag == false)
		{
			//�U���Ώۂ�I������J�[�\�����ړ�������֐�
			MoveCursor();
		}
		break;
	}

	//�ړ��\�͈͂̕\���֐�
	Search();
	//����v���C���[���؂�ւ�����ۂ̃J�b�g�C��
	fade_.CutInFade(playerTurn, &hoge);


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

	for (int i = 0; i < 2; i++)
	{
		//1P���S�ł����ꍇ�ɕ����t���O��true�ɂ���
		//���̌�Ń��U���g��ʂ֑J��
		if (mike_1P[i].AliveFlag == true || kuro_1P[i].AliveFlag == true || tyatora_1P[i].AliveFlag == true)
		{
			FirstPlayerLosingFlag = false;
		}
		else if (mike_1P[i].AliveFlag == false && kuro_1P[i].AliveFlag == false && tyatora_1P[i].AliveFlag == false)
		{
			FirstPlayerLosingFlag = true;
		}
	
		//2P���S�ł����ꍇ�ɕ����t���O��true�ɂ���
		//���̌�Ń��U���g��ʂ֑J��
		if (mike_2P[i].AliveFlag == true || kuro_2P[i].AliveFlag == true || tyatora_2P[i].AliveFlag == true)
		{
			SecondPlayerLosingFlag = false;			
		}
		else if (mike_2P[i].AliveFlag == false && kuro_2P[i].AliveFlag == false && tyatora_2P[i].AliveFlag == false)
		{
			SecondPlayerLosingFlag = true;
		}
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
			if (Map[y][x] >= 1)
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
				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < 2; j++)
					{
						//1P��2P�ɍU�������ۂ̐퓬���e�̏���
						mike_1P[i].UnitBattle(tyatora_2P[j].cat, &tyatora_2P[j].HitPoint, tyatora_2P[j].AttackChoiceFlag);
						mike_1P[i].UnitBattle(mike_2P[j].cat, &mike_2P[j].HitPoint, mike_2P[j].AttackChoiceFlag);
						//2P��1P�ɍU�������ۂ̐퓬���e�̏���
						mike_2P[i].UnitBattle(tyatora_1P[j].cat, &tyatora_1P[j].HitPoint, tyatora_1P[j].AttackChoiceFlag);
						mike_2P[i].UnitBattle(mike_1P[j].cat, &mike_1P[j].HitPoint, mike_1P[j].AttackChoiceFlag);

						//1P��2P�ɍU�������ۂ̐퓬���e�̏���
						kuro_1P[i].UnitBattle(mike_2P[j].cat, &mike_2P[j].HitPoint, mike_2P[j].AttackChoiceFlag);
						kuro_1P[i].UnitBattle(kuro_2P[j].cat, &kuro_2P[j].HitPoint, kuro_2P[j].AttackChoiceFlag);
						//2P��1P�ɍU�������ۂ̐퓬���e�̏���
						kuro_2P[i].UnitBattle(mike_1P[j].cat, &mike_1P[j].HitPoint, mike_1P[j].AttackChoiceFlag);
						kuro_2P[i].UnitBattle(kuro_1P[j].cat, &kuro_1P[j].HitPoint, kuro_1P[j].AttackChoiceFlag);

						//1P��2P�ɍU�������ۂ̐퓬���e�̏���
						tyatora_1P[i].UnitBattle(kuro_2P[j].cat, &kuro_2P[j].HitPoint, kuro_2P[j].AttackChoiceFlag);
						tyatora_1P[i].UnitBattle(tyatora_2P[j].cat, &tyatora_2P[j].HitPoint, tyatora_2P[j].AttackChoiceFlag);
						//2P��1P�ɍU�������ۂ̐퓬���e�̏���
						tyatora_2P[i].UnitBattle(kuro_1P[j].cat, &kuro_1P[j].HitPoint, kuro_1P[j].AttackChoiceFlag);
						tyatora_2P[i].UnitBattle(tyatora_1P[j].cat, &tyatora_1P[j].HitPoint, tyatora_1P[j].AttackChoiceFlag);
					}
				}

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
void GameState::MapChange(int myX, int myY, bool myFlag, int enemyX, int enemyY, bool enemyFlag)
{
	//�����̃^�[�����ɖ������j�b�g�̈ʒu�̐��l��-2��
	if (myFlag == true)
	{
		Map[myY][myX] = -2;
	}

	//�����̃^�[���Ƀ}�b�v�ł̓G�̈ʒu�̐��l��-3��
	if (enemyFlag == true)
	{
		Map[enemyY][enemyX] = -3;
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
void GameState::AttackFlag(int x, int y, bool*flag)
{
	//���j�b�g�̍��W�ƃJ�[�\���̍��W����v�����Ȃ��
	if (x == CursorX&&y == CursorY)
	{
		//���̃��j�b�g��flag�𑀍삷��
		*flag = true;
	}
}


void GameState::CheckAttack()
{
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
}

void GameState::TurnChange()
{
	//�ő�s���񐔂�0�ɂȂ�΃^�[�����؂�ւ��
	//���j�b�g���ړ����A�ҋ@���U�����s�����ۂ�PlayerActionNum�̓}�C�i�X�����
	//�J�b�g�C�����t�F�[�h�C��������̂͂��̏�����
	if (PlayerActionNum == 0 || TurnChangeFlag == true)
	{
		switch (playerTurn)
		{
		case FirstPlayer_Turn:

			playerTurn = SecondPlayer_Turn;
			for (int i = 0; i < 2; i++)
			{
				//�G���j�b�g�A2P�̈ړ��\���̃t���O��true�ɂ��čs���\�ɂ���
				UnitMovePossidleChange(mike_2P[i].AliveFlag, &mike_2P[i].MovePossidleFlag);
				UnitMovePossidleChange(kuro_2P[i].AliveFlag, &kuro_2P[i].MovePossidleFlag);
				UnitMovePossidleChange(tyatora_2P[i].AliveFlag, &tyatora_2P[i].MovePossidleFlag);
			}

			break;
		case SecondPlayer_Turn:

			playerTurn = FirstPlayer_Turn;
			for (int i = 0; i < 2; i++)
			{
				//�G���j�b�g�A1P�̈ړ��\���̃t���O��true�ɂ��čs���\�ɂ���
				UnitMovePossidleChange(mike_1P[i].AliveFlag, &mike_1P[i].MovePossidleFlag);
				UnitMovePossidleChange(kuro_1P[i].AliveFlag, &kuro_1P[i].MovePossidleFlag);
				UnitMovePossidleChange(tyatora_1P[i].AliveFlag, &tyatora_1P[i].MovePossidleFlag);
			}

			break;
		}
		hoge = FADE_OUT;
		PlayerActionNum = 3;
		TurnChangeFlag = false;
	}


	//���j�b�g�����Ȃ��Ȃ��Ă��āA����\���j�b�g���O�̖����ɂȂ������̃^�[���؂�ւ�
	switch (playerTurn)
	{
	case FirstPlayer_Turn:
		for (int i = 0; i < 2; i++)
		{
			if (mike_1P[i].MovePossidleFlag == false && kuro_1P[i].MovePossidleFlag == false && tyatora_1P[i].MovePossidleFlag == false)
			{
				TurnChangeFlag = true;
			}
			else if (mike_1P[i].MovePossidleFlag == true || kuro_1P[i].MovePossidleFlag == true || tyatora_1P[i].MovePossidleFlag == true)
			{
				TurnChangeFlag = false;
				break;
			}
		}
		break;
	case SecondPlayer_Turn:
		for (int i = 0; i < 2; i++)
		{
			if (mike_2P[i].MovePossidleFlag == false && kuro_2P[i].MovePossidleFlag == false && tyatora_2P[i].MovePossidleFlag == false)
			{
				TurnChangeFlag = true;
			}
			else if (mike_2P[i].MovePossidleFlag == true || kuro_2P[i].MovePossidleFlag == true || tyatora_2P[i].MovePossidleFlag == true)
			{
				TurnChangeFlag = false;
				break;
			}
		}
		break;
	}


}

//1P�̍s���֐�
void GameState::FirstPlayer_Update()
{
	DirectInput* pDi = DirectInput::GetInstance();

	switch (playerstate)
	{
	case CHOICE://�ǂ̔L�𓮂��������߂�


		for (int i = 0; i < 2; i++)
		{
			//�G���j�b�g�̂���ꏊ��-3�ɁA�����̃��j�b�g�̈ʒu��-2�ɂ���
			//���̊֐���1P�̃^�[���̎��Ɏ��s�����̂ł����ł̓G��2P�ɂȂ�
			MapChange(mike_1P[i].x, mike_1P[i].y, mike_1P[i].AliveFlag, mike_2P[i].x, mike_2P[i].y, mike_2P[i].AliveFlag);
			MapChange(kuro_1P[i].x, kuro_1P[i].y, kuro_1P[i].AliveFlag, kuro_2P[i].x, kuro_2P[i].y, kuro_2P[i].AliveFlag);
			MapChange(tyatora_1P[i].x, tyatora_1P[i].y, tyatora_1P[i].AliveFlag, tyatora_2P[i].x, tyatora_2P[i].y, tyatora_2P[i].AliveFlag);

			//�����̃��j�b�g��I��
			//���̊֐��͂��ꂩ��ړ����郆�j�b�g��MoveFlag�𑀍삷��
			//���Ɉړ��ς݂ł���Ȃ�ΑI���ł��Ȃ��悤�ɂȂ��Ă���
			UnitChoice(mike_1P[i].x, mike_1P[i].y, &mike_1P[i].MoveFlag, mike_1P[i].MovePossidleFlag);
			UnitChoice(kuro_1P[i].x, kuro_1P[i].y, &kuro_1P[i].MoveFlag, kuro_1P[i].MovePossidleFlag);
			UnitChoice(tyatora_1P[i].x, tyatora_1P[i].y, &tyatora_1P[i].MoveFlag, tyatora_1P[i].MovePossidleFlag);
		}


		break;
	case MOVE://�L���ړ�������

		for (int i = 0; i < 2; i++)
		{
			//�I���������j�b�g���ړ�
			//�ړ������̃t���O��true�������ꍇ�ɂ݈̂ړ�
			//�J�[�\���̍��W��I������Ă��郆�j�b�g�̍��W�ɑ������
			UnitMove(&mike_1P[i].x, &mike_1P[i].y, mike_1P[i].MoveFlag);
			UnitMove(&kuro_1P[i].x, &kuro_1P[i].y, kuro_1P[i].MoveFlag);
			UnitMove(&tyatora_1P[i].x, &tyatora_1P[i].y, tyatora_1P[i].MoveFlag);
		}

		break;
	case COMMAND://�ҋ@�A�U���A�߂�Ƃ������s�������߂�R�}���h�I��

		for (int i = 0; i < 2; i++)
		{
			//�I�������R�}���h�̌��ʂ���������֐�
			//�ړ��������j�b�g�̕������������s��
			CommandPlay(&mike_1P[i].x, &mike_1P[i].y, mike_1P[i].MoveFlag, &mike_1P[i].MovePossidleFlag);
			CommandPlay(&kuro_1P[i].x, &kuro_1P[i].y, kuro_1P[i].MoveFlag, &kuro_1P[i].MovePossidleFlag);
			CommandPlay(&tyatora_1P[i].x, &tyatora_1P[i].y, tyatora_1P[i].MoveFlag, &tyatora_1P[i].MovePossidleFlag);
		}

		break;

	case ATTACKCHOICE://�U������G���j�b�g������

		//�G���^�[�L�[������������
		if (pDi->KeyJustPressed(DIK_RETURN))
		{
			//���̏�̃}�b�v�̐��l��-5�ł���Ώ���
			if (Map[CursorY][CursorX] == -5)
			{
				//�t���O��true�ɂ��Ĉړ��J�n
				AttackUnitChoiceFlag = true;

				for (int i = 0; i < 2; i++)
				{
					//�U���ΏۂƂ��đI�΂ꂽ2P�L�����̃t���O��true�ɂ���
					//��������Ύl���ɂȂ��U���\�}�X��I�񂾏ꍇ�ł��U���͍s���Ȃ�
					AttackFlag(mike_2P[i].x, mike_2P[i].y, &mike_2P[i].AttackChoiceFlag);
					AttackFlag(kuro_2P[i].x, kuro_2P[i].y, &kuro_2P[i].AttackChoiceFlag);
					AttackFlag(tyatora_2P[i].x, tyatora_2P[i].y, &tyatora_2P[i].AttackChoiceFlag);
				}
			}
		}

		for (int i = 0; i < 2; i++)
		{
			//���j�b�g�����߂�ꂽ�����֍U�����Ă��镗�Ɍ�����
			//�J�[�\���ƃ��j�b�g�̈ʒu���r���ĕ���������
			//���̃��[�V�������I�������퓬�̌㏈�����s����HP�̌��Z��s���I���ɂ����肷��
			AttackMotion(mike_1P[i].x, mike_1P[i].y, &mike_1P[i].AttackPosX, &mike_1P[i].AttackPosY, &mike_1P[i].MoveFlag, &mike_1P[i].MovePossidleFlag);
			AttackMotion(kuro_1P[i].x, kuro_1P[i].y, &kuro_1P[i].AttackPosX, &kuro_1P[i].AttackPosY, &kuro_1P[i].MoveFlag, &kuro_1P[i].MovePossidleFlag);
			AttackMotion(tyatora_1P[i].x, tyatora_1P[i].y, &tyatora_1P[i].AttackPosX, &tyatora_1P[i].AttackPosY, &tyatora_1P[i].MoveFlag, &tyatora_1P[i].MovePossidleFlag);
		}
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

		for (int i = 0; i < 2; i++)
		{
			//�G���j�b�g�̂���ꏊ��-3�ɁA�����̃��j�b�g�̈ʒu��-2�ɂ���
			//���̊֐���2P�̃^�[���̎��Ɏ��s�����̂ł����ł̓G��1P�ɂȂ�
			MapChange(mike_2P[i].x, mike_2P[i].y, mike_2P[i].AliveFlag, mike_1P[i].x, mike_1P[i].y, mike_1P[i].AliveFlag);
			MapChange(kuro_2P[i].x, kuro_2P[i].y, kuro_2P[i].AliveFlag, kuro_1P[i].x, kuro_1P[i].y, kuro_1P[i].AliveFlag);
			MapChange(tyatora_2P[i].x, tyatora_2P[i].y, tyatora_2P[i].AliveFlag, tyatora_1P[i].x, tyatora_1P[i].y, tyatora_1P[i].AliveFlag);

			//�����̃��j�b�g��I��
			//���̊֐��͂��ꂩ��ړ����郆�j�b�g��MoveFlag�𑀍삷��
			//���Ɉړ��ς݂ł���Ȃ�ΑI���ł��Ȃ��悤�ɂȂ��Ă���
			UnitChoice(mike_2P[i].x, mike_2P[i].y, &mike_2P[i].MoveFlag, mike_2P[i].MovePossidleFlag);
			UnitChoice(kuro_2P[i].x, kuro_2P[i].y, &kuro_2P[i].MoveFlag, kuro_2P[i].MovePossidleFlag);
			UnitChoice(tyatora_2P[i].x, tyatora_2P[i].y, &tyatora_2P[i].MoveFlag, tyatora_2P[i].MovePossidleFlag);
		}

		break;
	case MOVE://�L���ړ�������

		for (int i = 0; i < 2; i++)
		{
			//�I���������j�b�g���ړ�
			//�ړ����̃t���O��true�������ꍇ�ɂ݈̂ړ�
			//�J�[�\���̍��W��I������Ă��郆�j�b�g�̍��W�ɑ������
			UnitMove(&mike_2P[i].x, &mike_2P[i].y, mike_2P[i].MoveFlag);
			UnitMove(&kuro_2P[i].x, &kuro_2P[i].y, kuro_2P[i].MoveFlag);
			UnitMove(&tyatora_2P[i].x, &tyatora_2P[i].y, tyatora_2P[i].MoveFlag);
		}

		break;
	case COMMAND://�ҋ@�A�U���A�߂�Ƃ������s�������߂�R�}���h�I��

		for (int i = 0; i < 2; i++)
		{
			//�I�������R�}���h�̌��ʂ���������֐�
			//�ړ��������j�b�g�̕������������s��
			CommandPlay(&mike_2P[i].x, &mike_2P[i].y, mike_2P[i].MoveFlag, &mike_2P[i].MovePossidleFlag);
			CommandPlay(&kuro_2P[i].x, &kuro_2P[i].y, kuro_2P[i].MoveFlag, &kuro_2P[i].MovePossidleFlag);
			CommandPlay(&tyatora_2P[i].x, &tyatora_2P[i].y, tyatora_2P[i].MoveFlag, &tyatora_2P[i].MovePossidleFlag);
		}

		break;

	case ATTACKCHOICE://�U������G���j�b�g������

		//�G���^�[�L�[������������
		if (pDi->KeyJustPressed(DIK_RETURN))
		{
			//���̏�̃}�b�v�̐��l��-5�ł���Ώ���
			if (Map[CursorY][CursorX] == -5)
			{
				//�t���O��true�ɂ��Ĉړ��J�n
				AttackUnitChoiceFlag = true;

				for (int i = 0; i < 2; i++)
				{
					//�U���ΏۂƂ��đI�΂ꂽ2P�L�����̃t���O��true�ɂ���
					//��������Ύl���ɂȂ��U���\�}�X��I�񂾏ꍇ�ł��U���͍s���Ȃ�
					AttackFlag(mike_1P[i].x, mike_1P[i].y, &mike_1P[i].AttackChoiceFlag);
					AttackFlag(kuro_1P[i].x, kuro_1P[i].y, &kuro_1P[i].AttackChoiceFlag);
					AttackFlag(tyatora_1P[i].x, tyatora_1P[i].y, &tyatora_1P[i].AttackChoiceFlag);
				}
			}
		}

		for (int i = 0; i < 2; i++)
		{
			//���j�b�g�����߂�ꂽ�����֍U�����Ă��镗�Ɍ�����
			//�J�[�\���ƃ��j�b�g�̈ʒu���r���ĕ���������
			//���̃��[�V�������I�������퓬�̌㏈�����s����HP�̌��Z��s���I���ɂ����肷��
			AttackMotion(mike_2P[i].x, mike_2P[i].y, &mike_2P[i].AttackPosX, &mike_2P[i].AttackPosY, &mike_2P[i].MoveFlag, &mike_2P[i].MovePossidleFlag);
			AttackMotion(kuro_2P[i].x, kuro_2P[i].y, &kuro_2P[i].AttackPosX, &kuro_2P[i].AttackPosY, &kuro_2P[i].MoveFlag, &kuro_2P[i].MovePossidleFlag);
			AttackMotion(tyatora_2P[i].x, tyatora_2P[i].y, &tyatora_2P[i].AttackPosX, &tyatora_2P[i].AttackPosY, &tyatora_2P[i].MoveFlag, &tyatora_2P[i].MovePossidleFlag);
		}
		break;
	}
}