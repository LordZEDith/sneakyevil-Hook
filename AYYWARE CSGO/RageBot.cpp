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
UC Community <3

*/

#include "RageBot.h"
#include "RenderManager.h"
#include "Resolver.h"
#include "Autowall.h"
#include <iostream>
#include "UTIL Functions.h"

#define M_PI 3.14159265358979323846
#define TICK_INTERVAL			( Interfaces::Globals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
extern int Shooted;

void CRageBot::Init()
{
	IsAimStepping = false;
	IsLocked = false;
	TargetID = -1;
}

void CRageBot::Draw()
{

}

bool IsAbleToShoot(IClientEntity* pLocal)
{
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!pLocal)
		return false;

	if (!pWeapon)
		return false;

	float flServerTime = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;

	return (!(pWeapon->GetNextPrimaryAttack() > flServerTime));
}

float hitchance(IClientEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
	//CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	float hitchance = 101;
	if (!pWeapon) return 0;
	if (Menu::Window.RageBotTab.AccuracyHitchance.GetValue() > 1)
	{//Inaccuracy method
		float inaccuracy = pWeapon->GetInaccuracy();
		if (inaccuracy == 0) inaccuracy = 0.0000001;
		inaccuracy = 1 / inaccuracy;
		hitchance = inaccuracy;
	}
	return hitchance;
}

void VectorAngles2(const Vector &vecForward, Vector &vecAngles)
{
	Vector vecView;
	if (vecForward[1] == 0.f && vecForward[0] == 0.f)
	{
		vecView[0] = 0.f;
		vecView[1] = 0.f;
	}
	else
	{
		vecView[1] = atan2(vecForward[1], vecForward[0]) * 180.f / M_PI;

		if (vecView[1] < 0.f)
			vecView[1] += 360.f;

		vecView[2] = sqrt(vecForward[0] * vecForward[0] + vecForward[1] * vecForward[1]);

		vecView[0] = atan2(vecForward[2], vecView[2]) * 180.f / M_PI;
	}

	vecAngles[0] = -vecView[0];
	vecAngles[1] = vecView[1];
	vecAngles[2] = 0.f;
}

void AngleVectors2(const Vector& qAngles, Vector& vecForward)
{
	float sp, sy, cp, cy;
	SinCos((float)(qAngles[1] * (M_PI / 180.f)), &sy, &cy);
	SinCos((float)(qAngles[0] * (M_PI / 180.f)), &sp, &cp);

	vecForward[0] = cp*cy;
	vecForward[1] = cp*sy;
	vecForward[2] = -sp;
}
// (DWORD)g_pNetVars->GetOffset("DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
// You need something like this
bool CanOpenFire() // Creds to untrusted guy
{
	IClientEntity* pLocalEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocalEntity)
		return false;

	CBaseCombatWeapon* entwep = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocalEntity->GetActiveWeaponHandle());

	float flServerTime = (float)pLocalEntity->GetTickBase() * Interfaces::Globals->interval_per_tick;
	float flNextPrimaryAttack = entwep->GetNextPrimaryAttack();

	std::cout << flServerTime << " " << flNextPrimaryAttack << std::endl;

	return !(flNextPrimaryAttack > flServerTime);
}

void CRageBot::Move(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocalEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (!pLocalEntity)
		return;

	// Master switch
	if (!Menu::Window.RageBotTab.Active.GetState())
		return;

	// Anti Aim 
	if (Menu::Window.RageBotTab.AntiAimEnable.GetState())
	{
		static int ChokedPackets = -1;

		if (!pWeapon)
			return;

		if (ChokedPackets < 1 && pLocalEntity->GetLifeState() == LIFE_ALIVE && pCmd->buttons & IN_ATTACK && CanOpenFire() && GameUtils::IsBallisticWeapon(pWeapon))
		{
			bSendPacket = false;
		}
		else
		{
			if (pLocalEntity->GetLifeState() == LIFE_ALIVE)
			{
				DoAntiAim(pCmd, bSendPacket);
			}
			ChokedPackets = -1;
		}
	}

	// Position Adjustment
	if (Menu::Window.RageBotTab.AccuracyPositionAdjustment.GetState())
		PositionAdjustment(pCmd);

	// Aimbot
	if (Menu::Window.RageBotTab.AimbotEnable.GetState())
		DoAimbot(pCmd, bSendPacket);

	// Recoil
	if (Menu::Window.RageBotTab.AccuracyRecoil.GetState())
		DoNoRecoil(pCmd);

	if (Menu::Window.RageBotTab.HitChanceAuto.GetState())
		HitChanceA(pCmd);

	if (Menu::Window.SettingsTab.SettingsHitScanEnable.GetState())
		HitScanA(pCmd);

	// Aimstep
	if (Menu::Window.RageBotTab.AimbotAimStep.GetState())
	{
		Vector AddAngs = pCmd->viewangles - LastAngle;
		if (AddAngs.Length2D() > 25.f)
		{
			Normalize(AddAngs, AddAngs);
			AddAngs *= 25;
			pCmd->viewangles = LastAngle + AddAngs;
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
		}
	}

	LastAngle = pCmd->viewangles;
}

Vector BestPoint(IClientEntity *targetPlayer, Vector &final)
{
	IClientEntity* pLocal = hackManager.pLocal();

	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = targetPlayer;
	ray.Init(final + Vector(0, 0, 10), final);
	Interfaces::Trace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	final = tr.endpos;
	return final;
}

// Functionality
void CRageBot::DoAimbot(CUserCmd *pCmd, bool &bSendPacket) // Creds to encore1337 for getting it to work
{
	IClientEntity* pTarget = nullptr;
	IClientEntity* pLocal = hackManager.pLocal();
	Vector Start = pLocal->GetViewOffset() + pLocal->GetOrigin();
	bool FindNewTarget = true;
	//IsLocked = false;

	CSWeaponInfo* weapInfo = ((CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle()))->GetCSWpnData();

	// Don't aimbot with the knife..
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (pWeapon)
	{
		if (pWeapon->GetAmmoInClip() == 0 || !GameUtils::IsBallisticWeapon(pWeapon))
		{
			//TargetID = 0;
			//pTarget = nullptr;
			//HitBox = -1;
			return;
		}
	}
	else
		return;

	// Make sure we have a good target
	if (IsLocked && TargetID >= 0 && HitBox >= 0)
	{
		pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		if (pTarget  && TargetMeetsRequirements(pTarget))
		{
			HitBox = HitScan(pTarget);
			if (HitBox >= 0)
			{
				Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
				Vector View;
				Interfaces::Engine->GetViewAngles(View);
				float FoV = FovToPlayer(ViewOffset, View, pTarget, HitBox);
				if (FoV < Menu::Window.RageBotTab.AimbotFov.GetValue())
					FindNewTarget = false;
			}
		}
	}

	// Find a new target, apparently we need to
	if (FindNewTarget)
	{
		TargetID = 0;
		pTarget = nullptr;
		HitBox = -1;

		// Target selection type
		switch (Menu::Window.RageBotTab.TargetSelection.GetIndex())
		{
		case 0:
			TargetID = GetTargetCrosshair();
			break;
		case 1:
			TargetID = GetTargetDistance();
			break;
		case 2:
			TargetID = GetTargetHealth();
			break;
		}

		// Memes
		if (TargetID >= 0)
		{
			pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		}
		else
		{
			pTarget = nullptr;
			HitBox = -1;
		}
	}

	Globals::Target = pTarget;
	Globals::TargetID = TargetID;

	// If we finally have a good target
	if (TargetID >= 0 && pTarget)
	{
		// Get the hitbox to shoot at
		HitBox = HitScan(pTarget);

		if (!CanOpenFire())
			return;

		// Stop key
		int StopKey = Menu::Window.RageBotTab.AimbotStopKey.GetKey();
		if (StopKey >= 0 && GUI.GetKeyState(StopKey))
		{
			TargetID = -1;
			pTarget = nullptr;
			HitBox = -1;
			return;
		}

		float pointscale = Menu::Window.RageBotTab.TargetPointscale.GetValue() - 5.f; // Aim height
																					  //		float value = Menu::Window.RageBotTab.AccuracyHitchance.GetValue(); // Hitchance

		Vector Point;
		Vector AimPoint = GetHitboxPosition(pTarget, HitBox) + Vector(0, 0, pointscale);

		if (Menu::Window.RageBotTab.TargetMultipoint.GetState())
		{
			Point = BestPoint(pTarget, AimPoint);
		}
		else
		{
			Point = AimPoint;
		}

		// Lets open fire
		if (GameUtils::IsScopedWeapon(pWeapon) && !pWeapon->IsScoped() && Menu::Window.RageBotTab.AccuracyAutoScope.GetState()) // Autoscope
		{
			pCmd->buttons |= IN_ATTACK2;
		}
		else
		{
			if ((Menu::Window.RageBotTab.AccuracyHitchance.GetValue() * 1.5 <= hitchance(pLocal, pWeapon)) || Menu::Window.RageBotTab.AccuracyHitchance.GetValue() == 0 || *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() == 64)
			{
				if (AimAtPoint(pLocal, Point, pCmd, bSendPacket))
				{
					if (Menu::Window.RageBotTab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
					{
						pCmd->buttons |= IN_ATTACK;
						Shooted++;
					}
					else
					{
						return;
					}
				}
				else if (Menu::Window.RageBotTab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
				{
					pCmd->buttons |= IN_ATTACK;
					Shooted++;
				}
			}
		}

		if (IsAbleToShoot(pLocal) && pCmd->buttons & IN_ATTACK)
			Globals::Shots += 1;

		// Stop and Crouch
		if (TargetID >= 0 && pTarget)
		{
			if (Menu::Window.RageBotTab.AccuracyAutoStop.GetState())
			{
				pCmd->forwardmove = 0.f;
				pCmd->sidemove = 0.f;	
			}

		}
	}
}

bool CRageBot::TargetMeetsRequirements(IClientEntity* pEntity)
{
	// Is a valid player
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex())
	{
		// Entity Type checks
		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && Interfaces::Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			// Team Check
			if (pEntity->GetTeamNum() != hackManager.pLocal()->GetTeamNum() || Menu::Window.RageBotTab.TargetFriendlyFire.GetState())
			{
				// Spawn Check
				if (!pEntity->HasGunGameImmunity())
				{
					return true;
				}
			}
		}
	}

	// They must have failed a requirement
	return false;
}

