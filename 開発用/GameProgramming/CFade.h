#ifndef CFADE_H
#define CFADE_H
class CFade
{
public:
	enum eFadeMode {
		FADE_IN = 0,	//フェードイン
		FADE_OUT,		//フェードアウト
	};
private:
	float mFade;		//フェード変動値
	float mFadeSpeed;	//フェード変動速度
	bool mbFade;		//フェードフラグ
	eFadeMode mFadeType;//フェード状態
	static CFade* gInst;//インスタンス保持用

public:
	//コンストラクタ
	CFade();

	//フェード設定
	static void SetFade(eFadeMode type, float speed = 0.01f);
	//フェード完了確認用
	static bool IsFadeEnd();
	void Update();//更新
	void Render();//描画
};
#endif

