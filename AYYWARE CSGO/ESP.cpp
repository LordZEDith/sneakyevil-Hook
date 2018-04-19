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

#include "ESP.h"
#include "Interfaces.h"
#include "NadePredict.h"
#include "RenderManager.h"
#include "nosmoke.h"
#include "string.h"
#include "EdgyLagComp.h"
#include "Resolver.h"
#include "GlowManager.h"

#define charenc( s ) ( s )

extern float AAlineLBYAngle;
extern float AAlineRealAngle;
extern float AAlineFakeAngle;
int c4_defuse = 0;
float c4casovac = 0;
extern int resolvermode;

int fgdgdfgdfgfghtrddgvterdf[512]{ 22315,7796,12712,10228,18428,5217,7094,29789,19578,16778,31171,26062,13481,6366,4915,16880,3265,29097,5163,24771,28816,2403,20044,9240,22643,84,6046,19917,31004,24657,11448,5086,2246,29380,4197,10856,1994,28601,24539,25362,29044,2687,32048,30070,11614,21449,13296,19360,28863,31775,6314,9509,9153,2066,29861,19912,20518,3471,20352,18343,31938,19944,31552,27687,5330,26085,23815,16867,2168,26826,14885,7431,4558,12568,28494,26128,3122,22741,30393,1193,19555,19452,23820,31996,18343,1938,28700,180,32286,17052,23104,31289,8249,6195,26190,9099,3902,4360,24739,18296,30400,29196,17191,2037,16889,31650,4951,29351,6338,3732,16790,30544,5845,19158,31555,12485,28891,32323,9344,3391,30151,23635,17347,17883,3722,16362,31948,31129,19144,19395,22226,31528,30768,12096,3560,23126,2260,21783,15106,19460,29754,32115,26609,28554,261,25326,25444,21858,7406,13705,12974,4345,27783,21348,20911,6765,11848,32075,31082,527,32468,6588,10207,19709,14272,18029,32543,26515,15598,8429,10224,7108,17217,22651,6404,30802,29792,21560,17622,11162,23685,10090,23723,5439,6420,17880,3744,9419,23952,29561,11277,2952,23467,14578,19758,23165,21908,6899,14281,14999,25550,31695,6260,31743,3480,5866,19895,5132,14944,22876,9739,23054,14452,19919,9609,22378,15632,4767,13289,29159,14284,23123,2563,18803,29918,7271,16407,2638,2892,16005,8894,22893,7279,2622,18378,24757,3830,24746,1958,554,30752,31379,27242,1075,19821,20990,21266,3547,4969,30147,7533,17140,26377,17889,714,2427,29131,20223,1825,28975,22225,17256,2239,31706,13981,19612,15775,11830,1541,17618,18547,23652,2809,12734,31173,15671,28648,1487,31749,8928,18771,12859,30436,5512,8471,24521,29283,22156,1907,15951,28393,16373,22921,5749,11095,13495,29140,28302,19861,208,5214,9436,2006,20117,11599,16648,14331,13642,19482,17810,6545,27643,21241,25663,10204,9365,19587,29493,23234,22227,23096,26385,9922,18356,21992,2174,730,29826,1639,17678,19298,345,14393,31540,3713,9335,10853,10318,26679,24590,10189,15641,2673,16864,9406,749,27899,30025,28312,1729,26675,32413,5067,30400,9879,11596,32281,16417,30891,9425,15917,17272,12002,1804,31278,6199,13249,22468,21950,869,31279,9268,2814,31411,12037,7454,23711,25824,8950,21242,7041,3472,1513,32676,23769,19652,28045,16760,21660,24884,9820,27202,12193,17254,8672,25684,3638,869,18353,3206,20267,5431,8074,27443,3217,17407,29599,6422,18362,2424,20651,6456,7467,9649,2383,12444,15758,12093,10802,11625,10921,29256,807,29832,23815,4077,29841,18326,22739,19120,3816,22842,7471,5743,975,1610,28164,6836,29459,6362,22820,21622,31164,19235,4233,3364,19735,2544,14870,28994,9925,24311,22834,6727,25051,23777,4624,11008,3628,31906,20332,11272,16450,32009,31375,30575,31751,1723,8673,773,17046,8922,15401,4439,21572,22066,30859,14338,25599,20450,17108,27296,3455,26511,9194,12271,8,744,13459,9964,29752,17155,3207,30319,18796,11547,5171,31921,20626,6424,4379,18012,26637,14097,11947,1003,8910,18394,17921,2985,25685,23052 };

void CEsp::Init()
{
	BombCarrier = nullptr;
}

// Yeah dude we're defo gunna do some sick moves for the esp yeah
void CEsp::Move(CUserCmd *pCmd,bool &bSendPacket) 
{

}

void DiLight()
{
	IClientEntity *pLocal = hackManager.pLocal();
	IClientEntity *pEntity = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	const float TIMEALIVE = 5.f;
	const float GRENADE_COEFFICIENT_OF_RESTITUTION = 0.4f;

	float fStep = 0.1f;
	float fGravity = 800.0f / 8.f;

	Vector vPos;
	Vector vStart;
	Vector vThrow;
	Vector vThrow2;
	int iCollisions = 0;

	Vector vViewAngles;
	Interfaces::Engine->GetViewAngles(vViewAngles);

	vThrow[0] = vViewAngles[0];
	vThrow[1] = vViewAngles[1];
	vThrow[2] = vViewAngles[2];

	if (vThrow[0] < 0)
		vThrow[0] = -10 + vThrow[0] * ((90 - 10) / 90.0);
	else
		vThrow[0] = -10 + vThrow[0] * ((90 + 10) / 90.0);

	float fVel = (90 - vThrow[0]) * 4;
	if (fVel > 500)
		fVel = 500;

	AngleVectors(vThrow, &vThrow2);

	Vector vEye = pEntity->GetEyePosition();
	vStart[0] = vEye[0] + vThrow2[0] * 16;
	vStart[1] = vEye[1] + vThrow2[1] * 16;
	vStart[2] = vEye[2] + vThrow2[2] * 16;

	vThrow2[0] = (vThrow2[0] * fVel) + pEntity->GetVelocity()[0];
	vThrow2[1] = (vThrow2[1] * fVel) + pEntity->GetVelocity()[1];
	vThrow2[2] = (vThrow2[2] * fVel) + pEntity->GetVelocity()[2]; // casualhacker for da magic calc help

	for (float fTime = 0.0f; fTime < TIMEALIVE; fTime += fStep)
	{
		vPos = vStart + vThrow2 * fStep;

		Ray_t ray;
		trace_t tr;

		CTraceFilter loc;
		loc.pSkip = pEntity;

		ray.Init(vStart, vPos);
		Interfaces::Trace->TraceRay(ray, MASK_SOLID, &loc, &tr);

		if (tr.DidHit()) // if the trace hits a surface
		{
			//float proj = DotProduct(throwvec, tr.plane.normal);
			vThrow2 = tr.plane.normal * -2.0f *DotProduct(vThrow2, tr.plane.normal) + vThrow2;
			vThrow2 *= GRENADE_COEFFICIENT_OF_RESTITUTION;

			iCollisions++;
			if (iCollisions > 2)
				break;

			vPos = vStart + vThrow2*tr.fraction*fStep;

			fTime += (fStep * (1 - tr.fraction));
		} // end of bounce

		Vector vOutStart, vOutEnd;
		if (GameUtils::IsG(pWeapon))
		{
			if (Render::WorldToScreen(vStart, vOutStart), Render::WorldToScreen(vPos, vOutEnd))
			{
				Render::Line(vOutStart.x, vOutStart.y, vOutEnd.x, vOutEnd.y, Color(Menu::Window.SettingsTab.OtherSettingsNadePredColorR.GetValue(), Menu::Window.SettingsTab.OtherSettingsNadePredColorG.GetValue(), Menu::Window.SettingsTab.OtherSettingsNadePredColorB.GetValue(), 255));
			}

			vStart = vPos;
			vThrow2.z -= fGravity*tr.fraction*fStep;
		}

	}
}

void CEsp::BacktrackingCross(IClientEntity* pEntity)
{
	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		player_info_t pinfo;
		if (pEntity && pEntity != hackManager.pLocal() && !pEntity->IsDormant())
		{
			if (Interfaces::Engine->GetPlayerInfo(i, &pinfo) && pEntity->IsAlive())
			{
				if (Menu::Window.LegitBotTab.LegitBackTracking.GetState())
				{
					if (hackManager.pLocal()->IsAlive())
					{
						for (int t = 0; t < 12; ++t)
						{
							Vector screenbacktrack[64][12];

							if (headPositions[i][t].simtime && headPositions[i][t].simtime + 1 > hackManager.pLocal()->GetSimulationTime())
							{
								if (Render::WorldToScreen(headPositions[i][t].hitboxPos, screenbacktrack[i][t]))
								{

									Interfaces::Surface->DrawSetColor(Color(255, 0, 0, 255));
									Interfaces::Surface->DrawOutlinedRect(screenbacktrack[i][t].x, screenbacktrack[i][t].y, screenbacktrack[i][t].x + 2, screenbacktrack[i][t].y + 2);

								}
							}
						}
					}
					else
					{
						memset(&headPositions[0][0], 0, sizeof(headPositions));
					}
				}
			}
		}
	}
}