float CRageBot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
{
	// Anything past 180 degrees is just going to wrap around
	CONST FLOAT MaxDegrees = 180.0f;

	// Get local angles
	Vector Angles = View;

	// Get local view / eye position
	Vector Origin = ViewOffSet;

	// Create and intiialize vectors for calculations below
	Vector Delta(0, 0, 0);
	//Vector Origin(0, 0, 0);
	Vector Forward(0, 0, 0);

	// Convert angles to normalized directional forward vector
	AngleVectors(Angles, &Forward);
	Vector AimPos = GetHitboxPosition(pEntity, aHitBox);
	// Get delta vector between our local eye position and passed vector
	VectorSubtract(AimPos, Origin, Delta);
	//Delta = AimPos - Origin;

	// Normalize our delta vector
	Normalize(Delta, Delta);

	// Get dot product between delta position and directional forward vectors
	FLOAT DotProduct = Forward.Dot(Delta);

	// Time to calculate the field of view
	return (acos(DotProduct) * (MaxDegrees / PI));
}

int CRageBot::GetTargetCrosshair()
{
	// Target selection
	int target = -1;
	float minFoV = Menu::Window.RageBotTab.AimbotFov.GetValue();

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetMaxEntities(); i++) //GetHighestEntityIndex()
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (fov < minFoV)
				{
					minFoV = fov;
					target = i;
				}
			}
		}
	}

	return target;
}

int CRageBot::GetTargetDistance()
{
	// Target selection
	int target = -1;
	int minDist = 99999;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetMaxEntities(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				Vector Difference = pLocal->GetOrigin() - pEntity->GetOrigin();
				int Distance = Difference.Length();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (Distance < minDist && fov < Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					minDist = Distance;
					target = i;
				}
			}
		}
	}

	return target;
}

int CRageBot::GetTargetHealth()
{
	// Target selection
	int target = -1;
	int minHealth = 101;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetMaxEntities(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				int Health = pEntity->GetHealth();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (Health < minHealth && fov < Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					minHealth = Health;
					target = i;
				}
			}
		}
	}

	return target;
}

int CRageBot::HitScan(IClientEntity* pEntity)
{
	IClientEntity* pLocal = hackManager.pLocal();
	std::vector<int> HitBoxesToScan;

	// Get the hitboxes to scan
#pragma region GetHitboxesToScan
	int HitScanMode = Menu::Window.RageBotTab.TargetHitscan.GetIndex();
	int huso = (pEntity->GetHealth());
	int health = Menu::Window.RageBotTab.BaimIfUnderXHealth.GetValue();
	bool AWall = Menu::Window.RageBotTab.AccuracyAutoWall.GetState();
	bool Multipoint = Menu::Window.RageBotTab.TargetMultipoint.GetState();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	
	if (Menu::Window.RageBotTab.AWPAtBody.GetState() && GameUtils::AWP(pWeapon))
	{
		HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
		HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
	}
	if (GameUtils::ZEUS(pWeapon))
	{
		HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
	}
	if (huso < health)
	{
		HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
		HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
	}
	else
	{
		if (HitScanMode = 0)
		{
			// No Hitscan, just a single hitbox
			switch (Menu::Window.RageBotTab.TargetHitbox.GetIndex())
			{
			case 0:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				break;
			case 1:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				break;
			case 2:
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				break;
			case 3:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				break;
			case 4:
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
				break;
			}
		}
		else
		{
			if (HitScanMode = 1)
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
			}
			if (HitScanMode = 2)
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
			}
			if (HitScanMode = 3)
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::NeckLower);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftHand);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightHand);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
			}
		}
	}
#pragma endregion Get the list of shit to scan

	// check hits
	// check hits
	for (auto HitBoxID : HitBoxesToScan)
	{
		if (AWall)
		{
			Vector Point = GetHitboxPosition(pEntity, HitBoxID);
			float Damage = 0.f;
			Color c = Color(255, 255, 255, 255);
			if (CanHit(Point, &Damage))
			{
				c = Color(0, 255, 0, 255);
				if (Damage >= Menu::Window.RageBotTab.AccuracyMinimumDamage.GetValue())
				{
						return HitBoxID;
				}
			}
		}
		else
		{
			if (GameUtils::IsVisible(hackManager.pLocal(), pEntity, HitBoxID))
				return HitBoxID;
		}
	}

	return -1;
}

void CRageBot::PositionAdjustment(CUserCmd* pCmd)
{
	static ConVar* cvar_cl_interp = Interfaces::CVar->FindVar("cl_interp");
	static ConVar* cvar_cl_updaterate = Interfaces::CVar->FindVar("cl_updaterate");
	static ConVar* cvar_sv_maxupdaterate = Interfaces::CVar->FindVar("sv_maxupdaterate");
	static ConVar* cvar_sv_minupdaterate = Interfaces::CVar->FindVar("sv_minupdaterate");
	static ConVar* cvar_cl_interp_ratio = Interfaces::CVar->FindVar("cl_interp_ratio");

	IClientEntity* pLocal = hackManager.pLocal();

	if (!pLocal)
		return;

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (!pWeapon)
		return;

	float cl_interp = cvar_cl_interp->GetFloat();
	int cl_updaterate = cvar_cl_updaterate->GetInt();
	int sv_maxupdaterate = cvar_sv_maxupdaterate->GetInt();
	int sv_minupdaterate = cvar_sv_minupdaterate->GetInt();
	int cl_interp_ratio = cvar_cl_interp_ratio->GetInt();

	if (sv_maxupdaterate <= cl_updaterate)
		cl_updaterate = sv_maxupdaterate;

	if (sv_minupdaterate > cl_updaterate)
		cl_updaterate = sv_minupdaterate;

	float new_interp = (float)cl_interp_ratio / (float)cl_updaterate;

	if (new_interp > cl_interp)
		cl_interp = new_interp;

	float flSimTime = pLocal->GetSimulationTime();
	float flOldAnimTime = pLocal->GetAnimTime();

	int iTargetTickDiff = (int)(0.5f + (flSimTime - flOldAnimTime) / Interfaces::Globals->interval_per_tick);

	int result = (int)floorf(TIME_TO_TICKS(cl_interp)) + (int)floorf(TIME_TO_TICKS(pLocal->GetSimulationTime()));

	if ((result - pCmd->tick_count) >= -50)
	{
		pCmd->tick_count = result;
	}
}

void CRageBot::DoNoRecoil(CUserCmd *pCmd)
{
	// Ghetto rcs shit, implement properly later
	IClientEntity* pLocal = hackManager.pLocal();
	if (pLocal)
	{
		Vector AimPunch = pLocal->localPlayerExclusive()->GetAimPunchAngle();
		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			pCmd->viewangles -= AimPunch * 2;
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
		}
	}
}

