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
	//コンストラクタ
	Character();
	//デストラクタ
	~Character();

	//初期化
	void Initialize();
	//実際の動き
	void Update();
	//描画
	void Draw();

	
};