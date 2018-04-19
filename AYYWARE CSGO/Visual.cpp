/*
Rest In Peace ApocalypseCheats
*/

#include "Visuals.h"
#include "Interfaces.h"
#include "RenderManager.h"

// Any init here
void CVisuals::Init()
{
	// Idk
}

// Don't really need to do anything in here
void CVisuals::Move(CUserCmd *pCmd, bool &bSendPacket) {}

// Main ESP Drawing loop
void CVisuals::Draw()
{
	// Crosshair
	if (Menu::Window.VisualsTab.OtherCrosshair.GetState())
		DrawCrosshair();

	// Spread
	if (Menu::Window.VisualsTab.OtherSpread.GetState())
		DrawSpread();

	// Recoil Crosshair
	if (Menu::Window.VisualsTab.OtherRecoilCrosshair.GetIndex() == 1) {
		DrawRecoilCrosshair();
	}

	switch (Menu::Window.OtherSettingsTab.OtherCrosshairType.GetIndex())
	{
	case 0:
		Interfaces::Engine->ClientCmd_Unrestricted("crosshair 1");
		break;
	case 1:
		Interfaces::Engine->ClientCmd_Unrestricted("crosshair 0");
		xHair();
		break;
	case 2:
		Interfaces::Engine->ClientCmd_Unrestricted("crosshair 0");
		Cross1();
		break;
	case 3:
		Interfaces::Engine->ClientCmd_Unrestricted("crosshair 0");
		Cross2();
		break;
	case 4:
		Interfaces::Engine->ClientCmd_Unrestricted("crosshair 0");
		Plus();
		break;
	case 5:
		Interfaces::Engine->ClientCmd_Unrestricted("crosshair 0");
		Circle();
		break;
	case 6:
		Interfaces::Engine->ClientCmd_Unrestricted("crosshair 0");
		CircleDot();
		break;
	}

	NoScopeBorder();
}

/* NoScopeBorder */
void CVisuals::NoScopeBorder()
{
	ConVar* matpostprocessing = Interfaces::CVar->FindVar("mat_postprocess_enable");

	if (Menu::Window.VisualsTab.OtherNoScope.GetState())
	{
		matpostprocessing->SetValue("0");
		if (hackManager.pLocal()->IsScoped() && hackManager.pLocal()->IsAlive())
		{
			int Width;
			int Height;
			Interfaces::Engine->GetScreenSize(Width, Height);

			Color cColor = Color(0, 0, 0, 255);
			Render::Line(Width / 2, 0, Width / 2, Height, cColor);
			Render::Line(0, Height / 2, Width, Height / 2, cColor);

		}
		else
		{

		}
	}
}

// Draw a basic crosshair
void CVisuals::DrawCrosshair()
{
	// Crosshair
	IClientEntity* pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;
	if (GameUtils::IsSniper(pWeapon))
	{
		if (!pLocal->IsScoped())
		{
			ConVar* cross = Interfaces::CVar->FindVar("weapon_debug_spread_show");
			SpoofedConvar* cross_spoofed = new SpoofedConvar(cross);
			cross_spoofed->SetInt(3);
		}
	}
}

void CVisuals::DrawSpread()
{
	IClientEntity *pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	IClientEntity* WeaponEnt = Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!hackManager.pLocal()->IsAlive())
		return;

	if (!GameUtils::IsBallisticWeapon(pWeapon))
		return;

	if (pWeapon == nullptr)
		return;

	int xs;
	int ys;
	Interfaces::Engine->GetScreenSize(xs, ys);
	xs /= 2; ys /= 2;

	auto accuracy = pWeapon->GetInaccuracy() * 550.f; //3000

	Render::FilledCircle(Vector2D(xs, ys), Color(24, 24, 24, 124), accuracy, 60);
}

// Crosshair Types - START

void CVisuals::xHair()
{
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;

	Render::Line(MidX - 4, MidY - 4, MidX + 4, MidY + 4, Color(Menu::Window.OtherSettingsTab.OtherCrosshairR.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairG.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairB.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairA.GetValue()));
	Render::Line(MidX + 4, MidY - 4, MidX - 4, MidY + 4, Color(Menu::Window.OtherSettingsTab.OtherCrosshairR.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairG.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairB.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairA.GetValue()));
}