void CRageBot::HitChanceA(CUserCmd *pCmd)
{
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (!pWeapon)
		return;

	if (GameUtils::IsScout(pWeapon)) // Scout
	{
		Menu::Window.RageBotTab.AccuracyHitchance.SetValue(Menu::Window.SettingsTab.SettingsHitChanceScout.GetValue());
	}
	else if (GameUtils::AWP(pWeapon)) // AWP
	{
		Menu::Window.RageBotTab.AccuracyHitchance.SetValue(Menu::Window.SettingsTab.SettingsHitChanceAWP.GetValue());
	}
	else if (GameUtils::IsPistol(pWeapon) && !GameUtils::IsDeagle(pWeapon)) // Pistols
	{
		Menu::Window.RageBotTab.AccuracyHitchance.SetValue(Menu::Window.SettingsTab.SettingsHitChancePistols.GetValue());
	}
	else if (GameUtils::IsMP(pWeapon)) // MPs
	{
		Menu::Window.RageBotTab.AccuracyHitchance.SetValue(Menu::Window.SettingsTab.SettingsHitChanceMPs.GetValue());
	}
	else if (GameUtils::IsRifle(pWeapon)) // Rifles
	{
		Menu::Window.RageBotTab.AccuracyHitchance.SetValue(Menu::Window.SettingsTab.SettingsHitChanceRifles.GetValue());
	}
	else if (GameUtils::IsAuto(pWeapon)) // Auto
	{
		Menu::Window.RageBotTab.AccuracyHitchance.SetValue(Menu::Window.SettingsTab.SettingsHitChanceAuto.GetValue());
	}
	else if (GameUtils::IsDeagle(pWeapon)) // Deagle
	{
		Menu::Window.RageBotTab.AccuracyHitchance.SetValue(Menu::Window.SettingsTab.SettingsHitChanceDeagle.GetValue());
	}
	else if (GameUtils::ZEUS(pWeapon)) // Zeus
	{
		Menu::Window.RageBotTab.AccuracyHitchance.SetValue(Menu::Window.SettingsTab.SettingsHitChanceZeus.GetValue());
	}
	else if (!GameUtils::IsScout(pWeapon) && !GameUtils::AWP(pWeapon) && !GameUtils::IsPistol(pWeapon) && !GameUtils::IsDeagle(pWeapon) && !GameUtils::IsMP(pWeapon) && !GameUtils::IsRifle(pWeapon) && !GameUtils::IsAuto(pWeapon) && !GameUtils::ZEUS(pWeapon))
	{
		Menu::Window.RageBotTab.AccuracyHitchance.SetValue(0);
	}
}

void CRageBot::HitScanA(CUserCmd *pCmd)
{
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (!pWeapon)
		return;

	if (GameUtils::IsScout(pWeapon)) // Scout
	{
		switch (Menu::Window.SettingsTab.SettingsHitScanScout.GetIndex())
		{
		case 0:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(0);
			break;
		case 1:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(1);
			break;
		case 2:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(2);
			break;
		case 3:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(3);
			break;
		}
	}
	else if (GameUtils::IsPistol(pWeapon) && !GameUtils::IsDeagle(pWeapon)) // Pistols
	{
		switch (Menu::Window.SettingsTab.SettingsHitScan2Pistols.GetIndex())
		{
		case 0:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(0);
			break;
		case 1:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(1);
			break;
		case 2:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(2);
			break;
		case 3:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(3);
			break;
		}
	}
	else if (GameUtils::IsMP(pWeapon)) // MPs
	{
		switch (Menu::Window.SettingsTab.SettingsHitScan2MPs.GetIndex())
		{
		case 0:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(0);
			break;
		case 1:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(1);
			break;
		case 2:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(2);
			break;
		case 3:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(3);
			break;
		}
	}
	else if (GameUtils::IsRifle(pWeapon)) // Rifles
	{
		switch (Menu::Window.SettingsTab.SettingsHitScanRifles.GetIndex())
		{
		case 0:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(0);
			break;
		case 1:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(1);
			break;
		case 2:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(2);
			break;
		case 3:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(3);
			break;
		}
	}
	else if (GameUtils::IsAuto(pWeapon)) // Auto
	{
		switch (Menu::Window.SettingsTab.SettingsHitScanAuto.GetIndex())
		{
		case 0:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(0);
			break;
		case 1:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(1);
			break;
		case 2:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(2);
			break;
		case 3:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(3);
			break;
		}
	}
	else if (GameUtils::IsDeagle(pWeapon)) // Deagle
	{
		switch (Menu::Window.SettingsTab.SettingsHitScan2Deagle.GetIndex())
		{
		case 0:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(0);
			break;
		case 1:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(1);
			break;
		case 2:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(2);
			break;
		case 3:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(3);
			break;
		}
	}
	else if (!GameUtils::IsScout(pWeapon) && !GameUtils::IsPistol(pWeapon) && !GameUtils::IsDeagle(pWeapon) && !GameUtils::IsMP(pWeapon) && !GameUtils::IsRifle(pWeapon) && !GameUtils::IsAuto(pWeapon))
	{
		switch (Menu::Window.SettingsTab.SettingsHitScan2Other.GetIndex())
		{
		case 0:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(0);
			break;
		case 1:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(1);
			break;
		case 2:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(2);
			break;
		case 3:
			Menu::Window.RageBotTab.TargetHitscan.SelectIndex(3);
			break;
		}
	}
}

bool CRageBot::AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd, bool &bSendPacket)
{
	bool ReturnValue = false;
	// Get the full angles
	if (point.Length() == 0) return ReturnValue;

	Vector angles;
	Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();

	CalcAngle(src, point, angles);
	GameUtils::NormaliseViewAngle(angles);

	if (angles[0] != angles[0] || angles[1] != angles[1])
	{
		return ReturnValue;
	}


	IsLocked = true;
	//-----------------------------------------------

	// Aim Step Calcs
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	if (!IsAimStepping)
		LastAimstepAngle = LastAngle; // Don't just use the viewangs because you need to consider aa

	float fovLeft = FovToPlayer(ViewOffset, LastAimstepAngle, Interfaces::EntList->GetClientEntity(TargetID), 0);

	if (fovLeft > 25.0f && Menu::Window.RageBotTab.AimbotAimStep.GetState())
	{
		Vector AddAngs = angles - LastAimstepAngle;
		Normalize(AddAngs, AddAngs);
		AddAngs *= 25;
		LastAimstepAngle += AddAngs;
		GameUtils::NormaliseViewAngle(LastAimstepAngle);
		angles = LastAimstepAngle;
	}
	else
	{
		ReturnValue = true;
	}

	// Silent Aim
	if (Menu::Window.RageBotTab.AimbotSilentAim.GetState() && !Menu::Window.RageBotTab.AimbotPerfectSilentAim.GetState())
	{
		pCmd->viewangles = angles;
	}

	// Normal Aim
	if (!Menu::Window.RageBotTab.AimbotSilentAim.GetState() && !Menu::Window.RageBotTab.AimbotPerfectSilentAim.GetState())
	{
		Interfaces::Engine->SetViewAngles(angles);
	}

	// pSilent Aim 
	Vector Oldview = pCmd->viewangles;

	if (Menu::Window.RageBotTab.AimbotPerfectSilentAim.GetState())
	{
		static int ChokedPackets = -1;
		ChokedPackets++;

		if (ChokedPackets < 6)
		{
			bSendPacket = false;
			pCmd->viewangles = angles;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles = Oldview;
			ChokedPackets = -1;
			ReturnValue = false;
		}

		//pCmd->viewangles.z = 0;
	}

	return ReturnValue;
}

namespace AntiAims // CanOpenFire checks for fake anti aims?
{
	// Pitches

	void JitterPitch(CUserCmd *pCmd)
	{
		static bool up = true;
		if (up)
		{
			pCmd->viewangles.x = 45;
			up = !up;
		}
		else
		{
			pCmd->viewangles.x = 89;
			up = !up;
		}
	}

