#pragma once

#include "ISceneChanger.h"
#include "BaseScene.h"

class SceneManager : public ISceneChanger, Task
{
private:
	BaseScene* mScene;		//�V�[���̊Ǘ��ϐ�
	STATE mNextScene;		//���̃V�[���Ǘ��ϐ�

public:
	SceneManager();
	void Initialize() override;	//������
	void Finalize() override;	//�I������
	void Update() override;		//�X�V
	void Draw() override;		//�`��

	//�����@NextScene�ɃV�[����ύX����
	void ChangeScene(STATE NextScene) override;
};