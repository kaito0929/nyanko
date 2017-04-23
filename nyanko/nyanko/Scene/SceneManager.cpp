#include "SceneManager.h"


SceneManager::SceneManager() : mNextScene(STATE_NONE)//���̃V�[���Ǘ��ϐ�
{
	mScene = (BaseScene*) new Title(this);
}

//������
void SceneManager::Initialize()
{
	mScene->Initialize();
}

//�I������
void SceneManager::Finalize()
{
	mScene->Finalize();
}

//�X�V
void SceneManager::Update()
{
	if (mNextScene != STATE_NONE)//���̃V�[�����Z�b�g����Ă�����
	{
		mScene->Finalize();//���݂̃V�[���̏I�����������s
		delete mScene;
		switch (mNextScene)//�V�[���ɂ���ď����𕪊�
		{
		case STATE_TITLE:
			mScene = (BaseScene*)new Title(this);	//�^�C�g����ʂ̃C���X�^���X�𐶐�����
			break;
		case STATE_MAIN:
			mScene = (BaseScene*)new Floor1Update(this);
			break;
		case STATE_RESULT:
			mScene = (BaseScene*)new Result(this);
			break;
		}
		mNextScene = STATE_NONE;	//���̃V�[�������N���A
		mScene->Initialize();		//�V�[����������
	}
	mScene->Update();			//�V�[���̍X�V
}

//�`��
void SceneManager::Draw()
{
	mScene->Draw();//�V�[���̕`��
}

//���� NexyScene�ɃV�[����ύX����
void SceneManager::ChangeScene(STATE NextScene)
{
	mNextScene = NextScene;		//���̃V�[�����Z�b�g����
}