	void FakePitch(CUserCmd *pCmd, bool &bSendPacket)
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false;
			pCmd->viewangles.x = 89;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.x = 51;
			ChokedPackets = -1;
		}
	}

	/*void StaticJitter(CUserCmd *pCmd)
	{
		static bool down = true;
		if (down)
		{
			pCmd->viewangles.x = 179.0f;
			down = !down;
		}
		else
		{
			pCmd->viewangles.x = 89.0f;
			down = !down;
		}
	}*/

	// Yaws

	void FastSpin(CUserCmd *pCmd)
	{
		static int y2 = -179;
		int spinBotSpeedFast = 100;

		y2 += spinBotSpeedFast;

		if (y2 >= 179)
			y2 = -179;

		pCmd->viewangles.y = y2;
	}

	void FakeEdge(CUserCmd *pCmd, bool &bSendPacket)
	{
		IClientEntity* pLocal = hackManager.pLocal();

		Vector vEyePos = pLocal->GetOrigin() + pLocal->GetViewOffset();

		CTraceFilter filter;
		filter.pSkip = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		for (int y = 0; y < 360; y++)
		{
			Vector qTmp(10.0f, pCmd->viewangles.y, 0.0f);
			qTmp.y += y;

			if (qTmp.y > 180.0)
				qTmp.y -= 360.0;
			else if (qTmp.y < -180.0)
				qTmp.y += 360.0;

			GameUtils::NormaliseViewAngle(qTmp);

			Vector vForward;

			VectorAngles(qTmp, vForward);

			float fLength = (19.0f + (19.0f * sinf(DEG2RAD(10.0f)))) + 7.0f;
			vForward *= fLength;

			trace_t tr;

			Vector vTraceEnd = vEyePos + vForward;

			Ray_t ray;

			ray.Init(vEyePos, vTraceEnd);
			Interfaces::Trace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &tr);

			if (tr.fraction != 1.0f)
			{
				Vector angles;

				Vector vNegative = Vector(tr.plane.normal.x * -1.0f, tr.plane.normal.y * -1.0f, tr.plane.normal.z * -1.0f);

				VectorAngles(vNegative, angles);

				GameUtils::NormaliseViewAngle(angles);

				qTmp.y = angles.y;

				GameUtils::NormaliseViewAngle(qTmp);

				trace_t trLeft, trRight;

				Vector vLeft, vRight;
				VectorAngles(qTmp + Vector(0.0f, 30.0f, 0.0f), vLeft);
				VectorAngles(qTmp + Vector(0.0f, 30.0f, 0.0f), vRight);

				vLeft *= (fLength + (fLength * sinf(DEG2RAD(30.0f))));
				vRight *= (fLength + (fLength * sinf(DEG2RAD(30.0f))));

				vTraceEnd = vEyePos + vLeft;

				ray.Init(vEyePos, vTraceEnd);
				Interfaces::Trace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &trLeft);

				vTraceEnd = vEyePos + vRight;

				ray.Init(vEyePos, vTraceEnd);
				Interfaces::Trace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &trRight);

				if ((trLeft.fraction == 1.0f) && (trRight.fraction != 1.0f))
					qTmp.y -= 90.f;
				else if ((trLeft.fraction != 1.0f) && (trRight.fraction == 1.0f))
					qTmp.y += 90.f;

				if (qTmp.y > 180.0)
					qTmp.y -= 360.0;
				else if (qTmp.y < -180.0)
					qTmp.y += 360.0;

				pCmd->viewangles.y = qTmp.y;

				int offset = Menu::Window.RageBotTab.AntiAimOffset.GetValue();

				static int ChokedPackets = -1;
				ChokedPackets++;
				if (ChokedPackets < 1)
				{
					bSendPacket = false; // +=180?
				}
				else
				{
					bSendPacket = true;
					pCmd->viewangles.y -= offset;
					ChokedPackets = -1;
				}
				return;
			}
		}
		pCmd->viewangles.y += 360.0f;
	}

	void BackJitter(CUserCmd *pCmd)
	{
		
		switch ((int)rand % 5)
		{
		case 0:
			pCmd->viewangles.y -= 180;
		case 1:
			pCmd->viewangles.y -= 180;
		case 2:
			pCmd->viewangles.y -= 180;
		case 3:
			switch ((int)rand % 2)
			{
			case 0:
				pCmd->viewangles.y -= (int)rand % 45;
			case 1:
				pCmd->viewangles.y = (int)rand % 360;
			}
		case 4:
			pCmd->viewangles.y -= (int)rand % 30;
		}		
	}

	void Testing4(CUserCmd *pCmd, bool &bSendPacket)
	{
		static bool xjitter = false;
		static bool switchAA = false;

		if (GetAsyncKeyState('D'))
			switchAA = true;

		if (GetAsyncKeyState('A'))
			switchAA = false;

		static int jittertimer = 1;
		if (jittertimer == 1)
		{
			if (switchAA == false) {
				pCmd->viewangles.y -= 97;
				pCmd->viewangles.y += 1800000;
				xjitter = true;
			}
			if (switchAA == true) {
				pCmd->viewangles.y -= -97;
				pCmd->viewangles.y += -1800000;
				xjitter = true;
			}
		}
		else
			xjitter = false;
		if (bSendPacket)
		{
			if (jittertimer >= 1)
				jittertimer = -1;
			jittertimer++;
		}
	}

	void JitterLisp(CUserCmd *pCmd, bool &bSendPacket)
	{
		static bool xjitter = false;
		static int jittertimer = 1;
		if (jittertimer == 1)
		{
			pCmd->viewangles.y -= 97;
			pCmd->viewangles.y += 1800000;
			xjitter = true;
		}
		else
			xjitter = false;
		if (bSendPacket)
		{
			if (jittertimer >= 1)
				jittertimer = -1;
			jittertimer++;
		}
	}

	void SlowSpin(CUserCmd *pCmd)
	{
		static int y2 = -179;
		int spinBotSpeedFast = 5;

		y2 += spinBotSpeedFast;

		if (y2 >= 179)
			y2 = -179;

		pCmd->viewangles.y = y2;
	}

	void Dance(CUserCmd *pCmd)
	{
		static int pDance = 1;
		pDance += 45.0f;
		if (pDance > 100)
			pDance = 0.0f;
		else if (pDance > 75.f)
			pCmd->viewangles.x = -89.f;
		else if (pDance < 75.f)
			pCmd->viewangles.x = 89.f;
	}

	void Backwards(CUserCmd *pCmd)
	{
		int random = rand() % 100;

		// Small chance of starting fowards
		if (random > 80)
			// Look backwards
			//pCmd->viewangles.y -= 180;
			pCmd->viewangles.y = 180;
		else
		{
			int glitch = rand() % 2;
			if (glitch > 1)
				pCmd->viewangles.y = 170;
			else
				pCmd->viewangles.y = -170;
		}
		
	}

	void Donut(CUserCmd *pCmd)
	{
		static int timer = 0;
		static int rotation = 0;
		if (timer == 3) {
			rotation += 89;
			if (rotation > 179) {
				rotation = 0;
			}
			pCmd->viewangles.y += rotation;
			timer = 0;
		}
		else {
			timer++;
		}
	}

	void Testing1(CUserCmd *pCmd)
	{
		IClientEntity* pLocal = hackManager.pLocal();
		static int timer = 0;
		static int flip = 0;
		int A = 0;
		if (!(pLocal->GetFlags() & FL_ONGROUND))
		{
			static int timer2 = 0;
			static int rotation = 0;
			if (timer2 >= 0 && timer2 < 2) {
				pCmd->viewangles.y -= 175;
			}
			if (timer2 >= 2 && timer2 < 4) {
				pCmd->viewangles.y -= 180;
			}
			if (timer2 >= 4 && timer2 < 6) {
				pCmd->viewangles.y -= 170;
				timer2 = 0;
			}
			timer2++;
		}
		else
		{
			if (timer >= 0 && timer < 3) {
				if (flip = 0) {
					pCmd->viewangles.y -= 70;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -160;
				}
			}
			if (timer >= 3 && timer < 6) {
				if (flip = 0) {
					pCmd->viewangles.y -= 75;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -165;
				}
			}
			if (timer >= 4 && timer < 9) {
				if (flip = 0) {
					pCmd->viewangles.y -= 80;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -170;
				}
			}
			if (timer >= 9 && timer < 12) {
				if (flip = 0) {
					pCmd->viewangles.y -= 85;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -175;
				}
			}
			if (timer >= 12 && timer < 15) {
				if (flip = 0) {
					pCmd->viewangles.y -= 90;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -180;
				}
			}
			if (timer == 15) {
				timer = 0;
			}
			timer++;
		}
	}

	void Testing2(CUserCmd *pCmd)
	{
		IClientEntity* pLocal = hackManager.pLocal();
		static int timer = 0;
		static int flip = 0;
		int A = 0;
		if (!(pLocal->GetFlags() & FL_ONGROUND))
		{
			static int timer2 = 0;
			static int rotation = 0;
			if (timer2 >= 0 && timer2 < 2) {
				pCmd->viewangles.y -= 60;
			}
			if (timer2 >= 2 && timer2 < 4) {
				pCmd->viewangles.y -= 180;
			}
			if (timer2 >= 4 && timer2 < 6) {
				pCmd->viewangles.y -= 120;
			}
			if (timer2 >= 6 && timer2 < 8) {
				pCmd->viewangles.y -= 180;
			}
			if (timer2 >= 8 && timer2 < 10) {
				pCmd->viewangles.y -= 0;
				timer2 = 0;
			}
			timer2++;
		}
		else
		{
			if (timer >= 0 && timer < 3) {
				if (flip = 0) {
					pCmd->viewangles.y -= 0;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -180;
				}
			}
			if (timer >= 3 && timer < 6) {
				if (flip = 0) {
					pCmd->viewangles.y -= 20;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -160;
				}
			}
			if (timer >= 4 && timer < 9) {
				if (flip = 0) {
					pCmd->viewangles.y -= 40;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -140;
				}
			}
			if (timer >= 9 && timer < 12) {
				if (flip = 0) {
					pCmd->viewangles.y -= 60;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -120;
				}
			}
			if (timer >= 12 && timer < 15) {
				if (flip = 0) {
					pCmd->viewangles.y -= 80;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -100;
				}
			}
			if (timer >= 15 && timer < 18) {
				if (flip = 0) {
					pCmd->viewangles.y -= 100;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -80;
				}
			}
			if (timer >= 18 && timer < 21) {
				if (flip = 0) {
					pCmd->viewangles.y -= 120;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -60;
				}
			}
			if (timer >= 21 && timer < 24) {
				if (flip = 0) {
					pCmd->viewangles.y -= 140;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -40;
				}
			}
			if (timer >= 24 && timer < 27) {
				if (flip = 0) {
					pCmd->viewangles.y -= 160;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -20;
				}
			}
			if (timer >= 27 && timer < 30) {
				if (flip = 0) {
					pCmd->viewangles.y -= 180;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -0;
				}
			}
			if (timer == 30) {
				timer = 0;
			}
			timer++;
		}
	}

	void Testing3(CUserCmd *pCmd)
	{
		IClientEntity* pLocal = hackManager.pLocal();
		static int timer = 0;
		static int flip = 0;
		int A = 0;
		if (!(pLocal->GetFlags() & FL_ONGROUND))
		{
			static int timer2 = 0;
			static int rotation = 0;
			if (timer2 >= 0 && timer2 < 2) {
				pCmd->viewangles.y -= 175;
			}
			if (timer2 >= 2 && timer2 < 4) {
				pCmd->viewangles.y -= 10;
			}
			if (timer2 >= 4 && timer2 < 6) {
				pCmd->viewangles.y -= 160;
			}
			if (timer2 >= 6 && timer2 < 8) {
				pCmd->viewangles.y -= 0;
			}
			if (timer2 >= 8 && timer2 < 10) {
				pCmd->viewangles.y -= 142;
				timer2 = 0;
			}
			timer2++;
		}
		else
		{
			if (timer >= 0 && timer < 3) {
				if (flip = 0) {
					pCmd->viewangles.y -= 15;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -95;
				}
			}
			if (timer >= 3 && timer < 6) {
				if (flip = 0) {
					pCmd->viewangles.y -= 95;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -15;
				}
			}
			if (timer >= 4 && timer < 9) {
				if (flip = 0) {
					pCmd->viewangles.y -= 175;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -175;
				}
			}
			if (timer >= 9 && timer < 12) {
				if (flip = 0) {
					pCmd->viewangles.y -= 15;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -95;
				}
			}
			if (timer >= 12 && timer < 15) {
				if (flip = 0) {
					pCmd->viewangles.y -= 35;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -25;
				}
			}
			if (timer >= 15 && timer < 18) {
				if (flip = 0) {
					pCmd->viewangles.y -= 10;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -180;
				}
			}
			if (timer >= 18 && timer < 21) {
				if (flip = 0) {
					pCmd->viewangles.y -= 160;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -95;
				}
			}
			if (timer >= 21 && timer < 24) {
				if (flip = 0) {
					pCmd->viewangles.y -= 45;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -21;
				}
			}
			if (timer >= 24 && timer < 27) {
				if (flip = 0) {
					pCmd->viewangles.y -= 142;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -58;
				}
			}
			if (timer >= 27 && timer < 30) {
				if (flip = 0) {
					pCmd->viewangles.y -= 60;
				}
				if (flip = 1) {
					pCmd->viewangles.y -= -45;
				}
			}
			if (timer == 30) {
				timer = 0;
			}
			timer++;
		}
	}

	void Hypno(CUserCmd *pCmd)
	{
		static int y2 = -179;
		int spinBotSpeedFast = 3600;

		y2 += spinBotSpeedFast;

		if (pCmd->viewangles.y >= 100.0f)
		{
			pCmd->viewangles.y -= 170.0f;
		}
		if (pCmd->viewangles.y <= 200.0f)
		{
			pCmd->viewangles.y += 305.00f;
		}

		pCmd->viewangles.y = y2;
	}

	void FakeSideways(CUserCmd *pCmd, bool &bSendPacket)
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false;
			pCmd->viewangles.y += 90;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y -= 180;
			ChokedPackets = -1;
		}
	}
	void FakeSideLBY(CUserCmd *pCmd, bool &bSendPacket)
	{
		int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); ++i;
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		static bool isMoving;
		float PlayerIsMoving = abs(pLocal->GetVelocity().Length());
		if (PlayerIsMoving > 0.1) isMoving = true;
		else if (PlayerIsMoving <= 0.1) isMoving = false;

		int flip = (int)floorf(Interfaces::Globals->curtime / 1.1) % 2;
		static bool bFlipYaw;
		float flInterval = Interfaces::Globals->interval_per_tick;
		float flTickcount = pCmd->tick_count;
		float flTime = flInterval * flTickcount;
		if (std::fmod(flTime, 1) == 0.f)
			bFlipYaw = !bFlipYaw;

		if (PlayerIsMoving <= 0.1)
		{
			if (bSendPacket)
			{
				pCmd->viewangles.y += 180.f;
			}
			else
			{
				if (flip)
				{
					pCmd->viewangles.y += bFlipYaw ? 90.f : -90.f;

				}
				else
				{
					pCmd->viewangles.y -= hackManager.pLocal()->GetLowerBodyYaw() + bFlipYaw ? 90.f : -90.f;
				}
			}
		}
		else if (PlayerIsMoving > 0.1)
		{
			if (bSendPacket)
			{
				pCmd->viewangles.y += 180.f;
			}
			else
			{
				pCmd->viewangles.y += 90.f;
			}
		}
	}

	void fakelowerbody2(CUserCmd *pCmd, bool &bSendPacket)
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
        #define RandomInt(min, max) (rand() % (max - min + 1) + min)
		if (ChokedPackets < 1)
		{
			bSendPacket = false;
			pCmd->viewangles.y -= hackManager.pLocal()->GetLowerBodyYaw() + RandomInt(30, 61);
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y += hackManager.pLocal()->GetLowerBodyYaw() - RandomInt(180, 360);
			ChokedPackets = -1;
		}
	}

	float OldLBY;
	float LBYBreakerTimer;
	float LastLBYUpdateTime;
	bool bSwitch;
	float CurrentVelocity(IClientEntity* LocalPlayer)
	{
		int vel = LocalPlayer->GetVelocity().Length2D();
		return vel;
	}
	float GetLatency()
	{
		INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
		if (nci)
		{
			float Latency = nci->GetAvgLatency(FLOW_OUTGOING) + nci->GetAvgLatency(FLOW_INCOMING);
			return Latency;
		}
		else
		{
			return 0.0f;
		}
	}
	bool NextLBYUpdate()
	{
		IClientEntity* LocalPlayer = hackManager.pLocal();

		float flServerTime = (float)(LocalPlayer->GetTickBase()  * Interfaces::Globals->interval_per_tick);


		if (OldLBY != LocalPlayer->GetLowerBodyYaw())
		{
			LBYBreakerTimer++;
			OldLBY = LocalPlayer->GetLowerBodyYaw();
			bSwitch = !bSwitch;
			LastLBYUpdateTime = flServerTime;
		}

		if (CurrentVelocity(LocalPlayer) > 0.5)
		{
			LastLBYUpdateTime = flServerTime;
			return false;
		}

		if ((LastLBYUpdateTime + 1 - (GetLatency() * 2) < flServerTime) && (LocalPlayer->GetFlags() & FL_ONGROUND))
		{
			if (LastLBYUpdateTime + 1.1 - (GetLatency() * 2) < flServerTime)
			{
				LastLBYUpdateTime += 1.1;
			}
			return true;
		}
		return false;
	}

	void DoLBYBreakReal(CUserCmd * pCmd, bool& bSendPacket)
	{
		if (!bSendPacket)
		{
			if (NextLBYUpdate())
				pCmd->viewangles.y -= 50;
			else
				pCmd->viewangles.y += 130;
		}
		else
		{
			if (NextLBYUpdate())
				pCmd->viewangles.y += 14;
			else
				pCmd->viewangles.y -= 67;
		}
	}

	void DoLBYBreak(CUserCmd * pCmd, bool& bSendPacket)
	{
		if (!bSendPacket)
		{
			if (NextLBYUpdate())
				pCmd->viewangles.y += 57;
			else
				pCmd->viewangles.y -= 89;
		}
		else
		{
			if (NextLBYUpdate())
				pCmd->viewangles.y -= 35;
			else
				pCmd->viewangles.y += 69;
		}
	}

	void Jitter(CUserCmd *pCmd)
	{
		static int jitterangle = 0;

		if (jitterangle <= 1)
		{
			pCmd->viewangles.y += 90;
		}
		else if (jitterangle > 1 && jitterangle <= 3)
		{
			pCmd->viewangles.y -= 90;
		}

		int re = rand() % 4 + 1;


		if (jitterangle <= 1)
		{
			if (re == 4)
				pCmd->viewangles.y += 180;
			jitterangle += 1;
		}
		else if (jitterangle > 1 && jitterangle <= 3)
		{
			if (re == 4)
				pCmd->viewangles.y -= 180;
			jitterangle += 1;
		}
		else
		{
			jitterangle = 0;
		}
	}

	void FakeStatic(CUserCmd *pCmd, bool &bSendPacket)
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false;
			static int y2 = -179;
			int spinBotSpeedFast = 360.0f / 1.618033988749895f;;

			y2 += spinBotSpeedFast;

			if (y2 >= 179)
				y2 = -179;

			pCmd->viewangles.y = y2;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y -= 180;
			ChokedPackets = -1;
		}
	}

	void FakeForward(CUserCmd *pCmd, bool &bSendPacket)
	{
		static int jitterangle = 0;

		if (jitterangle <= 1)
		{
			pCmd->viewangles.y += 0;
		}
		else if (jitterangle > 1 && jitterangle <= 3)
		{
			pCmd->viewangles.y += 0;
		}

		static int iChoked = -1;
		iChoked++;
		if (iChoked < 1)
		{
			bSendPacket = false;
			if (jitterangle <= 1)
			{
				pCmd->viewangles.y += 0;
				jitterangle += 1;
			}
			else if (jitterangle > 1 && jitterangle <= 3)
			{
				pCmd->viewangles.y -= 0;
				jitterangle += 1;
			}
			else
			{
				jitterangle = 0;
			}
		}
		else
		{
			bSendPacket = true;
			iChoked = -1;
		}
	}


	void FakeHead(CUserCmd *pCmd)
	{
		static bool ySwitch = false;
		int veloc = hackManager.pLocal()->GetVelocity().Length();
		if (ySwitch)
			pCmd->viewangles.y -= 90 * veloc;
		else
			pCmd->viewangles.y += 90 * veloc;

		ySwitch = !ySwitch;
	}

	void FakeTroll(CUserCmd *pCmd)
	{
		int m_iRand = rand() % 90 + -90;
		pCmd->viewangles.y = m_iRand;
	}


	void TJitter(CUserCmd *pCmd)
	{
		static bool Turbo = true;
		if (Turbo)
		{
			pCmd->viewangles.y -= 90;
			Turbo = !Turbo;
		}
		else
		{
			pCmd->viewangles.y += 90;
			Turbo = !Turbo;
		}
	}

	void TFake(CUserCmd *pCmd, bool &bSendPacket)
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false;
			pCmd->viewangles.y = -90;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y = 90;
			ChokedPackets = -1;
		}
	}

	void FakeJitter(CUserCmd* pCmd, bool &bSendPacket)
	{
		static int jitterangle = 0;

		if (jitterangle <= 1)
		{
			pCmd->viewangles.y += 90;
		}
		else if (jitterangle > 1 && jitterangle <= 3)
		{
			pCmd->viewangles.y += 180;
		}

		static int iChoked = -1;
		iChoked++;
		if (iChoked < 1)
		{
			bSendPacket = false;
			if (jitterangle <= 1)
			{
				pCmd->viewangles.y += 45;
				jitterangle += 1;
			}
			else if (jitterangle > 1 && jitterangle <= 3)
			{
				pCmd->viewangles.y -= 45;
				jitterangle += 1;
			}
			else
			{
				jitterangle = 0;
			}
		}
		else
		{
			bSendPacket = true;
			iChoked = -1;
		}
	}

	void Static(CUserCmd *pCmd)
	{
		static bool aa1 = false;
		aa1 = !aa1;
		if (aa1)
		{
			static bool turbo = false;
			turbo = !turbo;
			if (turbo)
			{
				pCmd->viewangles.y -= 90;
			}
			else
			{
				pCmd->viewangles.y += 90;
			}
		}
		else
		{
			pCmd->viewangles.y -= 180;
		}
	}

	void fakelowerbody(CUserCmd *pCmd, bool &bSendPacket)
	{
		static bool f_flip = true;
		f_flip = !f_flip;

		if (f_flip)
		{
			pCmd->viewangles.y -= hackManager.pLocal()->GetLowerBodyYaw() + 90.00f;
			bSendPacket = false;
		}
		else if (!f_flip)
		{
			pCmd->viewangles.y += hackManager.pLocal()->GetLowerBodyYaw() - 90.00f;
			bSendPacket = true;
		}
	}

	void realandfakelowerbody(CUserCmd *pCmd, bool &bSendPacket)
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = true;
			pCmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + rand() % 180 - rand() % 50;
		}
		else
		{
			bSendPacket = false;
			pCmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + rand() % 180 - rand() % 50;
			ChokedPackets = -1;
		}
	}

	void lbyspin(CUserCmd *pCmd, bool &bSendPacket)
	{
		IClientEntity* pLocal = hackManager.pLocal();
		static int skeet = 179;
		int SpinSpeed = 100;
		static int ChokedPackets = -1;
		ChokedPackets++;
		skeet += SpinSpeed;

		if
			(pCmd->command_number % 9)
		{
			bSendPacket = true;
			if (skeet >= pLocal->GetLowerBodyYaw() + 180);
			skeet = pLocal->GetLowerBodyYaw() - 0;
			ChokedPackets = -1;
		}
		else if
			(pCmd->command_number % 9)
		{
			bSendPacket = false;
			pCmd->viewangles.y += 179;
			ChokedPackets = -1;
		}
		pCmd->viewangles.y = skeet;
	}

	void AimAtTarget(CUserCmd *pCmd)
	{
		IClientEntity* pLocal = hackManager.pLocal();

		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

		if (!pLocal || !pWeapon)
			return;

		Vector eye_position = pLocal->GetEyePosition();

		float best_dist = pWeapon->GetCSWpnData()->flRange;

		IClientEntity* target = Interfaces::EntList->GetClientEntity(Globals::TargetID);

		if (target == NULL)
			return;

		if (target == pLocal)
			return;

		if ((target->GetTeamNum() == pLocal->GetTeamNum()) || target->IsDormant() || !target->IsAlive() || target->GetHealth() <= 0)
			return;

		Vector target_position = target->GetEyePosition();

		float temp_dist = eye_position.DistTo(target_position);

		if (best_dist > temp_dist)
		{
			best_dist = temp_dist;
			CalcAngle(eye_position, target_position, pCmd->viewangles);
		}
	}

	void AntiResolver(CUserCmd *pCmd)
	{
		static bool antiResolverFlip = false;
		if (pCmd->viewangles.y == hackManager.pLocal()->GetLowerBodyYaw())
		{
			if (antiResolverFlip)
				pCmd->viewangles.y += 60.f;
			else
				pCmd->viewangles.y -= 60.f;

			antiResolverFlip = !antiResolverFlip;
		}
	}

	void EdgeDetect(CUserCmd* pCmd, bool &bSendPacket)
	{
		//Ray_t ray;
		//trace_t tr;

		IClientEntity* pLocal = hackManager.pLocal();

		CTraceFilter traceFilter;
		traceFilter.pSkip = pLocal;

		bool bEdge = false;

		Vector angle;
		Vector eyePos = pLocal->GetOrigin() + pLocal->GetViewOffset();

		for (float i = 0; i < 360; i++)
		{
			Vector vecDummy(10.f, pCmd->viewangles.y, 0.f);
			vecDummy.y += i;

			Vector forward = vecDummy.Forward();

			//vecDummy.NormalizeInPlace();

			float flLength = ((16.f + 3.f) + ((16.f + 3.f) * sin(DEG2RAD(10.f)))) + 7.f;
			forward *= flLength;

			Ray_t ray;
			CGameTrace tr;

			ray.Init(eyePos, (eyePos + forward));
			Interfaces::Trace->EdgeTraceRay(ray, traceFilter, tr, true);

			if (tr.fraction != 1.0f)
			{
				Vector negate = tr.plane.normal;
				negate *= -1;

				Vector vecAng = negate.Angle();

				vecDummy.y = vecAng.y;

				//vecDummy.NormalizeInPlace();
				trace_t leftTrace, rightTrace;

				Vector left = (vecDummy + Vector(0, 45, 0)).Forward(); // or 45
				Vector right = (vecDummy - Vector(0, 45, 0)).Forward();

				left *= (flLength * cosf(rad(30)) * 2); //left *= (len * cosf(rad(30)) * 2);
				right *= (flLength * cosf(rad(30)) * 2); // right *= (len * cosf(rad(30)) * 2);

				ray.Init(eyePos, (eyePos + left));
				Interfaces::Trace->EdgeTraceRay(ray, traceFilter, leftTrace, true);

				ray.Init(eyePos, (eyePos + right));
				Interfaces::Trace->EdgeTraceRay(ray, traceFilter, rightTrace, true);

				if ((leftTrace.fraction == 1.f) && (rightTrace.fraction != 1.f))
				{
					vecDummy.y -= 45; // left
				}
				else if ((leftTrace.fraction != 1.f) && (rightTrace.fraction == 1.f))
				{
					vecDummy.y += 45; // right     
				}

				angle.y = vecDummy.y;
				angle.y += 360;
				bEdge = true;
			}
		}
	}
}