void CVisuals::Cross1()
{
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;

	Render::Line(MidX - 8, MidY, MidX - 4, MidY, Color(Menu::Window.OtherSettingsTab.OtherCrosshairRR.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairGG.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairBB.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairAA.GetValue()));
	Render::Line(MidX, MidY - 8, MidX, MidY - 4, Color(Menu::Window.OtherSettingsTab.OtherCrosshairR.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairG.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairB.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairA.GetValue()));
	Render::Line(MidX + 8, MidY, MidX + 4, MidY, Color(Menu::Window.OtherSettingsTab.OtherCrosshairRR.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairGG.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairBB.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairAA.GetValue()));
	Render::Line(MidX, MidY + 8, MidX, MidY + 4, Color(Menu::Window.OtherSettingsTab.OtherCrosshairR.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairG.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairB.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairA.GetValue()));
}

void CVisuals::Cross2()
{
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;
	Render::Line(MidX - 8, MidY - 0, MidX + 8, MidY + 0, Color(Menu::Window.OtherSettingsTab.OtherCrosshairR.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairG.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairB.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairA.GetValue()));
	Render::Line(MidX + 0, MidY - 8, MidX - 0, MidY + 8, Color(Menu::Window.OtherSettingsTab.OtherCrosshairR.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairG.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairB.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairA.GetValue()));
	Render::Line(MidX - 4, MidY - 0, MidX + 4, MidY + 0, Color(Menu::Window.OtherSettingsTab.OtherCrosshairRR.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairGG.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairBB.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairAA.GetValue()));
	Render::Line(MidX + 0, MidY - 4, MidX - 0, MidY + 4, Color(Menu::Window.OtherSettingsTab.OtherCrosshairRR.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairGG.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairBB.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairAA.GetValue()));
}

void CVisuals::Plus()
{
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;
	static int Scale = 2;

	Render::Clear(MidX - Scale - 1, MidY - (Scale * 2) - 1, (Scale * 2) - 2, (Scale * 4) - 2, Color(Menu::Window.OtherSettingsTab.OtherCrosshairR.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairG.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairB.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairA.GetValue()));
	Render::Clear(MidX - (Scale * 2) - 1, MidY - Scale - 1, (Scale * 4) - 2, (Scale * 2) - 2, Color(Menu::Window.OtherSettingsTab.OtherCrosshairR.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairG.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairB.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairA.GetValue()));
}

void CVisuals::Circle()
{
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;
	static int Scale = 2;

	Render::Circle(MidX, MidY, Menu::Window.OtherSettingsTab.OtherCrosshairCScale.GetValue() + 8, 600, Color(Menu::Window.OtherSettingsTab.OtherCrosshairR.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairG.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairB.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairA.GetValue()));
}

void CVisuals::CircleDot()
{
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;
	static int Scale = 2;

	Render::FilledCircle(Vector2D(MidX, MidY), Color(Menu::Window.OtherSettingsTab.OtherCrosshairR.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairG.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairB.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairA.GetValue()), 2, 99);
	Render::Circle(MidX, MidY, Menu::Window.OtherSettingsTab.OtherCrosshairCScale.GetValue() + 8, 600, Color(Menu::Window.OtherSettingsTab.OtherCrosshairR.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairG.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairB.GetValue(), Menu::Window.OtherSettingsTab.OtherCrosshairA.GetValue()));
}
// Crosshair Types - END

void CVisuals::DrawHitCrosshair()
{

}
// Recoil crosshair
void CVisuals::DrawRecoilCrosshair()
{
	if (Menu::Window.RageBotTab.AccuracyRecoil.GetState())
		return;

	IClientEntity *pLocal = hackManager.pLocal();

	// Get the view with the recoil
	Vector ViewAngles;
	Interfaces::Engine->GetViewAngles(ViewAngles);
	ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

	// Build a ray going fowards at that angle
	Vector fowardVec;
	AngleVectors(ViewAngles, &fowardVec);
	fowardVec *= 10000;

	// Get ray start / end
	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = start + fowardVec, endScreen;

	if (Render::WorldToScreen(end, endScreen) && pLocal->IsAlive())
	{
		Render::Line(endScreen.x - 4, endScreen.y - 4, endScreen.x + 4, endScreen.y + 4, Color(0, 255, 0, 255));
		Render::Line(endScreen.x + 4, endScreen.y - 4, endScreen.x - 4, endScreen.y + 4, Color(0, 255, 0, 255));
		Render::Line(endScreen.x - 2, endScreen.y - 2, endScreen.x + 2, endScreen.y + 2, Color(0, 0, 0, 200));
		Render::Line(endScreen.x + 2, endScreen.y - 2, endScreen.x - 2, endScreen.y + 2, Color(0, 0, 0, 200));
	}
}
