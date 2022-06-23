#ifndef CXPLAYER_H
#define CXPLAYER_H

#include"CEffect2.h"
#include "CXCharacter.h"
#include "CCollider.h"
#include"CColliderLine.h"
#include"CCollisionManager.h"
#include"CTaskManager.h"
#include"CVector.h"

#include"CTexture.h"
#include"CSound.h"
#include"CBoss.h"

class CXPlayer : public CXCharacter
{
private:
	CTexture mImageGauge; //ゲージ画像
	int mAnimationCount;//アニメーションが途中で変わらないようにする
	void SetAnimationCount(int v);
	int GetAnimationCount();
	int mDamageCount;//ダメージを受けた直後の無敵時間
	void SetDamageCount(int v);
	int GetDamageCount();
	bool mPlayerBgm;
	bool mAnimationFrameLock;
	bool mMoveCheck;//ダッシュかダッシュじゃないか
	bool mSpaceCount1;//1回目の攻撃のフラグ
	bool mSpaceCount2;//２回目の攻撃のフラグ
	bool mSpaceCount3;//３回目の攻撃のフラグ
	bool mEffectStopper;
	bool mJumpStopper;
	float mSpeed;
	float mColliderCount;//吹き飛ばされた時の初速度
	float mJump;//ジャンプする時の初速度
	float mStep;//攻撃時、少し前進
	static int mStamina;//スタミナの数値
	void SetStamina(int v);
	int GetStamina();
	
	static int mAttackCount;//攻撃のアニメーションが何度も再生されないように
	CBoss* mpBoss;
	CEffect2* mEffect1;
	CEffect2* mEffect2;
	CEffect2* mEffect3;
	CEffect2* mEffectSp;
	//コライダの宣言
	CCollider mColSphereSword;	//剣
	CCollider  mColSphereFoot;//ダメージが通る当たり判定
	CCollider mColliderSwordSp;//ジャンプ攻撃時の武器の当たり判定
	CColliderLine mColEscapeStopperLine;
    CVector mCollisionEnemy;//プレイヤーと敵のコライダーの位置の差
	CTexture mTexture;
     //プレイヤーの状態
	enum EPLAYERState
	{
		EIDLE,		//待機
		EMOVE,	//移動
		EDUSH,
		EESCAPE,
		EATTACK1,	//攻撃1
		EATTACK2,
		EATTACK3,
		EATTACKSP,
		EDAMAGED,	//被弾
		EDEATH,		//死亡
	};
	EPLAYERState mState;
public:
	bool mGaugeEnabled;
    bool mAttackHit;
	static int mSpAttack;//ジャンプ攻撃をするためのゲージ
	static int mHp;//体力
	//別のクラスで変数を呼び出す場合
	//staticでポインタを作る
	static CXPlayer* mpPlayerInstance;
	//staticで処理を作る
	static CXPlayer* GetInstance();
	CXPlayer();
	void TaskCollision();
	/*
	初期化(Xモデルクラスのポインタ)
	*/
	void Init(CModelX* model);
	void Update();
	void Collision(CCollider* m, CCollider* o);
	void Render2D();
};
void CXPlayer::SetAnimationCount(int v)
{
	if (v < 0) return;//< ここにブレークポイントを置けば誰が犯人なのかわかる
	this->mAnimationCount = v;
}
int CXPlayer::GetAnimationCount()
{
	return this->mAnimationCount;
}
void CXPlayer::SetDamageCount(int v) {
	if (v < 0) return;//< ここにブレークポイントを置けば誰が犯人なのかわかる
	this->mDamageCount = v;
}

int CXPlayer::GetDamageCount()
{
	return this->mDamageCount;
}
void CXPlayer::SetStamina(int v) {
	if (v < 0) return;//< ここにブレークポイントを置けば誰が犯人なのかわかる
	this->mStamina = v;
}
int CXPlayer::GetStamina()
{
	return this->mStamina;
}
#endif
