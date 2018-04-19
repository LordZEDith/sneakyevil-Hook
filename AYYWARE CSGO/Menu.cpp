/*
ApocalypseCheats
------------------------------
Contributors:
XBoom
Ma$$A$
Fldy.
madlifer
control1337
CyclesPrograming
FZCat1337
eeioruewo0iruwe
UC Community <3
*/

#include "Menu.h"
#include "Controls.h"
#include "Hooks.h"
#include "Interfaces.h"
#include "CRC32.h"
#include <string>
#include "ItemDefinitions.hpp"
#include "Hooks.h"
#include "Hacks.h"
#include "Chams.h"
#include "ESP.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "MiscHacks.h"
#include "CRC32.h"
#include "Resolver.h"
#include "MiscHacks.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 585

extern int pc;

ApocalypseWindow Menu::Window;

void Unloadbk()
{
	DoUnload = true;
}

void NApplyCallbk()
{
	
}
void Memz()
{
//	Hooks::XD3();
}
void SaveCallbk()
{
	switch (Menu::Window.SettingsTab.SetNr.GetIndex())
	{
	case 0:
		GUI.SaveWindowState(&Menu::Window, "legit.xml");
		break;
	case 1:
		GUI.SaveWindowState(&Menu::Window, "legit2.xml");
		break;
	case 2:
		GUI.SaveWindowState(&Menu::Window, "legitback.xml");
		break;
	case 3:
		GUI.SaveWindowState(&Menu::Window, "legitback2.xml");
		break;
	case 4:
		GUI.SaveWindowState(&Menu::Window, "semirage.xml");
		break;
	case 5:
		GUI.SaveWindowState(&Menu::Window, "ragemm1.xml");
		break;
	case 6:
		GUI.SaveWindowState(&Menu::Window, "ragemm2.xml");
		break;
	case 7:
		GUI.SaveWindowState(&Menu::Window, "rage1.xml");
		break;
	case 8:
		GUI.SaveWindowState(&Menu::Window, "rage2.xml");
		break;
	case 9:
		GUI.SaveWindowState(&Menu::Window, "rage3.xml");
		break;
	case 10:
		GUI.SaveWindowState(&Menu::Window, "faceit.xml");
		break;
	case 11:
		GUI.SaveWindowState(&Menu::Window, "faceit1.xml");
		break;
	case 12:
		GUI.SaveWindowState(&Menu::Window, "nospread.xml");
		break;
	case 13:
		GUI.SaveWindowState(&Menu::Window, "nospread1.xml");
		break;
	}
	
}

void LoadCallbk()
{
	switch (Menu::Window.SettingsTab.SetNr.GetIndex())
	{
	case 0:
		GUI.LoadWindowState(&Menu::Window, "legit.xml");
		break;
	case 1:
		GUI.LoadWindowState(&Menu::Window, "legit2.xml");
		break;
	case 2:
		GUI.LoadWindowState(&Menu::Window, "legitback.xml");
		break;
	case 3:
		GUI.LoadWindowState(&Menu::Window, "legitback2.xml");
		break;
	case 4:
		GUI.LoadWindowState(&Menu::Window, "semirage.xml");
		break;
	case 5:
		GUI.LoadWindowState(&Menu::Window, "ragemm1.xml");
		break;
	case 6:
		GUI.LoadWindowState(&Menu::Window, "ragemm2.xml");
		break;
	case 7:
		GUI.LoadWindowState(&Menu::Window, "rage1.xml");
		break;
	case 8:
		GUI.LoadWindowState(&Menu::Window, "rage2.xml");
		break;
	case 9:
		GUI.LoadWindowState(&Menu::Window, "rage3.xml");
		break;
	case 10:
		GUI.LoadWindowState(&Menu::Window, "faceit.xml");
		break;
	case 11:
		GUI.LoadWindowState(&Menu::Window, "faceit1.xml");
		break;
	case 12:
		GUI.LoadWindowState(&Menu::Window, "nospread.xml");
		break;
	case 13:
		GUI.LoadWindowState(&Menu::Window, "nospread1.xml");
		break;
	}
}

void UnLoadCallbk()
{
	DoUnload = true;
}

void KnifeApplyCallbk()
{
	static ConVar* Meme = Interfaces::CVar->FindVar("cl_fullupdate");
	Meme->nFlags &= ~FCVAR_CHEAT;
	Interfaces::Engine->ClientCmd_Unrestricted("cl_fullupdate");
}

void ClanTagApplyCallbk()
{
	if (Menu::Window.SettingsTab.ClanTagText.getText().length() > 1)
	{
		Menu::Window.MiscTab.OtherClantag.SelectIndex(1);
	}
	else
	{
		Menu::Window.MiscTab.OtherClantag.SelectIndex(0);
	}
}

/*void SetAKCB()
{
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	IClientEntity* WeaponEnt = Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	CBaseCombatWeapon* Weapon = (CBaseCombatWeapon*)WeaponEnt;
	*Weapon->FallbackPaintKit() = 524;

}*/
void ApocalypseWindow::Setup()
{
	SetPosition(50, 50);
	SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	if (pc == 1)
	{
		SetTitle("sneakyevil Hook | Welcome sneakyevil :)");
	}
	if (pc == 2)
	{
		SetTitle("sneakyevil Hook | Welcome problemikk :)");
	}
	if (pc == 3)
	{
		SetTitle("sneakyevil Hook | Welcome xde :)");
	}
	if (pc == 4)
	{
		SetTitle("sneakyevil Hook | Welcome kuchyy :)");
	}
	if (pc == 5)
	{
		SetTitle("sneakyevil Hook | Welcome Veselka :)");
	}
	if (pc == 6)
	{
		SetTitle("sneakyevil Hook | Welcome Tomik :)");
	}
	if (pc == 7)
	{
		SetTitle("sneakyevil Hook | Welcome vodka :)");
	}
	if (pc == 8)
	{
		SetTitle("sneakyevil Hook | Welcome xp1geoN :)");
	}

	RegisterTab(&RageBotTab);
	RegisterTab(&LegitBotTab);
	RegisterTab(&VisualsTab);
	RegisterTab(&MiscTab);
	RegisterTab(&ColorTab);
	RegisterTab(&SettingsTab);
	RegisterTab(&OtherSettingsTab);
	RegisterTab(&SkinchangerTab);

	//RECT Client = GetClientArea();
	//Client.bottom -= 29;

	RageBotTab.Setup();
	LegitBotTab.Setup();
	VisualsTab.Setup();
	MiscTab.Setup();
	ColorTab.Setup();
	SettingsTab.Setup();
	OtherSettingsTab.Setup();
	SkinchangerTab.Setup();
}

