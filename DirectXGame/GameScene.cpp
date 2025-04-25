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
	const uint32_t kNumBlockHorizontal = 20;
	// ブロック1個分の横幅
	const float kBlockWidth = 2.0f;
	// 要素数を変更する
	worldTransformBlocks_.resize(kNumBlockHorizontal);

	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockHorizontal; ++i) {
		worldTransformBlocks_[i] = new WorldTransform();
		worldTransformBlocks_[i]->Initialize();
		worldTransformBlocks_[i]->translation_.x = kBlockWidth * i;
		worldTransformBlocks_[i]->translation_.y = 0.0f;
	}
}

void GameScene::Update() {
	//自キャラの更新
	player_->Update();

	//ブロックの更新
	for ( WorldTransform* worldTransformBlock:worldTransformBlocks_){ 
		
		Matrix4x4 affin_mat = MakeAffineMatrix(

		)

		worldTransformBlock->matWorld_ = ;
		//定数バッファに転送する
		worldTransformBlock->TransferMatrix();
	}
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

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
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		delete worldTransformBlock;
	}
	worldTransformBlocks_.clear();
}