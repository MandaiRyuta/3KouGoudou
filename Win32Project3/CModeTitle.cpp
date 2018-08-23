#include "CModeTitle.h"
#include "gamemanager.h"
#include "CModeGame.h"
CScene2D *CModeTitle::presskey_;
void CModeTitle::Init()
{
	POLYGONSIZE polygonsize = {};
	polygonsize.dx_ = 0.0f;
	polygonsize.dy_ = 0.0f;
	polygonsize.dw_ = windows_rect::SCREEN_WIDTH + 24;
	polygonsize.dh_ = windows_rect::SCREEN_HEIGHT + 54;
	polygonsize.tcx_ = 0;
	polygonsize.tcy_ = 0;
	polygonsize.tcw_ = 1280;
	polygonsize.tch_ = 1024;
	polygonsize.color_ = D3DCOLOR_RGBA(255, 255, 255, 255);
	polygonsize.affine_ = true;
	CScene2D::Create(true,polygonsize, TEXTURERS::BACKGROUND, 1280, 1024,false);
	polygonsize.dx_ = 0.0f;
	polygonsize.dy_ = 0.0f;
	polygonsize.dw_ = windows_rect::SCREEN_WIDTH + 24;
	polygonsize.dh_ = windows_rect::SCREEN_HEIGHT + 54;
	polygonsize.tcx_ = 0;
	polygonsize.tcy_ = 0;
	polygonsize.tcw_ = 1000;
	polygonsize.tch_ = 1000;
	CScene2D::Create(true, polygonsize, TEXTURERS::TITLELOGO, 1000, 1000, false);
	polygonsize.dx_ = 100.0f;
	polygonsize.dy_ = 200.f;
	polygonsize.dw_ = 425.f;
	polygonsize.dh_ = 300.f;
	polygonsize.tch_ = 400;
	presskey_ = CScene2D::Create(true, polygonsize, TEXTURERS::PRESSSPACE, 1000, 400, false);
	alpha = 0;
}

void CModeTitle::Uninit()
{
	CScene::ReleaseAll();
}

void CModeTitle::Update()
{
	if (alpha >= 255)
	{
		alpha = 0;
	}
	presskey_->ColorSet(D3DCOLOR_RGBA(255, 255, 255, alpha));
	CInputKeyboard* pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	if (pInputKeyboard->GetKeyTrigger(DIK_SPACE))
		CManager::SetMode(new CModeGame());

	alpha+=3;
}

void CModeTitle::Draw()
{
}