// Main ESP Drawing loop
void CEsp::Draw()
{

	IClientEntity *pLocal = hackManager.pLocal();

	if (Menu::Window.VisualsTab.FiltersDead.GetState())
	{
		if (pLocal->IsAlive())
		{
			//Nothing :)
		}
		else
		{
			// Loop through all active entitys
			for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
			{
				// Get the entity
				IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
				player_info_t pinfo;

				// The entity isn't some laggy peice of shit or something
				if (pEntity &&  pEntity != pLocal && !pEntity->IsDormant())
				{
					// Radar
					if (Menu::Window.VisualsTab.OtherRadar.GetState())
					{
						DWORD m_bSpotted = NetVar.GetNetVar(0x839EB159);
						*(char*)((DWORD)(pEntity)+m_bSpotted) = 1;
					}

					// Is it a player?!
					if (Menu::Window.VisualsTab.FiltersPlayers.GetState() && Interfaces::Engine->GetPlayerInfo(i, &pinfo) && pEntity->IsAlive())
					{
						DrawPlayer(pEntity, pinfo);
					}

					// ~ Other ESP's here (items and shit) ~ //
					ClientClass* cClass = (ClientClass*)pEntity->GetClientClass();

					// Dropped weapons
					if (Menu::Window.VisualsTab.FiltersWeapons.GetState() && cClass->m_ClassID != (int)CSGOClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)CSGOClassID::CDEagle || cClass->m_ClassID == (int)CSGOClassID::CAK47)))
					{
						DrawDrop(pEntity, cClass);
					}

					// If entity is the bomb
					if (Menu::Window.VisualsTab.FiltersC4.GetState())
					{
						if (cClass->m_ClassID == (int)CSGOClassID::CPlantedC4)
							DrawBombPlanted(pEntity, cClass);

						if (cClass->m_ClassID == (int)CSGOClassID::CC4)
							DrawBomb(pEntity, cClass);
					}

					// If entity is a chicken
					if (Menu::Window.VisualsTab.FiltersChickens.GetState())
					{
						if (cClass->m_ClassID == (int)CSGOClassID::CChicken)
							DrawChicken(pEntity, cClass);
					}
				}
			}
		}
	}
	else
	{
		// Loop through all active entitys
		for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
		{
			// Get the entity
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
			player_info_t pinfo;

			// The entity isn't some laggy peice of shit or something
			if (pEntity &&  pEntity != pLocal && !pEntity->IsDormant())
			{
				// Radar
				if (Menu::Window.VisualsTab.OtherRadar.GetState())
				{
					DWORD m_bSpotted = NetVar.GetNetVar(0x839EB159);
					*(char*)((DWORD)(pEntity)+m_bSpotted) = 1;
				}

				// Is it a player?!
				if (Menu::Window.VisualsTab.FiltersPlayers.GetState() && Interfaces::Engine->GetPlayerInfo(i, &pinfo) && pEntity->IsAlive())
				{
					DrawPlayer(pEntity, pinfo);
				}

				// ~ Other ESP's here (items and shit) ~ //
				ClientClass* cClass = (ClientClass*)pEntity->GetClientClass();

				// Dropped weapons
				if (Menu::Window.VisualsTab.FiltersWeapons.GetState() && cClass->m_ClassID != (int)CSGOClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)CSGOClassID::CDEagle || cClass->m_ClassID == (int)CSGOClassID::CAK47)))
				{
					DrawDrop(pEntity, cClass);
				}

				if (Menu::Window.VisualsTab.FiltersNades.GetIndex() > 0)
				{
					if (cClass->m_ClassID == (int)CSGOClassID::CBaseCSGrenadeProjectile)
						DrawHE(pEntity, cClass);

					if (cClass->m_ClassID == (int)CSGOClassID::CMolotovProjectile)
						DrawMolotov(pEntity, cClass);

					if (cClass->m_ClassID == (int)CSGOClassID::CDecoyProjectile)
						DrawDecoy(pEntity, cClass);

					if (cClass->m_ClassID == (int)CSGOClassID::CSensorGrenadeProjectile)
						DrawMolotov(pEntity, cClass);

					if (cClass->m_ClassID == (int)CSGOClassID::CSmokeGrenadeProjectile)
						DrawSmoke(pEntity, cClass);
				}

				if (Menu::Window.VisualsTab.OtherFovDraw.GetState())
				{
					DrawFov();
				}

				// If entity is the bomb
				if (Menu::Window.VisualsTab.FiltersC4.GetState())
				{
					if (cClass->m_ClassID == (int)CSGOClassID::CPlantedC4)
						DrawBombPlanted(pEntity, cClass);

					if (cClass->m_ClassID == (int)CSGOClassID::CC4)
						DrawBomb(pEntity, cClass);
				}

				// If entity is a chicken
				if (Menu::Window.VisualsTab.FiltersChickens.GetState())
				{
					if (cClass->m_ClassID == (int)CSGOClassID::CChicken)
						DrawChicken(pEntity, cClass);
				}
			}
		}
	}


	if (Menu::Window.VisualsTab.OptionsNadePrediction.GetState())
	{
		//DiLight();
		grenade_prediction::instance().Paint();
	}

	//Those are good like this, no edit needed :)
	if (Menu::Window.MiscTab.OtherSpectators.GetState())
	{
		SpecList();
	}
	// Anti Flash
	if (Menu::Window.VisualsTab.OtherNoFlash.GetState())
	{
		DWORD m_flFlashMaxAlpha = NetVar.GetNetVar(0xFE79FB98);
		*(float*)((DWORD)pLocal + m_flFlashMaxAlpha) = Menu::Window.VisualsTab.OtherNoFlashVal.GetValue();
	}
}

void CEsp::NoSky()
{
	
}
void CEsp::DoFOV()
{
	float FoV;
	std::vector<int> HitBoxesToScan;
	IClientEntity* pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!pWeapon)
		return;

	if (GameUtils::IsPistol(pWeapon))
	{
		FoV = Menu::Window.LegitBotTab.WeaponPistFoV.GetValue();
		
	}
	else if (GameUtils::IsSniper(pWeapon))
	{
		FoV = Menu::Window.LegitBotTab.WeaponSnipFoV.GetValue();
		
	}
	else
	{
		FoV = Menu::Window.LegitBotTab.WeaponMainFoV.GetValue();
	}
	


	int w, h;
	Interfaces::Engine->GetScreenSize(w, h);
	float CenterX = (float)w / 2;
	float CenterY = (float)h / 2;

	
	Offsets::VMT::Surface_DrawOutlinedCircle;//(CenterX, CenterY, FoV, 360, 0, 255, 0, 255); //(int x, int y, int radius, int segments, int r, int g, int b, int a)
}
void CEsp::SpecList()
{
	IClientEntity *pLocal = hackManager.pLocal();

	RECT scrn = Render::GetViewport();
	int ayy = 0;

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
		if (Menu::Window.MiscTab.OtherSpectators.GetState())
		{
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
			player_info_t pinfo;

			if (pEntity &&  pEntity != pLocal)
			{
				if (Interfaces::Engine->GetPlayerInfo(i, &pinfo) && !pEntity->IsAlive() && !pEntity->IsDormant())
				{
					HANDLE obs = pEntity->GetObserverTargetHandle();

					if (obs)
					{
						IClientEntity *pTarget = Interfaces::EntList->GetClientEntityFromHandle(obs);
						player_info_t pinfo2;
						if (pTarget == pLocal)
						{
							if (Interfaces::Engine->GetPlayerInfo(pTarget->GetIndex(), &pinfo2))
							{
								char buf[255]; sprintf_s(buf, "%s", pinfo.name);
								RECT TextSize = Render::GetTextSize(Render::Fonts::ESP, buf);
								Render::Text(Menu::Window.SettingsTab.SpecVisGroupX.GetValue(), Menu::Window.SettingsTab.SpecVisGroupY.GetValue() - (15 * ayy), pTarget->GetIndex() == pLocal->GetIndex() ? Color(Menu::Window.ColorTab.MenuInnerR.GetValue(), Menu::Window.ColorTab.MenuInnerG.GetValue(), Menu::Window.ColorTab.MenuInnerB.GetValue(), 255) : Color(Menu::Window.ColorTab.MenuInnerR.GetValue(), Menu::Window.ColorTab.MenuInnerG.GetValue(), Menu::Window.ColorTab.MenuInnerB.GetValue(), 255), Render::Fonts::MenuBold, buf);
								ayy++;
							}
						}
					}
				}
			}
		}
}

//  Yeah m8

//-----------------------------------------------------------------------------
// Purpose: draws the zoom effect
//-----------------------------------------------------------------------------


