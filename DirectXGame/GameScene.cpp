#include "GameScene.h"


using namespace KamataEngine;



void GameScene::Initialize() {
	//ファイル名を指定してテクスチャハンドルを読み込む
	textureHandle_ = TextureManager::Load("playerk.png");
	//3Dモデルの生成(自機）
	model_ = Model::Create();
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//カメラの初期化
	camera_.Initialize();

	//自キャラの更新
	player_ = new Player();
	//自キャラの初期化
	player_->Initilize(model_, textureHandle_, &camera_);
	// 3Dモデルの生成(ブロック）
	blockModel_ = Model::Create();

	// 要素数
	const uint32_t kNumBlocVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	// ブロック1個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;
	// 要素数を変更する
	worldTransformBlocks_.resize(kNumBlocVirtical);

	// キューブの生成
	for (uint32_t i = 0; i < kNumBlocVirtical; ++i) {
		//1列の要素数を設定（横方向のブロック数）
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			 if ((i + j) % 2 == 0) {
				worldTransformBlocks_[i][j] = nullptr;
				continue;
			}

			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
		}
	}

	math_ = new Math();

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

}

void GameScene::Update() {
	//自キャラの更新
	player_->Update();

	//ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			worldTransformBlock->matWorld_ = math_->MakeAffineMatrox(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
			// 定数バッファに転送する
			worldTransformBlock->TransferMatrix();
		}
	}

	#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
	#endif // DEBUG

	if (isDebugCameraActive_) {

		//デバックカメラの更新
		debugCamera_->Update();

		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		camera_.UpdateMatrix();
	}
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

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
	//player_->Draw();

	Model::PostDraw();
}

GameScene::~GameScene() {
	//3Dモデルデータの解放(自機)
	delete model_;

	//自キャラの解放
	delete player_;

	// 3Dモデルデータの解放(ブロック)
	delete blockModel_;

	//可変個配列の解放
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	//デバックカメラ
	delete debugCamera_;
}