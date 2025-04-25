#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include <vector>

using namespace KamataEngine;

// ゲームシーン
class GameScene {
public:
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	~GameScene();

private:
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//3Dモデルデータ
	Model* model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//カメラ
	Camera camera_;

	//自キャラ
	Player* player_ = nullptr;

	//ブロックのモデル
	Model* blockModel_ = nullptr;

	//可変個配列
	std::vector<WorldTransform*> worldTransformBlocks_;
};