bool EdgeAntiAim(IClientEntity* pLocalBaseEntity, CUserCmd* cmd, float flWall, float flCornor)
{
	Ray_t ray;
	trace_t tr;

	CTraceFilter traceFilter;
	traceFilter.pSkip = pLocalBaseEntity;

	auto bRetVal = false;
	auto vecCurPos = pLocalBaseEntity->GetEyePosition();

	for (float i = 0; i < 360; i++)
	{
		Vector vecDummy(10.f, cmd->viewangles.y, 0.f);
		vecDummy.y += i;

		NormalizeVector(vecDummy);

		Vector vecForward;
		AngleVectors2(vecDummy, vecForward);

		auto flLength = ((16.f + 3.f) + ((16.f + 3.f) * sin(DEG2RAD(10.f)))) + 7.f;
		vecForward *= flLength;

		ray.Init(vecCurPos, (vecCurPos + vecForward));
		Interfaces::Trace->TraceRay(ray, MASK_SHOT, (CTraceFilter *)&traceFilter, &tr);

		if (tr.fraction != 1.0f)
		{
			Vector qAngles;
			auto vecNegate = tr.plane.normal;

			vecNegate *= -1.f;
			VectorAngles2(vecNegate, qAngles);

			vecDummy.y = qAngles.y;

			NormalizeVector(vecDummy);
			trace_t leftTrace, rightTrace;

			Vector vecLeft;
			AngleVectors2(vecDummy + Vector(0.f, 30.f, 0.f), vecLeft);

			Vector vecRight;
			AngleVectors2(vecDummy - Vector(0.f, 30.f, 0.f), vecRight);

			vecLeft *= (flLength + (flLength * sin(DEG2RAD(30.f))));
			vecRight *= (flLength + (flLength * sin(DEG2RAD(30.f))));

			ray.Init(vecCurPos, (vecCurPos + vecLeft));
			Interfaces::Trace->TraceRay(ray, MASK_SHOT, (CTraceFilter*)&traceFilter, &leftTrace);

			ray.Init(vecCurPos, (vecCurPos + vecRight));
			Interfaces::Trace->TraceRay(ray, MASK_SHOT, (CTraceFilter*)&traceFilter, &rightTrace);

			if ((leftTrace.fraction == 1.f) && (rightTrace.fraction != 1.f))
				vecDummy.y -= flCornor; // left
			else if ((leftTrace.fraction != 1.f) && (rightTrace.fraction == 1.f))
				vecDummy.y += flCornor; // right			

			cmd->viewangles.y = vecDummy.y;
			cmd->viewangles.y -= flWall;
			cmd->viewangles.x = 89.f;
			bRetVal = true;
		}
	}
	return bRetVal;
}

