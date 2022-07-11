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
#include"CTexture.h"
/*エネミークラス
キャラクタクラスを継承	*/
class CBoss :public CXCharacter {
private:
    CText mText;
	CVector mPoint;//目標地点
	CCollider mColSphereHead;
	CCollider mColSphereRightFront;
	CCollider mColSphereLeftFront;
	CCollider mColSphereAttack;
	CEffect2* mBossEffect;
	CVector mCollisionEnemy;
	CVector mJumpCount;
	CCharacter* mpPlayer;//プレイヤーのポインタ
	CText mFont;
	char buf[64];
	bool mBossBgm;
	bool mBossBgmDeath;
	bool mColSearchCount;
	bool mJumpStopper;
	int mMove;
	int mMove2;
	int mBossDamageCount;
	int mEnemy2StopCount;//プレイヤーのESTOPPERに当たっている間増加
    int mAttackPercent;
	int mEnemyDamage;
	int mBossJumpCount;
	int  mBossColliderCheck;
	int mAttack4MoveCount;
	int mAttack4Count;
	int mAttack4directionCount;
	int mEffectCount;
	int mBossAttackMove;
	float mJump;
	float mJumpZ;
	float mColliderCount;
	float mTime;//ジャンプする時の時間を計測
	float mAttack4MoveX;
	float mAttack4MoveZ;
	float mAttack4RotationCount;
	float mAttackRotation;
	float mRotationCount;
	CTexture mImageGauge; //ゲージ画像
						  //敵の状態
	enum EBossState
	{
		EIDLE,		//待機
		EAUTOMOVE,	//移動
		EATTACK,	//攻撃
        EATTACK2,
		EATTACK3,
		EATTACK4,
		EDAMAGED,	//被弾
		EDEATH,		//死亡
		
	};
public:

	EBossState mState;
	CCollider mColSearch;//サーチ用コライダ
	//モデルデータ
	static CModel mModel;
	static CBoss* GetInstance();
	static CBoss* mpBossInstance;
	bool mBossAttackHit;
	
	 int mHp;//体力
	 void SetHp(int hp) {
		 if (hp < 0) return;
		 this->mHp = hp;
	 }
	 int GetHp() {
		 return mHp;
	 }
	//コンストラクタ	
	CBoss();
	//CEnemy2(位置、回転、拡縮）
	CBoss(const CVector& position, const CVector& rotation, const CVector& scale);
	void Init(CModelX* model);
	//更新処理
	void Update();
	void Render2D();
	//衝突処理
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
	void Idle();		//待機処理
	void AutoMove();	//移動処理
	void Attack();	//攻撃処理
    void Attack2();
	void Attack3();
	void Attack4();
	void Damaged();		//被弾処理
	void Death();		//死亡処理
	
	
};
#endif