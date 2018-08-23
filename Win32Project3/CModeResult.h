#pragma once
#include "SceneMode.h"
#include "scene2d.h"
class CModeResult : public CMode
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	static CScene2D* gameclear_;
	int alpha;
	int red;
	int blue;
	int green;
};