// AntiAim
void CRageBot::DoAntiAim(CUserCmd *pCmd, bool &bSendPacket) // pCmd->viewangles.y = 0xFFFFF INT_MAX or idk
{
	IClientEntity* pLocal = hackManager.pLocal();

	if ((pCmd->buttons & IN_USE) || pLocal->GetMoveType() == MOVETYPE_LADDER)
		return;

	// If the aimbot is doing something don't do anything
	if ((IsAimStepping || pCmd->buttons & IN_ATTACK) && !Menu::Window.RageBotTab.AimbotPerfectSilentAim.GetState())
		return;

	// Weapon shit
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (pWeapon)
	{
		CSWeaponInfo* pWeaponInfo = pWeapon->GetCSWpnData();
		// Knives or grenades
		if (!GameUtils::IsBallisticWeapon(pWeapon))
		{
			if (Menu::Window.RageBotTab.AntiAimKnife.GetState())
			{
				if (!CanOpenFire() || pCmd->buttons & IN_ATTACK2)
					return;
			}
			else
			{
				return;
			}
		}
	}

	if (Menu::Window.RageBotTab.AntiAimEdge.GetState()) {
		auto bEdge = EdgeAntiAim(hackManager.pLocal(), pCmd, 360.f, 45.f);
		if (bEdge)
			return;
	}

	if (Menu::Window.RageBotTab.AntiAimTarget.GetState())
	{
		AntiAims::AimAtTarget(pCmd);
	}

	// Don't do antiaim
	// if (DoExit) return;

	// Anti-Aim Pitch
	switch (Menu::Window.RageBotTab.AntiAimPitch.GetIndex()) // Magic pitch is 69.69?
	{
	case 0:
		// No Pitch AA
		break;
	case 1:
		// Down
		pCmd->viewangles.x = 89.0f;
		break;
	case 2:
		// Up
		pCmd->viewangles.x = -89.0f;
		break;
	case 3:
		// Half Down
		pCmd->viewangles.x = 51.f;
		break;
	case 4:
		// SMAC / Casual safe
		AntiAims::JitterPitch(pCmd);
		break;
	case 5:
		// Jitter
		if (Menu::Window.SettingsTab.OtherSettingsAntiUntrusted.GetState()) {
			Menu::Window.RageBotTab.AntiAimPitch.SelectIndex(0);
			Beep(500, 1000);
		}
		else {
			pCmd->viewangles.x = 179.0f;
		}
		break;
	case 6:
		// Fake Pitch
		if (Menu::Window.SettingsTab.OtherSettingsAntiUntrusted.GetState()) {
			Menu::Window.RageBotTab.AntiAimPitch.SelectIndex(0);
			Beep(500, 1000);
		}
		else {
			pCmd->viewangles.x = -181.f;
		}
		break;
	case 7:
		// Static Down
		if (Menu::Window.SettingsTab.OtherSettingsAntiUntrusted.GetState()) {
			Menu::Window.RageBotTab.AntiAimPitch.SelectIndex(0);
			Beep(500, 1000);
		}
		else {
			pCmd->viewangles.x = 1800089.0f;
		}
		break;
	case 8:
		// Static Jitter
		if (Menu::Window.SettingsTab.OtherSettingsAntiUntrusted.GetState()) {
			Menu::Window.RageBotTab.AntiAimPitch.SelectIndex(0);
			Beep(500, 1000);
		}
		else {
			pCmd->viewangles.y = -1800089.0f;
		}
		break;
	case 9:
		// Emotion
		pCmd->viewangles.x = 86.792510943;
		break;
	case 10:
		// Dance
		AntiAims::Dance(pCmd);
		break;
	case 11:
		// Custom
		pCmd->viewangles.x += Menu::Window.RageBotTab.CustomPitch.GetValue();
	}

		//Anti-Aim Yaw

	if (hackManager.pLocal()->GetVelocity().Length() > 0 && (hackManager.pLocal()->GetFlags() & FL_ONGROUND) || Menu::Window.RageBotTab.AntiAimYawJumping.GetIndex() == 0 && hackManager.pLocal()->GetVelocity().Length() > 0 && !(hackManager.pLocal()->GetFlags() & FL_ONGROUND))
	{
		switch (Menu::Window.RageBotTab.AntiAimYawMoving.GetIndex())
		{
		case 0:
			// No Yaw AA
			break;
		case 1:
			// Fake Inverse
			AntiAims::FakeEdge(pCmd, bSendPacket);
			break;
		case 2:
			// Fake Sideways
			AntiAims::FakeSideways(pCmd, bSendPacket);
			break;
		case 3:
			// Fake Static
			AntiAims::FakeStatic(pCmd, bSendPacket);
			break;
		case 4:
			// Fake Jitter
			AntiAims::fakelowerbody2(pCmd, bSendPacket);
			break;
		case 5:
			// T Inverse
			AntiAims::fakelowerbody(pCmd, bSendPacket);
			break;
		case 6:
			// Real & Feke Lowerbody
			AntiAims::realandfakelowerbody(pCmd, bSendPacket);
			break;
		case 7:
			AntiAims::FakeSideLBY(pCmd, bSendPacket);
			break;
		case 8:
			// Fake Forward
			AntiAims::FakeForward(pCmd, bSendPacket);
			break;
		case 9:
			// Fake Head
			AntiAims::FakeHead(pCmd);
			break;
		case 10:
			AntiAims::FakeTroll(pCmd);
			break;
		case 11:
			// Jitter
			AntiAims::SlowSpin(pCmd);
			break;
		case 12:
			// T Jitter
			AntiAims::TJitter(pCmd);
			break;
		case 13:
			// Back Jitter
			AntiAims::BackJitter(pCmd);
			break;
		case 14:
			// JitterLisp
			if (Menu::Window.SettingsTab.OtherSettingsAntiUntrusted.GetState()) {
				Menu::Window.RageBotTab.AntiAimYawMoving.SelectIndex(0);
				Beep(500, 1000);
			}
			else {
				AntiAims::JitterLisp(pCmd, bSendPacket);
			}
			break;
		case 15:
			// T Inverse
			AntiAims::Backwards(pCmd);
			break;
		case 16:
			// Donut
			AntiAims::Donut(pCmd);
			break;
		case 17:
			// Hypno
			AntiAims::Hypno(pCmd);
			break;
		case 18:
			// LbySpin
			AntiAims::lbyspin(pCmd, bSendPacket);
			break;
		case 19:
			// LBY Break
			AntiAims::DoLBYBreakReal(pCmd, bSendPacket);
			break;
		case 20:
			// Testing 1
			AntiAims::Testing1(pCmd);
			break;
		case 21:
			// Testing 2
			AntiAims::Testing2(pCmd);
			break;
		case 22:
			// Testing 3
			AntiAims::Testing3(pCmd);
			break;
		case 23:
			// JitterLisp
			AntiAims::Testing4(pCmd, bSendPacket);
			break;
		}
	}
	if (hackManager.pLocal()->GetVelocity().Length() > 0 && !(hackManager.pLocal()->GetFlags() & FL_ONGROUND))
	{
		switch (Menu::Window.RageBotTab.AntiAimYawJumping.GetIndex())
		{
		case 0:
			// No Yaw AA
			break;
		case 1:
			// Fake Inverse
			AntiAims::FakeEdge(pCmd, bSendPacket);
			break;
		case 2:
			// Fake Sideways
			AntiAims::FakeSideways(pCmd, bSendPacket);
			break;
		case 3:
			// Fake Static
			AntiAims::FakeStatic(pCmd, bSendPacket);
			break;
		case 4:
			// Fake Jitter
			AntiAims::fakelowerbody2(pCmd, bSendPacket);
			break;
		case 5:
			// T Inverse
			AntiAims::fakelowerbody(pCmd, bSendPacket);
			break;
		case 6:
			// Real & Feke Lowerbody
			AntiAims::realandfakelowerbody(pCmd, bSendPacket);
			break;
		case 7:
			AntiAims::FakeSideLBY(pCmd, bSendPacket);
			break;
		case 8:
			// Fake Forward
			AntiAims::FakeForward(pCmd, bSendPacket);
			break;
		case 9:
			// Fake Head
			AntiAims::FakeHead(pCmd);
			break;
		case 10:
			AntiAims::FakeTroll(pCmd);
			break;
		case 11:
			// Jitter
			AntiAims::SlowSpin(pCmd);
			break;
		case 12:
			// T Jitter
			AntiAims::TJitter(pCmd);
			break;
		case 13:
			// Back Jitter
			AntiAims::BackJitter(pCmd);
			break;
		case 14:
			// JitterLisp
			if (Menu::Window.SettingsTab.OtherSettingsAntiUntrusted.GetState()) {
				Menu::Window.RageBotTab.AntiAimYawJumping.SelectIndex(0);
				Beep(500, 1000);
			}
			else {
				AntiAims::JitterLisp(pCmd, bSendPacket);
			}
			break;
		case 15:
			// T Inverse
			AntiAims::Backwards(pCmd);
			break;
		case 16:
			// Donut
			AntiAims::Donut(pCmd);
			break;
		case 17:
			// Hypno
			AntiAims::Hypno(pCmd);
			break;
		case 18:
			// LbySpin
			AntiAims::lbyspin(pCmd, bSendPacket);
			break;
		case 19:
			// LBY Break
			AntiAims::DoLBYBreakReal(pCmd, bSendPacket);
			break;
		case 20:
			// Testing 1
			AntiAims::Testing1(pCmd);
			break;
		case 21:
			// Testing 2
			AntiAims::Testing2(pCmd);
			break;
		case 22:
			// Testing 3
			AntiAims::Testing3(pCmd);
			break;
		case 23:
			// JitterLisp
			AntiAims::Testing4(pCmd, bSendPacket);
			break;
		}
	}
	if (hackManager.pLocal()->GetVelocity().Length() < 10 && (hackManager.pLocal()->GetFlags() & FL_ONGROUND) || Menu::Window.RageBotTab.AntiAimYawMoving.GetIndex() == 0 && hackManager.pLocal()->GetVelocity().Length() > 0 && (hackManager.pLocal()->GetFlags() & FL_ONGROUND) || Menu::Window.RageBotTab.AntiAimYawJumping.GetIndex() == 0 && hackManager.pLocal()->GetVelocity().Length() > 0 && !(hackManager.pLocal()->GetFlags() & FL_ONGROUND))
	{
		switch (Menu::Window.RageBotTab.AntiAimYaw.GetIndex())
		{
		case 0:
			// No Yaw AA
			break;
		case 1:
			// Fake Inverse
			AntiAims::FakeEdge(pCmd, bSendPacket);
			break;
		case 2:
			// Fake Sideways
			AntiAims::FakeSideways(pCmd, bSendPacket);
			break;
		case 3:
			// Fake Static
			AntiAims::FakeStatic(pCmd, bSendPacket);
			break;
		case 4:
			// Fake Jitter
			AntiAims::fakelowerbody2(pCmd, bSendPacket);
			break;
		case 5:
			// T Inverse
			AntiAims::fakelowerbody(pCmd, bSendPacket);
			break;
		case 6:
			// Real & Feke Lowerbody
			AntiAims::realandfakelowerbody(pCmd, bSendPacket);
			break;
		case 7:
			AntiAims::FakeSideLBY(pCmd, bSendPacket);
			break;
		case 8:
			// Fake Forward
			AntiAims::FakeForward(pCmd, bSendPacket);
			break;
		case 9:
			// Fake Head
			AntiAims::FakeHead(pCmd);
			break;
		case 10:
			AntiAims::FakeTroll(pCmd);
			break;
		case 11:
			// Jitter
			AntiAims::SlowSpin(pCmd);
			break;
		case 12:
			// T Jitter
			AntiAims::TJitter(pCmd);
			break;
		case 13:
			// Back Jitter
			AntiAims::BackJitter(pCmd);
			break;
		case 14:
			// JitterLisp
			if (Menu::Window.SettingsTab.OtherSettingsAntiUntrusted.GetState()) {
				Menu::Window.RageBotTab.AntiAimYaw.SelectIndex(0);
				Beep(500, 1000);
			}
			else {
				AntiAims::JitterLisp(pCmd, bSendPacket);
			}
			break;
		case 15:
			// T Inverse
			AntiAims::Backwards(pCmd);
			break;
		case 16:
			// Donut
			AntiAims::Donut(pCmd);
			break;
		case 17:
			// Hypno
			AntiAims::Hypno(pCmd);
			break;
		case 18:
			// LbySpin
			AntiAims::lbyspin(pCmd, bSendPacket);
			break;
		case 19:
			// LBY Break
			AntiAims::DoLBYBreakReal(pCmd, bSendPacket);
			break;
		case 20:
			// Testing 1
			AntiAims::Testing1(pCmd);
			break;
		case 21:
			// Testing 2
			AntiAims::Testing2(pCmd);
			break;
		case 22:
			// Testing 3
			AntiAims::Testing3(pCmd);
			break;
		case 23:
			// JitterLisp
			AntiAims::Testing4(pCmd, bSendPacket);
			break;
		}
	}
	/*switch (Menu::Window.RageBotTab.AntiAimZeta.GetIndex())
	{
	case 0:
		// No Zeta AA
		break;
	case 1:
		if (Menu::Window.SettingsTab.OtherSettingsAntiUntrusted.GetState()) {
			Menu::Window.RageBotTab.AntiAimZeta.SelectIndex(0);
			Beep(500, 1000);
		}
		else {
			// Sideways
			if (!bSendPacket)
				pCmd->viewangles.z += 90;
			if (bSendPacket)
				pCmd->viewangles.z += 45;
		}
		break;
	case 2:
		// Static
		if (Menu::Window.SettingsTab.OtherSettingsAntiUntrusted.GetState()) {
			Menu::Window.RageBotTab.AntiAimZeta.SelectIndex(0);
			Beep(500, 1000);
		}
		else {
			pCmd->viewangles.z += 180.f;
		}
		break;
	}*/
	if (Menu::Window.RageBotTab.AntiResolver.GetState())
	{
		AntiAims::AntiResolver(pCmd);
	}
}