void CEsp::DrawPlayer(IClientEntity* pEntity, player_info_t pinfo)
{
	//Here is our very advanced system yay

	ESPBox Box;
	Color Color;
	//terrorist red
	int TR = Menu::Window.ColorTab.TVisColorR.GetValue();
	int TNVR = Menu::Window.ColorTab.TNVisColorR.GetValue();
	//terrorist green
	int TG = Menu::Window.ColorTab.TVisColorG.GetValue();
	int TNVG = Menu::Window.ColorTab.TNVisColorG.GetValue();
	//terrorist green
	int TB = Menu::Window.ColorTab.TVisColorB.GetValue();
	int TNVB = Menu::Window.ColorTab.TNVisColorB.GetValue();

	//ct red
	int CTR = Menu::Window.ColorTab.CTVisColorR.GetValue();
	int CTNVR = Menu::Window.ColorTab.CTNVisColorR.GetValue();
	//ct green
	int CTG = Menu::Window.ColorTab.CTVisColorG.GetValue();
	int CTNVG = Menu::Window.ColorTab.CTNVisColorG.GetValue();
	//ct blue
	int CTB = Menu::Window.ColorTab.CTVisColorB.GetValue();
	int CTNVB = Menu::Window.ColorTab.CTNVisColorB.GetValue();

	if (Menu::Window.RageBotTab.AntiAimEnable.GetState()) {
		if (Menu::Window.RageBotTab.AntiAimDrawLines.GetState()) {
			if (hackManager.pLocal()->IsAlive()) {
				CEsp::DrawLinesAA(Color);
			}
		}
	}

	// Show own team false? well gtfo teammate lol
	if (Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() && (pEntity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
		return;

	if (GetBox(pEntity, Box))
	{
		Color = GetPlayerColor(pEntity);

		if (Menu::Window.VisualsTab.Barrels.GetState())
			Barrel(Box, Color, pEntity);

		if (!Menu::Window.VisualsTab.OptionsBox.GetIndex() == 0)
			DrawBox(Box, Color);

		if (Menu::Window.VisualsTab.OptionsName.GetState())
			DrawName(pinfo, Box, pEntity);

		if (Menu::Window.VisualsTab.OptionsSkeleton.GetState())
			DrawSkeleton(pEntity);

		switch (Menu::Window.VisualsTab.OptionsHealth.GetIndex())
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			DrawHealth2(pEntity, Box);
			break;
		case 3:
			DrawHealth3(pEntity, Box);
			break;
		case 4:
			DrawHealth4(pEntity, Box);
			break;
		}

		if (Menu::Window.VisualsTab.OptionsDefusing.GetIndex() > 0)
			IsPlayerDefusing(pinfo, Box, pEntity);

		if (Menu::Window.VisualsTab.OptionsArmor.GetIndex() > 1)
			DrawArmor(pEntity, Box);

		if (Menu::Window.VisualsTab.OptionsDistance.GetState())
			DrawDistanse(pEntity, Box);

		if (Menu::Window.VisualsTab.OtherBackTracking.GetState())
			BacktrackingCross(pEntity);

		if (Menu::Window.VisualsTab.OptionsInfo.GetState() || Menu::Window.VisualsTab.OptionsWeapon.GetIndex() > 0)
			DrawInfo(pinfo, pEntity, Box);
	}
}
void CEsp::DrawDistanse(IClientEntity* pEntity, CEsp::ESPBox size)
{
	IClientEntity *pLocal = hackManager.pLocal();

	Vector vecOrigin = pEntity->GetOrigin();
	Vector vecOriginLocal = pLocal->GetOrigin();
	static RECT defSize = Render::GetTextSize(Render::Fonts::Default, "");

	char dist_to[32];
	sprintf_s(dist_to, "%.0fm", DistanceTo(vecOrigin, vecOriginLocal));

	Render::Text(size.x + size.w + 3, size.y + (0.6*(defSize.bottom + 28)), Color(255, 255, 255, 255), Render::Fonts::ESP, dist_to);
}

float CEsp::DistanceTo(Vector vecDst, Vector vecSrc)
{
	Vector vDelta = vecDst - vecSrc;

	float fDistance = ::sqrtf((vDelta.Length()));

	if (fDistance < 1.0f)
		return 1.0f;

	return fDistance;
}

// Gets the 2D bounding box for the entity
// Returns false on failure nigga don't fail me
bool CEsp::GetBox(IClientEntity* pEntity, CEsp::ESPBox &result)
{
	// Variables
	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	// Get the locations
	vOrigin = pEntity->GetOrigin();
	min = pEntity->collisionProperty()->GetMins() + vOrigin;
	max = pEntity->collisionProperty()->GetMaxs() + vOrigin;

	// Points of a 3d bounding box
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	// Get screen positions
	if (!Render::WorldToScreen(points[3], flb) || !Render::WorldToScreen(points[5], brt)
		|| !Render::WorldToScreen(points[0], blb) || !Render::WorldToScreen(points[4], frt)
		|| !Render::WorldToScreen(points[2], frb) || !Render::WorldToScreen(points[1], brb)
		|| !Render::WorldToScreen(points[6], blt) || !Render::WorldToScreen(points[7], flt))
		return false;

	// Put them in an array (maybe start them off in one later for speed?)
	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Init this shit
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	// Width / height
	result.x = left;
	result.y = top;
	result.w = right - left;
	result.h = bottom - top;

	return true;
}

/*void NadePredic()
{
	IClientEntity *pLocal = hackManager.pLocal();
	IClientEntity *pEntity = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	const float TIMEALIVE = 5.f;
	const float GRENADE_COEFFICIENT_OF_RESTITUTION = 0.4f;

	float fStep = 0.1f;
	float fGravity = 800.0f / 8.f;

	Vector vPos;
	Vector vStart;
	Vector vThrow;
	Vector vThrow2;
	int iCollisions = 0;

	Vector vViewAngles;
	Interfaces::Engine->GetViewAngles(vViewAngles);

	vThrow[0] = vViewAngles[0];
	vThrow[1] = vViewAngles[1];
	vThrow[2] = vViewAngles[2];

	if (vThrow[0] < 0)
		vThrow[0] = -10 + vThrow[0] * ((90 - 10) / 90.0);
	else
		vThrow[0] = -10 + vThrow[0] * ((90 + 10) / 90.0);

	float fVel = (90 - vThrow[0]) * 4;
	if (fVel > 500)
		fVel = 500;

	AngleVectors(vThrow, &vThrow2);

	Vector vEye = pEntity->GetEyePosition();
	vStart[0] = vEye[0] + vThrow2[0] * 16;
	vStart[1] = vEye[1] + vThrow2[1] * 16;
	vStart[2] = vEye[2] + vThrow2[2] * 16;

	vThrow2[0] = (vThrow2[0] * fVel) + pEntity->GetVelocity()[0];
	vThrow2[1] = (vThrow2[1] * fVel) + pEntity->GetVelocity()[1];
	vThrow2[2] = (vThrow2[2] * fVel) + pEntity->GetVelocity()[2]; // casualhacker for da magic calc help

	for (float fTime = 0.0f; fTime < TIMEALIVE; fTime += fStep)
	{
		vPos = vStart + vThrow2 * fStep;

		Ray_t ray;
		trace_t tr;

		CTraceFilter loc;
		loc.pSkip = pEntity;

		ray.Init(vStart, vPos);
		Interfaces::Trace->TraceRay(ray, MASK_SOLID, &loc, &tr);

		if (tr.DidHit()) // if the trace hits a surface
		{
			//float proj = DotProduct(throwvec, tr.plane.normal);
			vThrow2 = tr.plane.normal * -2.0f *DotProduct(vThrow2, tr.plane.normal) + vThrow2;
			vThrow2 *= GRENADE_COEFFICIENT_OF_RESTITUTION;

			iCollisions++;
			if (iCollisions > 2)
				break;

			vPos = vStart + vThrow2*tr.fraction*fStep;

			fTime += (fStep * (1 - tr.fraction));
		} // end of bounce

		Vector vOutStart, vOutEnd;
		if (GameUtils::IsG(pWeapon))
		{
			if (Render::WorldToScreen(vStart, vOutStart), Render::WorldToScreen(vPos, vOutEnd))
			{
				Render::Line(vOutStart.x, vOutStart.y, vOutEnd.x, vOutEnd.y, Color(0, 150, 255, 255));
			}

			vStart = vPos;
			vThrow2.z -= fGravity*tr.fraction*fStep;
		}
	}
}*/

// Get an entities color depending on team and vis ect
Color CEsp::GetPlayerColor(IClientEntity* pEntity)
{
//#define hitboxes (int)CSGOHitboxID::Head, (int)CSGOHitboxID::Neck, (int)CSGOHitboxID::NeckLower, (int)CSGOHitboxID::Stomach, (int)CSGOHitboxID::Pelvis, (int)CSGOHitboxID::UpperChest,(int)CSGOHitboxID::Chest, (int)CSGOHitboxID::LowerChest,(int)CSGOHitboxID::LeftUpperArm, (int)CSGOHitboxID::RightUpperArm, (int)CSGOHitboxID::LeftThigh, (int)CSGOHitboxID::RightThigh, (int)CSGOHitboxID::LeftHand, (int)CSGOHitboxID::RightHand, (int)CSGOHitboxID::LeftFoot, (int)CSGOHitboxID::RightFoot, (int)CSGOHitboxID::LeftShin, (int)CSGOHitboxID::RightShin, (int)CSGOHitboxID::LeftLowerArm, (int)CSGOHitboxID::RightLowerArm
	int TeamNum = pEntity->GetTeamNum();
	bool IsVis;
	if (GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Chest) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Head) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Neck) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::NeckLower) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Pelvis) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Chest) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::LeftFoot) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::LeftHand) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::LeftLowerArm) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::LeftShin) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::LeftThigh) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::LeftUpperArm) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::LowerChest) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::RightFoot) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::RightHand) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::RightLowerArm) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::RightShin) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::RightThigh) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::RightUpperArm))
	{
		IsVis = true;
	}
	else
	{
		IsVis = false;
	}
	
	Color color;

	if (TeamNum == TEAM_CS_T)
	{
		if (IsVis)
		{	
			color = Color(Menu::Window.ColorTab.TVisColorR.GetValue(), Menu::Window.ColorTab.TVisColorG.GetValue(), Menu::Window.ColorTab.TVisColorB.GetValue(), 255);
		}
		else
		{

			color = Color(Menu::Window.ColorTab.TNVisColorR.GetValue(), Menu::Window.ColorTab.TNVisColorG.GetValue(), Menu::Window.ColorTab.TNVisColorB.GetValue(), 255);
		}	    
	}
	else
	{
		if (IsVis)
		{
			color = Color(Menu::Window.ColorTab.CTVisColorR.GetValue(), Menu::Window.ColorTab.CTVisColorG.GetValue(), Menu::Window.ColorTab.CTVisColorB.GetValue(), 255);
		}
		else
		{
			color = Color(Menu::Window.ColorTab.CTNVisColorR.GetValue(), Menu::Window.ColorTab.CTNVisColorG.GetValue(), Menu::Window.ColorTab.CTNVisColorB.GetValue(), 255);
		}
	}


	return color;
}

