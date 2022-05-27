#ifndef CBOSS_H
#define CBOSS_H
//キャラクタクラスのインクルード
#include"CCharacter.h"

#include "CXCharacter.h"
//コライダクラスのインクルード
#include"CCollider.h"
#include"CText.h"
#include"CEffect2.h"
#include"CSound.h"
/*エネミークラス
キャラクタクラスを継承	*/
class CBoss :public CXCharacter {
private:
    CText mText;
	CVector mPoint;//目標地点
	CCollider mColSphereHead;
	CCollider mColSphereRightFront;
	CCollider mColSphereLeftFront;
	CEffect2* mBossEffect;
	CVector mCollisionEnemy;
	CCharacter* mpPlayer;//プレイヤーのポインタ
	int mJump2;
	int mMove;
	int mMove2;
	int mBossDamageCount;
	int mEnemy2StopCount;//プレイヤーのESTOPPERに当たっている間増加
    int mAttackPercent;
	int mEnemyDamage;
	float mColliderCount;
	float mGravity;//重力
	float mTime;//ジャンプする時の時間を計測
	bool mBossBgm;
	bool mBossBgmDeath;
public:
	CCollider mColSearch;//サーチ用コライダ
	bool mColSearchCount;
	bool mBossAttackHit;
	//モデルデータ
	static CModel mModel;
	static CBoss* GetInstance();
	static CBoss* mpBossInstance;
	static int mBossAttackCount;
	static int mHp;//体力
	int mEffectCount;
	//コンストラクタ	
	CBoss();
	void Init(CModelX* model);
	//CEnemy2(位置、回転、拡縮）
	CBoss(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ１、コライダ２）
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	void Idle();		//待機処理
	void AutoMove();	//移動処理
	void Attack();	//攻撃処理
	void Damaged();		//被弾処理
	void Death();		//死亡処理
	void Attack2();
	//敵の状態
	enum EBossState
	{
		EIDLE,		//待機
		EAUTOMOVE,	//移動
		EATTACK,	//攻撃
		EDAMAGED,	//被弾
		EDEATH,		//死亡
		EATTACK2,
	};
	EBossState mState;
};
#endif