void CRageBotTab::Setup()
{
	
	SetTitle("Rage");

	ActiveLabel.SetPosition(16, 16);
	ActiveLabel.SetText("Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(66, 16);
	RegisterControl(&Active);

#pragma region Aimbot

	AimbotGroup.SetPosition(16, 48);
	AimbotGroup.SetText("Rage");
	AimbotGroup.SetSize(376, 270);
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl("Enable", this, &AimbotEnable);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl("Auto Fire", this, &AimbotAutoFire);

	AimbotFov.SetFileId("aim_fov");
	AimbotFov.SetBoundaries(0.f, 180.f);
	AimbotFov.SetValue(180.f);
	AimbotGroup.PlaceLabledControl("FOV Range", this, &AimbotFov);

	AimbotSilentAim.SetFileId("aim_silent");
	AimbotGroup.PlaceLabledControl("Silent Aim", this, &AimbotSilentAim);

	AimbotPerfectSilentAim.SetFileId("aim_psilent");
	AimbotGroup.PlaceLabledControl("Perfect Silent", this, &AimbotPerfectSilentAim);

	AWPAtBody.SetFileId("aim_awpatbody");
	AimbotGroup.PlaceLabledControl("AWP at Body", this, &AWPAtBody);

	HitChanceAuto.SetFileId("aim_hitchancefromsettings");
	AimbotGroup.PlaceLabledControl("Hit Chance From Settings", this, &HitChanceAuto);

	AimbotAimStep.SetFileId("aim_aimstep");
	AimbotGroup.PlaceLabledControl("Aim Step", this, &AimbotAimStep);

	AimbotKeyBind.SetFileId("aim_key");
	AimbotGroup.PlaceLabledControl("Baim Key", this, &AimbotKeyBind);

#pragma endregion Aimbot Controls Get Setup in here

#pragma region Target
	TargetGroup.SetPosition(16, 334);
	TargetGroup.SetText("Target");
	TargetGroup.SetSize(376, 168);
	RegisterControl(&TargetGroup);

	TargetSelection.SetFileId("tgt_selection");
	TargetSelection.AddItem("FOV");
	TargetSelection.AddItem("Distance");
	TargetSelection.AddItem("Health");
	TargetGroup.PlaceLabledControl("Selection", this, &TargetSelection);

	TargetFriendlyFire.SetFileId("tgt_friendlyfire");
	TargetGroup.PlaceLabledControl("Shot Trought Team", this, &TargetFriendlyFire);

	TargetHitbox.SetFileId("tgt_hitbox");
	TargetHitbox.AddItem("Head");
	TargetHitbox.AddItem("Neck");
	TargetHitbox.AddItem("Chest");
	TargetHitbox.AddItem("Stomach");
	TargetHitbox.AddItem("Shins");
	TargetGroup.PlaceLabledControl("Hitbox", this, &TargetHitbox);

	TargetHitscan.SetFileId("tgt_hitscan");
	TargetHitscan.AddItem("Off");
	TargetHitscan.AddItem("Simple");
	TargetHitscan.AddItem("Standard");
	TargetHitscan.AddItem("Extreme");
	TargetGroup.PlaceLabledControl("Hitscan", this, &TargetHitscan);

	TargetMultipoint.SetFileId("tgt_multipoint");
	TargetGroup.PlaceLabledControl("Multipoint", this, &TargetMultipoint);

	TargetPointscale.SetFileId("tgt_pointscale");
	TargetPointscale.SetBoundaries(0.f, 10.f);
	TargetPointscale.SetValue(5.f);
	TargetGroup.PlaceLabledControl("Point Scale", this, &TargetPointscale);
#pragma endregion Targetting controls 

#pragma region Accuracy
	AccuracyGroup.SetPosition(408, 260);
	AccuracyGroup.SetText("Accuracy");
	AccuracyGroup.SetSize(360, 258); //280
	RegisterControl(&AccuracyGroup);

	AccuracyRecoil.SetFileId("acc_norecoil");
	AccuracyGroup.PlaceLabledControl("NoRecoil", this, &AccuracyRecoil);

	AccuracyAutoWall.SetFileId("acc_awall");
	AccuracyGroup.PlaceLabledControl("AutoWall", this, &AccuracyAutoWall);

	AccuracyMinimumDamage.SetFileId("acc_mindmg");
	AccuracyMinimumDamage.SetBoundaries(1.f, 100.f);
	AccuracyMinimumDamage.SetValue(1.f);
	AccuracyGroup.PlaceLabledControl("AutoWall Minimum Damage", this, &AccuracyMinimumDamage);

	BaimIfUnderXHealth.SetFileId("aim_baimifunderxhealth");
	BaimIfUnderXHealth.SetBoundaries(0.f, 100.f);
	BaimIfUnderXHealth.SetValue(0.f);
	AccuracyGroup.PlaceLabledControl("Body-Aim if HP is lower than", this, &BaimIfUnderXHealth);

	AccuracyAutoStop.SetFileId("acc_stop");
	AccuracyGroup.PlaceLabledControl("Auto Stop", this, &AccuracyAutoStop);

	AccuracyAutoScope.SetFileId("acc_scope");
	AccuracyGroup.PlaceLabledControl("Auto Scope", this, &AccuracyAutoScope);

	AccuracyHitchance.SetFileId("acc_chance");
	AccuracyHitchance.SetBoundaries(0, 100);
	AccuracyHitchance.SetValue(0);
	AccuracyGroup.PlaceLabledControl("Hit Chance", this, &AccuracyHitchance);

	AccuracyPVS.SetFileId("acc_pvsfix");
	AccuracyGroup.PlaceLabledControl("PVS Fix", this, &AccuracyPVS);

	AccuracyPrediction.SetFileId("acc_accuracyprediciton");
	AccuracyPrediction.AddItem("Off");
	AccuracyPrediction.AddItem("Engine");
	AccuracyPrediction.AddItem("Backtracking");
	AccuracyPrediction.AddItem("Engine & Backtracking");
	AccuracyPrediction.AddItem("MemeTracking");
	AccuracyGroup.PlaceLabledControl("Prediction Mode", this, &AccuracyPrediction);

	AccuracyResolver.SetFileId("acc_aaa");
	AccuracyResolver.AddItem("Off");
	AccuracyResolver.AddItem("Ma$$A$$ Resolver");
	AccuracyResolver.AddItem("sneakyevil Resolver");
	AccuracyResolver.AddItem("Classic");
	AccuracyResolver.AddItem("Brute");
	AccuracyResolver.AddItem("LBY Breaker");
	AccuracyResolver.AddItem("LBY Delta");
	AccuracyGroup.PlaceLabledControl("Anti Aim Resolver", this, &AccuracyResolver);

	//AccuracyPositionAdjustment.SetFileId("acc_posadj");
	//AccuracyGroup.PlaceLabledControl("Position Adjustment", this, &AccuracyPositionAdjustment);

#pragma endregion  Accuracy controls get Setup in here

#pragma region AntiAim
	AntiAimGroup.SetPosition(408, 15); //344
	AntiAimGroup.SetText("Anti Aim");
	AntiAimGroup.SetSize(360, 235);
	RegisterControl(&AntiAimGroup);

	AntiAimEnable.SetFileId("aa_enable");
	AntiAimGroup.PlaceLabledControl("Enable", this, &AntiAimEnable);

	AntiAimPitch.SetFileId("aa_x");
	AntiAimPitch.AddItem("None");
	AntiAimPitch.AddItem("Down");
	AntiAimPitch.AddItem("Up");
	AntiAimPitch.AddItem("SMAC Safe");
	AntiAimPitch.AddItem("Jitter");
	AntiAimPitch.AddItem("Static (Anti-UT)");
	AntiAimPitch.AddItem("Fake Down (Anti-UT)");
	AntiAimPitch.AddItem("Lisp Down (Anti-UT)");
	AntiAimPitch.AddItem("Lisp Up (Anti-UT)");
	AntiAimPitch.AddItem("Emotion");
	AntiAimPitch.AddItem("Dance");
	AntiAimPitch.AddItem("Custom");
	AntiAimGroup.PlaceLabledControl("Pitch", this, &AntiAimPitch);

	AntiAimYaw.SetFileId("aa_y");
	AntiAimYaw.AddItem("None");
	AntiAimYaw.AddItem("Fake Edge");
	AntiAimYaw.AddItem("Fake Sideways (MM DD)");
	AntiAimYaw.AddItem("Fake Static (Anti-UT)");
	AntiAimYaw.AddItem("Fake LBY");
	AntiAimYaw.AddItem("Fake LBY 2");
	AntiAimYaw.AddItem("Fake & Real LBY");
	AntiAimYaw.AddItem("Fake Side LBY");
	AntiAimYaw.AddItem("Fake Forward");
	AntiAimYaw.AddItem("Fake Head");
	AntiAimYaw.AddItem("Fake Troll");
	AntiAimYaw.AddItem("Slow Spin");
	AntiAimYaw.AddItem("T Jitter");
	AntiAimYaw.AddItem("Back Jitter");
	AntiAimYaw.AddItem("Lisp Jitter (Anti-UT)");
	AntiAimYaw.AddItem("Backwards");
	AntiAimYaw.AddItem("Donut (MM DD)");
	AntiAimYaw.AddItem("Hypno (MM DD)");
	AntiAimYaw.AddItem("LBY Spin (MM DD)");
	AntiAimYaw.AddItem("LBY Break");
	AntiAimYaw.AddItem("Custom sneakyevil #1");
	AntiAimYaw.AddItem("Custom sneakyevil #2");
	AntiAimYaw.AddItem("Custom sneakyevil #3");
	AntiAimYaw.AddItem("Custom sneakyevil #4");
	AntiAimGroup.PlaceLabledControl("Yaw Standing", this, &AntiAimYaw);

	AntiAimYawMoving.SetFileId("aa_y_moving");
	AntiAimYawMoving.AddItem("None (Use Standing)");
	AntiAimYawMoving.AddItem("Fake Edge");
	AntiAimYawMoving.AddItem("Fake Sideways (MM DD)");
	AntiAimYawMoving.AddItem("Fake Static (Anti-UT)");
	AntiAimYawMoving.AddItem("Fake LBY");
	AntiAimYawMoving.AddItem("Fake LBY 2");
	AntiAimYawMoving.AddItem("Fake & Real LBY");
	AntiAimYawMoving.AddItem("Fake Side LBY");
	AntiAimYawMoving.AddItem("Fake Forward");
	AntiAimYawMoving.AddItem("Fake Head");
	AntiAimYawMoving.AddItem("Fake Troll");
	AntiAimYawMoving.AddItem("Slow Spin");
	AntiAimYawMoving.AddItem("T Jitter");
	AntiAimYawMoving.AddItem("Back Jitter");
	AntiAimYawMoving.AddItem("Lisp Jitter (Anti-UT)");
	AntiAimYawMoving.AddItem("Backwards");
	AntiAimYawMoving.AddItem("Donut (MM DD)");
	AntiAimYawMoving.AddItem("Hypno (MM DD)");
	AntiAimYawMoving.AddItem("LBY Spin (MM DD)");
	AntiAimYawMoving.AddItem("LBY Break");
	AntiAimYawMoving.AddItem("Custom sneakyevil #1");
	AntiAimYawMoving.AddItem("Custom sneakyevil #2");
	AntiAimYawMoving.AddItem("Custom sneakyevil #3");
	AntiAimYawMoving.AddItem("Custom sneakyevil #4");
	AntiAimGroup.PlaceLabledControl("Yaw Moving", this, &AntiAimYawMoving);

	AntiAimYawJumping.SetFileId("aa_y_jumping");
	AntiAimYawJumping.AddItem("None (Use Moving)");
	AntiAimYawJumping.AddItem("Fake Edge");
	AntiAimYawJumping.AddItem("Fake Sideways (MM DD)");
	AntiAimYawJumping.AddItem("Fake Static (Anti-UT)");
	AntiAimYawJumping.AddItem("Fake LBY");
	AntiAimYawJumping.AddItem("Fake LBY 2");
	AntiAimYawJumping.AddItem("Fake & Real LBY");
	AntiAimYawJumping.AddItem("Fake Side LBY");
	AntiAimYawJumping.AddItem("Fake Forward");
	AntiAimYawJumping.AddItem("Fake Head");
	AntiAimYawJumping.AddItem("Fake Troll");
	AntiAimYawJumping.AddItem("Slow Spin");
	AntiAimYawJumping.AddItem("T Jitter");
	AntiAimYawJumping.AddItem("Back Jitter");
	AntiAimYawJumping.AddItem("Lisp Jitter (Anti-UT)");
	AntiAimYawJumping.AddItem("Backwards");
	AntiAimYawJumping.AddItem("Donut (MM DD)");
	AntiAimYawJumping.AddItem("Hypno (MM DD)");
	AntiAimYawJumping.AddItem("LBY Spin (MM DD)");
	AntiAimYawJumping.AddItem("LBY Break");
	AntiAimYawJumping.AddItem("Custom sneakyevil #1");
	AntiAimYawJumping.AddItem("Custom sneakyevil #2");
	AntiAimYawJumping.AddItem("Custom sneakyevil #3");
	AntiAimYawJumping.AddItem("Custom sneakyevil #4");
	AntiAimGroup.PlaceLabledControl("Yaw Jumping", this, &AntiAimYawJumping);

	/*AntiAimZeta.SetFileId("aa_z");
	AntiAimZeta.AddItem("None");
	AntiAimZeta.AddItem("Sideways (Anti-UT)");
	AntiAimZeta.AddItem("Static (Anti-UT)");
	AntiAimGroup.PlaceLabledControl("Roll [NOSPREAD ONLY]", this, &AntiAimZeta);*/

	CustomPitch.SetFileId("aa_offset");
	CustomPitch.SetBoundaries(-89, 89);
	CustomPitch.SetValue(0);
	AntiAimGroup.PlaceLabledControl("Custom Pitch", this, &CustomPitch);

	AntiAimKnife.SetFileId("aa_knife");
	AntiAimGroup.PlaceLabledControl("AA with Knife", this, &AntiAimKnife);

	/*AntiAimTarget.SetFileId("aa_target");
	AntiAimGroup.PlaceLabledControl("AA at Target", this, &AntiAimTarget);*/

	AntiAimEdge.SetFileId("aa_edge");
	AntiAimGroup.PlaceLabledControl("Auto Edge", this, &AntiAimEdge);

	AntiAimDrawLines.SetFileId("aa_drawlines");
	AntiAimGroup.PlaceLabledControl("AA Lines", this, &AntiAimDrawLines);

	//AntiAimTarget.SetFileId("aa_target");
	//AntiAimGroup.PlaceLabledControl("SpinBot At Target", this, &AntiAimTarget);
#pragma endregion  AntiAim controls get setup in here
}

void CLegitBotTab::Setup()
{
	SetTitle("Legit");

	ActiveLabel.SetPosition(16, 16);
	ActiveLabel.SetText("Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(66, 16);
	RegisterControl(&Active);

#pragma region Aimbot
	AimbotGroup.SetPosition(16, 48);
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.SetSize(240, 210);
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl("Enable", this, &AimbotEnable);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl("Auto Fire", this, &AimbotAutoFire);

	AimbotFriendlyFire.SetFileId("aim_friendfire");
	AimbotGroup.PlaceLabledControl("Friendly Fire", this, &AimbotFriendlyFire);

	AimbotSmokeCheck.SetFileId("aim_smokecheck");
	AimbotGroup.PlaceLabledControl("Smoke Check", this, &AimbotSmokeCheck);
	
	AimbotAutoPistol.SetFileId("aim_apistol");
	AimbotGroup.PlaceLabledControl("Auto Pistol", this, &AimbotAutoPistol);

	LegitBackTracking.SetFileId("aim_backtracking");
	AimbotGroup.PlaceLabledControl("Backtracking", this, &LegitBackTracking);

	LegitBackTrackingTick.SetFileId("trig_time");
	LegitBackTrackingTick.SetBoundaries(1, 11.5);
	LegitBackTrackingTick.SetValue(1);
	AimbotGroup.PlaceLabledControl("Backtracking Tick", this, &LegitBackTrackingTick);

#pragma endregion Aimbot shit

#pragma region Triggerbot
	TriggerGroup.SetPosition(272, 48);
	TriggerGroup.SetText("Triggerbot");
	TriggerGroup.SetSize(240, 210);
	RegisterControl(&TriggerGroup);

	TriggerEnable.SetFileId("trig_enable");
	TriggerGroup.PlaceLabledControl("Enable", this, &TriggerEnable);

	TriggerKeyPress.SetFileId("trig_onkey");
	TriggerGroup.PlaceLabledControl("On Key Press", this, &TriggerKeyPress);

	TriggerKeyBind.SetFileId("trig_key");
	TriggerGroup.PlaceLabledControl("Key Bind", this, &TriggerKeyBind);

	TriggerDelay.SetFileId("trig_time");
	TriggerDelay.SetBoundaries(1.f, 1000.f);
	TriggerDelay.SetValue(1.f);
	TriggerGroup.PlaceLabledControl("Delay (ms)", this, &TriggerDelay);

	TriggerRecoil.SetFileId("trig_recoil");
	TriggerGroup.PlaceLabledControl("Recoil", this, &TriggerRecoil);

	TriggerFilterGroup.SetPosition(528, 48);
	TriggerFilterGroup.SetText("Triggerbot Filter");
	TriggerFilterGroup.SetSize(240, 210);
	RegisterControl(&TriggerFilterGroup);

	TriggerHead.SetFileId("trig_head");
	TriggerFilterGroup.PlaceLabledControl("Head", this, &TriggerHead);

	TriggerChest.SetFileId("trig_chest");
	TriggerFilterGroup.PlaceLabledControl("Chest", this, &TriggerChest);

	TriggerStomach.SetFileId("trig_stomach");
	TriggerFilterGroup.PlaceLabledControl("Stomach", this, &TriggerStomach);

	TriggerArms.SetFileId("trig_arms");
	TriggerFilterGroup.PlaceLabledControl("Arms", this, &TriggerArms);

	TriggerLegs.SetFileId("trig_legs");
	TriggerFilterGroup.PlaceLabledControl("Legs", this, &TriggerLegs);

	TriggerTeammates.SetFileId("trig_teammates");
	TriggerFilterGroup.PlaceLabledControl("Friendly Fire", this, &TriggerTeammates);
#pragma endregion Triggerbot stuff

#pragma region Main Weapon
	WeaponMainGroup.SetPosition(16, 274);
	WeaponMainGroup.SetText("Rifles/Other");
	WeaponMainGroup.SetSize(240, 220);
	RegisterControl(&WeaponMainGroup);

	WeaponMainSpeed.SetFileId("main_speed");
	WeaponMainSpeed.SetBoundaries(0.01f, 100.f); 
	WeaponMainSpeed.SetValue(1.0f);
	WeaponMainGroup.PlaceLabledControl("Speed", this, &WeaponMainSpeed);

	WeaponMainFoV.SetFileId("main_fov");
	WeaponMainFoV.SetBoundaries(0.f, 30.f);
	WeaponMainFoV.SetValue(5.f);
	WeaponMainGroup.PlaceLabledControl("Fov", this, &WeaponMainFoV);

	WeaponMainRecoil.SetFileId("main_recoil");
	WeaponMainRecoil.SetBoundaries(0.f, 2.f);
    WeaponMainRecoil.SetValue(0.f);
	WeaponMainGroup.PlaceLabledControl("Recoil", this, &WeaponMainRecoil);

	WeaponMainPSilent.SetFileId("main_psilent");
	WeaponMainGroup.PlaceLabledControl("Silent", this, &WeaponMainPSilent);

	WeaponMainInacc.SetFileId("main_inacc");
	WeaponMainInacc.SetBoundaries(0.f, 15.f);
	WeaponMainGroup.PlaceLabledControl("Inaccuracy", this, &WeaponMainInacc);

	WeaponMainHitbox.SetFileId("main_hitbox");
	WeaponMainHitbox.AddItem("Head");
	WeaponMainHitbox.AddItem("Neck");
	WeaponMainHitbox.AddItem("Chest");
	WeaponMainHitbox.AddItem("Stomach");
	WeaponMainGroup.PlaceLabledControl("Primary Hitbox", this, &WeaponMainHitbox);

	WeaponMainKey.SetFileId("main_key");
	WeaponMainGroup.PlaceLabledControl("Key Bind", this, &WeaponMainKey);

#pragma endregion

#pragma region Pistols
	WeaponPistGroup.SetPosition(272, 274);
	WeaponPistGroup.SetText("Pistols");
	WeaponPistGroup.SetSize(240, 220);
	RegisterControl(&WeaponPistGroup);

	WeaponPistSpeed.SetFileId("pist_speed");
	WeaponPistSpeed.SetBoundaries(0.01f, 100.f);
	WeaponPistSpeed.SetValue(1.0f);
	WeaponPistGroup.PlaceLabledControl("Speed", this, &WeaponPistSpeed);

	WeaponPistFoV.SetFileId("pist_fov");
	WeaponPistFoV.SetBoundaries(0.1f, 5.f);
	WeaponPistFoV.SetValue(0.78f);
	WeaponPistGroup.PlaceLabledControl("Fov", this, &WeaponPistFoV);

	WeaponPistRecoil.SetFileId("pist_recoil");
	WeaponPistRecoil.SetBoundaries(0.f, 2.f);
	WeaponPistRecoil.SetValue(0.f);
	WeaponPistGroup.PlaceLabledControl("Recoil", this, &WeaponPistRecoil);

	WeaponPistPSilent.SetFileId("pist_psilent");
	WeaponPistGroup.PlaceLabledControl("Silent", this, &WeaponPistPSilent);

	WeaponPistInacc.SetFileId("pist_inacc");
	WeaponPistInacc.SetBoundaries(0.f, 15.f);
	WeaponPistInacc.SetValue(0.f);
	WeaponPistGroup.PlaceLabledControl("Inaccuracy", this, &WeaponPistInacc);

	WeaponPistHitbox.SetFileId("pist_hitbox");
	WeaponPistHitbox.AddItem("Head");
	WeaponPistHitbox.AddItem("Neck");
	WeaponPistHitbox.AddItem("Chest");
	WeaponPistHitbox.AddItem("Stomach");
	WeaponPistGroup.PlaceLabledControl("Primary Hitbox", this, &WeaponPistHitbox);

	WeaponPistKey.SetFileId("pist_key");
	WeaponPistGroup.PlaceLabledControl("Key Bind", this, &WeaponPistKey);

#pragma endregion

#pragma region Snipers
	WeaponSnipGroup.SetPosition(528, 274);
	WeaponSnipGroup.SetText("Snipers");
	WeaponSnipGroup.SetSize(240, 220);
	RegisterControl(&WeaponSnipGroup);

	

	WeaponSnipSpeed.SetFileId("wconf_speed_ak");
	WeaponSnipSpeed.SetBoundaries(0.01f, 100.f);
	WeaponSnipSpeed.SetValue(1.0f);
	WeaponSnipGroup.PlaceLabledControl("Speed", this, &WeaponSnipSpeed);

	WeaponSnipFoV.SetFileId("wconf_fov_ak");
	WeaponSnipFoV.SetBoundaries(0.1f, 30.f);
	WeaponSnipFoV.SetValue(5.f);
	WeaponSnipGroup.PlaceLabledControl("Fov", this, &WeaponSnipFoV);

	WeaponSnipRecoil.SetFileId("wconf_recoil_ak");
	WeaponSnipRecoil.SetBoundaries(0.f, 2.f);
	WeaponSnipRecoil.SetValue(0.f);
	WeaponSnipGroup.PlaceLabledControl("Recoil", this, &WeaponSnipRecoil);

	WeaponSnipPSilent.SetFileId("wconf_psilent_ak");
	WeaponSnipGroup.PlaceLabledControl("Silent", this, &WeaponSnipPSilent);

	WeaponSnipInacc.SetFileId("wconf_inacc_ak");
	WeaponSnipInacc.SetBoundaries(0.f, 15.f);
	WeaponSnipGroup.PlaceLabledControl("Inaccuracy", this, &WeaponSnipInacc);

	WeaponSnipHitbox.SetFileId("wconf_hitbox_ak");
	WeaponSnipHitbox.AddItem("Head");
	WeaponSnipHitbox.AddItem("Neck");
	WeaponSnipHitbox.AddItem("Chest");
	WeaponSnipHitbox.AddItem("Stomach");
	WeaponSnipGroup.PlaceLabledControl("Primary Hitbox", this, &WeaponSnipHitbox);

	WeaponSnipScoutHitbox.SetFileId("wconf_hitbox_scout_ak");
	WeaponSnipScoutHitbox.AddItem("Head");
	WeaponSnipScoutHitbox.AddItem("Neck");
	WeaponSnipScoutHitbox.AddItem("Chest");
	WeaponSnipScoutHitbox.AddItem("Stomach");
	WeaponSnipGroup.PlaceLabledControl("Scout Hitbox", this, &WeaponSnipScoutHitbox);

	WeaponSnipKey.SetFileId("wconf_key");
	WeaponSnipGroup.PlaceLabledControl("Key Bind", this, &WeaponSnipKey);

#pragma endregion

#pragma More
	


#pragma endregion
}

void CVisualTab::Setup()
{
	SetTitle("Visuals");

	ActiveLabel.SetPosition(16, 16);
	ActiveLabel.SetText("Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(66, 16);
	RegisterControl(&Active);

#pragma region Options
	OptionsGroup.SetText("Options");
	OptionsGroup.SetPosition(16, 48);
	OptionsGroup.SetSize(193, 450);
	RegisterControl(&OptionsGroup);

	OptionsBox.SetFileId("opt_box");
	OptionsBox.AddItem("None");
	OptionsBox.AddItem("Corners");
	OptionsBox.AddItem("Full");
	OptionsBox.AddItem("OutLine");
	OptionsBox.AddItem("Corners - Dead");
	OptionsBox.AddItem("Full - Dead");
	OptionsBox.AddItem("OutLine - Dead");
	OptionsGroup.PlaceLabledControl("Box", this, &OptionsBox);

	OptionsBox.SetFileId("opt_boxfill");
	OptionsGroup.PlaceLabledControl("Fill", this, &OptionsBoxFill);
	
	OptionsBox.SetFileId("opt_boxcolor");
	OptionsGroup.PlaceLabledControl("Colorize fill", this, &OptionsBoxFillColor);

	OptionsName.SetFileId("opt_name");
	OptionsGroup.PlaceLabledControl("Name", this, &OptionsName);

	OptionsHealth.SetFileId("opt_hp");
	OptionsHealth.AddItem("Off");
	OptionsHealth.AddItem("Number");
	OptionsHealth.AddItem("Default");
	OptionsHealth.AddItem("Interwebz");
	OptionsGroup.PlaceLabledControl("Health", this, &OptionsHealth);

	OptionsArmor.SetFileId("opt_arm");
	OptionsArmor.AddItem("Off");
	OptionsArmor.AddItem("Number");
	OptionsArmor.AddItem("Default");
	OptionsArmor.AddItem("Both");
	OptionsGroup.PlaceLabledControl("Armor", this, &OptionsArmor);

	OptionsDistance.SetFileId("opt_dist");
	OptionsGroup.PlaceLabledControl("Distance", this, &OptionsDistance);

	OptionsWeapon.SetFileId("opt_weapon");
	OptionsWeapon.AddItem("Off");
	OptionsWeapon.AddItem("Text");
	OptionsWeapon.AddItem("Icon");
	OptionsGroup.PlaceLabledControl("Weapon", this, &OptionsWeapon);

	OptionsInfo.SetFileId("opt_info");
	OptionsGroup.PlaceLabledControl("Info", this, &OptionsInfo);

	OptionsChams.SetFileId("opt_chams");
	OptionsChams.AddItem("Off");
	OptionsChams.AddItem("Textured");
	OptionsChams.AddItem("Flat");
	OptionsChams.AddItem("XQZ");
	OptionsGroup.PlaceLabledControl("Chams", this, &OptionsChams);

	OptionsFriendly.SetFileId("opt_friendlyenemy");
	OptionsFriendly.AddItem("Off");
	OptionsFriendly.AddItem("Red/Green");
	OptionsFriendly.AddItem("Green/Red");
	OptionsFriendly.AddItem("Custom");
	OptionsGroup.PlaceLabledControl("Team Vis. Color", this, &OptionsFriendly);

	OptionsFriendly2.SetFileId("opt_friendlyenemy2");
	OptionsFriendly2.AddItem("Off");
	OptionsFriendly2.AddItem("Red/Green");
	OptionsFriendly2.AddItem("Green/Red");
	OptionsFriendly2.AddItem("Custom");
	OptionsGroup.PlaceLabledControl("Team NoVis. Color", this, &OptionsFriendly2);

	OptionsSkeleton.SetFileId("opt_bone");
	OptionsGroup.PlaceLabledControl("Skeleton", this, &OptionsSkeleton);

	Barrels.SetFileId("opt_barrels");
	OptionsGroup.PlaceLabledControl("Trace Lines", this, &Barrels);
	
	OptionsCompRank.SetFileId("opt_comprank");
	OptionsGroup.PlaceLabledControl("Player Ranks", this, &OptionsCompRank);

	OptionsDefusing.SetFileId("opt_defusing");
	OptionsDefusing.AddItem("Off");
	OptionsDefusing.AddItem("ESP");
	OptionsDefusing.AddItem("ESP & Watermark");
	OptionsGroup.PlaceLabledControl("Defusing", this, &OptionsDefusing);

	OptionsScoped.SetFileId("opt_scoped");
	OptionsGroup.PlaceLabledControl("Is Scoped", this, &OptionsScoped);

	OptionsMoney.SetFileId("opt_money");
	OptionsGroup.PlaceLabledControl("Money", this, &OptionsMoney);
	
#pragma endregion Setting up the Options controls

#pragma region Filters
	FiltersGroup.SetText("Filters");
	FiltersGroup.SetPosition(225, 48);
	FiltersGroup.SetSize(193, 450);
	RegisterControl(&FiltersGroup);

	FiltersAll.SetFileId("ftr_all");
	FiltersGroup.PlaceLabledControl("All", this, &FiltersAll);

	FiltersPlayers.SetFileId("ftr_players");
	FiltersGroup.PlaceLabledControl("Players", this, &FiltersPlayers);

	FiltersEnemiesOnly.SetFileId("ftr_enemyonly");
	FiltersGroup.PlaceLabledControl("Enemies Only", this, &FiltersEnemiesOnly);

	FiltersWeapons.SetFileId("ftr_weaps");
	FiltersGroup.PlaceLabledControl("Weapons", this, &FiltersWeapons);

	FiltersNades.SetFileId("ftr_nades");
	FiltersNades.AddItem("Off");
	FiltersNades.AddItem("Text");
	FiltersNades.AddItem("Text & Polygon");
	FiltersGroup.PlaceLabledControl("Nades", this, &FiltersNades);

	OptionsNadePrediction.SetFileId("opt_nadepredict");
	FiltersGroup.PlaceLabledControl("Nade Pred", this, &OptionsNadePrediction);

	FiltersChickens.SetFileId("ftr_chickens");
	FiltersGroup.PlaceLabledControl("Chickens", this, &FiltersChickens);

	FiltersC4.SetFileId("ftr_c4");
	FiltersGroup.PlaceLabledControl("C4", this, &FiltersC4);

	FiltersDead.SetFileId("ftr_spec");
	FiltersGroup.PlaceLabledControl("Only Dead", this, &FiltersDead);



#pragma endregion Setting up the Filters controls

#pragma region Other
	OtherGroup.SetText("Other");
	OtherGroup.SetPosition(434, 48);
	OtherGroup.SetSize(334, 450);
	RegisterControl(&OtherGroup);

	OtherCrosshair.SetFileId("otr_xhair");
	OtherGroup.PlaceLabledControl("Sniper Crosshair", this, &OtherCrosshair);

	OtherSpread.SetFileId("otr_xspread");
	OtherGroup.PlaceLabledControl("Spread Circle", this, &OtherSpread);

	OtherRecoilCrosshair.SetFileId("otr_recoilhair");
	OtherRecoilCrosshair.AddItem("Off");
	OtherRecoilCrosshair.AddItem("Recoil Position");
	OtherGroup.PlaceLabledControl("Recoil Crosshair", this, &OtherRecoilCrosshair);

	OtherRadar.SetFileId("otr_radar");
	OtherGroup.PlaceLabledControl("Radar", this, &OtherRadar);

	OtherNoVisualRecoil.SetFileId("otr_visrecoil");
	OtherGroup.PlaceLabledControl("No Visual Recoil", this, &OtherNoVisualRecoil);

	OtherNoFlash.SetFileId("otr_noflash");
	//OtherNoFlas.SetBoundaries(0.f, 90.f);
	//OtherViewmodelFOV.SetValue(0.f);
	OtherGroup.PlaceLabledControl("No Flash Enable", this, &OtherNoFlash);

	OtherNoFlashVal.SetFileId("otr_noflashval");
	OtherNoFlashVal.SetBoundaries(0,255);
	OtherViewmodelFOV.SetValue(200);
	OtherGroup.PlaceLabledControl("Max Flash", this, &OtherNoFlashVal);

	OtherNoHands.SetFileId("otr_hands");
	OtherNoHands.AddItem("Off");
	OtherNoHands.AddItem("None");
	OtherNoHands.AddItem("Transparent");
	OtherNoHands.AddItem("Chams");
	OtherNoHands.AddItem("Rainbow Trans");
	OtherNoHands.AddItem("Rainbow Chams");
	OtherGroup.PlaceLabledControl("Hands", this, &OtherNoHands);

	OtherWeapons.SetFileId("otr_weapons");
	OtherWeapons.AddItem("Off");
	OtherWeapons.AddItem("Red");
	OtherWeapons.AddItem("Blue");
	OtherWeapons.AddItem("Green");
	OtherWeapons.AddItem("Yellow");
	OtherWeapons.AddItem("Purple");
	OtherWeapons.AddItem("Red WireFrame");
	OtherWeapons.AddItem("Blue WireFrame");
	OtherWeapons.AddItem("Green WireFrame");
	OtherWeapons.AddItem("Yellow WireFrame");
	OtherWeapons.AddItem("Purple WireFrame");
	OtherWeapons.AddItem("Rainbow");
	OtherWeapons.AddItem("Rainbow WireFrame");
	OtherGroup.PlaceLabledControl("Weapons", this, &OtherWeapons);

	OtherViewmodelFOV.SetFileId("otr_viewfov");
	OtherViewmodelFOV.SetBoundaries(0.f, 90.f);
	OtherViewmodelFOV.SetValue(0.f);
	OtherGroup.PlaceLabledControl("Viewmodel FOV Changer", this, &OtherViewmodelFOV);

	OtherFOV.SetFileId("otr_fov");
	OtherFOV.SetBoundaries(0.f, 180.f);
	OtherFOV.SetValue(90.f);
	OtherGroup.PlaceLabledControl("Field of View Changer", this, &OtherFOV);

	OtherNoSky.SetFileId("otr_nosky");
	OtherGroup.PlaceLabledControl("No Sky", this, &OtherNoSky);

	OtherNoScope.SetFileId("otr_noscope");
	OtherGroup.PlaceLabledControl("No Scope", this, &OtherNoScope);

	OtherNoSmoke.SetFileId("otr_nosmoke");
	OtherGroup.PlaceLabledControl("No Smoke", this, &OtherNoSmoke);

	OtherFovDraw.SetFileId("otr_drawfov");
	OtherGroup.PlaceLabledControl("Draw Fov", this, &OtherFovDraw);

	OtherBackTracking.SetFileId("otr_backtracking");
	OtherGroup.PlaceLabledControl("Backtracking Line", this, &OtherBackTracking);

	OtherLBYDraw.SetFileId("otr_drawlby");
	OtherGroup.PlaceLabledControl("LBY Indicator", this, &OtherLBYDraw);

	OtherAsus.SetFileId("otr_asus");
	OtherGroup.PlaceLabledControl("Disable Post Processing", this, &OtherAsus);

#pragma endregion Setting up the Other controls
}

void CMiscTab::Setup()
{
	SetTitle("Misc");

#pragma region Knife
	/*KnifeGroup.SetPosition(16, 16);
	KnifeGroup.SetSize(360, 126);
	KnifeGroup.SetText("Knife Changer");
	RegisterControl(&KnifeGroup);

	KnifeEnable.SetFileId("knife_enable");
	KnifeGroup.PlaceLabledControl("Enable", this, &KnifeEnable);

	KnifeModel.SetFileId("knife_model");
	KnifeModel.AddItem("Karambit");
	KnifeModel.AddItem("Bayonet");
	KnifeModel.AddItem("M9 Bayonet");
	KnifeModel.AddItem("Flip Knife");
	KnifeModel.AddItem("Gut Knife");
	KnifeModel.AddItem("Huntsman Knife");
	KnifeModel.AddItem("Falchion Knife");
    KnifeModel.AddItem("Bowie Knife");
	KnifeModel.AddItem("Butterfly Knife");
	KnifeModel.AddItem("Shadow Daggers");
	KnifeGroup.PlaceLabledControl("Knife", this, &KnifeModel);

	KnifeSkin.SetFileId("knife_skin");
	KnifeSkin.AddItem("Doppler Sapphire");
	KnifeSkin.AddItem("Doppler Ruby");
	KnifeSkin.AddItem("Tiger Tooth");
	KnifeSkin.AddItem("Lore");
	KnifeSkin.AddItem("Forest DDPAT");
	KnifeSkin.AddItem("Crimson Web");
	KnifeSkin.AddItem("Slaughter");
	KnifeSkin.AddItem("Vanilla");
	KnifeSkin.AddItem("Fade");
	KnifeSkin.AddItem("Night");
	KnifeSkin.AddItem("Blue Steel");
	KnifeSkin.AddItem("Stained");
	KnifeSkin.AddItem("Case Hardended");
	KnifeSkin.AddItem("Safari Mesh");
	KnifeSkin.AddItem("Boreal Forest");
	KnifeSkin.AddItem("Ultaviolet");
	KnifeSkin.AddItem("Rust Coat");
	KnifeSkin.AddItem("Romania's Flag <3");
	

	KnifeGroup.PlaceLabledControl("Skin", this, &KnifeSkin);

	KnifeApply.SetText("Apply Knife");
	KnifeApply.SetCallback(KnifeApplyCallbk);
	KnifeGroup.PlaceLabledControl("", this, &KnifeApply);
*/
#pragma endregion

#pragma region Other
	OtherGroup.SetPosition(408, 16);
	OtherGroup.SetSize(360, 500);
	OtherGroup.SetText("Other");
	RegisterControl(&OtherGroup);

	OtherAutoJump.SetFileId("otr_autojump");
	OtherGroup.PlaceLabledControl("Bhop", this, &OtherAutoJump);

	OtherAutoStrafe.SetFileId("otr_strafe");
	OtherAutoStrafe.AddItem("Off");
	OtherAutoStrafe.AddItem("Legit");
	OtherAutoStrafe.AddItem("Rage");
	OtherGroup.PlaceLabledControl("Auto Strafer", this, &OtherAutoStrafe);

	OtherCircleStrafe.SetFileId("otr_circlestrafe");
	OtherGroup.PlaceLabledControl("Circle Strafe", this, &OtherCircleStrafe);

	OtherCircleStrafeKey.SetFileId("otr_circlestrafeKey");
	OtherGroup.PlaceLabledControl("Circle Strafe Key", this, &OtherCircleStrafeKey);

	OtherSafeMode.SetFileId("otr_safemode");
	OtherSafeMode.SetState(true);
	OtherGroup.PlaceLabledControl("Safe Mode", this, &OtherSafeMode);

	OtherNameSpam.AddItem("Off");
	OtherNameSpam.AddItem("NoName");
	OtherNameSpam.AddItem("NoName - Extreme Spam");
	OtherNameSpam.AddItem("NoName - sneakyevilhook");
	OtherNameSpam.AddItem("Namestealer");
	OtherNameSpam.AddItem("sneakyevil.win");
	OtherNameSpam.AddItem("sneakyevil.xyz");
	OtherNameSpam.AddItem("problemikk.win");
	OtherNameSpam.AddItem("sneakyevil");
	OtherNameSpam.AddItem("xde");
	OtherNameSpam.AddItem("tomik");
	OtherNameSpam.AddItem("Crash Game for pShit v1");
	OtherNameSpam.AddItem("Crash Game for pShit v2");
	OtherNameSpam.AddItem("Crash Game for pShit v3");
	OtherGroup.PlaceLabledControl("Name Spam", this, &OtherNameSpam);

	OtherChatSpam.AddItem("Off");
	OtherChatSpam.AddItem("sneakyevil.win");
	OtherChatSpam.AddItem("sneakyevilhook.win");
	OtherChatSpam.AddItem("insertgang.xyz");
	OtherChatSpam.AddItem("AimTux");
	OtherChatSpam.AddItem("Aimware.net");
	OtherChatSpam.AddItem("EzFrags");
	OtherChatSpam.AddItem("Unityhacks.com");
	OtherChatSpam.AddItem("QQQQQQ");
	OtherChatSpam.AddItem("C4 Timer");
	OtherChatSpam.AddItem("Fake Reports");
	OtherGroup.PlaceLabledControl("Chat Spam", this, &OtherChatSpam);

	OtherKillSpam.AddItem("Off");
	OtherKillSpam.AddItem("Classic");
	OtherKillSpam.AddItem("Toxic");
	OtherKillSpam.AddItem("Scream");
	OtherGroup.PlaceLabledControl("Kill Spam", this, &OtherKillSpam);

	OtherLyricsSpam.AddItem("Off");
	OtherLyricsSpam.AddItem("Flex Like Ouu");
	OtherLyricsSpam.AddItem("$$ - 2nd Hand");
	OtherGroup.PlaceLabledControl("Lyrics Spam", this, &OtherLyricsSpam);

	OtherRadioSpam.AddItem("Off");
	OtherRadioSpam.AddItem("Yes / No");
	OtherRadioSpam.AddItem("Get Out");
	OtherRadioSpam.AddItem("Random");
	OtherGroup.PlaceLabledControl("Radio Spam", this, &OtherRadioSpam);

	OtherClantag.AddItem("Off");
	OtherClantag.AddItem("Custom");
	OtherClantag.AddItem("sneakyevil.win");
	OtherClantag.AddItem("sneakyevil.xyz");
	OtherClantag.AddItem("sneakyevil Hook");
	OtherClantag.AddItem("Commend Me");
	OtherClantag.AddItem("skeet.cc");
	OtherClantag.AddItem("gamesense");
	OtherClantag.AddItem("Valve");
	OtherClantag.AddItem("Real Time");
	OtherGroup.PlaceLabledControl("Clan Tag", this, &OtherClantag);

	OtherTeamChat.SetFileId("otr_teamchat");
	OtherGroup.PlaceLabledControl("Team Chat Only", this, &OtherTeamChat);

	OtherChatDelay.SetFileId("otr_chatdelay");
	OtherChatDelay.SetBoundaries(0.1, 3.0);
	OtherChatDelay.SetValue(0.1);
	OtherGroup.PlaceLabledControl("Spam Delay", this, &OtherChatDelay);

	OtherAirStuck.SetFileId("otr_astuck");
	OtherGroup.PlaceLabledControl("Air Stuck", this, &OtherAirStuck);

	OtherSlowMotion.SetFileId("otr_slowmotionbutton");
	OtherGroup.PlaceLabledControl("Slow Motion", this, &OtherSlowMotion);

	OtherFakeWalk.SetFileId("otr_fakewalk");
	OtherGroup.PlaceLabledControl("Fake Walk", this, &OtherFakeWalk);

	OtherSpectators.SetFileId("otr_speclist");
	OtherGroup.PlaceLabledControl("Spectators List", this, &OtherSpectators);

	OtherThirdperson.SetFileId("aa_thirdpsr");
	OtherThirdperson.AddItem("Off");
	OtherThirdperson.AddItem("On");
	OtherThirdperson.AddItem("On / Grenades: Off");
	OtherGroup.PlaceLabledControl("Thirdperson", this, &OtherThirdperson);

    OtherWatermark.SetFileId("otr_watermark");
	OtherWatermark.SetState(true);
	OtherGroup.PlaceLabledControl("Watermark", this, &OtherWatermark);

	OtherClock.SetFileId("otr_clock");
	OtherClock.SetState(true);
	OtherGroup.PlaceLabledControl("Clock", this, &OtherClock);

	//DisableAll.SetFileId("otr_disableall");
	//OtherGroup.PlaceLabledControl("Disable All", this, &DisableAll);

#pragma endregion other random options

#pragma region FakeLag
	FakeLagGroup.SetPosition(16, 16);
	FakeLagGroup.SetSize(360, 101);
	FakeLagGroup.SetText("Fake Lag");
	RegisterControl(&FakeLagGroup);

	FakeLagEnable.SetFileId("fakelag_enable");
	FakeLagGroup.PlaceLabledControl("Fake Lag", this, &FakeLagEnable);

	FakeLagChoke.SetFileId("fakelag_choke");
	FakeLagChoke.SetBoundaries(0, 16);
	FakeLagChoke.SetValue(0);
	FakeLagGroup.PlaceLabledControl("Choke Factor", this, &FakeLagChoke);

	FakeLagSend.SetFileId("fakelag_send");
	FakeLagSend.SetBoundaries(0, 16);
	FakeLagSend.SetValue(0);
	FakeLagGroup.PlaceLabledControl("Send Factor", this, &FakeLagSend);
#pragma endregion fakelag shit

#pragma region MPs
	MPGroup.SetPosition(16, 136);
	MPGroup.SetText("MPs");
	MPGroup.SetSize(376, 165);
	RegisterControl(&MPGroup);

	MAC10Skin.SetFileId("mac10_skin");
	MAC10Skin.AddItem("Tornado");
	MAC10Skin.AddItem("Candy Apple");
	MAC10Skin.AddItem("Silver");
	MAC10Skin.AddItem("Forest DDPAT");
	MAC10Skin.AddItem("Urban DDPAT");
	MAC10Skin.AddItem("Fade");
	MAC10Skin.AddItem("Neon Rider");
	MAC10Skin.AddItem("Ultraviolet");
	MAC10Skin.AddItem("Palm");
	MAC10Skin.AddItem("Graven");
	MAC10Skin.AddItem("Tatter");
	MAC10Skin.AddItem("Amber Fade");
	MAC10Skin.AddItem("Heat");
	MAC10Skin.AddItem("Curse");
	MAC10Skin.AddItem("Indigo");
	MAC10Skin.AddItem("Commuter");
	MAC10Skin.AddItem("Nuclear Garden");
	MAC10Skin.AddItem("Malachite");
	MAC10Skin.AddItem("Rangeen");
	MAC10Skin.AddItem("Lapis Gator");
	MAC10Skin.AddItem("Last Dive");
	MAC10Skin.AddItem("Aloha");
	MPGroup.PlaceLabledControl("MAC-10", this, &MAC10Skin);

	P90Skin.SetFileId("p90_skin");
	P90Skin.AddItem("Leather");
	P90Skin.AddItem("Virus");
	P90Skin.AddItem("Contrast Spray");
	P90Skin.AddItem("Storm");
	P90Skin.AddItem("Cold Blooded");
	P90Skin.AddItem("Glacier Mesh");
	P90Skin.AddItem("Sand Spray");
	P90Skin.AddItem("Death by Kitty");
	P90Skin.AddItem("Ash Wood");
	P90Skin.AddItem("Fallout Warning");
	P90Skin.AddItem("Scorched");
	P90Skin.AddItem("Emerald Dragon");
	P90Skin.AddItem("Teardown");
	P90Skin.AddItem("Blind Spot");
	P90Skin.AddItem("Trigon");
	P90Skin.AddItem("Desert Warfare");
	P90Skin.AddItem("Module");
	P90Skin.AddItem("Asiimov");
	P90Skin.AddItem("Elite Build");
	P90Skin.AddItem("Shapewood");
	P90Skin.AddItem("Shallow Grave");
	P90Skin.AddItem("Drath Grip");
	MPGroup.PlaceLabledControl("P90", this, &P90Skin);

	UMP45Skin.SetFileId("ump45_skin");
	UMP45Skin.AddItem("Blaze");
	UMP45Skin.AddItem("Forest DDPAT");
	UMP45Skin.AddItem("Gunsmoke");
	UMP45Skin.AddItem("Urban DDPAT");
	UMP45Skin.AddItem("Grand Prix");
	UMP45Skin.AddItem("Carbon Fiber");
	UMP45Skin.AddItem("Caramel");
	UMP45Skin.AddItem("Fallout Warning");
	UMP45Skin.AddItem("Scorched");
	UMP45Skin.AddItem("Bone Pile");
	UMP45Skin.AddItem("Delusion");
	UMP45Skin.AddItem("Corporal");
	UMP45Skin.AddItem("Indigo");
	UMP45Skin.AddItem("Labyrinth");
	UMP45Skin.AddItem("Minotaur's Labyrinth");
	UMP45Skin.AddItem("Riot");
	UMP45Skin.AddItem("Primal Saber");
	UMP45Skin.AddItem("Scaffold");
	UMP45Skin.AddItem("Metal Flowers");
	MPGroup.PlaceLabledControl("UMP-45", this, &UMP45Skin);

	BIZONSkin.SetFileId("bizon_skin");
	BIZONSkin.AddItem("Blue Streak");
	BIZONSkin.AddItem("Modern Hunter");
	BIZONSkin.AddItem("Forest Leaves");
	BIZONSkin.AddItem("Bone Mask");
	BIZONSkin.AddItem("Carbon Fiber");
	BIZONSkin.AddItem("Sand Dashed");
	BIZONSkin.AddItem("Urban Dashed");
	BIZONSkin.AddItem("Brass");
	BIZONSkin.AddItem("VariCamo");
	BIZONSkin.AddItem("Irradiated Alert");
	BIZONSkin.AddItem("Rust Coat");
	BIZONSkin.AddItem("Water Sigil");
	BIZONSkin.AddItem("Night Ops");
	BIZONSkin.AddItem("Cobalt Halftone");
	BIZONSkin.AddItem("Antique");
	BIZONSkin.AddItem("Rust Coat");
	BIZONSkin.AddItem("Osiris");
	BIZONSkin.AddItem("Chemical Green");
	BIZONSkin.AddItem("Bamboo Print");
	BIZONSkin.AddItem("Bamboo Forest");
	BIZONSkin.AddItem("Fuel Rod");
	BIZONSkin.AddItem("Photic Zone");
	BIZONSkin.AddItem("Judgement of Anubis");
	BIZONSkin.AddItem("Jungle Slipstream");
	BIZONSkin.AddItem("High Roller");

	MPGroup.PlaceLabledControl("PP-Bizon", this, &BIZONSkin);

	MP7Skin.SetFileId("mp7_skin");
	MP7Skin.AddItem("Groundwater");
	MP7Skin.AddItem("Whiteout");
	MP7Skin.AddItem("Forest DDPAT");
	MP7Skin.AddItem("Anodized Navy");
	MP7Skin.AddItem("Skulls");
	MP7Skin.AddItem("Gunsmoke");
	MP7Skin.AddItem("Contrast Spray");
	MP7Skin.AddItem("Bone Mask");
	MP7Skin.AddItem("Ossified");
	MP7Skin.AddItem("Orange Peel");
	MP7Skin.AddItem("VariCamo");
	MP7Skin.AddItem("Army Recon");
	MP7Skin.AddItem("Groundwater");
	MP7Skin.AddItem("Ocean Foam");
	MP7Skin.AddItem("Full Stop");
	MP7Skin.AddItem("Urban Hazard");
	MP7Skin.AddItem("Olive Plaid");
	MP7Skin.AddItem("Armor Core");
	MP7Skin.AddItem("Asterion");
	MP7Skin.AddItem("Nemesis");
	MP7Skin.AddItem("Special Delivery");
	MP7Skin.AddItem("Impire");
	MP7Skin.AddItem("Akoben");
	MPGroup.PlaceLabledControl("MP7", this, &MP7Skin);

	MP9Skin.SetFileId("mp9_skin");
	MP9Skin.AddItem("Ruby Poison Dart");
	MP9Skin.AddItem("Bone Mask");
	MP9Skin.AddItem("Hot Rod");
	MP9Skin.AddItem("Storm");
	MP9Skin.AddItem("Bulldozer");
	MP9Skin.AddItem("Hypnotic");
	MP9Skin.AddItem("Sand Dashed");
	MP9Skin.AddItem("Orange Peel");
	MP9Skin.AddItem("Dry Season");
	MP9Skin.AddItem("Dark Age");
	MP9Skin.AddItem("Rose Iron");
	MP9Skin.AddItem("Green Plaid");
	MP9Skin.AddItem("Setting Sun");
	MP9Skin.AddItem("Dart");
	MP9Skin.AddItem("Deadly Poison");
	MP9Skin.AddItem("Pandora's Box");
	MP9Skin.AddItem("Bioleak");
	MP9Skin.AddItem("Airlock");
	MPGroup.PlaceLabledControl("MP9", this, &MP9Skin);

/*#pragma region OverideFov
	FOVGroup.SetPosition(16, 365);
	FOVGroup.SetSize(360, 75);
	FOVGroup.SetText("FOV Changer");
	RegisterControl(&FOVGroup);

	FOVEnable.SetFileId("fov_enable");
	FOVGroup.PlaceLabledControl("Enable", this, &FOVEnable);

	FOVSlider.SetFileId("fov_slider");
	FOVSlider.SetBoundaries(0, 200);
	FOVSlider.SetValue(0);
	FOVGroup.PlaceLabledControl("FOV Amount", this, &FOVSlider);

#pragma endregion*/
}

/*void CPlayersTab::Setup()
{
	SetTitle("PlayerList");

#pragma region PList

	pListGroup.SetPosition(16, 16);
	pListGroup.SetSize(680, 200);
	pListGroup.SetText("Player List");
	pListGroup.SetColumns(2);
	RegisterControl(&pListGroup);

	pListPlayers.SetPosition(26, 46);
	pListPlayers.SetSize(640, 50);
	pListPlayers.SetHeightInItems(20);
	RegisterControl(&pListPlayers);

#pragma endregion

#pragma region Options
	
	OptionsGroup.SetPosition(16, 257);
	OptionsGroup.SetSize(450, 120);
	OptionsGroup.SetText("Player Options");
	RegisterControl(&OptionsGroup);

	OptionsFriendly.SetFileId("pl_friendly");
	OptionsGroup.PlaceLabledControl("Friendly", this, &OptionsFriendly);

	OptionsAimPrio.SetFileId("pl_priority");
	OptionsGroup.PlaceLabledControl("Priority", this, &OptionsAimPrio);

	OptionsCalloutSpam.SetFileId("pl_callout");
	OptionsGroup.PlaceLabledControl("Callout Spam", this, &OptionsCalloutSpam);

#pragma endregion
}

DWORD GetPlayerListIndex(int EntId)
{
	player_info_t pinfo;
	Interfaces::Engine->GetPlayerInfo(EntId, &pinfo);

	// Bot
	if (pinfo.guid[0] == 'B' && pinfo.guid[1] == 'O')
	{
		char buf[64]; sprintf_s(buf, "BOT_420%sAY", pinfo.name);
		return CRC32(buf, 64);
	}
	else
	{
		return CRC32(pinfo.guid, 32);
	}
}

bool IsFriendly(int EntId)
{
	DWORD plistId = GetPlayerListIndex(EntId);
	if (PlayerList.find(plistId) != PlayerList.end())
	{
		return PlayerList[plistId].Friendly;
	}

	return false;
}

bool IsAimPrio(int EntId)
{
	DWORD plistId = GetPlayerListIndex(EntId);
	if (PlayerList.find(plistId) != PlayerList.end())
	{
		return PlayerList[plistId].AimPrio;
	}

	return false;
}

bool IsCalloutTarget(int EntId)
{
	DWORD plistId = GetPlayerListIndex(EntId);
	if (PlayerList.find(plistId) != PlayerList.end())
	{
		return PlayerList[plistId].Callout;
	}

	return false;
}

void UpdatePlayerList()
{
	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && pLocal && pLocal->IsAlive())
	{
		Menu::Window.Playerlist.pListPlayers.ClearItems();

		// Loop through all active entitys
		for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
		{
			// Get the entity

			player_info_t pinfo;
			if (i != Interfaces::Engine->GetLocalPlayer() && Interfaces::Engine->GetPlayerInfo(i, &pinfo))
			{
				IClientEntity* pEntity = Interfaces::EntList->GetClientEntity(i);
				int HP = 100; char* Location = "Unknown";
				char *Friendly = " ", *AimPrio = " ";

				DWORD plistId = GetPlayerListIndex(Menu::Window.Playerlist.pListPlayers.GetValue());
				if (PlayerList.find(plistId) != PlayerList.end())
				{
					Friendly = PlayerList[plistId].Friendly ? "Friendly" : "";
					AimPrio = PlayerList[plistId].AimPrio ? "AimPrio" : "";
				}

				if (pEntity && !pEntity->IsDormant())
				{
					HP = pEntity->GetHealth();
					Location = pEntity->GetLastPlaceName();
				}

				char nameBuffer[512];
				sprintf_s(nameBuffer, "%-24s %-10s %-10s [%d HP] [Last Seen At %s]", pinfo.name, IsFriendly(i) ? "Friend" : " ", IsAimPrio(i) ? "AimPrio" : " ", HP, Location);
				Menu::Window.Playerlist.pListPlayers.AddItem(nameBuffer, i);

			}

		}

		DWORD meme = GetPlayerListIndex(Menu::Window.Playerlist.pListPlayers.GetValue());

		// Have we switched to a different player?
		static int PrevSelectedPlayer = 0;
		if (PrevSelectedPlayer != Menu::Window.Playerlist.pListPlayers.GetValue())
		{
			if (PlayerList.find(meme) != PlayerList.end())
			{
				Menu::Window.Playerlist.OptionsFriendly.SetState(PlayerList[meme].Friendly);
				Menu::Window.Playerlist.OptionsAimPrio.SetState(PlayerList[meme].AimPrio);
				Menu::Window.Playerlist.OptionsCalloutSpam.SetState(PlayerList[meme].Callout);

			}
			else
			{
				Menu::Window.Playerlist.OptionsFriendly.SetState(false);
				Menu::Window.Playerlist.OptionsAimPrio.SetState(false);
				Menu::Window.Playerlist.OptionsCalloutSpam.SetState(false);

			}
		}
		PrevSelectedPlayer = Menu::Window.Playerlist.pListPlayers.GetValue();

		PlayerList[meme].Friendly = Menu::Window.Playerlist.OptionsFriendly.GetState();
		PlayerList[meme].AimPrio = Menu::Window.Playerlist.OptionsAimPrio.GetState();
		PlayerList[meme].Callout = Menu::Window.Playerlist.OptionsCalloutSpam.GetState();
	}
}*/

void CColorTab::Setup()
{
	SetTitle("Colors");
#pragma region Colors
	CTNVisGroup.SetPosition(16, 16);
	CTNVisGroup.SetSize(360, 100);
	CTNVisGroup.SetText("CT Not Visible");
	RegisterControl(&CTNVisGroup);

	// CT NOT VISIBLE
	CTNVisColorR.SetFileId("ct_not_vis_r");
	CTNVisColorR.SetBoundaries(1, 255);
	CTNVisColorR.SetValue(15);
	CTNVisGroup.PlaceLabledControl("Red", this, &CTNVisColorR);

	CTNVisColorG.SetFileId("ct_not_vis_g");
	CTNVisColorG.SetBoundaries(1, 255);
	CTNVisColorG.SetValue(1);
	CTNVisGroup.PlaceLabledControl("Green", this, &CTNVisColorG);

	CTNVisColorB.SetFileId("ct_not_vis_b");
	CTNVisColorB.SetBoundaries(1, 255);
	CTNVisColorB.SetValue(220);
	CTNVisGroup.PlaceLabledControl("Blue", this, &CTNVisColorB);



	//CT VISIBLE
	CTVisGroup.SetPosition(408, 16);
	CTVisGroup.SetSize(360, 100);
	CTVisGroup.SetText("CT Visible");
	RegisterControl(&CTVisGroup);

	CTVisColorR.SetFileId("ct_vis_r");
	CTVisColorR.SetBoundaries(1, 255);
	CTVisColorR.SetValue(187);
	CTVisGroup.PlaceLabledControl("Red", this, &CTVisColorR);

	CTVisColorG.SetFileId("ct_vis_g");
	CTVisColorG.SetBoundaries(1, 255);
	CTVisColorG.SetValue(1);
	CTVisGroup.PlaceLabledControl("Green", this, &CTVisColorG);

	CTVisColorB.SetFileId("ct_vis_b");
	CTVisColorB.SetBoundaries(1, 255);
	CTVisColorB.SetValue(255);
	CTVisGroup.PlaceLabledControl("Blue", this, &CTVisColorB);

	//T Not Visible
	TNVisGroup.SetPosition(16, 137);
	TNVisGroup.SetSize(360, 100);
	TNVisGroup.SetText("T Not Visible");
	RegisterControl(&TNVisGroup);

	TNVisColorR.SetFileId("t_not_vis_r");
	TNVisColorR.SetBoundaries(1, 255);
	TNVisColorR.SetValue(235);
	TNVisGroup.PlaceLabledControl("Red", this, &TNVisColorR);

	TNVisColorG.SetFileId("t_not_vis_g");
	TNVisColorG.SetBoundaries(1, 255);
	TNVisColorG.SetValue(50);
	TNVisGroup.PlaceLabledControl("Green", this, &TNVisColorG);

	TNVisColorB.SetFileId("t_not_vis_b");
	TNVisColorB.SetBoundaries(1, 255);
	TNVisColorB.SetValue(1);
	TNVisGroup.PlaceLabledControl("Blue", this, &TNVisColorB);

	// T Vis Color

	TVisGroup.SetPosition(408, 137);
	TVisGroup.SetSize(350, 100);
	TVisGroup.SetText("T Visible");
	RegisterControl(&TVisGroup);

	TVisColorR.SetFileId("t_vis_r");
	TVisColorR.SetBoundaries(1, 255);
	TVisColorR.SetValue(255);
	TVisGroup.PlaceLabledControl("Red", this, &TVisColorR);

	TVisColorG.SetFileId("t_vis_g");
	TVisColorG.SetBoundaries(1, 255);
	TVisColorG.SetValue(200);
	TVisGroup.PlaceLabledControl("Green", this, &TVisColorG);

	TVisColorB.SetFileId("t_vis_b");
	TVisColorB.SetBoundaries(1, 255);
	TVisColorB.SetValue(1);
	TVisGroup.PlaceLabledControl("Blue", this, &TVisColorB);


#pragma endregion
	/**/
#pragma MenuGroup
	MenuGroup.SetPosition(16, 258);
	MenuGroup.SetSize(360, 100);
	MenuGroup.SetText("Bar Color 1");
	RegisterControl(&MenuGroup);

	//Menu Gradient Color 1
	MenuBar1R.SetFileId("bar_grad_1R");
	MenuBar1R.SetBoundaries(0, 255);
	MenuBar1R.SetValue(255);
	MenuGroup.PlaceLabledControl("Red", this, &MenuBar1R);

	MenuBar1G.SetFileId("bar_grad_1G");
	MenuBar1G.SetBoundaries(0, 255);
	MenuBar1G.SetValue(0);
	MenuGroup.PlaceLabledControl("Green", this, &MenuBar1G);

	MenuBar1B.SetFileId("bar_grad_1B");
	MenuBar1B.SetBoundaries(0, 255);
	MenuBar1B.SetValue(0);
	MenuGroup.PlaceLabledControl("Blue", this, &MenuBar1B);


	//Menu Gradient Color 2
	MenuBar2.SetPosition(408, 258);
	MenuBar2.SetSize(360, 100);
	MenuBar2.SetText("Bar Color 2");
	RegisterControl(&MenuBar2);

	MenuBar2R.SetFileId("bar_grad_2R");
	MenuBar2R.SetBoundaries(0, 255);
	MenuBar2R.SetValue(220);
	MenuBar2.PlaceLabledControl("Red", this, &MenuBar2R);

	MenuBar2G.SetFileId("bar_grad_2G");
	MenuBar2G.SetBoundaries(0, 255);
	MenuBar2G.SetValue(0);
	MenuBar2.PlaceLabledControl("Green", this, &MenuBar2G);

	MenuBar2B.SetFileId("bar_grad_2B");
	MenuBar2B.SetBoundaries(0, 255);
	MenuBar2B.SetValue(0);
	MenuBar2.PlaceLabledControl("Blue", this, &MenuBar2B);

	//Inner
	MenuInside.SetPosition(16, 379);
	MenuInside.SetSize(360, 100);
	MenuInside.SetText("Menu Inner Color");
	RegisterControl(&MenuInside);

	MenuInnerR.SetFileId("in_R");
	MenuInnerR.SetBoundaries(0, 255);
	MenuInnerR.SetValue(250);
	MenuInside.PlaceLabledControl("Red", this, &MenuInnerR);

	MenuInnerG.SetFileId("in_G");
	MenuInnerG.SetBoundaries(0, 255);
	MenuInnerG.SetValue(0);
	MenuInside.PlaceLabledControl("Green", this, &MenuInnerG);

	MenuInnerB.SetFileId("in_B");
	MenuInnerB.SetBoundaries(0, 255);
	MenuInnerB.SetValue(0);
	MenuInside.PlaceLabledControl("Blue", this, &MenuInnerB);

	//Menu Background

	MenuBackround.SetPosition(408, 379);
	MenuBackround.SetSize(360, 135);
	MenuBackround.SetText("More");
	RegisterControl(&MenuBackround);

	CharmsA.SetFileId("charms_a");
	CharmsA.SetBoundaries(0, 255);
	CharmsA.SetValue(255);
	MenuBackround.PlaceLabledControl("Charms Alpha", this, &CharmsA);

	CrossVisColorR.SetFileId("cross_vis_r");
	CrossVisColorR.SetBoundaries(1, 255);
	CrossVisColorR.SetValue(225);
	MenuBackround.PlaceLabledControl("Crosshair Red", this, &CrossVisColorR);

	CrossVisColorG.SetFileId("cross_vis_g");
	CrossVisColorG.SetBoundaries(1, 255);
	CrossVisColorG.SetValue(0);
	MenuBackround.PlaceLabledControl("Crosshair Green", this, &CrossVisColorG);

	CrossVisColorB.SetFileId("cross_vis_b");
	CrossVisColorB.SetBoundaries(1, 255);
	CrossVisColorB.SetValue(0);
	MenuBackround.PlaceLabledControl("CrossHair Blue", this, &CrossVisColorB);


#pragma endregion

}

void CSettingsTab::Setup()
{

	SetTitle("Settings");
#pragma ButtonGroup
	ButtonGroup.SetPosition(16, 16);
	ButtonGroup.SetSize(360, 140);
	ButtonGroup.SetText("Config");
	RegisterControl(&ButtonGroup);

	SaveButton.SetText("Save Configuration");
	SaveButton.SetCallback(SaveCallbk);
	ButtonGroup.PlaceLabledControl("Save", this, &SaveButton);

	LoadButton.SetText("Load Configuration");
	LoadButton.SetCallback(LoadCallbk);
	ButtonGroup.PlaceLabledControl("Load", this, &LoadButton);

	//Options

	SetNr.AddItem("Legit 1");
	SetNr.AddItem("Legit 2");
	SetNr.AddItem("Legit BackTrack 1");
	SetNr.AddItem("Legit BackTrack 2");
	SetNr.AddItem("Semi Rage");
	SetNr.AddItem("Rage MM 1");
	SetNr.AddItem("Rage MM 2");
	SetNr.AddItem("Rage 1");
	SetNr.AddItem("Rage 2");
	SetNr.AddItem("Rage 3");
	SetNr.AddItem("FaceIT 1");
	SetNr.AddItem("FaceIT 2");
	SetNr.AddItem("NoSpread 1");
	SetNr.AddItem("Nospread 2");
	ButtonGroup.PlaceLabledControl("Config: ", this, &SetNr);

	Unload.SetText("Force Crash(time out)");
	Unload.SetCallback(Unloadbk);
	ButtonGroup.PlaceLabledControl("", this, &Unload);
#pragma endregion

#pragma SettingsHitChance
	SettingsHitChance.SetPosition(13, 165);
	SettingsHitChance.SetSize(180, 220);
	SettingsHitChance.SetText("Rage Bot - Hit Chance");
	RegisterControl(&SettingsHitChance);

	SettingsHitChanceAuto.SetFileId("settingshitchance_auto");
	SettingsHitChanceAuto.SetBoundaries(0, 50);
	SettingsHitChanceAuto.SetValue(0);
	SettingsHitChance.PlaceLabledControl("Auto Sniper", this, &SettingsHitChanceAuto);

	SettingsHitChanceAWP.SetFileId("settingshitchance_awp");
	SettingsHitChanceAWP.SetBoundaries(0, 50);
	SettingsHitChanceAWP.SetValue(0);
	SettingsHitChance.PlaceLabledControl("AWP", this, &SettingsHitChanceAWP);

	SettingsHitChanceScout.SetFileId("settingshitchance_scout");
	SettingsHitChanceScout.SetBoundaries(0, 50);
	SettingsHitChanceScout.SetValue(0);
	SettingsHitChance.PlaceLabledControl("Scout", this, &SettingsHitChanceScout);

	SettingsHitChanceRifles.SetFileId("settingshitchance_rifles");
	SettingsHitChanceRifles.SetBoundaries(0, 50);
	SettingsHitChanceRifles.SetValue(0);
	SettingsHitChance.PlaceLabledControl("Rifles", this, &SettingsHitChanceRifles);

	SettingsHitChanceMPs.SetFileId("settingshitchance_mps");
	SettingsHitChanceMPs.SetBoundaries(0, 50);
	SettingsHitChanceMPs.SetValue(0);
	SettingsHitChance.PlaceLabledControl("MPs", this, &SettingsHitChanceMPs);

	SettingsHitChancePistols.SetFileId("settingshitchance_pistols");
	SettingsHitChancePistols.SetBoundaries(0, 50);
	SettingsHitChancePistols.SetValue(0);
	SettingsHitChance.PlaceLabledControl("Pistols", this, &SettingsHitChancePistols);

	SettingsHitChanceDeagle.SetFileId("settingshitchance_deagle");
	SettingsHitChanceDeagle.SetBoundaries(0, 50);
	SettingsHitChanceDeagle.SetValue(0);
	SettingsHitChance.PlaceLabledControl("Deagle", this, &SettingsHitChanceDeagle);

	SettingsHitChanceZeus.SetFileId("settingshitchance_zeus");
	SettingsHitChanceZeus.SetBoundaries(0, 50);
	SettingsHitChanceZeus.SetValue(0);
	SettingsHitChance.PlaceLabledControl("Zeus", this, &SettingsHitChanceZeus);

#pragma endregion

#pragma SettingsAALines
	SettingsAALines.SetPosition(198, 165);
	SettingsAALines.SetSize(180, 220);
	SettingsAALines.SetText("Rage Bot - AA Lines");
	RegisterControl(&SettingsAALines);

	SettingsAALinesRealR.SetFileId("settingsaalines_realred");
	SettingsAALinesRealR.SetBoundaries(0, 255);
	SettingsAALinesRealR.SetValue(0);
	SettingsAALines.PlaceLabledControl("Real: Red", this, &SettingsAALinesRealR);

	SettingsAALinesRealG.SetFileId("settingsaalines_realgreen");
	SettingsAALinesRealG.SetBoundaries(0, 255);
	SettingsAALinesRealG.SetValue(255);
	SettingsAALines.PlaceLabledControl("Real: Green", this, &SettingsAALinesRealG);

	SettingsAALinesRealB.SetFileId("settingsaalines_realblue");
	SettingsAALinesRealB.SetBoundaries(0, 255);
	SettingsAALinesRealB.SetValue(0);
	SettingsAALines.PlaceLabledControl("Real: Blue", this, &SettingsAALinesRealB);

	SettingsAALinesRealLenght.SetFileId("settingsaalines_reallength");
	SettingsAALinesRealLenght.SetBoundaries(0, 125);
	SettingsAALinesRealLenght.SetValue(50);
	SettingsAALines.PlaceLabledControl("Real: Length", this, &SettingsAALinesRealLenght);

	SettingsAALinesFakeR.SetFileId("settingsaalines_fakered");
	SettingsAALinesFakeR.SetBoundaries(0, 255);
	SettingsAALinesFakeR.SetValue(255);
	SettingsAALines.PlaceLabledControl("Fake: Red", this, &SettingsAALinesFakeR);

	SettingsAALinesFakeG.SetFileId("settingsaalines_fakegreen");
	SettingsAALinesFakeG.SetBoundaries(0, 255);
	SettingsAALinesFakeG.SetValue(0);
	SettingsAALines.PlaceLabledControl("Fake: Green", this, &SettingsAALinesFakeG);

	SettingsAALinesFakeB.SetFileId("settingsaalines_fakeblue");
	SettingsAALinesFakeB.SetBoundaries(0, 255);
	SettingsAALinesFakeB.SetValue(0);
	SettingsAALines.PlaceLabledControl("Real: Blue", this, &SettingsAALinesFakeB);

	SettingsAALinesFakeLenght.SetFileId("settingsaalines_reallength");
	SettingsAALinesFakeLenght.SetBoundaries(0, 125);
	SettingsAALinesFakeLenght.SetValue(50);
	SettingsAALines.PlaceLabledControl("Fake: Length", this, &SettingsAALinesFakeLenght);

#pragma endregion

#pragma SettingsHitScan1
	SettingsHitScan.SetPosition(13, 395);
	SettingsHitScan.SetSize(180, 120);
	SettingsHitScan.SetText("Rage Bot - HitScan");
	RegisterControl(&SettingsHitScan);

	SettingsHitScanEnable.SetFileId("settingshitscan_enable");
	SettingsHitScan.PlaceLabledControl("Enable", this, &SettingsHitScanEnable);

	SettingsHitScanAuto.SetFileId("settingshitscan_auto");
	SettingsHitScanAuto.AddItem("Off");
	SettingsHitScanAuto.AddItem("Simple");
	SettingsHitScanAuto.AddItem("Standard");
	SettingsHitScanAuto.AddItem("Extreme");
	SettingsHitScan.PlaceLabledControl("Auto Sniper", this, &SettingsHitScanAuto);

	SettingsHitScanScout.SetFileId("settingshitscan_scout");
	SettingsHitScanScout.AddItem("Off");
	SettingsHitScanScout.AddItem("Simple");
	SettingsHitScanScout.AddItem("Standard");
	SettingsHitScanScout.AddItem("Extreme");
	SettingsHitScan.PlaceLabledControl("Scout", this, &SettingsHitScanScout);

	SettingsHitScanRifles.SetFileId("settingshitscan_rifles");
	SettingsHitScanRifles.AddItem("Off");
	SettingsHitScanRifles.AddItem("Simple");
	SettingsHitScanRifles.AddItem("Standard");
	SettingsHitScanRifles.AddItem("Extreme");
	SettingsHitScan.PlaceLabledControl("Rifles", this, &SettingsHitScanRifles);

#pragma endregion

#pragma SettingsHitScan2
	SettingsHitScan2.SetPosition(198, 395);
	SettingsHitScan2.SetSize(180, 120);
	SettingsHitScan2.SetText("Rage Bot - HitScan");
	RegisterControl(&SettingsHitScan2);

	SettingsHitScan2MPs.SetFileId("settingshitscan_mps");
	SettingsHitScan2MPs.AddItem("Off");
	SettingsHitScan2MPs.AddItem("Simple");
	SettingsHitScan2MPs.AddItem("Standard");
	SettingsHitScan2MPs.AddItem("Extreme");
	SettingsHitScan2.PlaceLabledControl("MPs", this, &SettingsHitScan2MPs);

	SettingsHitScan2Pistols.SetFileId("settingshitscan_pistols");
	SettingsHitScan2Pistols.AddItem("Off");
	SettingsHitScan2Pistols.AddItem("Simple");
	SettingsHitScan2Pistols.AddItem("Standard");
	SettingsHitScan2Pistols.AddItem("Extreme");
	SettingsHitScan2.PlaceLabledControl("Pistols", this, &SettingsHitScan2Pistols);

	SettingsHitScan2Deagle.SetFileId("settingshitscan_deagle");
	SettingsHitScan2Deagle.AddItem("Off");
	SettingsHitScan2Deagle.AddItem("Simple");
	SettingsHitScan2Deagle.AddItem("Standard");
	SettingsHitScan2Deagle.AddItem("Extreme");
	SettingsHitScan2.PlaceLabledControl("Deagle", this, &SettingsHitScan2Deagle);

	SettingsHitScan2Other.SetFileId("settingshitscan_other");
	SettingsHitScan2Other.AddItem("Off");
	SettingsHitScan2Other.AddItem("Simple");
	SettingsHitScan2Other.AddItem("Standard");
	SettingsHitScan2Other.AddItem("Extreme");
	SettingsHitScan2.PlaceLabledControl("Other", this, &SettingsHitScan2Other);

#pragma endregion

#pragma SpecVisGroup
	SpecVisGroup.SetPosition(408, 16);
	SpecVisGroup.SetSize(360, 75);
	SpecVisGroup.SetText("Spectator Position");
	RegisterControl(&SpecVisGroup);

	SpecVisGroupX.SetFileId("spec_x");
	SpecVisGroupX.SetBoundaries(1, 800);
	SpecVisGroupX.SetValue(535);
	SpecVisGroup.PlaceLabledControl("Position X", this, &SpecVisGroupX);

	SpecVisGroupY.SetFileId("spec_y");
	SpecVisGroupY.SetBoundaries(1.1, 600);
	SpecVisGroupY.SetValue(90);
	SpecVisGroup.PlaceLabledControl("Position Y", this, &SpecVisGroupY);
#pragma endregion

#pragma CustomClanTag
	CustomClanTag.SetPosition(408, 105);
	CustomClanTag.SetSize(360, 105);
	CustomClanTag.SetText("Custom Clan Tag");
	RegisterControl(&CustomClanTag);

	ClanTagText.SetFileId("settings_clantag_text");
	CustomClanTag.PlaceLabledControl("ClanTag", this, &ClanTagText);

	ClanTagType.SetFileId("settings_clantag_type");
	ClanTagType.AddItem("Static");
	ClanTagType.AddItem("Flashing");
	CustomClanTag.PlaceLabledControl("Type", this, &ClanTagType);

	ClanTagApply.SetText("Apply");
	ClanTagApply.SetCallback(ClanTagApplyCallbk);
	ClanTagApply.SetPosition(440, 175);
	ClanTagApply.SetSize(300, 75);
	RegisterControl(&ClanTagApply);
#pragma endregion

#pragma OtherSettings
	OtherSettings.SetPosition(408, 224);
	OtherSettings.SetSize(360, 290);
	OtherSettings.SetText("Other Settings");
	RegisterControl(&OtherSettings);


	OtherSettingsNadePredColorR.SetFileId("nadecolor_R");
	OtherSettingsNadePredColorR.SetBoundaries(0, 255);
	OtherSettingsNadePredColorR.SetValue(0);
	OtherSettings.PlaceLabledControl("Nade Pred: Red", this, &OtherSettingsNadePredColorR);

	OtherSettingsNadePredColorG.SetFileId("nadecolor_G");
	OtherSettingsNadePredColorG.SetBoundaries(0, 255);
	OtherSettingsNadePredColorG.SetValue(150);
	OtherSettings.PlaceLabledControl("Nade Pred: Green", this, &OtherSettingsNadePredColorG);

	OtherSettingsNadePredColorB.SetFileId("nadecolor_B");
	OtherSettingsNadePredColorB.SetBoundaries(0, 255);
	OtherSettingsNadePredColorB.SetValue(255);
	OtherSettings.PlaceLabledControl("Nade Pred: Blue", this, &OtherSettingsNadePredColorB);

	OtherSettingsImpacts.SetFileId("othersettings_impacts");
	OtherSettingsImpacts.AddItem("Off");
	OtherSettingsImpacts.AddItem("Client Side");
	OtherSettingsImpacts.AddItem("Server Side");
	OtherSettingsImpacts.AddItem("Both Side");
	OtherSettings.PlaceLabledControl("Show Impacts", this, &OtherSettingsImpacts);

	OtherSettingsMemeWalk.SetFileId("othersettings_memewalk");
	OtherSettings.PlaceLabledControl("Meme Walk (Skate)", this, &OtherSettingsMemeWalk);

	OtherSettingsHitmarkerSound.SetFileId("othersettings_hitmarkersound");
	OtherSettings.PlaceLabledControl("HitMarker Sound", this, &OtherSettingsHitmarkerSound);

	OtherSettingsAutoAccept.SetFileId("othersettings_autoaccept");
	OtherSettings.PlaceLabledControl("Auto Accept", this, &OtherSettingsAutoAccept);

	OtherSettingsBruteAfterX.SetFileId("othersettings_bruteforceafter");
	OtherSettingsBruteAfterX.SetBoundaries(0, 10);
	OtherSettingsBruteAfterX.SetValue(5);
	OtherSettings.PlaceLabledControl("BruteForce After X Bullets:", this, &OtherSettingsBruteAfterX);

	OtherSettingsDebug.SetFileId("othersettings_debug");
	OtherSettings.PlaceLabledControl("Debug Info", this, &OtherSettingsDebug);

	OtherSettingsAntiUntrusted.SetFileId("othersettings_antiuntrusted");
	OtherSettingsAntiUntrusted.SetState(true);
	OtherSettings.PlaceLabledControl("Anti-Untrusted (Anti-UT)", this, &OtherSettingsAntiUntrusted);

#pragma endregion

};
void COtherSettingsTab::Setup()
{

	SetTitle("Other-Settings");

	CrosshairGroup.SetPosition(13, 16);
	CrosshairGroup.SetSize(180, 325);
	CrosshairGroup.SetText("Crosshair");
	RegisterControl(&CrosshairGroup);

	OtherCrosshairType.SetFileId("othersettings_ocrosshairtype");
	OtherCrosshairType.AddItem("None");
	OtherCrosshairType.AddItem("xHair");
	OtherCrosshairType.AddItem("Cross 1#");
	OtherCrosshairType.AddItem("Cross 2#");
	OtherCrosshairType.AddItem("Plus");
	OtherCrosshairType.AddItem("Circle");
	OtherCrosshairType.AddItem("Circle Dot");
	CrosshairGroup.PlaceLabledControl("Crosshair Type", this, &OtherCrosshairType);

	OtherCrosshairR.SetFileId("othersettings_ocrosshairR");
	OtherCrosshairR.SetBoundaries(0, 255);
	OtherCrosshairR.SetValue(255);
	CrosshairGroup.PlaceLabledControl("Color 1: Red", this, &OtherCrosshairR);

	OtherCrosshairG.SetFileId("othersettings_ocrosshairG");
	OtherCrosshairG.SetBoundaries(0, 255);
	OtherCrosshairG.SetValue(0);
	CrosshairGroup.PlaceLabledControl("Color 1: Green", this, &OtherCrosshairG);

	OtherCrosshairB.SetFileId("othersettings_ocrosshairB");
	OtherCrosshairB.SetBoundaries(0, 255);
	OtherCrosshairB.SetValue(0);
	CrosshairGroup.PlaceLabledControl("Color 1: Blue", this, &OtherCrosshairB);

	OtherCrosshairA.SetFileId("othersettings_ocrosshairA");
	OtherCrosshairA.SetBoundaries(0, 255);
	OtherCrosshairA.SetValue(255);
	CrosshairGroup.PlaceLabledControl("Color 1: Alpha", this, &OtherCrosshairA);

	OtherCrosshairRR.SetFileId("othersettings_ocrosshairRR");
	OtherCrosshairRR.SetBoundaries(0, 255);
	OtherCrosshairRR.SetValue(255);
	CrosshairGroup.PlaceLabledControl("Color 2: Red", this, &OtherCrosshairRR);

	OtherCrosshairGG.SetFileId("othersettings_ocrosshairGG");
	OtherCrosshairGG.SetBoundaries(0, 255);
	OtherCrosshairGG.SetValue(255);
	CrosshairGroup.PlaceLabledControl("Color 2: Green", this, &OtherCrosshairGG);

	OtherCrosshairBB.SetFileId("othersettings_ocrosshairBB");
	OtherCrosshairBB.SetBoundaries(0, 255);
	OtherCrosshairBB.SetValue(255);
	CrosshairGroup.PlaceLabledControl("Color 2: Blue", this, &OtherCrosshairBB);

	OtherCrosshairAA.SetFileId("othersettings_ocrosshairAA");
	OtherCrosshairAA.SetBoundaries(0, 255);
	OtherCrosshairAA.SetValue(255);
	CrosshairGroup.PlaceLabledControl("Color 2: Alpha", this, &OtherCrosshairAA);

	OtherCrosshairCScale.SetFileId("othersettings_ocrosshairCScale");
	OtherCrosshairCScale.SetBoundaries(0, 18);
	OtherCrosshairCScale.SetValue(0);
	CrosshairGroup.PlaceLabledControl("Circle: Scale", this, &OtherCrosshairCScale);

	CustomFriendlyColor.SetPosition(198, 16);
	CustomFriendlyColor.SetSize(230, 345);
	CustomFriendlyColor.SetText("Custom Friendly Color");
	RegisterControl(&CustomFriendlyColor);

	customfriendlycolorTEAMVISR.SetFileId("othersettings_customfriendlycolorTEAMVISR");
	customfriendlycolorTEAMVISR.SetBoundaries(0, 255);
	customfriendlycolorTEAMVISR.SetValue(255);
	CustomFriendlyColor.PlaceLabledControl("Team Vis: Red", this, &customfriendlycolorTEAMVISR);

	customfriendlycolorTEAMVISG.SetFileId("othersettings_customfriendlycolorTEAMVISG");
	customfriendlycolorTEAMVISG.SetBoundaries(0, 255);
	customfriendlycolorTEAMVISG.SetValue(0);
	CustomFriendlyColor.PlaceLabledControl("Team Vis: Green", this, &customfriendlycolorTEAMVISG);

	customfriendlycolorTEAMVISB.SetFileId("othersettings_customfriendlycolorTEAMVISB");
	customfriendlycolorTEAMVISB.SetBoundaries(0, 255);
	customfriendlycolorTEAMVISB.SetValue(0);
	CustomFriendlyColor.PlaceLabledControl("Team Vis: Blue", this, &customfriendlycolorTEAMVISB);

	customfriendlycolorTEAMNOVISR.SetFileId("othersettings_customfriendlycolorTEAMNOVISR");
	customfriendlycolorTEAMNOVISR.SetBoundaries(0, 255);
	customfriendlycolorTEAMNOVISR.SetValue(255);
	CustomFriendlyColor.PlaceLabledControl("Team NoVis: Red", this, &customfriendlycolorTEAMNOVISR);

	customfriendlycolorTEAMNOVISG.SetFileId("othersettings_customfriendlycolorTEAMNOVISG");
	customfriendlycolorTEAMNOVISG.SetBoundaries(0, 255);
	customfriendlycolorTEAMNOVISG.SetValue(125);
	CustomFriendlyColor.PlaceLabledControl("Team NoVis: Green", this, &customfriendlycolorTEAMNOVISG);

	customfriendlycolorTEAMNOVISB.SetFileId("othersettings_customfriendlycolorTEAMNOVISB");
	customfriendlycolorTEAMNOVISB.SetBoundaries(0, 255);
	customfriendlycolorTEAMNOVISB.SetValue(0);
	CustomFriendlyColor.PlaceLabledControl("Team NoVis: Blue", this, &customfriendlycolorTEAMNOVISB);

	customfriendlycolorENEMYVISR.SetFileId("othersettings_customfriendlycolorENEMYVISR");
	customfriendlycolorENEMYVISR.SetBoundaries(0, 255);
	customfriendlycolorENEMYVISR.SetValue(0);
	CustomFriendlyColor.PlaceLabledControl("Enemy Vis: Red", this, &customfriendlycolorENEMYVISR);

	customfriendlycolorENEMYVISG.SetFileId("othersettings_customfriendlycolorENEMYVISG");
	customfriendlycolorENEMYVISG.SetBoundaries(0, 255);
	customfriendlycolorENEMYVISG.SetValue(255);
	CustomFriendlyColor.PlaceLabledControl("Enemy Vis: Green", this, &customfriendlycolorENEMYVISG);

	customfriendlycolorENEMYVISB.SetFileId("othersettings_customfriendlycolorENEMYVISB");
	customfriendlycolorENEMYVISB.SetBoundaries(0, 255);
	customfriendlycolorENEMYVISB.SetValue(0);
	CustomFriendlyColor.PlaceLabledControl("Enemy Vis: Blue", this, &customfriendlycolorENEMYVISB);

	customfriendlycolorENEMYNOVISR.SetFileId("othersettings_customfriendlycolorENEMYNOVISR");
	customfriendlycolorENEMYNOVISR.SetBoundaries(0, 255);
	customfriendlycolorENEMYNOVISR.SetValue(0);
	CustomFriendlyColor.PlaceLabledControl("Enemy NoVis: Red", this, &customfriendlycolorENEMYNOVISR);

	customfriendlycolorENEMYNOVISG.SetFileId("othersettings_customfriendlycolorENEMYNOVISG");
	customfriendlycolorENEMYNOVISG.SetBoundaries(0, 255);
	customfriendlycolorENEMYNOVISG.SetValue(125);
	CustomFriendlyColor.PlaceLabledControl("Enemy NoVis: Green", this, &customfriendlycolorENEMYNOVISG);

	customfriendlycolorENEMYNOVISB.SetFileId("othersettings_customfriendlycolorENEMYNOVISB");
	customfriendlycolorENEMYNOVISB.SetBoundaries(0, 255);
	customfriendlycolorENEMYNOVISB.SetValue(255);
	CustomFriendlyColor.PlaceLabledControl("Enemy NoVis: Blue", this, &customfriendlycolorENEMYNOVISB);
};
void CSkinchangerTab::Setup()
{
	SetTitle("Skin-Changer");

	SkinActive.SetPosition(16, 16);
	SkinActive.SetText("Active");
	RegisterControl(&SkinActive);

	SkinEnable.SetFileId("skin_enable");
	SkinEnable.SetPosition(66, 16);
	RegisterControl(&SkinEnable);

	SkinApply.SetText("Apply");
	SkinApply.SetCallback(KnifeApplyCallbk);
	SkinApply.SetPosition(408, 10);
	SkinApply.SetSize(360, 106);
	RegisterControl(&SkinApply);

#pragma region Knife
	KnifeGroup.SetPosition(16, 48);
	KnifeGroup.SetText("Knife");
	KnifeGroup.SetSize(376, 120);
	RegisterControl(&KnifeGroup);

	OtherSkinApply.SetFileId("otr_skinapply");
	KnifeGroup.PlaceLabledControl("Skin Apply", this, &OtherSkinApply);

	OtherSkinToggle.SetFileId("otr_skintoggle");
	KnifeGroup.PlaceLabledControl("Skin Toggle", this, &OtherSkinToggle);

	KnifeModel.SetFileId("knife_model");
	KnifeModel.AddItem("Bayonet");
	KnifeModel.AddItem("Bowie Knife");
	KnifeModel.AddItem("Butterfly Knife");
	KnifeModel.AddItem("Falchion Knife");
	KnifeModel.AddItem("Flip Knife");
	KnifeModel.AddItem("Gut Knife");
	KnifeModel.AddItem("Huntsman Knife");
	KnifeModel.AddItem("Karambit");
	KnifeModel.AddItem("M9 Bayonet");
	KnifeModel.AddItem("Shadow Daggers");
	KnifeGroup.PlaceLabledControl("Knife", this, &KnifeModel);

	KnifeSkin.SetFileId("knife_skin");
	KnifeSkin.AddItem("None");
	KnifeSkin.AddItem("Crimson Web");
	KnifeSkin.AddItem("Bone Mask");
	KnifeSkin.AddItem("Fade");
	KnifeSkin.AddItem("Night");
	KnifeSkin.AddItem("Blue Steel");
	KnifeSkin.AddItem("Stained");
	KnifeSkin.AddItem("Case Hardened");
	KnifeSkin.AddItem("Slaughter");
	KnifeSkin.AddItem("Safari Mesh");
	KnifeSkin.AddItem("Boreal Forest");
	KnifeSkin.AddItem("Ultraviolet");
	KnifeSkin.AddItem("Urban Masked");
	KnifeSkin.AddItem("Scorched");
	KnifeSkin.AddItem("Rust Coat");
	KnifeSkin.AddItem("Tiger Tooth");
	KnifeSkin.AddItem("Damascus Steel");
	KnifeSkin.AddItem("Damascus Steel");
	KnifeSkin.AddItem("Marble Fade");
	KnifeSkin.AddItem("Rust Coat");
	KnifeSkin.AddItem("Doppler Ruby");
	KnifeSkin.AddItem("Doppler Sapphire");
	KnifeSkin.AddItem("Doppler Blackpearl");
	KnifeSkin.AddItem("Doppler Phase 1");
	KnifeSkin.AddItem("Doppler Phase 2");
	KnifeSkin.AddItem("Doppler Phase 3");
	KnifeSkin.AddItem("Doppler Phase 4");
	KnifeSkin.AddItem("Gamma Doppler Phase 1");
	KnifeSkin.AddItem("Gamma Doppler Phase 2");
	KnifeSkin.AddItem("Gamma Doppler Phase 3");
	KnifeSkin.AddItem("Gamma Doppler Phase 4");
	KnifeSkin.AddItem("Gamma Doppler Emerald");
	KnifeSkin.AddItem("Lore");
	KnifeGroup.PlaceLabledControl("Skin", this, &KnifeSkin);
#pragma endregion

#pragma region Machineguns
	MachinegunsGroup.SetPosition(408, 48);
	MachinegunsGroup.SetText("Machineguns");
	MachinegunsGroup.SetSize(360, 80);
	RegisterControl(&MachinegunsGroup);

	NEGEVSkin.SetFileId("negev_skin");
	NEGEVSkin.AddItem("Anodized Navy");
	NEGEVSkin.AddItem("Man-o'-war");
	NEGEVSkin.AddItem("Palm");
	NEGEVSkin.AddItem("VariCamo");
	NEGEVSkin.AddItem("Palm");
	NEGEVSkin.AddItem("CaliCamo");
	NEGEVSkin.AddItem("Terrain");
	NEGEVSkin.AddItem("Army Sheen");
	NEGEVSkin.AddItem("Bratatat");
	NEGEVSkin.AddItem("Desert-Strike");
	NEGEVSkin.AddItem("Nuclear Waste");
	NEGEVSkin.AddItem("Loudmouth");
	NEGEVSkin.AddItem("Power Loader");
	MachinegunsGroup.PlaceLabledControl("Negev", this, &NEGEVSkin);

	M249Skin.SetFileId("m249_skin");
	M249Skin.AddItem("Contrast Spray");
	M249Skin.AddItem("Blizzard Marbleized");
	M249Skin.AddItem("Jungle DDPAT");
	M249Skin.AddItem("Gator Mesh");
	M249Skin.AddItem("Magma");
	M249Skin.AddItem("System Lock");
	M249Skin.AddItem("Shipping Forecast");
	M249Skin.AddItem("Impact Drill");
	M249Skin.AddItem("Nebula Crusader");
	M249Skin.AddItem("Spectre");
	M249Skin.AddItem("Emerald Poison Dart");
	MachinegunsGroup.PlaceLabledControl("M249", this, &M249Skin);

#pragma endregion

#pragma region Snipers
	Snipergroup.SetPosition(16, 175);
	Snipergroup.SetText("Snipers");
	Snipergroup.SetSize(376, 125);
	RegisterControl(&Snipergroup);

	AWPSkin.SetFileId("awp_skin");
	AWPSkin.AddItem("BOOM");
	AWPSkin.AddItem("Dragon Lore");
	AWPSkin.AddItem("Pink DDPAT");
	AWPSkin.AddItem("Snake Camo");
	AWPSkin.AddItem("Lightning Strike");
	AWPSkin.AddItem("Safari Mesh");
	AWPSkin.AddItem("Corticera");
	AWPSkin.AddItem("Redline");
	AWPSkin.AddItem("Man-o'-war");
	AWPSkin.AddItem("Graphite");
	AWPSkin.AddItem("Electric Hive");
	AWPSkin.AddItem("Pit Viper");
	AWPSkin.AddItem("Asiimov");
	AWPSkin.AddItem("Worm God");
	AWPSkin.AddItem("Medusa");
	AWPSkin.AddItem("Sun in Leo");
	AWPSkin.AddItem("Hyper Beast");
	AWPSkin.AddItem("Elite Build");
	AWPSkin.AddItem("Fever Dream");
	AWPSkin.AddItem("Oni Taiji");
	Snipergroup.PlaceLabledControl("AWP", this, &AWPSkin);

	SSG08Skin.SetFileId("sgg08_skin");
	SSG08Skin.AddItem("Lichen Dashed");
	SSG08Skin.AddItem("Dark Water");
	SSG08Skin.AddItem("Blue Spruce");
	SSG08Skin.AddItem("Sand Dune");
	SSG08Skin.AddItem("Palm");
	SSG08Skin.AddItem("Mayan Dreams");
	SSG08Skin.AddItem("Blood in the Water");
	SSG08Skin.AddItem("Tropical Storm");
	SSG08Skin.AddItem("Acid Fade");
	SSG08Skin.AddItem("Slashed");
	SSG08Skin.AddItem("Detour");
	SSG08Skin.AddItem("Abyss");
	SSG08Skin.AddItem("Big Iron");
	SSG08Skin.AddItem("Necropos");
	SSG08Skin.AddItem("Ghost Crusader");
	SSG08Skin.AddItem("Dragonfire");
	SSG08Skin.AddItem("Death's Head");
	Snipergroup.PlaceLabledControl("SGG 08", this, &SSG08Skin);

	SCAR20Skin.SetFileId("scar20_skin");
	SCAR20Skin.AddItem("Splash Jam");
	SCAR20Skin.AddItem("Storm");
	SCAR20Skin.AddItem("Contractor");
	SCAR20Skin.AddItem("Carbon Fiber");
	SCAR20Skin.AddItem("Sand Mesh");
	SCAR20Skin.AddItem("Palm");
	SCAR20Skin.AddItem("Emerald");
	SCAR20Skin.AddItem("Crimson Web");
	SCAR20Skin.AddItem("Cardiac");
	SCAR20Skin.AddItem("Army Sheen");
	SCAR20Skin.AddItem("Cyrex");
	SCAR20Skin.AddItem("Grotto");
	SCAR20Skin.AddItem("Emerald");
	SCAR20Skin.AddItem("Green Marine");
	SCAR20Skin.AddItem("Outbreak");
	SCAR20Skin.AddItem("Bloodsport");
	SCAR20Skin.AddItem("Blueprint");
	Snipergroup.PlaceLabledControl("SCAR-20", this, &SCAR20Skin);

	G3SG1Skin.SetFileId("g3sg1_skin");
	G3SG1Skin.AddItem("Desert Storm");
	G3SG1Skin.AddItem("Arctic Camo");
	G3SG1Skin.AddItem("Bone Mask");
	G3SG1Skin.AddItem("Contractor");
	G3SG1Skin.AddItem("Safari Mesh");
	G3SG1Skin.AddItem("Polar Camo");
	G3SG1Skin.AddItem("Jungle Dashed");
	G3SG1Skin.AddItem("VariCamo");
	G3SG1Skin.AddItem("Predator");
	G3SG1Skin.AddItem("Demeter");
	G3SG1Skin.AddItem("Azure Zebra");
	G3SG1Skin.AddItem("Green Apple");
	G3SG1Skin.AddItem("Orange Kimono");
	G3SG1Skin.AddItem("Neon Kimono");
	G3SG1Skin.AddItem("Murky");
	G3SG1Skin.AddItem("Chronos");
	G3SG1Skin.AddItem("Flux");
	G3SG1Skin.AddItem("The Executioner");
	G3SG1Skin.AddItem("Orange Crash");
	Snipergroup.PlaceLabledControl("G3SG1", this, &G3SG1Skin);
#pragma endregion

#pragma region Shotguns
	Shotgungroup.SetPosition(408, 135);
	Shotgungroup.SetText("Shotguns");
	Shotgungroup.SetSize(360, 125);
	RegisterControl(&Shotgungroup);

	MAG7Skin.SetFileId("mag7_skin");
	MAG7Skin.AddItem("Counter Terrace");
	MAG7Skin.AddItem("Metallic DDPAT");
	MAG7Skin.AddItem("Silver");
	MAG7Skin.AddItem("Storm");
	MAG7Skin.AddItem("Bulldozer");
	MAG7Skin.AddItem("Heat");
	MAG7Skin.AddItem("Sand Dune");
	MAG7Skin.AddItem("Irradiated Alert");
	MAG7Skin.AddItem("Memento");
	MAG7Skin.AddItem("Hazard");
	MAG7Skin.AddItem("Heaven Guard");
	MAG7Skin.AddItem("Firestarter");
	MAG7Skin.AddItem("Seabird");
	MAG7Skin.AddItem("Cobalt Core");
	MAG7Skin.AddItem("Praetorian");
	MAG7Skin.AddItem("Hard Water");
	Shotgungroup.PlaceLabledControl("Mag-7", this, &MAG7Skin);

	XM1014Skin.SetFileId("xm1014_skin");
	XM1014Skin.AddItem("Blaze Orange");
	XM1014Skin.AddItem("VariCamo Blue");
	XM1014Skin.AddItem("Bone Mask");
	XM1014Skin.AddItem("Blue Steel");
	XM1014Skin.AddItem("Blue Spruce");
	XM1014Skin.AddItem("Grassland");
	XM1014Skin.AddItem("Urban Perforated");
	XM1014Skin.AddItem("Jungle");
	XM1014Skin.AddItem("VariCamo");
	XM1014Skin.AddItem("VariCamo");
	XM1014Skin.AddItem("Fallout Warning");
	XM1014Skin.AddItem("Jungle");
	XM1014Skin.AddItem("CaliCamo");
	XM1014Skin.AddItem("Pit Viper");
	XM1014Skin.AddItem("Tranquility");
	XM1014Skin.AddItem("Red Python");
	XM1014Skin.AddItem("Heaven Guard");
	XM1014Skin.AddItem("Red Leather");
	XM1014Skin.AddItem("Bone Machine");
	XM1014Skin.AddItem("Quicksilver");
	XM1014Skin.AddItem("Scumbria");
	XM1014Skin.AddItem("Teclu Burner");
	XM1014Skin.AddItem("Black Tie");
	XM1014Skin.AddItem("Seasons");
	Shotgungroup.PlaceLabledControl("XM1014", this, &XM1014Skin);

	SAWEDOFFSkin.SetFileId("sawedoff_skin");
	SAWEDOFFSkin.AddItem("First Class");
	SAWEDOFFSkin.AddItem("Forest DDPAT");
	SAWEDOFFSkin.AddItem("Contrast Spray");
	SAWEDOFFSkin.AddItem("Snake Camo");
	SAWEDOFFSkin.AddItem("Orange DDPAT");
	SAWEDOFFSkin.AddItem("Fade");
	SAWEDOFFSkin.AddItem("Copper");
	SAWEDOFFSkin.AddItem("Origami");
	SAWEDOFFSkin.AddItem("Sage Spray");
	SAWEDOFFSkin.AddItem("VariCamo");
	SAWEDOFFSkin.AddItem("Irradiated Alert");
	SAWEDOFFSkin.AddItem("Mosaico");
	SAWEDOFFSkin.AddItem("Serenity");
	SAWEDOFFSkin.AddItem("Amber Fade");
	SAWEDOFFSkin.AddItem("Full Stop");
	SAWEDOFFSkin.AddItem("Highwayman");
	SAWEDOFFSkin.AddItem("The Kraken");
	SAWEDOFFSkin.AddItem("Rust Coat");
	SAWEDOFFSkin.AddItem("Bamboo Shadow");
	SAWEDOFFSkin.AddItem("Bamboo Forest");
	SAWEDOFFSkin.AddItem("Yorick");
	SAWEDOFFSkin.AddItem("Fubar");
	SAWEDOFFSkin.AddItem("Wasteland Princess");
	SAWEDOFFSkin.AddItem("Zander");
	Shotgungroup.PlaceLabledControl("Sawed-Off", this, &SAWEDOFFSkin);

	NOVASkin.SetFileId("nova_skin");
	NOVASkin.AddItem("Candy Apple");
	NOVASkin.AddItem("Blaze Orange");
	NOVASkin.AddItem("Modern Hunter");
	NOVASkin.AddItem("Forest Leaves");
	NOVASkin.AddItem("Bloomstick");
	NOVASkin.AddItem("Sand Dune");
	NOVASkin.AddItem("Polar Mesh");
	NOVASkin.AddItem("Walnut");
	NOVASkin.AddItem("Predator");
	NOVASkin.AddItem("Tempest");
	NOVASkin.AddItem("Graphite");
	NOVASkin.AddItem("Ghost Camo");
	NOVASkin.AddItem("Rising Skull");
	NOVASkin.AddItem("Antique");
	NOVASkin.AddItem("Green Apple");
	NOVASkin.AddItem("Caged Steel");
	NOVASkin.AddItem("Koi");
	NOVASkin.AddItem("Moon in Libra");
	NOVASkin.AddItem("Ranger");
	NOVASkin.AddItem("HyperBeast");
	Shotgungroup.PlaceLabledControl("Nova", this, &NOVASkin);
#pragma endregion

#pragma region Rifles
	Riflegroup.SetPosition(16, 310);
	Riflegroup.SetText("Rifles");
	Riflegroup.SetSize(376, 195);
	RegisterControl(&Riflegroup);

	AK47Skin.SetFileId("ak47_skin");
	AK47Skin.AddItem("First Class");
	AK47Skin.AddItem("Red Laminate");
	AK47Skin.AddItem("Case Hardened");
	AK47Skin.AddItem("Black Laminate");
	AK47Skin.AddItem("Fire Serpent");
	AK47Skin.AddItem("Cartel");
	AK47Skin.AddItem("Emerald Pinstripe");
	AK47Skin.AddItem("Blue Laminate");
	AK47Skin.AddItem("Redline");
	AK47Skin.AddItem("Vulcan");
	AK47Skin.AddItem("Jaguar");
	AK47Skin.AddItem("Jet Set");
	AK47Skin.AddItem("Wasteland Rebel");
	AK47Skin.AddItem("Elite Build");
	AK47Skin.AddItem("Hydroponic");
	AK47Skin.AddItem("Aquamarine Revenge");
	AK47Skin.AddItem("Frontside Misty");
	AK47Skin.AddItem("Point Disarray");
	AK47Skin.AddItem("Fuel Injector");
	AK47Skin.AddItem("Neon Revolution");
	AK47Skin.AddItem("BloodSport");
	AK47Skin.AddItem("Orbit Mk01");
	AK47Skin.AddItem("The Empress");
	Riflegroup.PlaceLabledControl("AK-47", this, &AK47Skin);

	M41SSkin.SetFileId("m4a1s_skin");
	M41SSkin.AddItem("Dark Water");
	M41SSkin.AddItem("Hyper Beast");
	M41SSkin.AddItem("Boreal Forest");
	M41SSkin.AddItem("VariCamo");
	M41SSkin.AddItem("Nitro");
	M41SSkin.AddItem("Bright Water");
	M41SSkin.AddItem("Atomic Alloy");
	M41SSkin.AddItem("Blood Tiger");
	M41SSkin.AddItem("Guardian");
	M41SSkin.AddItem("Master Piece");
	M41SSkin.AddItem("Knight");
	M41SSkin.AddItem("Cyrex");
	M41SSkin.AddItem("Basilisk");
	M41SSkin.AddItem("Icarus Fell");
	M41SSkin.AddItem("Hot Rod");
	M41SSkin.AddItem("Golden Coi");
	M41SSkin.AddItem("Chantico's Fire");
	M41SSkin.AddItem("Mecha Industries");
	M41SSkin.AddItem("Flashback");
	M41SSkin.AddItem("Decimator");
	M41SSkin.AddItem("Briefing");
	Riflegroup.PlaceLabledControl("M4A1-S", this, &M41SSkin);

	M4A4Skin.SetFileId("m4a4_skin");
	M4A4Skin.AddItem("Bullet Rain");
	M4A4Skin.AddItem("Zirka");
	M4A4Skin.AddItem("Asiimov");
	M4A4Skin.AddItem("Howl");
	M4A4Skin.AddItem("X-Ray");
	M4A4Skin.AddItem("Desert-Strike");
	M4A4Skin.AddItem("Griffin");
	M4A4Skin.AddItem("Dragon King");
	M4A4Skin.AddItem("Poseidon");
	M4A4Skin.AddItem("Daybreak");
	M4A4Skin.AddItem("Evil Daimyo");
	M4A4Skin.AddItem("Royal Paladin");
	M4A4Skin.AddItem("The BattleStar");
	M4A4Skin.AddItem("Desolate Space");
	M4A4Skin.AddItem("Buzz Kill");
	M4A4Skin.AddItem("Hell Fire");
	Riflegroup.PlaceLabledControl("M4A4", this, &M4A4Skin);

	AUGSkin.SetFileId("aug_skin");
	AUGSkin.AddItem("Bengal Tiger");
	AUGSkin.AddItem("Hot Rod");
	AUGSkin.AddItem("Chameleon");
	AUGSkin.AddItem("Torque");
	AUGSkin.AddItem("Radiation Hazard");
	AUGSkin.AddItem("Asterion");
	AUGSkin.AddItem("Daedalus");
	AUGSkin.AddItem("Akihabara Accept");
	AUGSkin.AddItem("Ricochet");
	AUGSkin.AddItem("Fleet Flock");
	AUGSkin.AddItem("Syd Mead");
	Riflegroup.PlaceLabledControl("AUG", this, &AUGSkin);

	FAMASSkin.SetFileId("famas_skin");
	FAMASSkin.AddItem("Contrast Spray");
	FAMASSkin.AddItem("Colony");
	FAMASSkin.AddItem("Cyanospatter");
	FAMASSkin.AddItem("Djinn");
	FAMASSkin.AddItem("Afterimage");
	FAMASSkin.AddItem("Doomkitty");
	FAMASSkin.AddItem("Spitfire");
	FAMASSkin.AddItem("Teardown");
	FAMASSkin.AddItem("Hexane");
	FAMASSkin.AddItem("Pulse");
	FAMASSkin.AddItem("Sergeant");
	FAMASSkin.AddItem("Styx");
	FAMASSkin.AddItem("Neural Net");
	FAMASSkin.AddItem("Survivor");
	FAMASSkin.AddItem("Valence");
	FAMASSkin.AddItem("Roll Cage");
	FAMASSkin.AddItem("Mecha Industries");
	FAMASSkin.AddItem("Macabre");
	Riflegroup.PlaceLabledControl("FAMAS", this, &FAMASSkin);

	GALILSkin.SetFileId("galil_skin");
	GALILSkin.AddItem("Forest DDPAT");
	GALILSkin.AddItem("Contrast Spray");
	GALILSkin.AddItem("Orange DDPAT");
	GALILSkin.AddItem("Eco");
	GALILSkin.AddItem("Winter Forest");
	GALILSkin.AddItem("Sage Spray");
	GALILSkin.AddItem("VariCamo");
	GALILSkin.AddItem("VariCamo");
	GALILSkin.AddItem("Chatterbox");
	GALILSkin.AddItem("Shattered");
	GALILSkin.AddItem("Kami");
	GALILSkin.AddItem("Blue Titanium");
	GALILSkin.AddItem("Urban Rubble");
	GALILSkin.AddItem("Hunting Blind");
	GALILSkin.AddItem("Sandstorm");
	GALILSkin.AddItem("Tuxedo");
	GALILSkin.AddItem("Cerberus");
	GALILSkin.AddItem("Aqua Terrace");
	GALILSkin.AddItem("Rocket Pop");
	GALILSkin.AddItem("Stone Cold");
	GALILSkin.AddItem("Firefight");
	GALILSkin.AddItem("Crimson Tsunami");
	GALILSkin.AddItem("Suger Rush");
	Riflegroup.PlaceLabledControl("GALIL", this, &GALILSkin);

	SG553Skin.SetFileId("sg552_skin");
	SG553Skin.AddItem("Bulldozer");
	SG553Skin.AddItem("Ultraviolet");
	SG553Skin.AddItem("Damascus Steel");
	SG553Skin.AddItem("Fallout Warning");
	SG553Skin.AddItem("Damascus Steel");
	SG553Skin.AddItem("Pulse");
	SG553Skin.AddItem("Army Sheen");
	SG553Skin.AddItem("Traveler");
	SG553Skin.AddItem("Fallout Warning");
	SG553Skin.AddItem("Cyrex");
	SG553Skin.AddItem("Tiger Moth");
	SG553Skin.AddItem("Atlas");
	Riflegroup.PlaceLabledControl("SG552", this, &SG553Skin);
#pragma endregion



#pragma endregion

#pragma region Pistols
	PistolGroup.SetPosition(408, 270);
	PistolGroup.SetText("Pistols");
	PistolGroup.SetSize(360, 245);
	RegisterControl(&PistolGroup);

	GLOCKSkin.SetFileId("glock_skin");
	GLOCKSkin.AddItem("Groundwater");
	GLOCKSkin.AddItem("Candy Apple");
	GLOCKSkin.AddItem("Fade");
	GLOCKSkin.AddItem("Night");
	GLOCKSkin.AddItem("Dragon Tattoo");
	GLOCKSkin.AddItem("Twilight Galaxy");
	GLOCKSkin.AddItem("Sand Dune");
	GLOCKSkin.AddItem("Brass");
	GLOCKSkin.AddItem("Catacombs");
	GLOCKSkin.AddItem("Sand Dune");
	GLOCKSkin.AddItem("Steel Disruption");
	GLOCKSkin.AddItem("Blue Fissure");
	GLOCKSkin.AddItem("Death Rattle");
	GLOCKSkin.AddItem("Water Elemental");
	GLOCKSkin.AddItem("Reactor");
	GLOCKSkin.AddItem("Grinder");
	GLOCKSkin.AddItem("Bunsen Burner");
	GLOCKSkin.AddItem("Wraith");
	GLOCKSkin.AddItem("Royal Legion");
	GLOCKSkin.AddItem("Weasel");
	GLOCKSkin.AddItem("Wasteland Rebel");
	PistolGroup.PlaceLabledControl("Glock", this, &GLOCKSkin);

	USPSSkin.SetFileId("usps_skin");
	USPSSkin.AddItem("Forest Leaves");
	USPSSkin.AddItem("Dark Water");
	USPSSkin.AddItem("VariCamo");
	USPSSkin.AddItem("Overgrowth");
	USPSSkin.AddItem("Caiman");
	USPSSkin.AddItem("Blood Tiger");
	USPSSkin.AddItem("Serum");
	USPSSkin.AddItem("Night Ops");
	USPSSkin.AddItem("Stainless");
	USPSSkin.AddItem("Guardian");
	USPSSkin.AddItem("Orion");
	USPSSkin.AddItem("Road Rash");
	USPSSkin.AddItem("Royal Blue");
	USPSSkin.AddItem("Business Class");
	USPSSkin.AddItem("Para Green");
	USPSSkin.AddItem("Torque");
	USPSSkin.AddItem("Kill Confirmed");
	USPSSkin.AddItem("Lead Conduit");
	USPSSkin.AddItem("Cyrex");
	USPSSkin.AddItem("Neo-Noir");
	USPSSkin.AddItem("Blueprint");
	PistolGroup.PlaceLabledControl("USP-S", this, &USPSSkin);

	DEAGLESkin.SetFileId("deagle_skin");
	DEAGLESkin.AddItem("Blaze");
	DEAGLESkin.AddItem("Pilot");
	DEAGLESkin.AddItem("Midnight Storm");
	DEAGLESkin.AddItem("Sunset Storm");
	DEAGLESkin.AddItem("Forest DDPAT");
	DEAGLESkin.AddItem("Crimson Web");
	DEAGLESkin.AddItem("Urban DDPAT");
	DEAGLESkin.AddItem("Night");
	DEAGLESkin.AddItem("Hypnotic");
	DEAGLESkin.AddItem("Mudder");
	DEAGLESkin.AddItem("VariCamo");
	DEAGLESkin.AddItem("Golden Koi");
	DEAGLESkin.AddItem("Cobalt Disruption");
	DEAGLESkin.AddItem("Urban Rubble");
	DEAGLESkin.AddItem("Naga");
	DEAGLESkin.AddItem("Hand Cannon");
	DEAGLESkin.AddItem("Heirloom");
	DEAGLESkin.AddItem("Meteorite");
	DEAGLESkin.AddItem("Conspiracy");
	DEAGLESkin.AddItem("Bronze Deco");
	DEAGLESkin.AddItem("Sunset Storm");
	DEAGLESkin.AddItem("Corinthian");
	DEAGLESkin.AddItem("Kumicho Dragon");
	DEAGLESkin.AddItem("Oxide Blaze");
	PistolGroup.PlaceLabledControl("Deagle", this, &DEAGLESkin);

	DUALSSkin.SetFileId("duals_skin");
	DUALSSkin.AddItem("Anodized Navy");
	DUALSSkin.AddItem("Stained");
	DUALSSkin.AddItem("Contractor");
	DUALSSkin.AddItem("Colony");
	DUALSSkin.AddItem("Demolition");
	DUALSSkin.AddItem("Black Limba");
	DUALSSkin.AddItem("Cobalt Quartz");
	DUALSSkin.AddItem("Hemoglobin");
	DUALSSkin.AddItem("Urban Shock");
	DUALSSkin.AddItem("Marina");
	DUALSSkin.AddItem("Panther");
	DUALSSkin.AddItem("Retribution");
	DUALSSkin.AddItem("Briar");
	DUALSSkin.AddItem("Duelist");
	DUALSSkin.AddItem("Moon in Libra");
	DUALSSkin.AddItem("Dualing Dragons");
	DUALSSkin.AddItem("Cartel");
	DUALSSkin.AddItem("Ventilators");
	DUALSSkin.AddItem("Cobra Strike");
	PistolGroup.PlaceLabledControl("Duals", this, &DUALSSkin);

	FIVESEVENSkin.SetFileId("fiveseven_skin");
	FIVESEVENSkin.AddItem("Candy Apple");
	FIVESEVENSkin.AddItem("Bone Mask");
	FIVESEVENSkin.AddItem("Case Hardened");
	FIVESEVENSkin.AddItem("Contractor");
	FIVESEVENSkin.AddItem("Forest Night");
	FIVESEVENSkin.AddItem("Orange Peel");
	FIVESEVENSkin.AddItem("Jungle");
	FIVESEVENSkin.AddItem("Nitro");
	FIVESEVENSkin.AddItem("Red Quartz");
	FIVESEVENSkin.AddItem("Anodized Gunmetal");
	FIVESEVENSkin.AddItem("Nightshade");
	FIVESEVENSkin.AddItem("Silver Quartz");
	FIVESEVENSkin.AddItem("Kami");
	FIVESEVENSkin.AddItem("Copper Galaxy");
	FIVESEVENSkin.AddItem("Neon Kimono");
	FIVESEVENSkin.AddItem("Fowl Play");
	FIVESEVENSkin.AddItem("Hot Shot");
	FIVESEVENSkin.AddItem("Urban Hazard");
	FIVESEVENSkin.AddItem("Monkey Business");
	FIVESEVENSkin.AddItem("Retrobution");
	FIVESEVENSkin.AddItem("Triumvirate");
	FIVESEVENSkin.AddItem("Capillary");
	FIVESEVENSkin.AddItem("Hyper Beast");
	PistolGroup.PlaceLabledControl("Five-Seven", this, &FIVESEVENSkin);

	TECNINESkin.SetFileId("tec9_skin");
	TECNINESkin.AddItem("Tornado");
	TECNINESkin.AddItem("Groundwater");
	TECNINESkin.AddItem("Forest DDPAT");
	TECNINESkin.AddItem("Terrace");
	TECNINESkin.AddItem("Urban DDPAT");
	TECNINESkin.AddItem("Ossified");
	TECNINESkin.AddItem("Hades");
	TECNINESkin.AddItem("Brass");
	TECNINESkin.AddItem("VariCamo");
	TECNINESkin.AddItem("Nuclear Threat");
	TECNINESkin.AddItem("Red Quartz");
	TECNINESkin.AddItem("Tornado");
	TECNINESkin.AddItem("Blue Titanium");
	TECNINESkin.AddItem("Army Mesh");
	TECNINESkin.AddItem("Titanium Bit");
	TECNINESkin.AddItem("Sandstorm");
	TECNINESkin.AddItem("Isaac");
	TECNINESkin.AddItem("Toxic");
	TECNINESkin.AddItem("Bamboo Forest");
	TECNINESkin.AddItem("Avalanche");
	TECNINESkin.AddItem("Jambiya");
	TECNINESkin.AddItem("Re-Entry");
	TECNINESkin.AddItem("Fuel Injector");
	TECNINESkin.AddItem("Cut Out");
	PistolGroup.PlaceLabledControl("Tec-9", this, &TECNINESkin);

	P2000Skin.SetFileId("p2000_skin");
	P2000Skin.AddItem("Grassland Leaves");
	P2000Skin.AddItem("Silver");
	P2000Skin.AddItem("Granite Marbleized");
	P2000Skin.AddItem("Forest Leaves");
	P2000Skin.AddItem("Ossified");
	P2000Skin.AddItem("Handgun");
	P2000Skin.AddItem("Fade");
	P2000Skin.AddItem("Scorpion");
	P2000Skin.AddItem("Grassland");
	P2000Skin.AddItem("Corticera");
	P2000Skin.AddItem("Ocean Foam");
	P2000Skin.AddItem("Pulse");
	P2000Skin.AddItem("Amber Fade");
	P2000Skin.AddItem("Red FragCam");
	P2000Skin.AddItem("Chainmail");
	P2000Skin.AddItem("Coach Class");
	P2000Skin.AddItem("Ivory");
	P2000Skin.AddItem("Fire Elemental");
	P2000Skin.AddItem("Asterion");
	P2000Skin.AddItem("Pathfinder");
	P2000Skin.AddItem("Imperial");
	P2000Skin.AddItem("Oceanic");
	P2000Skin.AddItem("Imperial Dragon");
	P2000Skin.AddItem("Woodsman");
	PistolGroup.PlaceLabledControl("P2000", this, &P2000Skin);

	P250Skin.SetFileId("p250_skin");
	P250Skin.AddItem("Whiteout");
	P250Skin.AddItem("Metallic DDPAT");
	P250Skin.AddItem("Splash");
	P250Skin.AddItem("Gunsmoke");
	P250Skin.AddItem("Modern Hunter");
	P250Skin.AddItem("Bone Mask");
	P250Skin.AddItem("Boreal Forest");
	P250Skin.AddItem("Sand Dune");
	P250Skin.AddItem("Nuclear Threat");
	P250Skin.AddItem("Mehndi");
	P250Skin.AddItem("Facets");
	P250Skin.AddItem("Hive");
	P250Skin.AddItem("Muertos");
	P250Skin.AddItem("Steel Disruption");
	P250Skin.AddItem("Undertow");
	P250Skin.AddItem("Franklin");
	P250Skin.AddItem("Neon Kimono");
	P250Skin.AddItem("Supernova");
	P250Skin.AddItem("Contamination");
	P250Skin.AddItem("Cartel");
	P250Skin.AddItem("Valence");
	P250Skin.AddItem("Crimson Kimono");
	P250Skin.AddItem("Mint Kimono");
	P250Skin.AddItem("Wing Shot");
	P250Skin.AddItem("Asiimov");
	P250Skin.AddItem("Ripple");
	P250Skin.AddItem("Red Rock");
	P250Skin.AddItem("See Ya Later");
	PistolGroup.PlaceLabledControl("P250", this, &P250Skin);
	
	CZSkin.SetFileId("cz_skin");
	CZSkin.AddItem("Victoria");
	CZSkin.AddItem("Crimson Web");
	CZSkin.AddItem("Yellow Jacket");
	CZSkin.AddItem("Fuschia");
	CZSkin.AddItem("Xiangliu");
	CZSkin.AddItem("Pole Position");
	CZSkin.AddItem("Tigris");
	CZSkin.AddItem("Red Astor");
	CZSkin.AddItem("Tread Plate");
	CZSkin.AddItem("Chalice");
	CZSkin.AddItem("Imprint");
	CZSkin.AddItem("Twist");
	CZSkin.AddItem("Polymer");
	CZSkin.AddItem("Hexane");
	CZSkin.AddItem("Tuxedo");
	CZSkin.AddItem("Nitro");
	CZSkin.AddItem("Emerald");
	CZSkin.AddItem("Poison Dart");
	PistolGroup.PlaceLabledControl("CZ75", this, &CZSkin);
	
	

#pragma endregion

	
#pragma endregion other random options
}

void Menu::SetupMenu()
{
	Window.Setup();

	GUI.RegisterWindow(&Window);
	GUI.BindWindow(VK_INSERT, &Window);
}

void Menu::DoUIFrame()
{
	static bool sumfuk = false;
	if (!sumfuk)
	{
		if (Menu::Window.SkinchangerTab.SkinEnable.GetState())
		{
			KnifeApplyCallbk();
		}
		sumfuk = true;
	}
	// General Processing

	// If the "all filter is selected tick all the others
	if (Window.VisualsTab.FiltersAll.GetState())
	{
		Window.VisualsTab.FiltersC4.SetState(true);
		Window.VisualsTab.FiltersChickens.SetState(true);
		Window.VisualsTab.FiltersPlayers.SetState(true);
		Window.VisualsTab.FiltersWeapons.SetState(true);
	}

	if (Window.MiscTab.OtherSkinChanger.GetState())
	{
//		RegisterTab(&ApocalypseWindow::Setup.SkinchangerTab);
	}
	

	GUI.Update();
	GUI.Draw();
}