// 2D  Esp box
void CEsp::DrawBox(CEsp::ESPBox size, Color color)
{
	if (Menu::Window.VisualsTab.OptionsBoxFill.GetState())
	{
		//Render::Clear(size.x, size.y, size.w, size.h, color);
		if (!Menu::Window.VisualsTab.OptionsBoxFillColor.GetState())
		{
			Render::Clear(size.x - 1, size.y - 1, size.w + 2, size.h + 2, Color(10, 10, 10, 100));
			Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(10, 10, 10, 100));
		}
		else
		{
			Render::Clear(size.x - 1, size.y - 1, size.w + 2, size.h + 2, Color(10, 10, 10, 150));
			Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(10, 10, 10, 150));
		}
	}
	//if (PlayerBoxes->GetStringIndex() == 1)
	//{
		// Full Box
	//Render::Clear(size.x, size.y, size.w, size.h, color);
	//Render::Clear(size.x - 1, size.y - 1, size.w + 2, size.h + 2, Color(10, 10, 10, 150)); 
	//Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(10, 10, 10, 150));
	//}
	//else
	{
		int VertLine;
		int HorzLine;
		// Corner Box
		//bool IsVis = GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Chest);  da dream
		int xd = Menu::Window.VisualsTab.OptionsBox.GetIndex();
		if (Menu::Window.VisualsTab.OptionsBox.GetIndex() == 1)
		{
			VertLine = (((float)size.w) * (0.2f));
			HorzLine = (((float)size.h) * (0.2f));
		}
		if (Menu::Window.VisualsTab.OptionsBox.GetIndex() == 2)
		{
			VertLine = (((float)size.w) * (1.0f));
			HorzLine = (((float)size.h) * (1.0f));
		}
		if (Menu::Window.VisualsTab.OptionsBox.GetIndex() == 3)
		{
			int VertLine = (((float)size.w) * (0.20f));
			int HorzLine = (((float)size.h) * (0.20f));

			int BVertLine = (((float)size.w) * (0.25f));
			int BHorzLine = (((float)size.h) * (1.0f));

			Render::Clear(size.x, size.y, VertLine, 1, color);	Render::Clear(size.x + size.w - VertLine, size.y, VertLine, 1, color);
			Render::Clear(size.x, size.y + size.h, VertLine, 1, color);
			Render::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, color);
			Render::Clear(size.x, size.y, 1, BHorzLine, color);

			Render::Clear(size.x, size.y + size.h - BHorzLine, 1, BHorzLine, color);
			Render::Clear(size.x + size.w, size.y, 1, BHorzLine, color);
			Render::Clear(size.x + size.w, size.y + size.h - BHorzLine, 1, BHorzLine, color);

		}
		if (Menu::Window.VisualsTab.OptionsBox.GetIndex() == 4)
		{
			IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
			if (!pLocal->IsAlive()) {
				VertLine = (((float)size.w) * (0.2f));
				HorzLine = (((float)size.h) * (0.2f));

				Render::Clear(size.x, size.y - 1, VertLine, 1, Color(10, 10, 10, 150));
				Render::Clear(size.x + size.w - VertLine, size.y - 1, VertLine, 1, Color(10, 10, 10, 150));
				Render::Clear(size.x, size.y + size.h - 1, VertLine, 1, Color(10, 10, 10, 150));
				Render::Clear(size.x + size.w - VertLine, size.y + size.h - 1, VertLine, 1, Color(10, 10, 10, 150));

				Render::Clear(size.x - 1, size.y, 1, HorzLine, Color(10, 10, 10, 150));
				Render::Clear(size.x - 1, size.y + size.h - HorzLine, 1, HorzLine, Color(10, 10, 10, 150));
				Render::Clear(size.x + size.w - 1, size.y, 1, HorzLine, Color(10, 10, 10, 150));
				Render::Clear(size.x + size.w - 1, size.y + size.h - HorzLine, 1, HorzLine, Color(10, 10, 10, 150));

				Render::Clear(size.x, size.y, VertLine, 1, color);
				Render::Clear(size.x + size.w - VertLine, size.y, VertLine, 1, color);
				Render::Clear(size.x, size.y + size.h, VertLine, 1, color);
				Render::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, color);

				Render::Clear(size.x, size.y, 1, HorzLine, color);
				Render::Clear(size.x, size.y + size.h - HorzLine, 1, HorzLine, color);
				Render::Clear(size.x + size.w, size.y, 1, HorzLine, color);
				Render::Clear(size.x + size.w, size.y + size.h - HorzLine, 1, HorzLine, color);
			}
		}

		if (Menu::Window.VisualsTab.OptionsBox.GetIndex() == 5)
		{
			IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
			if (!pLocal->IsAlive()) {
				VertLine = (((float)size.w) * (1.0f));
				HorzLine = (((float)size.h) * (1.0f));

				Render::Clear(size.x, size.y - 1, VertLine, 1, Color(10, 10, 10, 150));
				Render::Clear(size.x + size.w - VertLine, size.y - 1, VertLine, 1, Color(10, 10, 10, 150));
				Render::Clear(size.x, size.y + size.h - 1, VertLine, 1, Color(10, 10, 10, 150));
				Render::Clear(size.x + size.w - VertLine, size.y + size.h - 1, VertLine, 1, Color(10, 10, 10, 150));

				Render::Clear(size.x - 1, size.y, 1, HorzLine, Color(10, 10, 10, 150));
				Render::Clear(size.x - 1, size.y + size.h - HorzLine, 1, HorzLine, Color(10, 10, 10, 150));
				Render::Clear(size.x + size.w - 1, size.y, 1, HorzLine, Color(10, 10, 10, 150));
				Render::Clear(size.x + size.w - 1, size.y + size.h - HorzLine, 1, HorzLine, Color(10, 10, 10, 150));

				Render::Clear(size.x, size.y, VertLine, 1, color);
				Render::Clear(size.x + size.w - VertLine, size.y, VertLine, 1, color);
				Render::Clear(size.x, size.y + size.h, VertLine, 1, color);
				Render::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, color);

				Render::Clear(size.x, size.y, 1, HorzLine, color);
				Render::Clear(size.x, size.y + size.h - HorzLine, 1, HorzLine, color);
				Render::Clear(size.x + size.w, size.y, 1, HorzLine, color);
				Render::Clear(size.x + size.w, size.y + size.h - HorzLine, 1, HorzLine, color);
			}
		}
		if (Menu::Window.VisualsTab.OptionsBox.GetIndex() == 3)
		{
			IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
			if (!pLocal->IsAlive()) {
				int VertLine = (((float)size.w) * (0.20f));
				int HorzLine = (((float)size.h) * (0.20f));

				int BVertLine = (((float)size.w) * (0.25f));
				int BHorzLine = (((float)size.h) * (1.0f));

				Render::Clear(size.x, size.y, VertLine, 1, color);	Render::Clear(size.x + size.w - VertLine, size.y, VertLine, 1, color);
				Render::Clear(size.x, size.y + size.h, VertLine, 1, color);
				Render::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, color);
				Render::Clear(size.x, size.y, 1, BHorzLine, color);

				Render::Clear(size.x, size.y + size.h - BHorzLine, 1, BHorzLine, color);
				Render::Clear(size.x + size.w, size.y, 1, BHorzLine, color);
				Render::Clear(size.x + size.w, size.y + size.h - BHorzLine, 1, BHorzLine, color);
			}
		}

		if (Menu::Window.VisualsTab.OptionsBox.GetIndex() > 0 && Menu::Window.VisualsTab.OptionsBox.GetIndex() < 3)
		{
			Render::Clear(size.x, size.y - 1, VertLine, 1, Color(10, 10, 10, 150));
			Render::Clear(size.x + size.w - VertLine, size.y - 1, VertLine, 1, Color(10, 10, 10, 150));
			Render::Clear(size.x, size.y + size.h - 1, VertLine, 1, Color(10, 10, 10, 150));
			Render::Clear(size.x + size.w - VertLine, size.y + size.h - 1, VertLine, 1, Color(10, 10, 10, 150));

			Render::Clear(size.x - 1, size.y, 1, HorzLine, Color(10, 10, 10, 150));
			Render::Clear(size.x - 1, size.y + size.h - HorzLine, 1, HorzLine, Color(10, 10, 10, 150));
			Render::Clear(size.x + size.w - 1, size.y, 1, HorzLine, Color(10, 10, 10, 150));
			Render::Clear(size.x + size.w - 1, size.y + size.h - HorzLine, 1, HorzLine, Color(10, 10, 10, 150));

			Render::Clear(size.x, size.y, VertLine, 1, color);
			Render::Clear(size.x + size.w - VertLine, size.y, VertLine, 1, color);
			Render::Clear(size.x, size.y + size.h, VertLine, 1, color);
			Render::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, color);

			Render::Clear(size.x, size.y, 1, HorzLine, color);
			Render::Clear(size.x, size.y + size.h - HorzLine, 1, HorzLine, color);
			Render::Clear(size.x + size.w, size.y, 1, HorzLine, color);
			Render::Clear(size.x + size.w, size.y + size.h - HorzLine, 1, HorzLine, color);
		}

	//	Color color23(0, 0, 0);
		
	//	Render::Clear(size.x + size.y ,size.w, size.h, Color(0,0,0,40));
	}
}

// Unicode Conversions
static wchar_t* CharToWideChar(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs_s(NULL, wa, size/4, text, size);
	return wa;
}
void CEsp::DrawArmor(IClientEntity* pEntity, CEsp::ESPBox size)
{
	ESPBox ArmorBar = size;
	ArmorBar.y += (ArmorBar.h + 6);
	ArmorBar.h = 4;

	float ArmorValue = pEntity->ArmorValue();
	float ArmorPerc = ArmorValue / 100.f;
	float Width = (size.w * ArmorPerc);
	ArmorBar.w = Width;

	int armor = pEntity->ArmorValue();

	if (armor > 100)
		armor = 100;
	/* I dont want a change yellow is good */
	//int r = 255 - armor * 2.55;
	//int g = armor * 2.55;

	int armorBar = size.h / 100 * armor;
	int armorBarDelta = size.h - armorBar;

	if (!armor < 1)
	{
		Render::Clear(size.x - 10, size.y - 1, 2, size.h + 2, Color(0, 0, 0, 150));
		Render::Clear(size.x - 9, size.y + 1, 1, size.h * 0.01 * armor, Color(0, 150, 255, 255));
	}
}

void CEsp::DrawFov()
{
	IClientEntity* pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	int ScreenWidth, ScreenHeight;
	Interfaces::Engine->GetScreenSize(ScreenWidth, ScreenHeight);

	int x = (int)(ScreenWidth * 0.5f);
	int y = 0;
	int FOV = 0;

	y = ScreenHeight / 2;

	if (Menu::Window.LegitBotTab.Active.GetState() && !Menu::Window.RageBotTab.Active.GetState())
	{
		if (GameUtils::IsPistol(pWeapon))
		{
			FOV = Menu::Window.LegitBotTab.WeaponPistFoV.GetValue() * 7.5;
		}
		else if (GameUtils::IsSniper(pWeapon))
		{
			FOV = Menu::Window.LegitBotTab.WeaponSnipFoV.GetValue() * 7.5;
		}
		else
		{
			FOV = Menu::Window.LegitBotTab.WeaponMainFoV.GetValue() * 7.5;
		}
	}
	if (!Menu::Window.LegitBotTab.Active.GetState() && Menu::Window.RageBotTab.Active.GetState())
	{
		FOV = Menu::Window.RageBotTab.AimbotFov.GetValue() * 7.5;
	}
	if (!GameUtils::IsBallisticWeapon(pWeapon) || !GameUtils::IsG(pWeapon))
	{
		Render::Circle(x, y, FOV, 25, Color(Menu::Window.ColorTab.MenuInnerR.GetValue(), Menu::Window.ColorTab.MenuInnerG.GetValue(), Menu::Window.ColorTab.MenuInnerB.GetValue(), 255));
	}
}

void CEsp::DrawMolotov(IClientEntity* pEntity, ClientClass* cClass)
{
	ESPBox Box;
	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();

	if (GetBox(pEntity, Box))
	{
		if (Render::WorldToScreen(vOrig, vScreen))
		{
			if (Menu::Window.VisualsTab.FiltersNades.GetIndex() > 0)
				Render::Text(vScreen.x - 10, vScreen.y + 8, Color(255, 133, 51, 255), Render::Fonts::ESP, "*MOLOTOV*");
			if (Menu::Window.VisualsTab.FiltersNades.GetIndex() > 1)
				Render::Circle(vScreen.x, vScreen.y, 10, 5, Color(255, 133, 51, 255));
		}
	}
}

void CEsp::DrawSmoke(IClientEntity* pEntity, ClientClass* cClass)
{
	ESPBox Box;
	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();

	if (GetBox(pEntity, Box))
	{
		if (Render::WorldToScreen(vOrig, vScreen))
		{
			if (Menu::Window.VisualsTab.FiltersNades.GetIndex() > 0)
				Render::Text(vScreen.x - 10, vScreen.y + 8, Color(204, 204, 204, 255), Render::Fonts::ESP, "*SMOKE*");
		}
	}
}

