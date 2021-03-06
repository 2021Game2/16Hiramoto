
#include"CPlayer.h"
#include"CEffect.h"
#include"CCollisionManager.h"
#include"CColliderTriangle.h"
#include"CRock.h"
#include"CCollider.h"
#define OBJ "Resource\\3DModel\\Ground\\Ground.obj"
#define MTL "Resource\\3DModel\\Ground\\Ground.mtl"

CModel CRock::mModel;//モデルデータ作成

CRock::CRock(const CVector& position, const CVector& rotation, const CVector& scale)
	
{
	
	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	
	//優先度を2に変更する
	mPriority = 2;
	CTaskManager::Get()->Remove(this);//削除して
	CTaskManager::Get()->Add(this);//追加する
	 CTransform::Update();//行列の更新
	 
	//モデルのポインタ設定
	mpModel = &mModel;
		//モデルが無いときは読み込む
	if (mModel.mTriangles.size() == 0) {
			mModel.Load(OBJ, MTL);
	}
	mColliderMesh.Set(this, &mMatrix, &mModel);//モデルをコライダにする
    mTag = EROCK;
}
void CRock::Collision(CCollider* m, CCollider* o) {
	m->mpParent->mTag = EROCK;

}
void CRock::Update() {

	//左向き（X軸）のベクトルを求める
	CVector vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
	//上向き（Y軸）のベクトルを求める
	CVector vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
	//前方向（Z軸）のベクトルを求める
	CVector vz = CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;


	//CCharacterの更新
	CTransform::Update();
}

void CRock::Render() {
	//親の描画処理
	CCharacter::Render();
}
void CRock::TaskCollision() {
	
}
