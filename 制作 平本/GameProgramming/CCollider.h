#ifndef CCOLLIDER_H
#define CCOLLIDER_H
#include"CCharacter.h"
#include"CTransform.h"
#include "CCollisionManager.h"

//コライダ、、、衝突装置
class CCollider :public CTransform,public CTask {
public:
	
	CCharacter* mpParent;//親
	CMatrix* mpMatrix;//親行列
	float mRadius;//半径
	//コンストラクタ
	//CCollider(親、親行列、位置、回転、拡縮、半径）
	CCollider(CCharacter* parent, CMatrix* matrix, CVector position, float radius);
	//デストラクタ
	~CCollider();
	//描画
	void Render();
	//衝突判定
	//CCollision(コライダ１，コライダ２）
	//return true(衝突している）　false(衝突していない）
	static bool Collision(CCollider* m, CCollider* o);
	enum EType {
		ESPHERE,//弾コライダ
		ETRIANGLE,//三角コライダ
		ELINE,  //線分コライダ
       
	};
	EType mType;//コライダタイプ
	enum ETag {
        EWEAPONCOLLIDER,
		EPLAYERCOLLIDER,
		EBODY,//本体用
		ESEARCH,//サーチ用
		EITEMCCOLLIDER,
	    EENEMYCOLLIDER,
		EENEMY2COLLIDER,
	};
	ETag mTag;

	//頂点
	CVector mV[3];
	//デフォルトコンストラクタ
	CCollider();

    //CCollisionTriangleLine(三角コライダ、線分コライダ、調整値）
	//return:true(衝突している）、false(衝突していない)
	//調整値：衝突しない位置まで戻す値
	static bool CollisionTriangleLine(CCollider* triangle, CCollider* line, CVector* adjust);
    //CCollisionTriangleSphere(三角コライダ、球コライダ、調整値）
	//return:true(衝突している）false(衝突していない)
	//調整値：調整しない位置まで戻す値
	static bool CCollider::CollisionTriangleSphere(CCollider* triangle, CCollider* sphere, CVector* adjust);
	void ChangePriority();
	
};
#endif
