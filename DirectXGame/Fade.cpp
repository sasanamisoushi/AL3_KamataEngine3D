#include "Fade.h"

using namespace KamataEngine;

void Fade::Initialize() {

	sprite_ = new Sprite();
	sprite_ = Sprite::Create(0, Vector2{});
	sprite_->SetSize(Vector2(WinApp::kWindowWidth, WinApp::kWindowHeight));
	sprite_->SetColor(Vector4(0, 0, 0, 1));

}

void Fade::Update() {}

void Fade::Draw() {

	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	sprite_->Draw();
	Sprite::PostDraw();


}