void CEsp::DrawDecoy(IClientEntity* pEntity, ClientClass* cClass)
{
	ESPBox Box;
	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();

	if (GetBox(pEntity, Box))
	{
		if (Render::WorldToScreen(vOrig, vScreen))
		{
			if (Menu::Window.VisualsTab.FiltersNades.GetIndex() > 0)
				Render::Text(vScreen.x - 10, vScreen.y + 8, Color(89, 89, 89, 255), Render::Fonts::ESP, "*DECOY*");

		}

	}
}

void CEsp::DrawHE(IClientEntity* pEntity, ClientClass* cClass)
{
	ESPBox Box;
	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();


	if (GetBox(pEntity, Box))
	{
		if (Render::WorldToScreen(vOrig, vScreen))
		{
			std::string sWeapon = Interfaces::ModelInfo->GetModelName(pEntity->GetModel());
			if (sWeapon.find("flashbang", 0) != std::string::npos)
			{
				if (Menu::Window.VisualsTab.FiltersNades.GetIndex() > 0)
					Render::Text(vScreen.x - 10, vScreen.y + 8, Color(0, 153, 255, 255), Render::Fonts::ESP, "*FLASH*");
				if (Menu::Window.VisualsTab.FiltersNades.GetIndex() > 1)
					Render::Circle(vScreen.x, vScreen.y, 10, 5, Color(0, 153, 255, 255));
			}
			else
			{
				if (Menu::Window.VisualsTab.FiltersNades.GetIndex() > 0)
					Render::Text(vScreen.x - 10, vScreen.y + 8, Color(255, 0, 0, 255), Render::Fonts::ESP, "*HE*");
				if (Menu::Window.VisualsTab.FiltersNades.GetIndex() > 1)
					Render::Circle(vScreen.x, vScreen.y, 10, 5, Color(255, 0, 0, 255));
			}
		}
	}
}

// Player name
void CEsp::DrawName(player_info_t pinfo, CEsp::ESPBox size, IClientEntity* pEntity)
{
	RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, pinfo.name);
	RECT Size = Render::GetTextSize(Render::Fonts::ESP, "");
	int i = 0;
	if (strlen(pinfo.name) > 16)
	{
		pinfo.name[12] = 0;
		strcat(pinfo.name, "...");
		puts(pinfo.name);
		if (Menu::Window.VisualsTab.OptionsScoped.GetState())
		{
			if (pEntity->IsScoped())
			{
				Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 16, Color(255, 0, 0, 255), Render::Fonts::ESP, pinfo.name);
			}
			else
			{
				Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 16, Color(255, 255, 0, 255), Render::Fonts::ESP, pinfo.name);
			}
		}
		else
		{
			Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 16, Color(255, 255, 255, 255), Render::Fonts::ESP, pinfo.name);
		}
	}
	else
	{
		if (Menu::Window.VisualsTab.OptionsScoped.GetState())
		{
			if (pEntity->IsScoped())
			{
				Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 16, Color(255, 0, 0, 255), Render::Fonts::ESP, pinfo.name);
			}
			else
			{
				Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 16, Color(255, 255, 0, 255), Render::Fonts::ESP, pinfo.name);
			}
		}
		else
		{
			Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 16, Color(255, 255, 255, 255), Render::Fonts::ESP, pinfo.name);
		}
	}
	if (Menu::Window.RageBotTab.AccuracyResolver.GetIndex() == 2)
	{
		if (resolvermode == 0)
		{
			Render::Text(size.x + size.w + 3, size.y + (0 * (nameSize.bottom + 0)), Color(255, 255, 255, 255), Render::Fonts::ESP, "R: None");
		}
		else if (resolvermode == 1)
		{
			Render::Text(size.x + size.w + 3, size.y + (0 * (nameSize.bottom + 0)), Color(255, 255, 255, 255), Render::Fonts::ESP, "R: Bruteforce");
		}
		else if (resolvermode == 2)
		{
			Render::Text(size.x + size.w + 3, size.y + (0 * (nameSize.bottom + 0)), Color(255, 255, 255, 255), Render::Fonts::ESP, "R: LBY Delta");
		}
		else if (resolvermode == 3)
		{
			Render::Text(size.x + size.w + 3, size.y + (0 * (nameSize.bottom + 0)), Color(255, 255, 255, 255), Render::Fonts::ESP, "R: LBY");
		}
		else if (resolvermode == 4)
		{
			Render::Text(size.x + size.w + 3, size.y + (0 * (nameSize.bottom + 0)), Color(255, 255, 255, 255), Render::Fonts::ESP, "R: Fake");
		}
		else if (resolvermode == 5)
		{
			Render::Text(size.x + size.w + 3, size.y + (0 * (nameSize.bottom + 0)), Color(255, 255, 255, 255), Render::Fonts::ESP, "R: Real");
		}
	}
	if (Menu::Window.VisualsTab.OptionsHealth.GetIndex() > 0)
	{
		int HealthValue = 100;
		HealthValue = pEntity->GetHealth();
		int ArmorValue = 100;
		ArmorValue = pEntity->ArmorValue();
		char nameBuffer[512];
		float HealthPerc = HealthValue / 1;
		if (HealthValue > 100)
			HealthValue = 100;
		char faszommeretejobbra_cmbe[64];
		sprintf_s(faszommeretejobbra_cmbe, "HP: %d", HealthValue);
		if (Menu::Window.VisualsTab.OptionsArmor.GetIndex() == 1 || Menu::Window.VisualsTab.OptionsArmor.GetIndex() == 3)
		{
			if (Menu::Window.VisualsTab.OptionsFriendly.GetIndex() == 3) {
				int health = pEntity->GetHealth();

				if (health > 100)
					health = 100;

				int r = 255 - health * 2.55;
				int g = health * 2.55;
				if (Menu::Window.VisualsTab.OptionsWeapon.GetIndex() == 2) {
					Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 24, Color(r, g, 0, 255), Render::Fonts::ESP, faszommeretejobbra_cmbe);
				}
				else {
					Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 32, Color(r, g, 0, 255), Render::Fonts::ESP, faszommeretejobbra_cmbe);
				}
			}
			else {
				if (ArmorValue > 15)
				{
					if (Menu::Window.VisualsTab.OptionsWeapon.GetIndex() == 2) {
						Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 24, Color(15, 10, 168, 255), Render::Fonts::ESP, faszommeretejobbra_cmbe);
					}
					else {
						Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 32, Color(15, 10, 168, 255), Render::Fonts::ESP, faszommeretejobbra_cmbe);
					}
				}
				else
				{
					if (Menu::Window.VisualsTab.OptionsWeapon.GetIndex() == 2) {
						Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 24, Color(255, 255, 255, 255), Render::Fonts::ESP, faszommeretejobbra_cmbe);
					}
					else {
						Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 32, Color(255, 255, 255, 255), Render::Fonts::ESP, faszommeretejobbra_cmbe);
					}
				}
			}
		}
		else
		{
			if (Menu::Window.VisualsTab.OptionsWeapon.GetIndex() == 2) {
				Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 24, Color(255, 255, 255, 255), Render::Fonts::ESP, faszommeretejobbra_cmbe);
			}
			else {
				Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 32, Color(255, 255, 255, 255), Render::Fonts::ESP, faszommeretejobbra_cmbe);
			}
		}
	}
	if (Menu::Window.VisualsTab.OptionsMoney.GetState())
	{
		int Money = 0;
		Money = pEntity->GetMoney();
		char showmoney[64];
		sprintf_s(showmoney, "$%d", Money);
		if (!Menu::Window.VisualsTab.OptionsWeapon.GetIndex() == 0 || !Menu::Window.VisualsTab.OptionsWeapon.GetIndex() == 1)
		{
			if (Menu::Window.VisualsTab.OptionsFriendly.GetIndex() == 3) {
				int health = pEntity->GetHealth();

				if (health > 100)
					health = 100;

				int r = 255 - health * 2.55;
				int g = health * 2.55;
				Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 32, Color(r, g, 0, 255), Render::Fonts::ESP, showmoney);
			}
			else {
				Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 32, Color(0, 255, 0, 255), Render::Fonts::ESP, showmoney);
			}
		}
		else
		{
			if (Menu::Window.VisualsTab.OptionsFriendly.GetIndex() == 3) {
				int health = pEntity->GetHealth();

				if (health > 100)
					health = 100;

				int r = 255 - health * 2.55;
				int g = health * 2.55;
				Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 40, Color(r, g, 0, 255), Render::Fonts::ESP, showmoney);
			}
			else {
				Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 40, Color(0, 255, 0, 255), Render::Fonts::ESP, showmoney);
			}
		}
	}
}

void CEsp::IsPlayerDefusing(player_info_t pinfo, CEsp::ESPBox size, IClientEntity* pEntity)
{
	RECT defSize = Render::GetTextSize(Render::Fonts::ESP, "");
	CCSBomb* Bomb = (CCSBomb*)pEntity;
	float countdown = Bomb->GetC4DefuseCountDown() - (pEntity->GetTickBase() * Interfaces::Globals->interval_per_tick);
	float flBlow = Bomb->GetC4BlowTime();
	float lifetime = flBlow - (Interfaces::Globals->interval_per_tick * pEntity->GetTickBase());
	if (pEntity->IsDefusing())
	{
		c4_defuse = 1;
	}
	else
	{
		c4_defuse = 0;
	}
}

