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

#pragma once

#include "Hacks.h"

class CEsp : public CHack
{
public:
	void Init();
	void BacktrackingCross(IClientEntity * pEntity);
	void Draw();
	void Move(CUserCmd *pCmd, bool &bSendPacket);
	std::vector<IMaterial*> SkyTextures;
	std::vector<IMaterial*> WorldTextures;
private:

	// Other shit
	IClientEntity *BombCarrier;

	struct ESPBox
	{
		int x, y, w, h;
	};

	// Draw a player
	void NoSky();
	void DoFOV();
	void DrawPlayer(IClientEntity* pEntity, player_info_t pinfo);

	// Get player info
	Color GetPlayerColor(IClientEntity* pEntity);
	bool GetBox(IClientEntity* pEntity, ESPBox &result);
	

	// Draw shit about player
	void SpecList();
	void DrawGlow(Color color);
	void DrawBox(ESPBox size, Color color);
	void DrawFov();
	void DrawMolotov(IClientEntity * pEntity, ClientClass * cClass);
	void DrawSmoke(IClientEntity * pEntity, ClientClass * cClass);
	void DrawDecoy(IClientEntity * pEntity, ClientClass * cClass);
	void DrawHE(IClientEntity * pEntity, ClientClass * cClass);
	void DrawName(player_info_t pinfo, ESPBox size, IClientEntity* pEntity);
	void IsPlayerDefusing(player_info_t pinfo, CEsp::ESPBox size, IClientEntity* pEntity);
	void DrawHealth(IClientEntity* pEntity, ESPBox size);
	void DrawArmor(IClientEntity* pEntity, CEsp::ESPBox size);
	void DrawDistanse(IClientEntity* pEntity, CEsp::ESPBox size);
	float DistanceTo(Vector vecDst, Vector vecSrc);
	void DrawHealth2(IClientEntity* pEntity, ESPBox size);
	void DrawHealth3(IClientEntity* pEntity, ESPBox size);
	void DrawHealth4(IClientEntity* pEntity, ESPBox box);
	void DrawInfo(player_info_t pinfo, IClientEntity* pEntity, ESPBox size);
	void DrawCross(IClientEntity* pEntity);
	void DrawLinesAA(Color color);
	void DrawSkeleton(IClientEntity* pEntity);

	void DrawChicken(IClientEntity* pEntity, ClientClass* cClass);
	void BulletTrace(IClientEntity* pEntity, Color color);
	void Barrel(ESPBox size, Color color, IClientEntity* pEntity);
	void DrawDrop(IClientEntity* pEntity, ClientClass* cClass);
	void DrawBombPlanted(IClientEntity* pEntity, ClientClass* cClass);
	void DrawBomb(IClientEntity* pEntity, ClientClass* cClass);
};

void DiLight();

inline float CSGO_Armor(float flDamage, int ArmorValue)
{
	float flArmorRatio = 0.5f;
	float flArmorBonus = 0.5f;
	if (ArmorValue > 0) {
		float flNew = flDamage * flArmorRatio;
		float flArmor = (flDamage - flNew) * flArmorBonus;

		if (flArmor > static_cast<float>(ArmorValue)) {
			flArmor = static_cast<float>(ArmorValue) * (1.f / flArmorBonus);
			flNew = flDamage - flArmor;
		}

		flDamage = flNew;
	}
	return flDamage;
}