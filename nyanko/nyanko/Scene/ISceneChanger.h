#pragma once

typedef enum
{
	STATE_TITLE,		//�^�C�g�����
	STATE_MAIN,			//�Q�[�����
	STATE_RESULT,		//���U���g���

	STATE_NONE			//����
}STATE;

//�V�[����ύX���邽�߂̃C���^�[�t�F�C�X�N���X
class ISceneChanger
{
public:
	virtual ~ISceneChanger() = 0;
	virtual void ChangeScene(STATE NextScene) = 0;//�w��V�[���ɕύX����
};