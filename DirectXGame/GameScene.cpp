#include "GameScene.h"
using namespace KamataEngine;

void GameScene::Initialize() {

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");
	// スプライト生成
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	// 3Dモデル生成
	model_ = Model::Create();
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// カメラの初期化
	camera_.Initialize();

	// マップチップの生成
	mapChipField_ = new MapChipField;

	// マップチップの更新
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	// 表示ブロックの生成
	GenerateBlocks();

	// 自キャラの更新
	player_ = new Player();

	// プレイヤーモデル
	playerModel_ = Model::CreateFromOBJ("player");
	playerAttackModel_ = Model::CreateFromOBJ("attack_effect");

	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 18);

	// 3Dモデルの生成(ブロック）
	blockModel_ = Model::CreateFromOBJ("block", true);

	player_->SetMapChipField(mapChipField_);

	// 自キャラの初期化
	player_->Initilize(playerModel_,playerAttackModel_, &camera_, playerPosition);

	// 敵のモデル
	enemyModel_ = Model::CreateFromOBJ("enemy");

	for (int32_t i = 0; i < 2; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(14, 18 - i * 2);
		newEnemy->Initialize(enemyModel_, &camera_, enemyPosition);

		enemies_.push_back(newEnemy);
	}

	deathParticlesModel_ = Model::CreateFromOBJ("deathParticle");

	//ヒットエフェクト用のモデルの読み込み
	modelParticle_ = Model::CreateFromOBJ("particle");
	HitEffect::SetModel(modelParticle_);
	HitEffect::SetCamera(&camera_);

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 02_03天球
	//  3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// 天球の生成
	skydome_ = new Skydome();
	// 天球の初期化
	skydome_->Initialize(modelSkydome_, &camera_);

	// カメラコントローラの生成
	cameraController_ = new CameraController();
	// カメラコントローラの初期化
	cameraController_->Initialize(&camera_);
	// 追従対象をセット
	cameraController_->SetTarget(player_);
	// リセット
	cameraController_->Reset();
	// 移動範囲の指定
	CameraController::Rect camerArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(camerArea);

	// ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;
}

void GameScene::Update() {

	//デスフラグの立ったエフェクトを削除
	hitEffects_.remove_if([](HitEffect* hitEffect) { 
		if (hitEffect->IsDead()) {
			delete hitEffect;
			return true;
		}
		return false;
	});

	//デスフラグの立った敵を削除
	enemies_.remove_if([](Enemy* enemy) { 
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	ChangePhase();

	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			fade_->Start(Fade::Status::FedeOut, 1.0f);
			phase_ = Phase::kPlay;
		}

		skydome_->Update();
		cameraController_->Update();

		// 自キャラの更新
		player_->Update();

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		for (HitEffect* hitEffect : hitEffects_) {
			hitEffect->Update();
		}

#ifdef _DEBUG
		if (Input::GetInstance()->TriggerKey(DIK_W)) {
			// フラグをトグル
			isDebugCameraActive_ = !isDebugCameraActive_;
		}
#endif

		if (isDebugCameraActive_) {

			// デバックカメラの更新
			debugCamera_->Update();

			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			camera_.TransferMatrix();
		} else {
			// ビュープロジェクション行列の更新と転送
			camera_.UpdateMatrix();
		}

		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				// worldTransform(* worldTransformBlock);
				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				// 定数バッファに転送する
				worldTransformBlock->TransferMatrix();
			}
		}

		break;

	case Phase::kPlay:
		// 天球の更新
		skydome_->Update();
		// 自キャラの更新
		player_->Update();

		// 敵キャラの更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// カメラコントローラの更新
		cameraController_->Update();

		if (isDebugCameraActive_) {

			// デバックカメラの更新
			debugCamera_->Update();

			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			camera_.TransferMatrix();
		} else {
			// ビュープロジェクション行列の更新と転送
			camera_.UpdateMatrix();
		}

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				// worldTransform(* worldTransformBlock);
				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				// 定数バッファに転送する
				worldTransformBlock->TransferMatrix();
			}
		}

		// 全ての当たり判定を行う
		CheckAllCollisions();
		break;
	case Phase::kDeath:
		// 天球の更新
		skydome_->Update();

		// カメラコントローラの更新
		cameraController_->Update();

		// 敵キャラの更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		if (deathParticles_ && deathParticles_->IsFinished()) {
			finished_ = true;
		}

		if (deathParticles_) {
			deathParticles_->Update();
		}

	

		break;
	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}

		// 天球の更新
		skydome_->Update();

		// カメラコントローラの更新
		cameraController_->Update();

		// 敵キャラの更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}
		
		break;
	}
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 天球の描画
	skydome_->Draw();

	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			blockModel_->Draw(*worldTransformBlock, camera_);
		}
	}

	// 自キャラの描画
	if (!player_->isDead()) {
		player_->Draw();
	}

	// 敵の描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	// デスパーティクルの描画
	if (deathParticles_) {
		deathParticles_->Draw();
	}

	for (HitEffect* hitEffect : hitEffects_) {
		hitEffect->Draw();
	}

	Model::PostDraw();

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	// スプライト描画後処理
	Sprite::PostDraw();
}

GameScene::~GameScene() {
	// 3Dモデルデータの解放(自機)
	delete model_;
	// 敵キャラの解放
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}


	for (HitEffect* hitEffect : hitEffects_) {
		delete hitEffect;
	}

	// 自キャラの解放
	delete player_;

	// 可変個配列の解放
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	delete playerModel_;
	delete playerAttackModel_;
	delete enemyModel_;

	// 3Dモデルデータの解放(ブロック)
	delete blockModel_;

	// デバックカメラ
	delete debugCamera_;

	// 天球
	delete modelSkydome_;
	delete skydome_;

	// マップチップの解放
	delete mapChipField_;

	delete modelParticle_;

	delete cameraController_;
	// デスパーティクルの解放
	delete deathParticles_;

	delete fade_;
}

void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	worldTransformBlocks_.resize(numBlockVirtical);

	// キューブの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		// 1列の要素数を設定（横方向のブロック数）
		worldTransformBlocks_[i].resize(numBlockHorizontal);
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {

				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::CheckAllCollisions() {

	// 判定対象1と2の座標
	AABB aabb1, aabb2;

#pragma region 自キャラと敵キャラのあたり判定

	// 自キャラの座標
	aabb1 = player_->GetAABB();

	// 自キャラと敵弾全てのあたり判定
	for (Enemy* enemy : enemies_) {
		if (enemy->IsCollisionDisabled()) {
			continue;
		}
		// 敵弾の座標
		aabb2 = enemy->GetAABB();

		// AABB同士の交差判定
		if (IsCollision(aabb1, aabb2)) {
			// 自キャラの衝突時間数を呼び出す
			player_->OnCollision(enemy);
			// 敵の衝突時間数を呼び出す
			enemy->OnCollision(player_);
		}
	}
#pragma endregion
}

void GameScene::ChangePhase() {
	switch (phase_) {
	case Phase::kPlay:
		if (player_->isDead()) {
			// 死亡演出フェーズに切り替え
			phase_ = Phase::kDeath;
			// 自キャラの座標を取得
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			deathParticles_ = new DeathPerticles;
			deathParticles_->Initialize(deathParticlesModel_, &camera_, deathParticlesPosition);
		}
		break;
	case Phase::kDeath:
		break;
	}
}

void GameScene::CreateHitEffect(const KamataEngine::Vector3& position) { 
	HitEffect* newHitEffect = HitEffect::Create(position);
	hitEffects_.push_back(newHitEffect);
}
