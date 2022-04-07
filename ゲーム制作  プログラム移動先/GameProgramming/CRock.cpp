
#include"CPlayer.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CColliderTriangle.h"
#include"CRock.h"
#include"CCollider.h"
#define OBJ "3DModel\\Rock\\Rock.obj"
#define MTL "3DModel\\Rock\\Rock.mtl"

CModel CRock::mModel;//モデルデータ作成

CRock::CRock(const CVector& position, const CVector& rotation, const CVector& scale)
	: CRock()
{
	
	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mTag = EROCK;
	 CTransform::Update();//行列の更新
	//優先度を2に変更する
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//削除して
	CTaskManager::Get()->Add(this);//追加する
}
CRock::CRock()

{
	//mPosition = CVector(-100.0f, 0.0f, 50.0f);
	//mPosition = CVector(0.0f, 0.0f, 0.0f);
	//mScale = CVector(1.0f, 1.0f, 1.0f);
   
	

	//モデルのポインタ設定
	mpModel = &mModel;

		//モデルが無いときは読み込む
	if (mModel.mTriangles.size() == 0) {
		mModel.Load(OBJ, MTL);
	}

	mColliderMesh.Set(this, NULL, &mModel);//モデルをコライダにする
	
}
void CRock::Update() {
	
	//CCharacterの更新
	CTransform::Update();
}
void CRock::Collision(CCollider* m, CCollider* o) {
	switch (m->mType) {
	case CCollider::ESPHERE:
		if (o->mType == CCollider::ESPHERE) {
			if (o->mpParent->mTag == EPLAYER) {

				//衝突しているとき
				if (CCollider::Collision(m, o)) {
					
					
				}

			}
		}
	}

}
void CRock::Render() {
	//親の描画処理
	//CCharacter::Render();
}
void CRock::TaskCollision() {
	//コライダの優先度変更
	//mCollider.ChangePriority();
	//衝突処理を実行
	//CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}
