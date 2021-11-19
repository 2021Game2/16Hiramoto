#ifndef CENEMY2_H
#define CENEMY2_H
//キャラクタクラスのインクルード
#include"CCharacter.h"
//コライダクラスのインクルード
#include"CCollider.h"
#include"CText.h"
/*エネミークラス
キャラクタクラスを継承	*/
class CEnemy2 :public CCharacter {
public:
	//モデルデータ
	static CModel mModel;
	//コライダ
	CCollider mCollider;
	//コンストラクタ	
	CEnemy2();
	CVector mPoint;//目標地点
	int mHp;//体力
	int mJump;
	int mJump2;
	CText mText;
	int mMove;
	int mMove2;
	int mColliderCount;
	static int mEnemy2AttackCount;
	void Init(CModelX* model);
	CVector mCollisionEnemy;
	//CEnemy2(位置、回転、拡縮）
	CEnemy2(const CVector& position, const CVector& rotation, const CVector& scale);
	CCollider mColSearch;//サーチ用コライダ
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ１、コライダ２）
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	CCharacter* mpPlayer;//プレイヤーのポインタ
	int mEnemyDamage;
};
#endif