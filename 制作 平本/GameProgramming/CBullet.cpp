#include"CBullet.h"
#include"CTransform.h"
#include"CCollisionManager.h"
#define COLLISIONRANGE 30
CBullet::CBullet()
:mLife(50)
, mCollider4(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 3.0f)
{

}

void CBullet::Set(float w, float d) {
	//スケール設定（大きさ）
	mScale = CVector(1.0f, 1.0f, 1.0f);
	//三角形の頂点設定
	mT.SetVertex(CVector(w, 0.0f, 0.0f),CVector(-w, 0.0f, 0.0f),CVector(0.0f, 0.0f, -d));
	//三角形の法線設定
	mT.SetNormal(CVector(0.0f, 1.0f, 0.0f));

}
void CBullet::Update() {
	//mLifeが０より大きいなら
	if (mLife-- > 0) {
		CTransform::Update();
		//位置更新(移動する方向）
		mPosition = CVector(0.0f, 0.0f, 1.0f) * mMatrix;
	}
	else {
		mEnabled = false;
	}

}
void CBullet::Render() {
	//Deffuse黄色設定
	float c[] = { 1.0f,1.0f,0.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	//三角形描画
	//mMatrixと同じ場所に描画
	mT.Render(mMatrix);
	
}
//コライダの衝突処理
void CBullet::Collision(CCollider* m, CCollider* o) {
	//相手がサーチのときは戻る
	if (o->mTag == CCollider::ESEARCH) {
		return;
	}
	
	//コライダのｍとoが衝突しているか判定
	if (CCollider::Collision(m, o)) {

		mEnabled = false;
	}
}
void CBullet::TaskCollision() {
	//コライダの優先度変更
	mCollider4.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mCollider4, COLLISIONRANGE);
}