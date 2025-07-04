#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include <vector>
#include "Math.h"
#include "Skydome.h"
#include "MapChipField.h"
#include "CameraController.h"
#include "Enemy.h"
#include "DeathPerticles.h"


using namespace KamataEngine;

//ゲームのフェーズ
enum class Phase {
	kPlay,   //ゲームプレイ
	kDeath,  //デス演出
};

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

	// 全ての当たり判定を行う
	void CheckAllCollisions();

	//フェーズの切り替え
	void ChangePhase();

	//デスフラグのgetter
	bool IsFinished() const { return finished_; }

private:
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// スプライト
	Sprite* sprite_ = nullptr;

	//3Dモデルデータ
	Model* model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//カメラ
	Camera camera_;

	//自キャラ
	Player* player_ = nullptr;

	// プレイヤーモデル
	KamataEngine::Model* playerModel_ = nullptr;

	//ブロックのモデル
	KamataEngine::Model* blockModel_ = nullptr;

	//可変個配列
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	

	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	//天球
	Skydome* skydome_ = nullptr;

	//3Dモデル
	Model* modelSkydome_ = nullptr;
	
	//マップチップワールド
	MapChipField* mapChipField_;

	//カメラコントローラー
	CameraController* cameraController_=nullptr;

	//敵キャラ
	std::list<Enemy*> enemies_;

	// 敵モデル
	KamataEngine::Model* enemyModel_ = nullptr;

	//02_11の15ページ　デスパーティクル
	DeathPerticles* deathParticles_ = nullptr;

	KamataEngine::Model* deathParticlesModel_ = nullptr;

	//ゲームの現在フェーズ
	Phase phase_;

	//終了フラグ
	bool finished_ = false;
};
