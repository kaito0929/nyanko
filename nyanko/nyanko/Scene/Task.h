#pragma once

//�^�X�N�N���X�B�����̃��W���[���͑S�Ă���Task�N���X���p������
class Task
{
public:
	virtual ~Task(){}
	virtual void Initialize(){}     //�����������͎������Ă����Ȃ��Ă�����
	virtual void Finalize()  {}     //�I�������͎������Ă����Ȃ��Ă�����
	virtual void Update() = 0;		//�X�V�����͕K���p����Ŏ�������
	virtual void Draw() = 0;		//�`�揈���͕K���p����Ŏ�������
};