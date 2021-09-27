#ifndef CTASKMANAGER_H
#define CTASKMANAGER_H
#include"CTask.h"
class CTaskManager {
	//シングルトンモデル
	//インスタンスモデルを一つだけ生成し、そのインスタンスにのみアクセスさせる仕組み
private:
	//タスクマネージャのインスタンス
	static CTaskManager* mpInstance;
protected:
	//デフォルトコンストラクタ
		CTaskManager();
public:
	//インスタンスの取得
	static CTaskManager* Get();
	CTask mHead;//先頭タスク
	CTask mTail;//最終タスク
		
		//リストから削除
		//Remove(タスクのポインタ）
		void Remove(CTask *task);
	//デストラクタ
	virtual ~CTaskManager();
	//リストに追加
	//Add（タスクのポインタ）
	void Add(CTask* addTask);
	//更新	
	void Update();
	//描画
	void Render();
	//タスクの削除
	void Delete();
	void TaskCollision();
};


#endif

