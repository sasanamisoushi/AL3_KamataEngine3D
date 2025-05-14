#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include <vector>
#include "Math.h"
#include "Skydome.h"
#include "MapChipField.h"

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

	//デバックカメラ有効
	bool isDebugCameraActive_ = false;

	void GenerateBlocks();

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
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	//計算
	Math* math_=nullptr;

	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	//天球
	Skydome* skydome_ = nullptr;

	//3Dモデル
	Model* modelSkydome_ = nullptr;
	
	//マップチップワールド
	MapChipField* mapChipField_;
};
