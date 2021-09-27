#include "CCollisionManager.h"
#include"CCollider.h"
#include"CTaskManager.h"
//マネージャのインスタンス
CCollisionManager* CCollisionManager::mpInstance = 0;
//インスタンスの取得
CCollisionManager* CCollisionManager::Get() {
	//インスタンスがなければ
	if (mpInstance == 0) {
		mpInstance = new CCollisionManager();
	}
	return mpInstance;
}
void CCollisionManager::Collision() {
	//タスクリストを使う
	//現在位置を先頭にする
	CCollider* task = (CCollider*)mHead.mpNext;
	//最後まで来たら終了
	//taskの次の場合
	while (task->mpNext) {
		//現在位置の次を決める
		CCollider* next = (CCollider*)task->mpNext;
		//次が０になったら終了
		//nextの次の場合
		while (next->mpNext) {
			//親の衝突処理を呼び出す
			//自分の衝突処理
			if (task->mpParent)//taskの親のとき
				task->mpParent->Collision(task, next);//taskからnext
			//相手の衝突処理
			if (next->mpParent)//nextの親のとき
				next->mpParent->Collision(next, task);//nextからtask
			//次を求める
			next = (CCollider*)next->mpNext;

		}
		//現在位置を次にする
		task = (CCollider*)task->mpNext;

	}
}
void CCollisionManager::Collision(CCollider* c, int range) {

	//現在位置を先頭にする
	CCollider* task = (CCollider*)mHead.mpNext;
	//範囲まで読み飛ばし
	//range:適用する範囲を設定する数字
	//優先度+rangeより優先度の小さいタスクに適用

	while (task->mpNext) {
		if (task->mPriority <= c->mPriority + range) {
	      //ループをやめる
			break;
		}
		//現在位置を次にする
		task = (CCollider*)task->mpNext;
	}
	//最後まで来たら終了する
	//優先度ーrangeより優先度の小さいタスクには適用しない
	while (task->mpNext) {
		if (task->mPriority <= c->mPriority - range) {
			break;
		}
		//親の衝突処理を呼び出す
		//自分の衝突処理
		if (c->mpParent && c != task)
			c->mpParent->Collision(c, task);
		//現在位置を次にする
		task = (CCollider*)task->mpNext;
	}
}