
#include"CModel.h"
#include"CCharacter.h"
#include"CXPlayer.h"
#ifndef CTARGET_H
#define CTARGET_H

class CTarget : public CCharacter {
private:

	
public:

	CVector mPoint;
	static bool mItemAttackHit;
	//モデルデータ
	static CModel mModel;
	CTarget();

	//CEnemy2(位置、回転、拡縮）
	CTarget(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();

};
#endif
