#ifndef CCHARACTER_H
#define CCHARACTER_H
#include"CTask.h"

//コライダクラスの宣言
	class CCollider;
//変換行列クラスのインクルード
#include"CTransform.h"
//モデルクラスのインクルード
#include"CModel.h"
/*キャラクタークラス
ゲームキャラクターの基本的な機能と定義する
*/
//.を使って２つの親クラスを追加
class CCharacter :public CTransform,public CTask {
public:
	//コンストラクタ
	CCharacter();

	CModel* mpModel;//モデルのポインタ

	enum ETag {
		EZERO,//初期値
		EPLAYER,//プレイヤー
		EENEMY,//敵
		EWEAPON,//武器
		EENEMY2,//敵２
		EITEM,//アイテム
		EBULLETPLAYER,//プレイヤーの弾
		EBULLETENEMY,//敵の弾
	};
	ETag mTag;
	
	//描画処理
	void Render();
	//デストラクタ
	~CCharacter();
	
	//衝突処理
	virtual void Collision(CCollider* m, CCollider* o) {}
};
#endif