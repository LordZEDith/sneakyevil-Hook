/*
Rest In Peace ApocalypseCheats
*/

#pragma once

#include "Hacks.h"

// All misc visuals (Crosshairs etc) go in this class

class CVisuals : public CHack
{
public:
	void Init();
	void Draw();
	void NoScopeBorder();
	void Move(CUserCmd *pCmd, bool &bSendPacket);
private:
	void DrawCrosshair();
	void DrawSpread();
	void SpreadCrosshair();
	void DrawRecoilCrosshair();
	void DrawCenterRadar();
	void xHair();
	void Cross1();
	void Cross2();
	void Plus();
	void Circle();
	void CircleDot();
	void Heart();
	void DrawHitCrosshair();
};