#include "../directInput.h"
#include "../Xinput.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../mesh.h"
#include "../Direct3D.h"

//enum PlayerState
//{
//	FirstPlayer,
//	SecondPlayer,
//};

class Character
{
protected:

	int x;
	int y;

	bool AliveFlag;

public:
	//�R���X�g���N�^
	Character();
	//�f�X�g���N�^
	~Character();

	//������
	void Initialize();
	//���ۂ̓���
	void Update();
	//�`��
	void Draw();

	
};