#ifndef CENEMY3_H
#define CENEMY3_H
//キャラクタクラスのインクルード
#include"CXCharacter.h"
//コライダクラスのインクルード
#include"CCollider.h"
#include"CText.h"
#include"CBullet.h"

#include"CSound.h"
/*エネミークラス
キャラクタクラスを継承	*/
class CEnemy3 :public CCharacter {
private:

	
	int mJump;
	int mJump2;
	int mMove2;
	int mColliderCount;
	int mCount;
	int mFireCount;
	int mEnemy3Fry;
	int mEnemyDamage;
    int mMoveCount;
	int mEffectCount;//エフェクトを表示させる時間
    int mHp;//体力
	CCollider mCollider;
	CBullet* mpBullet;
	CVector mPoint;//目標地点
	CVector mCollisionEnemy;
	CText mText;
	CCharacter* mpPlayer;//プレイヤーのポインタ

public:
	CCollider mColSearch2;//サーチ用コライダ
	//モデルデータ
	static CModel mModel;
	//コンストラクタ
	CEnemy3();
	CEnemy3(const CVector& position, const CVector& rotation, const CVector& scale);
	void Move1();
	void Move2();
	void Move3();
	void Move4();
	void Move5();
	//void AutoMove1();
	//void AutoMove2();
	//void Damage();
	void Death();
	void Idle();
	//更新処理
	void Update();
	//衝突処理
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	//敵の状態
	enum EEnemy3State
	{
		EIDLE,		//待機
		EMOVE1,//移動１
		EMOVE2,//移動２
		EMOVE3,//移動３
		EMOVE4,//移動４
		EMOVE5,//移動５
		EDAMAGED,	//被弾
		EDEATH,		//死亡
	};
	EEnemy3State mState;
};

#endif