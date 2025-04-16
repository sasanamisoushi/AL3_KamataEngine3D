#pragma once
#include "KamataEngine.h"

//ゲームシーン
class GameScene 
{
public:
	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();

	~GameScene();

	//スプライト
	KamataEngine::Sprite* sprite_ = nullptr;

private:
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

};
