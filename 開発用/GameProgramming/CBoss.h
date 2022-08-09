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
#include"CColliderLine.h"
/*エネミークラス
キャラクタクラスを継承	*/
class CBoss :public CXCharacter {
public:
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
private:
    CText mText;
	CVector mPoint;//目標地点
	CCollider mColSphereHead;
	CCollider mColSphereRightFront;
	CCollider mColSphereLeftFront;
	CCollider mColSphereAttack;
	CCollider mColSearch;//サーチ用コライダ
	CColliderLine mColliderLine;
	CEffect2* mBossEffect;
	CVector mCollisionEnemy;
	CVector mJumpCount;
	CVector mMoveRecord;
	CCharacter* mpPlayer;//プレイヤーのポインタ
	CText mFont;
	CTexture mImageGauge; //ゲージ画像
	char buf[64];
	bool mBossBgm;
	bool mBossBgmDeath;
	bool mColSearchCount;
	bool mJumpStopper;
	bool mBossGaugeSwitch;
	bool mBossStopper;
	bool mBossAttackHit;
	int mHp;//体力
	int mMove;
	int mMove2;
	int mBossDamageCount;
	int mEnemy2StopCount;//プレイヤーのESTOPPERに当たっている間増加
    int mAttackPercent;
	int mEnemyDamage;
	int mBossJumpCount;
	int mBossColliderCheck;
	int mAttack4MoveCount;
	int mAttack4Count;
	int mAttack4directionCount;
	int mEffectCount;
	int mBossAttackMove;
	int mBossPositionLengthCount;//ボスの行動範囲を計測
	float mJump;
	float mJumpZ;
	float mColliderCount;
	float mTime;//ジャンプする時の時間を計測
	float mAttack4MoveX;
	float mAttack4MoveZ;
	float mAttack4RotationCount;
	float mAttackRotation;
	float mRotationCount;
	float mBossPositionLengthX;//ボスの行動範囲のX軸
	float mBossPositionLengthZ;//ボスの行動範囲のZ軸
	float mBossPositionLengthSum;//ボスの行動範囲の半径
	float mBossCircle;//ボスの行動範囲
public:

	EBossState mState;
	//モデルデータ
	static CModel mModel;
	static CBoss* GetInstance();
	static CBoss* mpBossInstance;
	bool GetBossAttackHit() {
		return mBossAttackHit;
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
	void Idle();//待機処理
	void AutoMove();//移動処理
	void Attack();//攻撃処理1
    void Attack2();//攻撃処理2
	void Attack3();//攻撃処理3
	void Attack4();//攻撃処理4
	void Damaged();//被弾処理
	void Death();//死亡処理
	
	
};
#endif