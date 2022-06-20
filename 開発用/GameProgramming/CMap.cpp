#include "CMap.h"
#include"CCollider.h"
#include"CColliderTriangle.h"
#include"CCollisionManager.h"
#define OBJ "Resource\\3DModel\\Map\\sky.obj"
#define MTL "Resource\\3DModel\\Map\\sky.mtl"
//CColliderMesh mColliderMesh;
CModel CMap::mModel;//モデルデータ作成


CMap::CMap(const CVector& position, const CVector& rotation, const CVector& scale)
:CMap()
{
	mTag = EMAP;
	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	//優先度を１に変更する
	mPriority = 1;
	CTaskManager::Get()->Remove(this);//削除して
	CTaskManager::Get()->Add(this);//追加する
	CTransform::Update();//行列の更新

  
	//mColliderMesh.Set(this, &mMatrix, &mModel);//モデルをコライダにする
	
	
	
}
 CMap::CMap() {
	 //モデルのポインタ設定
	 mpModel = &mModel;
	 //モデルが無いときは読み込む
	 if (mModel.mTriangles.size() == 0) {
		 mModel.Load(OBJ, MTL);
	 }
}
 void CMap::Update() {
	 //CCharacterの更新
	 CTransform::Update();
 }

void CMap::TaskCollision() {
	
}