// Draw a health bar. For Tf2 when a bar is bigger than max health a second bar is displayed
void CEsp::DrawHealth(IClientEntity* pEntity, CEsp::ESPBox size)
{
	ESPBox HealthBar = size;
	HealthBar.y += (HealthBar.h + 6);
	HealthBar.h = 4;

	float HealthValue = pEntity->GetHealth();
	float HealthPerc = HealthValue / 100.f;
	float Width = (size.w * HealthPerc);
	HealthBar.w = Width;

	// --  Main Bar -- //

	Vertex_t Verts[4];
	Verts[0].Init(Vector2D(HealthBar.x, HealthBar.y));
	Verts[1].Init(Vector2D(HealthBar.x + size.w, HealthBar.y));
	Verts[2].Init(Vector2D(HealthBar.x + size.w, HealthBar.y + 5));
	Verts[3].Init(Vector2D(HealthBar.x, HealthBar.y + 5));

	Render::PolygonOutline(4, Verts, Color(10, 10, 10, 255), Color(255, 255, 255, 170));

	Vertex_t Verts2[4];
	Verts2[0].Init(Vector2D(HealthBar.x, HealthBar.y + 1));
	Verts2[1].Init(Vector2D(HealthBar.x + HealthBar.w, HealthBar.y + 1));
	Verts2[2].Init(Vector2D(HealthBar.x + HealthBar.w, HealthBar.y + 5));
	Verts2[3].Init(Vector2D(HealthBar.x, HealthBar.y + 5));
	
	Color colorxd(0, 255, 0);
	Color colorxd2(255, 255, 0);
	Color colorxd3(255, 119, 0);
	Color colorxd4(255, 0, 0);


	Render::Polygon(4, Verts2, GetPlayerColor(pEntity));




	Verts2[0].Init(Vector2D(HealthBar.x + 1, HealthBar.y + 1));

	Verts2[1].Init(Vector2D(HealthBar.x + HealthBar.w, HealthBar.y + 1));
	Verts2[2].Init(Vector2D(HealthBar.x + HealthBar.w, HealthBar.y + 2));
	Verts2[3].Init(Vector2D(HealthBar.x , HealthBar.y + 2));
	/*Verts2[1].Init(Vector2D(HealthBar.x + HealthBar.w + 2, HealthBar.y + 1));
	Verts2[2].Init(Vector2D(HealthBar.x + HealthBar.w, HealthBar.y + 2));
	Verts2[3].Init(Vector2D(HealthBar.x - 2, HealthBar.y + 2));*/

	Render::Polygon(4, Verts2, Color(255, 255, 255, 40));

}


// Cleans the internal class name up to something human readable and nice
std::string CleanItemName(std::string name)
{
	std::string Name = name;
	// Tidy up the weapon Name
	if (Name[0] == 'C')
		Name.erase(Name.begin());

	// Remove the word Weapon
	auto startOfWeap = Name.find("Weapon");
	if (startOfWeap != std::string::npos)
		Name.erase(Name.begin() + startOfWeap, Name.begin() + startOfWeap + 6);

	return Name;
}

// Anything else: weapons, class state? idk
void CEsp::DrawInfo(player_info_t pinfo, IClientEntity* pEntity, CEsp::ESPBox size)
{
	RECT defSize = Render::GetTextSize(Render::Fonts::ESP2, "");
	std::vector<std::string> Info;

	// Player Weapon ESP
	IClientEntity* pWeapon = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)pEntity->GetActiveWeaponHandle());
	if (Menu::Window.VisualsTab.OptionsWeapon.GetIndex() > 0 && pWeapon)
	{
		ClientClass* cClass = (ClientClass*)pWeapon->GetClientClass();
		if (cClass)
		{
			// Draw it
			Info.push_back(CleanItemName(cClass->m_pNetworkName));
		}
	}

	// Bomb Carrier
	if (Menu::Window.VisualsTab.OptionsInfo.GetState() && pEntity == BombCarrier)
	{
		Info.push_back("C4");
	}
	

	RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, pinfo.name);
	ClientClass* cClass = (ClientClass*)pEntity->GetClientClass();
	int i = 0;
	for (auto Text : Info)
	{
		if (Menu::Window.VisualsTab.OptionsWeapon.GetIndex() == 1) {
			// PISTOLS

			// P2000
			if (!strncmp(Text.c_str(), "HKP2000", 8))
				Text.assign("P2000");
			// USP
			if (!strncmp(Text.c_str(), "USP_SILENCER", 13))
				Text.assign("USP-S");
			// GLOCK-18
			if (!strncmp(Text.c_str(), "Glock", 5))
				Text.assign("GLOCK-18");
			// P250
			if (!strncmp(Text.c_str(), "P250", 4))
				Text.assign("P250");
			// TEC-9
			if (!strncmp(Text.c_str(), "Tec9", 4))
				Text.assign("TEC-9");
			// DEAGLE
			if (!strncmp(Text.c_str(), "DEAGLE", 7))
				Text.assign("DEAGLE");

			// SHOTGUNS

				// NOVA
			if (!strncmp(Text.c_str(), "NOVA", 4))
				Text.assign("NOVA");
			// XM1014
			if (!strncmp(Text.c_str(), "XM1014", 6))
				Text.assign("XM1014");
			// MAG-7
			if (!strncmp(Text.c_str(), "Mag7", 4))
				Text.assign("MAG-7");
			// SAWEDOFF
			if (!strncmp(Text.c_str(), "Sawedoff", 9))
				Text.assign("SAWED-OFF");
			// M249
			if (!strncmp(Text.c_str(), "M249", 4))
				Text.assign("M249");
			// NEGEV
			if (!strncmp(Text.c_str(), "Negev", 5))
				Text.assign("NEGEV");

			// SMGS

				// MP9
			if (!strncmp(Text.c_str(), "MP9", 3))
				Text.assign("MP9");
			// MAC-10
			if (!strncmp(Text.c_str(), "MAC10", 5))
				Text.assign("MAC-10");
			// MP7
			if (!strncmp(Text.c_str(), "MP7", 3))
				Text.assign("MP7");
			// UMP-45
			if (!strncmp(Text.c_str(), "UMP45", 5))
				Text.assign("UMP-45");
			// P90
			if (!strncmp(Text.c_str(), "P90", 3))
				Text.assign("P90");
			// PP-BIZON
			if (!strncmp(Text.c_str(), "Bizon", 5))
				Text.assign("PP-BIZON");

			// RIFLES

				// FAMAS
			if (!strncmp(Text.c_str(), "Famas", 5))
				Text.assign("FAMAS");
			// GALIL
			if (!strncmp(Text.c_str(), "Galil", 5))
				Text.assign("GALIL");

			if (!strncmp(Text.c_str(), "GalilAR", 5))
				Text.assign("GALIL");
			// M4A4
			if (!strncmp(Text.c_str(), "M4A1", 4))
				Text.assign("M4A4");
			// AK-47
			if (!strncmp(Text.c_str(), "AK47", 4))
				Text.assign("AK-47");
			// SCOUT
			if (!strncmp(Text.c_str(), "SSG08", 5))
				Text.assign("SCOUT");
			// AUG
			if (!strncmp(Text.c_str(), "Aug", 3))
				Text.assign("AUG");
			// SG553
			if (!strncmp(Text.c_str(), "SG550", 5))
				Text.assign("SG553");

			if (!strncmp(Text.c_str(), "SG552", 5))
				Text.assign("SG553");

			if (!strncmp(Text.c_str(), "SG556", 5))
				Text.assign("SG553");
			// AWP
			if (!strncmp(Text.c_str(), "AWP", 3))
				Text.assign("AWP");
			// SCAR-20
			if (!strncmp(Text.c_str(), "SCAR20", 6))
				Text.assign("SCAR");
			// G3SG1
			if (!strncmp(Text.c_str(), "G3SG1", 5))
				Text.assign("G3SG1");

			// GRENADES
				// HE
			if (!strncmp(Text.c_str(), "HEGRENADE", 10))
				Text.assign("HE");
			// FLASH
			if (!strncmp(Text.c_str(), "FLASHBANG", 10))
				Text.assign("FLASH");
			// MOLOTOV
			if (!strncmp(Text.c_str(), "MOLOTOV", 8))
				Text.assign("MOLOTOV");

			if (!strncmp(Text.c_str(), "INCGRENADE", 11))
				Text.assign("MOLOTOV");
			// DECOY
			if (!strncmp(Text.c_str(), "DECOY", 5))
				Text.assign("DECOY");
			// SMOKE
			if (!strncmp(Text.c_str(), "SMOKEGRENADE", 13))
				Text.assign("SMOKE");

			if (Menu::Window.VisualsTab.OptionsHealth.GetIndex() > 0)
			{
				if (pEntity->IsDefusing()) {
					Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 24, Color(255, 0, 0, 255), Render::Fonts::ESP, Text.c_str());
				}
				else {
					Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 24, Color(255, 255, 255, 255), Render::Fonts::ESP, Text.c_str());
				}
			}
			else
			{
				if (pEntity->IsDefusing()) {
					Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 32, Color(255, 0, 0, 255), Render::Fonts::ESP, Text.c_str());
				}
				else {
					Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 32, Color(255, 255, 255, 255), Render::Fonts::ESP, Text.c_str());
				}
			}

			i++;
		}
		if (Menu::Window.VisualsTab.OptionsWeapon.GetIndex() == 2) {
			if (!strncmp(Text.c_str(), "C4", 2))
				Text.assign("o");

			if (!strncmp(Text.c_str(), "KNIFE_T", 7))
				Text.assign("[");

			if (!strncmp(Text.c_str(), "KNIFE_CT", 8))
				Text.assign("]");

			// PISTOLS

			// P2000
			if (!strncmp(Text.c_str(), "HKP2000", 7))
				Text.assign("E");
			// USP
			if (!strncmp(Text.c_str(), "USP_SILENCER", 12))
				Text.assign("G");
			// GLOCK-18
			if (!strncmp(Text.c_str(), "Glock", 5))
				Text.assign("D");
			// P250
			if (!strncmp(Text.c_str(), "P250", 4))
				Text.assign("F");
			// TEC-9
			if (!strncmp(Text.c_str(), "Tec9", 4))
				Text.assign("H");
			// DEAGLE
			if (!strncmp(Text.c_str(), "DEAGLE", 7))
				Text.assign("A");
			// ELITE
			if (!strncmp(Text.c_str(), "ELITE", 5))
				Text.assign("B");

			// SHOTGUNS

			// NOVA
			if (!strncmp(Text.c_str(), "NOVA", 4))
				Text.assign("e");
			// XM1014
			if (!strncmp(Text.c_str(), "XM1014", 6))
				Text.assign("b");
			// MAG-7
			if (!strncmp(Text.c_str(), "Mag7", 4))
				Text.assign("d");
			// SAWEDOFF
			if (!strncmp(Text.c_str(), "Sawedoff", 9))
				Text.assign("c");
			// M249
			if (!strncmp(Text.c_str(), "M249", 4))
				Text.assign("g");
			// NEGEV
			if (!strncmp(Text.c_str(), "Negev", 5))
				Text.assign("f");

			// SMGS

			// MP9
			if (!strncmp(Text.c_str(), "MP9", 3))
				Text.assign("O");
			// MAC-10
			if (!strncmp(Text.c_str(), "MAC10", 5))
				Text.assign("K");
			// MP7
			if (!strncmp(Text.c_str(), "MP7", 3))
				Text.assign("N");
			// UMP-45
			if (!strncmp(Text.c_str(), "UMP45", 5))
				Text.assign("L");
			// P90
			if (!strncmp(Text.c_str(), "P90", 3))
				Text.assign("P");
			// PP-BIZON
			if (!strncmp(Text.c_str(), "Bizon", 5))
				Text.assign("M");

			// RIFLES

			// FAMAS
			if (!strncmp(Text.c_str(), "Famas", 5))
				Text.assign("R");
			// GALIL
			if (!strncmp(Text.c_str(), "Galil", 5))
				Text.assign("Q");

			if (!strncmp(Text.c_str(), "GalilAR", 5))
				Text.assign("Q");
			// M4A4
			if (!strncmp(Text.c_str(), "M4A1", 4))
				Text.assign("S");
			// AK-47
			if (!strncmp(Text.c_str(), "AK47", 4))
				Text.assign("W");
			// SCOUT
			if (!strncmp(Text.c_str(), "SSG08", 5))
				Text.assign("a");
			// AUG
			if (!strncmp(Text.c_str(), "Aug", 3))
				Text.assign("U");
			// SG553
			if (!strncmp(Text.c_str(), "SG550", 5))
				Text.assign("V");

			if (!strncmp(Text.c_str(), "SG552", 5))
				Text.assign("V");

			if (!strncmp(Text.c_str(), "SG556", 5))
				Text.assign("V");
			// AWP
			if (!strncmp(Text.c_str(), "AWP", 3))
				Text.assign("Z");
			// SCAR-20
			if (!strncmp(Text.c_str(), "SCAR20", 6))
				Text.assign("Y");
			// G3SG1
			if (!strncmp(Text.c_str(), "G3SG1", 5))
				Text.assign("X");

			// GRENADES
			// HE
			if (!strncmp(Text.c_str(), "HEGRENADE", 10))
				Text.assign("j");
			// FLASH
			if (!strncmp(Text.c_str(), "FLASHBANG", 10))
				Text.assign("i");
			// MOLOTOV
			if (!strncmp(Text.c_str(), "MOLOTOV", 8))
				Text.assign("l");

			if (!strncmp(Text.c_str(), "INCGRENADE", 11))
				Text.assign("l");
			// DECOY
			if (!strncmp(Text.c_str(), "DECOY", 5))
				Text.assign("m");
			// SMOKE
			if (!strncmp(Text.c_str(), "SMOKEGRENADE", 13))
				Text.assign("k");

			if (Menu::Window.VisualsTab.OptionsFriendly.GetIndex() == 3) {
				int health = pEntity->GetHealth();

				if (health > 100)
					health = 100;

				int r = 255 - health * 2.55;
				int g = health * 2.55;
				Render::Text(size.x + size.w + 3, size.y + (0 * (defSize.bottom + 2)), Color(r, g, 0, 255), Render::Fonts::ESP2, Text.c_str());
			}
			else {
				Render::Text(size.x + size.w + 3, size.y + (0 * (defSize.bottom + 2)), Color(255, 255, 255, 255), Render::Fonts::ESP2, Text.c_str());
			}

			i++;
		}
	}
}

