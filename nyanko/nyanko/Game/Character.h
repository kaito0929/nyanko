#include "../directInput.h"
#include "../Xinput.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../mesh.h"
#include "../Direct3D.h"

class Character
{
protected:

	int x;
	int y;

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