void CEsp::DrawHealth2(IClientEntity* pEntity, CEsp::ESPBox size)
{
	ESPBox HealthBar = size;
	HealthBar.y += (HealthBar.h + 20);
	HealthBar.h = 4;

	float HealthValue = pEntity->GetHealth();
	float HealthPerc = HealthValue / 100.f;
	float Width = (size.w * HealthPerc);
	HealthBar.w = Width;

	int health = pEntity->GetHealth();

	if (health > 100)
		health = 100;

	int r = 255 - health * 2.55;
	int g = health * 2.55;

	int healthBar = size.h / 100 * health;
	int healthBarDelta = size.h - healthBar;

	//int iClampedHealth = pEntity->GetHealth();
	//  if (iClampedHealth >= 100)
	//  iClampedHealth = 100;

	Render::Outline(size.x - 4, size.y + 1, 1, size.h * 0.01 * health, Color(r, g, 0, 255));
	Render::Outline(size.x - 5, size.y - 1, 3, size.h + 2, Color(0, 0, 0, 150));
}

void CEsp::DrawHealth3(IClientEntity* pEntity, CEsp::ESPBox size)
{
	ESPBox HealthBar = size;
	HealthBar.y += (HealthBar.h + 6);
	HealthBar.h = 4;

	float HealthValue = pEntity->GetHealth();
	float HealthPerc = HealthValue / 100.f;
	float Width = (size.w * HealthPerc);
	HealthBar.w = Width;

	int health = pEntity->GetHealth();

	// --  Main Bar -- //

	Vertex_t Verts[4];
	Verts[0].Init(Vector2D(HealthBar.x, HealthBar.y));
	Verts[1].Init(Vector2D(HealthBar.x + size.w + 5, HealthBar.y));
	Verts[2].Init(Vector2D(HealthBar.x + size.w, HealthBar.y + 5));
	Verts[3].Init(Vector2D(HealthBar.x - 5, HealthBar.y + 5));

	Render::PolygonOutline(4, Verts, Color(0, 0, 0, 150), Color(0, 0, 0, 150));

	Vertex_t Verts2[4];
	Verts2[0].Init(Vector2D(HealthBar.x + 1, HealthBar.y + 1));
	Verts2[1].Init(Vector2D(HealthBar.x + HealthBar.w + 4, HealthBar.y + 1));
	Verts2[2].Init(Vector2D(HealthBar.x + HealthBar.w, HealthBar.y + 5));
	Verts2[3].Init(Vector2D(HealthBar.x - 4, HealthBar.y + 5));

	int r = 255 - health * 2.55;
	int g = health * 2.55;

	Render::Polygon(4, Verts2, Color(r, g, 0, 255));

	Verts2[0].Init(Vector2D(HealthBar.x + 1, HealthBar.y + 1));
	Verts2[1].Init(Vector2D(HealthBar.x + HealthBar.w + 2, HealthBar.y + 1));
	Verts2[2].Init(Vector2D(HealthBar.x + HealthBar.w, HealthBar.y + 2));
	Verts2[3].Init(Vector2D(HealthBar.x - 2, HealthBar.y + 2));

	Render::Polygon(4, Verts2, Color(0, 0, 0, 150));

}

#define Surface2 Interfaces::Surface

void DrawOutlinedRect(int x, int y, int w, int h, Color col)
{
	Surface2->DrawSetColor(col);
	Surface2->DrawOutlinedRect(x, y, x + w, y + h);
}

void DrawOutlinedCircle(int x, int y, int r, Color col)
{
	Surface2->DrawSetColor(col);
	Surface2->DrawOutlinedCircle(x, y, r, 1);
}

void DrawLine(int x0, int y0, int x1, int y1, Color col)
{
	Surface2->DrawSetColor(col);
	Surface2->DrawLine(x0, y0, x1, y1);
}

void CEsp::DrawHealth4(IClientEntity* pEntity, CEsp::ESPBox size)
{
	ESPBox HealthBar = size;
	HealthBar.y += (HealthBar.h + 6);
	HealthBar.h = 4;

	float HealthValue = pEntity->GetHealth();
	float HealthPerc = HealthValue / 100.f;
	float flBoxes = std::ceil(HealthValue / 10.f);
	float Width = (size.w * HealthPerc);
	HealthBar.w = Width;
	float h = (size.h);
	float offset = (h / 4.f) + 5;
	float w = h / 64.f;
	float health = pEntity->GetHealth();
	float flMultiplier = 12 / 360.f; flMultiplier *= flBoxes - 1;
	Color ColHealth = Color::FromHSB(flMultiplier, 1, 1);

	UINT hp = h - (UINT)((h * health) / 100); // Percentage

	int Red = 255 - (health*2.55);
	int Green = health*2.55;
	DrawOutlinedRect((size.x - 6), size.y - 1, 3, h + 2, Color(0, 0, 0, 255));
	DrawOutlinedRect(size.x - 5, size.y - 1, 1, (h / 10) * flBoxes + 1, ColHealth);

	for (int i = 0; i < 10; i++)
	{
		DrawLine((size.x - 6), size.y + i * (h / 10) - 1, size.x - 3, size.y + i * (h / 10), Color::Black());
	}
}

// Little cross on their heads
void CEsp::DrawCross(IClientEntity* pEntity)
{
	Vector cross = pEntity->GetHeadPos(), screen;
	static int Scale = 2;
	if (Render::WorldToScreen(cross, screen))
	{
		Render::Clear(screen.x - Scale, screen.y - (Scale * 2), (Scale * 2), (Scale * 4), Color(20, 20, 20, 160));
		Render::Clear(screen.x - (Scale * 2), screen.y - Scale, (Scale * 4), (Scale * 2), Color(20, 20, 20, 160));
		Render::Clear(screen.x - Scale - 1, screen.y - (Scale * 2) - 1, (Scale * 2) - 2, (Scale * 4) - 2, Color(250, 250, 250, 160));
		Render::Clear(screen.x - (Scale * 2) - 1, screen.y - Scale - 1, (Scale * 4) - 2, (Scale * 2) - 2, Color(250, 250, 250, 160));
	}
}

void CEsp::DrawLinesAA(Color color) {
	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = hackManager.pLocal();

	src3D = hackManager.pLocal()->GetOrigin();

	// REAL AGNEL
	AngleVectors(QAngle(0, AAlineRealAngle, 0), &forward);
	dst3D = src3D + (forward * Menu::Window.SettingsTab.SettingsAALinesRealLenght.GetValue()); //replace 50 with the length you want the line to have

	ray.Init(src3D, dst3D);

	Interfaces::Trace->TraceRay(ray, 0, &filter, &tr);

	if (!Render::WorldToScreen(src3D, src) || !Render::WorldToScreen(tr.endpos, dst))
		return;

	Render::Line(src.x, src.y, dst.x, dst.y, Color(Menu::Window.SettingsTab.SettingsAALinesRealR.GetValue(), Menu::Window.SettingsTab.SettingsAALinesRealG.GetValue(), Menu::Window.SettingsTab.SettingsAALinesRealB.GetValue(), 255));

	// Fake AGNEL
	AngleVectors(QAngle(0, AAlineFakeAngle, 0), &forward);
	dst3D = src3D + (forward * Menu::Window.SettingsTab.SettingsAALinesFakeLenght.GetValue()); //replace 50 with the length you want the line to have

	ray.Init(src3D, dst3D);

	Interfaces::Trace->TraceRay(ray, 0, &filter, &tr);

	if (!Render::WorldToScreen(src3D, src) || !Render::WorldToScreen(tr.endpos, dst))
		return;

	Render::Line(src.x, src.y, dst.x, dst.y, Color(Menu::Window.SettingsTab.SettingsAALinesFakeR.GetValue(), Menu::Window.SettingsTab.SettingsAALinesFakeG.GetValue(), Menu::Window.SettingsTab.SettingsAALinesFakeB.GetValue(), 255));
}

void CEsp::BulletTrace(IClientEntity* pEntity, Color color)
{
	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	AngleVectors(pEntity->GetEyeAngles(), &forward);
	filter.pSkip = pEntity;
	src3D = pEntity->GetBonePos(1) - Vector(0, 0, 0);
	dst3D = src3D + (forward * 85);

	ray.Init(src3D, dst3D);

	Interfaces::Trace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	if (!Render::WorldToScreen(src3D, src) || !Render::WorldToScreen(tr.endpos, dst))
		return;

	Render::Line(src.x, src.y, dst.x, dst.y, color);
	Render::DrawRect(dst.x - 3, dst.y - 3, 6, 6, color);
};

void CEsp::Barrel(CEsp::ESPBox size, Color color, IClientEntity* pEntity)
{
	Vector src3D, src;
	src3D = pEntity->GetOrigin() - Vector(0, 0, 0);

	if (!Render::WorldToScreen(src3D, src))
		return;

	int ScreenWidth, ScreenHeight;
	Interfaces::Engine->GetScreenSize(ScreenWidth, ScreenHeight);

	int x = (int)(ScreenWidth * 0.5f);
	int y = 0;


	y = ScreenHeight / 2;

	Render::Line((int)(src.x), (int)(src.y), x, y, Color(255, 0, 0, 255));
}

// Draws a dropped CS:GO Item
void CEsp::DrawDrop(IClientEntity* pEntity, ClientClass* cClass)
{
	Vector Box;
	CBaseCombatWeapon* Weapon = (CBaseCombatWeapon*)pEntity;
	IClientEntity* plr = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)Weapon->GetOwnerHandle());
	if (!plr && Render::WorldToScreen(Weapon->GetOrigin(), Box))
	{
		if (Menu::Window.VisualsTab.OptionsInfo.GetState())
		{
			std::string ItemName = CleanItemName(cClass->m_pNetworkName);
			RECT TextSize = Render::GetTextSize(Render::Fonts::ESP, ItemName.c_str());
			Render::Text(Box.x - (TextSize.right / 2), Box.y - 16, Color(255, 255, 255, 255), Render::Fonts::ESP, ItemName.c_str());
		}
	}
}

// Draws a chicken
void CEsp::DrawChicken(IClientEntity* pEntity, ClientClass* cClass)
{
	ESPBox Box;

	if (GetBox(pEntity, Box))
	{
		player_info_t pinfo; strcpy_s(pinfo.name, "Chicken");
		if (!Menu::Window.VisualsTab.OptionsBox.GetIndex() == 0)
			DrawBox(Box, Color(255,255,255,255));

		if (Menu::Window.VisualsTab.OptionsName.GetState())
			DrawName(pinfo, Box, pEntity);
	}
}

float damage;
// Draw the planted bomb and timer
void CEsp::DrawBombPlanted(IClientEntity* pEntity, ClientClass* cClass) 
{
	// Null it out incase bomb has been dropped or planted
	BombCarrier = nullptr;

	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	CCSBomb* Bomb = (CCSBomb*)pEntity;

	IClientEntity* pLocal = hackManager.pLocal();

	if (Render::WorldToScreen(vOrig, vScreen))
	{
		if (Menu::Window.VisualsTab.OptionsBox.GetIndex() > 0)
		{
			ESPBox Box;
			GetBox(pEntity, Box);
			DrawBox(Box, Color(240, 0, 0, 255));
		}
		float flBlow = Bomb->GetC4BlowTime();
		float TimeRemaining = flBlow - (Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase());
		float TimeRemaining2;
		bool exploded = true;
		if (TimeRemaining < 0)
		{
			!exploded;

			TimeRemaining2 = 0;
			c4casovac = 0;
		}
		else
		{
			exploded = true;
			TimeRemaining2 = TimeRemaining;
			c4casovac = TimeRemaining;
		}
		char buffer[64];
		char buffer2[64];
		sprintf_s(buffer, "C4: %.1f", TimeRemaining2);
		if (pLocal->IsAlive())
		{
			float flDistance = pLocal->GetEyePosition().DistTo(pEntity->GetEyePosition());
			float a = 450.7f;
			float b = 75.68f;
			float c = 789.2f;
			float d = ((flDistance - b) / c);
			float flDamage = a*exp(-d * d);

			damage = float((std::max)((int)ceilf(CSGO_Armor(flDamage, pLocal->ArmorValue())), 0));

			sprintf_s(buffer2, "Health left: %.0f", pLocal->GetHealth() - damage);
			if (damage >= pLocal->GetHealth())
			{
				Render::Text(vScreen.x, vScreen.y + 10, Color(240, 0, 0, 255), Render::Fonts::ESP, buffer2);
			}
			else if (pLocal->GetHealth() > damage)
			{
				Render::Text(vScreen.x, vScreen.y + 10, Color(0, 240, 0, 255), Render::Fonts::ESP, buffer2);
			}
		}
		Render::Text(vScreen.x, vScreen.y, Color(240, 0, 0, 255), Render::Fonts::ESP, buffer);
	}
}

// Draw the bomb if it's dropped, or store the player who's carrying 
void CEsp::DrawBomb(IClientEntity* pEntity, ClientClass* cClass)
{
	// Null it out incase bomb has been dropped or planted
	BombCarrier = nullptr;
	CBaseCombatWeapon *BombWeapon = (CBaseCombatWeapon *)pEntity;
	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	bool adopted = true;
	HANDLE parent = BombWeapon->GetOwnerHandle();
	if (parent || (vOrig.x == 0 && vOrig.y == 0 && vOrig.z == 0))
	{
		IClientEntity* pParentEnt = (Interfaces::EntList->GetClientEntityFromHandle(parent));
		if (pParentEnt && pParentEnt->IsAlive())
		{
			BombCarrier = pParentEnt;
			adopted = false;
		}
	}

	if (adopted)
	{
		if (Render::WorldToScreen(vOrig, vScreen))
		{
			if (Menu::Window.VisualsTab.OptionsBox.GetIndex() > 0)
			{
				ESPBox Box;
				GetBox(pEntity, Box);
				DrawBox(Box, Color(0, 240, 0, 255));
			}
			Render::Text(vScreen.x, vScreen.y, Color(0, 240, 0, 255), Render::Fonts::ESP, "C4");
		}
	}
}

void DrawBoneArray(int* boneNumbers, int amount, IClientEntity* pEntity, Color color)
{
	Vector LastBoneScreen;
	for (int i = 0; i < amount; i++)
	{
		Vector Bone = pEntity->GetBonePos(boneNumbers[i]);
		Vector BoneScreen;

		if (Render::WorldToScreen(Bone, BoneScreen))
		{
			if (i>0)
			{
				Render::Line(LastBoneScreen.x, LastBoneScreen.y, BoneScreen.x, BoneScreen.y, color);
			}
		}
		LastBoneScreen = BoneScreen;
	}
}

void DrawBoneTest(IClientEntity *pEntity)
{
	for (int i = 0; i < 127; i++)
	{
		Vector BoneLoc = pEntity->GetBonePos(i);
		Vector BoneScreen;
		if (Render::WorldToScreen(BoneLoc, BoneScreen))
		{
			char buf[10];
			_itoa_s(i, buf, 10);
			Render::Text(BoneScreen.x, BoneScreen.y, Color(255, 255, 255, 180), Render::Fonts::ESP, buf);
		}
	}
}

void CEsp::DrawSkeleton(IClientEntity* pEntity)
{
	studiohdr_t* pStudioHdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);

		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = pEntity->GetBonePos(j);
			vParent = pEntity->GetBonePos(pBone->parent);
			if (Render::WorldToScreen(vParent, sParent) && Render::WorldToScreen(vChild, sChild))
			{
				if (Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState()) {
					if (hackManager.pLocal()->GetTeamNum() == 2) {
						if (Menu::Window.VisualsTab.OptionsFriendly.GetIndex() == 3) {
							int health = pEntity->GetHealth();

							if (health > 100)
								health = 100;

							int r = 255 - health * 2.55;
							int g = health * 2.55;
							Render::Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(r, g, 0, 255));
						}
						else {
							Render::Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(Menu::Window.ColorTab.CTVisColorR.GetValue(), Menu::Window.ColorTab.CTVisColorG.GetValue(), Menu::Window.ColorTab.CTVisColorB.GetValue(), 255));
						}
					}
					else {
						if (Menu::Window.VisualsTab.OptionsFriendly.GetIndex() == 3) {
							int health = pEntity->GetHealth();

							if (health > 100)
								health = 100;

							int r = 255 - health * 2.55;
							int g = health * 2.55;
							Render::Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(r, g, 0, 255));
						}
						else {
							Render::Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(Menu::Window.ColorTab.TVisColorR.GetValue(), Menu::Window.ColorTab.TVisColorG.GetValue(), Menu::Window.ColorTab.TVisColorB.GetValue(), 255));
						}
					}
				}
				else {
					Render::Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(255, 255, 255, 255));
				}
			}
		}
	}
}

// Glow ESP
DWORD GlowManager = *(DWORD*)(Utilities::Memory::FindPatternV2("client.dll", "0F 11 05 ?? ?? ?? ?? 83 C8 01 C7 05 ?? ?? ?? ?? 00 00 00 00") + 3);

void CEsp::DrawGlow(Color color)
{
	int GlowA = 255;

	CGlowObjectManager* GlowObjectManager = (CGlowObjectManager*)GlowManager;

	for (int i = 0; i < GlowObjectManager->size; ++i)
	{
		CGlowObjectManager::GlowObjectDefinition_t* glowEntity = &GlowObjectManager->m_GlowObjectDefinitions[i];
		IClientEntity* Entity = glowEntity->getEntity();

		if (glowEntity->IsEmpty() || !Entity)
			continue;

		switch (Entity->GetClientClass()->m_ClassID)
		{

		case 35:
			if (Menu::Window.VisualsTab.OtherGlow.GetState() && Entity->GetClientClass()->m_ClassID == (int)CSGOClassID::CCSPlayer)
			{
				if (Menu::Window.VisualsTab.FiltersPlayers.GetState())
					glowEntity->set(Color(color));
			}
		}
	}
}