/*
Rest In Peace ApocalypseCheats
*/

#define _CRT_SECURE_NO_WARNINGS

#include "MiscHacks.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include <stdio.h>
#include "string.h"
#include <wchar.h>
#include <time.h>
//#include "Hooks.h"
#include "Hacks.h"
#include "Chams.h"
#include "Menu.h"
#include "ESP.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "MiscHacks.h"
#include "CRC32.h"
#include "Resolver.h"
#include "RageBot.h"
#include "Autowall.h"
#include <iostream>
#include "UTIL Functions.h"
template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

inline float bitsToFloat(unsigned long i)
{
	return *reinterpret_cast<float*>(&i);
}

inline float FloatNegate(float f)
{
	return bitsToFloat(FloatBits(f) ^ 0x80000000);
}

int lyrics = 0;
extern float c4casovac;

Vector AutoStrafeView;

void CMiscHacks::Init()
{
	// Any init
}

void CMiscHacks::Draw()
{
	// Any drawing	
	// Spams
	switch (Menu::Window.MiscTab.OtherChatSpam.GetIndex())
	{
	case 0:
		// No Chat Spam
		break;
	case 1:
		ChatSpamRegular();
		break;
	case 2:
		ChatSpamRegular2();
		break;
	case 3:
		ChatSpamDisperseName();
		break;
	case 4:
		ChatSpamAimwareChat();
		break;
	case 5:
		ChatSpamAimware2Chat();
		break;
	case 6:
		ChatSpamEzFrags();
		break;
	case 7:
		ChatSpamUnityhacks();
		break;
	case 8:
		ChatSpamRegular3();
		break;
	case 9:
		ChatBombTimer();
		break;
	case 10:
		ChatSpamPosition();
		break;
	}
	switch (Menu::Window.MiscTab.OtherLyricsSpam.GetIndex())
	{
	case 0:
		// No Chat Spam
		lyrics = 0;
		break;
	case 1:
		ChatSpamLilPump();
		break;
	case 2:
		ChatSpam2ndHand();
		break;
	}
	switch (Menu::Window.MiscTab.OtherNameSpam.GetIndex())
	{
	case 0:
		// No Name Change
		break;
	case 1:
		ChatSpamAimwareName();
		break;
	case 2:
		ChatSpamAimware3Name();
		break;
	case 3:
		ChatSpamAimware2Name();
		break;
	case 4:
		ChatSpamName();
		break;
	case 5:
		ChatSpamInterwebz();
		break;
	case 6:
		ChatSpamInterwebz5();
		break;
	case 7:
		ChatSpamInterwebz4();
		break;
	case 8:
		ChatSpamKrevetak();
		break;
	case 9:
		ChatSpamxde();
		break;
	case 10:
		ChatSpamtomik();
		break;
	case 11:
		ChatSpamInterwebz2();
		break;
	case 12:
		ChatSpamInterwebz3();
		break;
	case 13:
		ChatSpamCrasher3();
		break;
	}
	switch (Menu::Window.MiscTab.OtherRadioSpam.GetIndex())
	{
	case 0:
		// No Chat Spam
		break;
	case 1:
		RadioSpamRegular();
		break;
	case 2:
		RadioSpamGetOut();
		break;
	case 3:
		RadioSpamRandom();
		break;
	}
}

void CMiscHacks::RotateMovement(CUserCmd* pCmd, float rotation)
{
	rotation = DEG2RAD(rotation);

	float cos_rot = cos(rotation);
	float sin_rot = sin(rotation);

	float new_forwardmove = (cos_rot * pCmd->forwardmove) - (sin_rot * pCmd->sidemove);
	float new_sidemove = (sin_rot * pCmd->forwardmove) + (cos_rot * pCmd->sidemove);

	pCmd->forwardmove = new_forwardmove;
	pCmd->sidemove = new_sidemove;
}

bool CMiscHacks::CircularStrafe(CUserCmd* pCmd, Vector& OriginalView)
{
	if (!(Menu::Window.MiscTab.OtherCircleStrafe.GetState()))
		return false;

	IClientEntity* pLocalEntity = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!pLocalEntity)
		return false;

	if (!pLocalEntity->IsAlive())
		return false;

	CircleFactor++;
	if (CircleFactor > 360)
		CircleFactor = 0;

	int GetItDoubled = 3.0 * CircleFactor - Interfaces::Globals->interval_per_tick;

	Vector StoredViewAngles = pCmd->viewangles;

	int CIRCLEKey = Menu::Window.MiscTab.OtherCircleStrafeKey.GetKey();

	if (CIRCLEKey > 0 && GUI.GetKeyState(CIRCLEKey))
	{
		pCmd->viewangles = OriginalView;
		RotateMovement(pCmd, GetItDoubled);
	}
	return true;
}

void CMiscHacks::Move(CUserCmd *pCmd, bool &bSendPacket)
{
	// Any Move Stuff

	// Circle
	if (Menu::Window.MiscTab.OtherCircleStrafe.GetState())
		CircularStrafe(pCmd, pCmd->viewangles);

	// Bhop
	bool bhop = Menu::Window.MiscTab.OtherAutoJump.GetState();
			AutoJump(pCmd);


   /* if (Menu::Window.MiscTab.OtherAutoCounterStafe.GetState())
			{
				int Key = Menu::Window.MiscTab.OtherAutoCounterStafeKey.GetKey();
				if (Key >= 0 && !GUI.GetKeyState(Key))
				{
					TargetID = -1;
					pTarget = nullptr;
					HitBox = -1;
					return;
				}
			}*/
	

	// AutoStrafe
	Interfaces::Engine->GetViewAngles(AutoStrafeView);
	switch (Menu::Window.MiscTab.OtherAutoStrafe.GetIndex())
	{
	case 0:
		// Off
		break;
	case 1:
		LegitStrafe(pCmd);
		break;
	case 2:
		RageStrafe(pCmd);
		break;
	}

	//Fake Lag
	if (Menu::Window.MiscTab.FakeLagEnable.GetState())
		Fakelag(pCmd, bSendPacket);

	if (Menu::Window.MiscTab.OtherSlowMotion.GetKey())
		SlowMo(pCmd);
}

static __declspec(naked) void __cdecl Invoke_NET_SetConVar(void* pfn, const char* cvar, const char* value)
{
	__asm 
	{
		push    ebp
			mov     ebp, esp
			and     esp, 0FFFFFFF8h
			sub     esp, 44h
			push    ebx
			push    esi
			push    edi
			mov     edi, cvar
			mov     esi, value
			jmp     pfn
	}
}
void DECLSPEC_NOINLINE NET_SetConVar(const char* value, const char* cvar)
{
	static DWORD setaddr = Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x8D\x4C\x24\x1C\xE8\x00\x00\x00\x00\x56", "xxxxx????x");
	if (setaddr != 0) 
	{
		void* pvSetConVar = (char*)setaddr;
		Invoke_NET_SetConVar(pvSetConVar, cvar, value);
	}
}

void change_name(const char* name)
{
	if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected())
		NET_SetConVar(name, "name");
}

void CMiscHacks::AutoJump(CUserCmd *pCmd)
{
	if (Menu::Window.MiscTab.OtherAutoJump.GetState())
	{
		if (pCmd->buttons & IN_JUMP && GUI.GetKeyState(VK_SPACE))
		{
			int iFlags = hackManager.pLocal()->GetFlags();
			if (!(iFlags & FL_ONGROUND))
				pCmd->buttons &= ~IN_JUMP;

			if (hackManager.pLocal()->GetVelocity().Length() <= 50)
			{
				pCmd->forwardmove = 450.f;
			}
		}
	}
}

void CMiscHacks::SlowMo(CUserCmd *pCmd)
{
	int SlowMotionKey = Menu::Window.MiscTab.OtherSlowMotion.GetKey();
	if (SlowMotionKey > 0 && GUI.GetKeyState(SlowMotionKey))
	{
		static bool slowmo;
		slowmo = !slowmo;
		if (slowmo)
		{
			pCmd->tick_count = INT_MAX;
		}
	}
}

/*void CMiscHacks::JumpThrow(CUserCmd *pCmd)
{
	int JumpThrowKey = Menu::Window.MiscTab.OtherJumpThrow.GetKey();
	if (GUI.GetKeyState(JumpThrowKey))
	{
		int iFlags = hackManager.pLocal()->GetFlags();
		if (!(iFlags & FL_ONGROUND))
		{
			pCmd->buttons &= ~IN_JUMP;
			pCmd->buttons &= ~IN_ATTACK;
		}
	}
}*/

void CMiscHacks::LegitStrafe(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();
	if (!(pLocal->GetFlags() & FL_ONGROUND))
	{
		pCmd->forwardmove = 0.0f;

		if (pCmd->mousedx < 0)
		{
			pCmd->sidemove = -450.0f;
		}
		else if (pCmd->mousedx > 0)
		{
			pCmd->sidemove = 450.0f;
		}
	}
}

void CMiscHacks::RageStrafe(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();

	bool bKeysPressed = true;
	if (GUI.GetKeyState(0x41) || GUI.GetKeyState(0x57) || GUI.GetKeyState(0x53) || GUI.GetKeyState(0x44)) bKeysPressed = false;

	if ((GetAsyncKeyState(VK_SPACE) && !(pLocal->GetFlags() & FL_ONGROUND)) && bKeysPressed)
	{
		if (pCmd->mousedx > 1 || pCmd->mousedx < -1) {
			pCmd->sidemove = pCmd->mousedx < 0.f ? -450.f : 450.f;
		}
		else {
			pCmd->forwardmove = (1800.f * 4.f) / pLocal->GetVelocity().Length2D();
			pCmd->sidemove = (pCmd->command_number % 2) == 0 ? -450.f : 450.f;
			if (pCmd->forwardmove > 450.f)
				pCmd->forwardmove = 450.f;
		}
	}
}

Vector GetAutostrafeView()
{
	return AutoStrafeView;
}

// …e Õ½Ê¿
void CMiscHacks::ChatSpamInterwebz()
{
	change_name ("\xe1\xab\xa8 sneakyevil.win");
}

void CMiscHacks::ChatSpamInterwebz4()
{
	char name[154];
	char fucked_char = static_cast<char>(-1);
	for (auto i = 0; i <= 153; i++)
		name[i] = fucked_char;

	const char nick[15] = "problemikk.win";
	memcpy(name, nick, 14);

	change_name(name);
}

void CMiscHacks::ChatSpamInterwebz5()
{
	char name[154];
	char fucked_char = static_cast<char>(-1);
	for (auto i = 0; i <= 153; i++)
		name[i] = fucked_char;

	const char nick[15] = "sneakyevil.xyz";
	memcpy(name, nick, 14);

	change_name(name);
}

void CMiscHacks::ChatSpamxde()
{
	char name[141];
	char fucked_char = static_cast<char>(-1);
	for (auto i = 0; i <= 140; i++)
		name[i] = fucked_char;

	const char nick[4] = "xde";
	memcpy(name, nick, 3);

	change_name(name);
}

void CMiscHacks::ChatSpamtomik()
{
	char name[145];
	char fucked_char = static_cast<char>(-1);
	for (auto i = 0; i <= 144; i++)
		name[i] = fucked_char;

	const char nick[6] = "tomik";
	memcpy(name, nick, 5);

	change_name(name);
}

void CMiscHacks::ChatSpamKrevetak()
{
	change_name("sneakyevil");
}

void CMiscHacks::ChatSpamInterwebz2()
{
	change_name("Your Cheats can't handle this:ʄǟʄǟֆ°°°·.°·..·°¯°·._.·ʄǟֆʄǟʄǟֆʄǟֆ :*\x0A \x0a :*\x0A \x0a :*\x0A \x0a :*\x0A \x0a :*");
}

void CMiscHacks::ChatSpamCrasher3()
{
	char name[158];
	char fucked_char = static_cast<char>(-1);
	for (auto i = 0; i <= 157; i++)
		name[i] = fucked_char;

	const char nick[19] = "sneakyevilhook.win";
	memcpy(name, nick, 18);

	change_name(name);
}

void CMiscHacks::ChatSpamInterwebz3()
{
	change_name("\n Your Cheats can't handle this: j̸̛̛̺̖̏͌̓̈́̓̊̏͒͐̄͐̊̾̋͗͐̆̍̆̄̒̆́̋̓̃͐͊̄̓́́̒̊̔̃̾̾̂͂̓̀́̍̈́̿̾͊̾̂͒̇̀̊̈́̆̂́̍͗́͋̉̏̒́̌̔̿̈͒̄̎͋́͆̀̈́̾̌̓́̀͐̀̕͘̚̚̕͝͝͝͝͠͝͝͝͝͝ơ̴̧̨̧̨̨̢̧̢̨̨̧̨̧̡̡̢̨̢̧̛̛̛̛̛̛̛̛̛͚̞̤̤̼̞̤̜͔̦͇͓͚̦̠͔̬͕͉̫̹͔͓̹̪̪͍̬̗͕̮͚̫̤͙̭͎̹̺̰̠͈̼̤̹̭͖̤͈̟̩̝̻̩̻͇͍̤̲̹̫̭͎̹̤͎̣͔̮̫̹̞̱̰͙̱̘̫̺͓͚̱͔͇̯̪̤̺̻̥̲̤͚̲̤̖̻̰͇͙̮̰̘̬͍̜̩̗̯͉͔̳̹̩͕̱̣͎͓̰̟͓̲͓̩͇̞͇͕̣̣͈͓͚̱̲͍̻̗͈̟͍̝͇̗͖̣̹̻̺̬͙͖͔̟͔͔̯͖̥̲̜͎͖̟̰͚͍̻̹͚͈̞̠͉̘̟̱͔̗͈̫͎̻̮͙̽̍̓̉̒̋͗̀͑͛̑̐̓̐̒͆́̋̾̒͋̄͐͆͋̌̀̔̊̋͑͒̆̓̈́͐̆̋̓́̋͌͋̈́̽͌̎̄̌̈͒͌̊̄͗̀́́̽͐͋̔̐̓͋͊̾́̅͊̎͊͛̓͋͗̍͗̈́͒̀̀̾̅͒͐͆̈́̈̾̅͋̿̂͊̅͒́͒͑̑͆̍͗̾̌͋̅̀͐̑̓̍͆̈́̈́̓̂̓̔̓̅̎͊̔̂̏̍̒͂͋͌͌̄͑͂̽̌͒̓̑̊͐̃̅̈́̃̈̃̈̈̓̐̂̎̒̌̓̆̑͗̒̈́̈͐̀̐͆̅̈́̈́̑̄̆̈́̒͒̾͆̄̾́̒͐̑̅͗̈́̌̈́͂̒̈̔́̓̈̇̒̄̏́̑̏̄̔̅̓̓̐̂͑̿͘̚͘̚̚͘̚̕̕͜͜͜͜͝͝͝͠͝͝͠͝͠͠͝͠͝͝͝͝͠͝ͅͅͅͅḯ̷̛̛͋̆̊̈͛̓̂̉̓̓͊̆̓̇̏͂̆̄̏͋̂̔̆̏̉̆̒̂̎̓̈́͗̒̆̉̇̍͑̅̇͆̉̒͂̿̊͂͗͊̒͛̀̊̃͗̈́̄͗͑͆̏̎̈̎͗̄̆͊͆̅́̆̋̽̊͒͌͗̀̓͑̓̊͂̔̏̏̒̔̿̊̀̇͐͗̓͊̊̈̓͗͛͊̄̚̚̕̚̕̕͝͝͝͝͝͠͝͝͝͝ḩ̵̢̡̢̡̧̨̨̧̧̢̡̡̨̧̧̢̨̨̛̛̛̛̱͓͕͓̖̝͙̩̲̮͉͈̮͔̥̼̝͍̜͇̝̪̞̯̖̹̦͙̹̗̼͕̠̲̳̫̹̟͙̤̹̪̳͙͕̼̰̼̬̠̲̱͓̦̟͕͓͔̳̝̗̜̘̫̬̥̝̯̯̯̙̜̣̺̮̯̜̭͕͎̠͓͎̝͉̱̬͕̝͙̠͙̬̘̦̳̩̱̗̯̫̳̭̗̯̫̱͎̩̤̰̖͓̳̗͕̞̹̺̞̲̱̖̞̜̖̤͔̣̹̯̯̻͖̲̙̙̮̥̼̫̘̥̜̣̗̘̣̱̲͚̱͇̘͖̼̰̣͓͍̭͇͈̟̼̼̜͚͇̼͓̬͙̞̣̯͖̬̳̬̻͓̝͙͙͚̣̘͚̫̙̯͖̜̟̼̦͇̩̩̞̼͈͈̯̻̩̰͚̩͇̦͙̱̺̟̬̪̭̟̬̮͈͓̖̼̜̲̤̱̘̰̙͖̭͇͔̳̮̯͇͇̭̠͍͔͎̤͔͕̞̯̹̪̭̩͖͇͚̫̗̟̞̣͕̱̗̘̪͕̮͕͍̦̭͓͓͕͉̪̯͈̭̲̠͓̱̪͖͔͙͉̲͚͎̣̞͖̳̥̣̤̭̺̟͓̗̫̖͕̩̦̮͚̰̣̯̫͉͈͍̳͚̠̺̦̭͈̙͈̣̜̼̗̬͇̤̟͚͇̤̦̲̽̏̿̓̍̔̌̎̒͑̏̎̑̿̔̀͐̓̈́̌̔̈͌̉̅̄̒̍̽͆́̾̑͒̈́͐̾̊͐̐̃̽͂̎̏͋̃̄̆̋̿̍̐͛̓̅̄̽̈́̿̀͛̍͂̒̋͆͗̔͑͒̈̀̎̍̈̊̌̽̌͒̎̄̀̐̌́̿͗̒̈́̒̍̈́̄̋͑̐̆͂̏̈́͂̑́̆̆̀̓͛͒́̆̒͐̓̋͂͗͊̿͐͌͊̈́̎̑̓͒̑̅̿̇̿̏͑͘̚̕͘̚͜͜͜͜͜͜͜͠͠͠͝͝͠͝͝͝͝͝͠͝͠͝ͅͅͅͅͅͅͅͅ  \n");
}

void CMiscHacks::ChatSpamDisperseName()
{
	static clock_t start_t = clock();
	int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < spamtime + 0.85)
		return;

	static bool holzed = true;

	if (Menu::Window.MiscTab.OtherTeamChat.GetState())
	{
		SayInTeamChat("u can stay afk, i got this");
	}
	else
	{
		int m_iRand = rand() % 4 + 1;

		if (m_iRand == 1) {
			SayInChat("INSERTGANG.XYZ > ALL");
		}
		if (m_iRand == 2) {
			SayInChat("INSERTGANG.XYZ > NEVER RQ");
		}
		if (m_iRand == 3) {
			SayInChat("INSERTGANG.XYZ > YOUR PASTA");
		}
	}

	start_t = clock();
}

void CMiscHacks::ChatSpamName()
{
	static clock_t start_t = clock();
	int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < spamtime)
		return;

	std::vector < std::string > Names;

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		// Get the entity
		IClientEntity *entity = Interfaces::EntList->GetClientEntity(i);

		player_info_t pInfo;
		// If it's a valid entity and isn't the player
		if (entity && hackManager.pLocal()->GetTeamNum() == entity->GetTeamNum() && entity != hackManager.pLocal())
		{
			ClientClass* cClass = (ClientClass*)entity->GetClientClass();

			// If entity is a player
			if (cClass->m_ClassID == (int)CSGOClassID::CCSPlayer)
			{
				if (Interfaces::Engine->GetPlayerInfo(i, &pInfo))
				{
					if (!strstr(pInfo.name, "GOTV"))
						Names.push_back(pInfo.name);
				}
			}
		}
	}

	static bool wasSpamming = true;
	//static std::string nameBackup = "INTERWEBZ.CC";

	int randomIndex = rand() % Names.size();
	char buffer[128];
	sprintf_s(buffer, "%s ", Names[randomIndex].c_str());

	if (wasSpamming)
	{
		change_name(buffer);
	}
	else
	{
		change_name ("p$d 1337");
	}

	start_t = clock();
}

void CMiscHacks::ChatSpamPosition()
{
	static clock_t start_t = clock();
	int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < spamtime)
		return;

	std::vector < std::string > Names;

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		// Get the entity
		IClientEntity *entity = Interfaces::EntList->GetClientEntity(i);

		player_info_t pInfo;
		// If it's a valid entity and isn't the player
		if (entity && hackManager.pLocal()->GetTeamNum() != entity->GetTeamNum())
		{
			ClientClass* cClass = (ClientClass*)entity->GetClientClass();
			// If entity is a player
			if (cClass->m_ClassID == (int)CSGOClassID::CCSPlayer)
			{
				if (Interfaces::Engine->GetPlayerInfo(i, &pInfo))
				{
					if (!strstr(pInfo.name, "GOTV"))
						Names.push_back(pInfo.name);
				}
			}
		}
	}

	static bool wasSpamming = true;

	int randomIndex = rand() % Names.size();
	char buffer1[128]; char buffer2[128]; char buffer3[128]; char buffer4[128]; char buffer5[128]; 
	char buffer6[128]; char buffer7[128]; char buffer8[128]; char buffer9[128]; char buffer10[128];
	sprintf_s(buffer1, "Report for %s submitted, report id 3234888752958537750.", Names[randomIndex].c_str());
	sprintf_s(buffer2, "Report for %s submitted, report id 3234888752789791654.", Names[randomIndex].c_str());
	sprintf_s(buffer3, "Report for %s submitted, report id 3234898749292000794.", Names[randomIndex].c_str());
	sprintf_s(buffer4, "Report for %s submitted, report id 3234888587942080079.", Names[randomIndex].c_str());
	sprintf_s(buffer5, "Report for %s submitted, report id 3234888549825982746.", Names[randomIndex].c_str());
	sprintf_s(buffer6, "Report for %s submitted, report id 3234888756549879424.", Names[randomIndex].c_str());
	sprintf_s(buffer7, "Report for %s submitted, report id 3234888752479847946.", Names[randomIndex].c_str());
	sprintf_s(buffer8, "Report for %s submitted, report id 3234888752958795809.", Names[randomIndex].c_str());
	sprintf_s(buffer9, "Report for %s submitted, report id 3234886879788779721.", Names[randomIndex].c_str());
	sprintf_s(buffer10, "Report for %s submitted, report id 3234888654949724940.", Names[randomIndex].c_str());

	int m_iRand = rand() % 10 + 1;

	if (m_iRand == 1) {
		SayInChat(buffer1);
	}
	if (m_iRand == 2) {
		SayInChat(buffer2);
	}
	if (m_iRand == 3) {
		SayInChat(buffer3);
	}
	if (m_iRand == 4) {
		SayInChat(buffer4);
	}
	if (m_iRand == 5) {
		SayInChat(buffer5);
	}
	if (m_iRand == 6) {
		SayInChat(buffer6);
	}
	if (m_iRand == 7) {
		SayInChat(buffer7);
	}
	if (m_iRand == 8) {
		SayInChat(buffer8);
	}
	if (m_iRand == 9) {
		SayInChat(buffer9);
	}
	if (m_iRand == 10) {
		SayInChat(buffer10);
	}
}

void CMiscHacks::ChatSpamRegular()
{
	static int counter = 0;
	static clock_t start_t = clock();
	int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < spamtime + 0.85)
		return;

	char buffer1[128]; char buffer2[128]; char buffer3[128];
	sprintf_s(buffer1, "I'AM NOT USING sneakyevil.win, YOU ARE JUST BAD.");
	sprintf_s(buffer2, "LEARN HOW TO ACE WITH sneakyevil.win");
	sprintf_s(buffer3, "sneakyevil.win OWNS U AND ALL!");

	if (Menu::Window.MiscTab.OtherTeamChat.GetState())
	{
		int m_iRand = rand() % 3 + 1;

		if (m_iRand == 1) {
			SayInTeamChat(buffer1);
		}
		if (m_iRand == 2) {
			SayInTeamChat(buffer2);
		}
		if (m_iRand == 3) {
			SayInTeamChat(buffer3);
		}
	}
	else
	{
		int m_iRand = rand() % 3 + 1;

		if (m_iRand == 1) {
			SayInChat(buffer1);
		}
		if (m_iRand == 2) {
			SayInChat(buffer2);
		}
		if (m_iRand == 3) {
			SayInChat(buffer3);
		}
	}

	start_t = clock();
}
void CMiscHacks::ChatSpamRegular2()
{
	static int counter = 0;
	static clock_t start_t = clock();
	int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < spamtime + 0.85)
		return;


	char buffer1[128]; char buffer2[128]; char buffer3[128]; char buffer4[128]; char buffer5[128]; char buffer6[128]; char buffer7[128];
	sprintf_s(buffer1, "Think you could do better? Not without sneakyevilhook.win");
	sprintf_s(buffer2, "I'm not cheating, but If I was I would recommend sneakyevilhook.win");
	sprintf_s(buffer3, "sneakyevilhook.win owns me and all!");
	sprintf_s(buffer4, "sneakyevilhook.win - It's ayyware, but it's better than yours!");
	sprintf_s(buffer5, "Git Gud! Git sneakyevilhook.win");
	sprintf_s(buffer6, ">tfw no vac with sneakyevilhook.win");
	sprintf_s(buffer7, "I don't care if i lose, u still get vacced. sneakyevilhook.win - UD Forever");

	if (Menu::Window.MiscTab.OtherTeamChat.GetState())
	{
		int m_iRand = rand() % 7 + 1;

		if (m_iRand == 1) {
			SayInTeamChat(buffer1);
		}
		if (m_iRand == 2) {
			SayInTeamChat(buffer2);
		}
		if (m_iRand == 3) {
			SayInTeamChat(buffer3);
		}
		if (m_iRand == 4) {
			SayInTeamChat(buffer4);
		}
		if (m_iRand == 5) {
			SayInTeamChat(buffer5);
		}
		if (m_iRand == 6) {
			SayInTeamChat(buffer6);
		}
		if (m_iRand == 7) {
			SayInTeamChat(buffer7);
		}
	}
	else
	{
		int m_iRand = rand() % 7 + 1;

		if (m_iRand == 1) {
			SayInChat(buffer1);
		}
		if (m_iRand == 2) {
			SayInChat(buffer2);
		}
		if (m_iRand == 3) {
			SayInChat(buffer3);
		}
		if (m_iRand == 4) {
			SayInChat(buffer4);
		}
		if (m_iRand == 5) {
			SayInChat(buffer5);
		}
		if (m_iRand == 6) {
			SayInChat(buffer6);
		}
		if (m_iRand == 7) {
			SayInChat(buffer7);
		}
	}

	start_t = clock();
}
void CMiscHacks::ChatSpamAimwareChat()
{
	static int counter = 0;
	static clock_t start_t = clock();
	int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < spamtime + 0.85)
		return;

	static bool holzed = true;

	if (Menu::Window.MiscTab.OtherTeamChat.GetState())
	{
		counter = counter + 1;
		if (counter > 5)
			counter = 0;
		SayInTeamChat(tuxlist[counter]);
	}
	else
	{
		counter = counter + 1;
		if (counter > 5)
		counter = 0;
		SayInChat(tuxlist[counter]);
	}

	start_t = clock();
}
void CMiscHacks::ChatSpamEzFrags()
{
	static int counter = 0;
	static clock_t start_t = clock();
	int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < spamtime + 0.85)
		return;

	static bool holzed = true;

	if (Menu::Window.MiscTab.OtherTeamChat.GetState())
	{
		counter = counter + 1;
		if (counter > 5)
			counter = 0;
		SayInTeamChat(ezfragslist[counter]);
	}
	else
	{
		counter = counter + 1;
		if (counter > 5)
			counter = 0;
		SayInChat(ezfragslist[counter]);
	}

	start_t = clock();
}
void CMiscHacks::ChatSpamUnityhacks()
{
	static int counter = 0;
	static clock_t start_t = clock();
	int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < spamtime + 0.85)
		return;

	static bool holzed = true;

	if (Menu::Window.MiscTab.OtherTeamChat.GetState())
	{
		counter = counter + 1;
		if (counter > 17)
			counter = 0;
		SayInTeamChat(unitylist[counter]);
	}
	else
	{
		counter = counter + 1;
		if (counter > 17)
			counter = 0;
		SayInChat(unitylist[counter]);
	}

	start_t = clock();
}
void CMiscHacks::ChatSpamAimware2Chat()
{
	// Don't spam it too fast so you can still do stuff
	static clock_t start_t = clock();
	int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < spamtime + 0.45)
		return;

	static bool holzed = true;

	SayInChat("www.AIMWARE.net | Premium CSGO Cheat");

	start_t = clock();
}
void CMiscHacks::ChatSpamRegular3()
{
	// Don't spam it too fast so you can still do stuff
	static clock_t start_t = clock();
	int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < spamtime + 0.85)
		return;

	static bool holzed = true;

	if (Menu::Window.MiscTab.OtherTeamChat.GetState())
	{
		SayInTeamChat("QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ");
	}
	else
	{
		SayInChat("QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ");
	}

	start_t = clock();
}
void CMiscHacks::ChatBombTimer()
{
	// Don't spam it too fast so you can still do stuff
	static clock_t start_t = clock();
	int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 1)
		return;

	static bool holzed = true;
	
	if (c4casovac < 1)
	{

	}
	else
	{
		char aaaa[64];
		sprintf_s(aaaa, "[sneakyevilhook.win] C4 Explode Time: %.1f", c4casovac);
		if (Menu::Window.MiscTab.OtherTeamChat.GetState())
		{
			SayInTeamChat(aaaa);
		}
		else
		{
			SayInChat(aaaa);
		}
	}
	c4casovac = 0;

	start_t = clock();
}
void CMiscHacks::ChatSpamAimwareName()
{	
	change_name("\n");
}

void CMiscHacks::ChatSpamAimware2Name()
{
	change_name("\n sneakyevilhook.win \n");
}

void CMiscHacks::ChatSpamAimware3Name()
{
	char name[141];
	char fucked_char = static_cast<char>(-1);
	for (auto i = 0; i <= 140; i++)
		name[i] = fucked_char;

	const char nick[2] = "\n";
	memcpy(name, nick, 1);

	change_name(name);
}

void CMiscHacks::ChatSpamTestName()
{
	change_name("\x01\x0B\x02""M""\x05""E""\x02""R""\x05""R""\x02""Y ""\x05""C""\x02""H""\x05""R""\x02""I""\x05""S""\x02""T""\x05""M""\x02""A""\x05""S""\x03");
}

void CMiscHacks::NoName()
{
	change_name("");
}

void CMiscHacks::RadioSpamRegular()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.95)
		return;

	static bool holzed = true;

	if (Menu::Window.MiscTab.OtherTeamChat.GetState())
	{
		int m_iRand = rand() % 2 + 1;

		if (m_iRand == 1) {
			Interfaces::Engine->ClientCmd_Unrestricted("roger");
		}
		if (m_iRand == 2) {
			Interfaces::Engine->ClientCmd_Unrestricted("negative");
		}
	}
	else
	{
		int m_iRand = rand() % 2 + 1;

		if (m_iRand == 1) {
			Interfaces::Engine->ClientCmd_Unrestricted("roger");
		}
		if (m_iRand == 2) {
			Interfaces::Engine->ClientCmd_Unrestricted("negative");
		}
	}

	start_t = clock();
}

void CMiscHacks::RadioSpamGetOut()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.95)
		return;

	static bool holzed = true;

	if (Menu::Window.MiscTab.OtherTeamChat.GetState())
	{
		int m_iRand = rand() % 2 + 1;

		if (m_iRand == 1) {
			Interfaces::Engine->ClientCmd_Unrestricted("getout");
		}
		if (m_iRand == 2) {
			Interfaces::Engine->ClientCmd_Unrestricted("getout");
		}
	}
	else
	{
		int m_iRand = rand() % 2 + 1;

		if (m_iRand == 1) {
			Interfaces::Engine->ClientCmd_Unrestricted("getout");
		}
		if (m_iRand == 2) {
			Interfaces::Engine->ClientCmd_Unrestricted("getout");
		}
	}

	start_t = clock();
}

void CMiscHacks::RadioSpamRandom()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.95)
		return;

	static bool holzed = true;

	if (Menu::Window.MiscTab.OtherTeamChat.GetState())
	{
		int m_iRand = rand() % 19 + 1;

		if (m_iRand == 1) {
			Interfaces::Engine->ClientCmd_Unrestricted("coverme");
		}
		if (m_iRand == 2) {
			Interfaces::Engine->ClientCmd_Unrestricted("takepoint");
		}
		if (m_iRand == 3) {
			Interfaces::Engine->ClientCmd_Unrestricted("holdpos");
		}
		if (m_iRand == 4) {
			Interfaces::Engine->ClientCmd_Unrestricted("regroup");
		}
		if (m_iRand == 5) {
			Interfaces::Engine->ClientCmd_Unrestricted("followme");
		}
		if (m_iRand == 6) {
			Interfaces::Engine->ClientCmd_Unrestricted("takingfire");
		}
		if (m_iRand == 7) {
			Interfaces::Engine->ClientCmd_Unrestricted("go");
		}
		if (m_iRand == 8) {
			Interfaces::Engine->ClientCmd_Unrestricted("fallback");
		}
		if (m_iRand == 9) {
			Interfaces::Engine->ClientCmd_Unrestricted("sticktog");
		}
		if (m_iRand == 10) {
			Interfaces::Engine->ClientCmd_Unrestricted("report");
		}
		if (m_iRand == 11) {
			Interfaces::Engine->ClientCmd_Unrestricted("roger");
		}
		if (m_iRand == 12) {
			Interfaces::Engine->ClientCmd_Unrestricted("enemyspot");
		}
		if (m_iRand == 13) {
			Interfaces::Engine->ClientCmd_Unrestricted("needbackup");
		}
		if (m_iRand == 14) {
			Interfaces::Engine->ClientCmd_Unrestricted("sectorclear");
		}
		if (m_iRand == 15) {
			Interfaces::Engine->ClientCmd_Unrestricted("inposition");
		}
		if (m_iRand == 16) {
			Interfaces::Engine->ClientCmd_Unrestricted("reportingin");
		}
		if (m_iRand == 17) {
			Interfaces::Engine->ClientCmd_Unrestricted("getout");
		}
		if (m_iRand == 18) {
			Interfaces::Engine->ClientCmd_Unrestricted("negative");
		}
		if (m_iRand == 19) {
			Interfaces::Engine->ClientCmd_Unrestricted("enemydown");
		}
	}
	else
	{
		int m_iRand = rand() % 19 + 1;

		if (m_iRand == 1) {
			Interfaces::Engine->ClientCmd_Unrestricted("coverme");
		}
		if (m_iRand == 2) {
			Interfaces::Engine->ClientCmd_Unrestricted("takepoint");
		}
		if (m_iRand == 3) {
			Interfaces::Engine->ClientCmd_Unrestricted("holdpos");
		}
		if (m_iRand == 4) {
			Interfaces::Engine->ClientCmd_Unrestricted("regroup");
		}
		if (m_iRand == 5) {
			Interfaces::Engine->ClientCmd_Unrestricted("followme");
		}
		if (m_iRand == 6) {
			Interfaces::Engine->ClientCmd_Unrestricted("takingfire");
		}
		if (m_iRand == 7) {
			Interfaces::Engine->ClientCmd_Unrestricted("go");
		}
		if (m_iRand == 8) {
			Interfaces::Engine->ClientCmd_Unrestricted("fallback");
		}
		if (m_iRand == 9) {
			Interfaces::Engine->ClientCmd_Unrestricted("sticktog");
		}
		if (m_iRand == 10) {
			Interfaces::Engine->ClientCmd_Unrestricted("report");
		}
		if (m_iRand == 11) {
			Interfaces::Engine->ClientCmd_Unrestricted("roger");
		}
		if (m_iRand == 12) {
			Interfaces::Engine->ClientCmd_Unrestricted("enemyspot");
		}
		if (m_iRand == 13) {
			Interfaces::Engine->ClientCmd_Unrestricted("needbackup");
		}
		if (m_iRand == 14) {
			Interfaces::Engine->ClientCmd_Unrestricted("sectorclear");
		}
		if (m_iRand == 15) {
			Interfaces::Engine->ClientCmd_Unrestricted("inposition");
		}
		if (m_iRand == 16) {
			Interfaces::Engine->ClientCmd_Unrestricted("reportingin");
		}
		if (m_iRand == 17) {
			Interfaces::Engine->ClientCmd_Unrestricted("getout");
		}
		if (m_iRand == 18) {
			Interfaces::Engine->ClientCmd_Unrestricted("negative");
		}
		if (m_iRand == 19) {
			Interfaces::Engine->ClientCmd_Unrestricted("enemydown");
		}
	}

	start_t = clock();
}

void CMiscHacks::ChatSpamLilPump()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.0918)
		return;

	if (lyrics == 0) {
		SayInChat("Move them bricks to the bando like, ouu");
	}
	if (lyrics == 16) {
		SayInChat("Move them bricks to the bando like, ouu");
	}
	if (lyrics == 33) {
		SayInChat("Diamond on my wrist and it look like glue");
	}
	if (lyrics == 50) {
		SayInChat("Diamond on my wrist and it look like glue");
	}
	if (lyrics == 67) {
		SayInChat("Walk in the club and I flex like ouu");
	}
	if (lyrics == 84) {
		SayInChat("Walk in the club and I flex like ouu");
	}
	if (lyrics == 100) {
		SayInChat("Maison Margiela all over my shoe");
	}
	if (lyrics == 117) {
		SayInChat("Maison Margiela all over my shoe");
	}
	if (lyrics == 134) {
		SayInChat("Pull up swerving in a coupe");
	}
	if (lyrics == 151) {
		SayInChat("Pull up swerving in a coupe");
	}
	if (lyrics == 168) {
		SayInChat("Pull up swerving in a coupe");
	}
	if (lyrics == 185) {
		SayInChat("Pull up swerving in a coupe");
	}
	if (lyrics == 202) {
		SayInChat("Move them bricks to the bando like, ouu");
	}
	if (lyrics == 218) {
		SayInChat("Diamond on my wrist and it look like glue");
	}
	if (lyrics == 236) {
		SayInChat("Walk in the club and I flex like, ouu");
	}
	if (lyrics == 253) {
		SayInChat("Maison Margiela all over my shoe");
	}
	if (lyrics == 269) {
		SayInChat("Made a couple bands in the kitchen (kitchen)");
	}
	if (lyrics == 286) {
		SayInChat("Made a couple bands water whippin' (brrr)");
	}
	if (lyrics == 303) {
		SayInChat("Take a nigga bitch, go missing, oou");
	}
	if (lyrics == 320) {
		SayInChat("Bitch I ball like a piston, ouu");
	}
	if (lyrics == 337) {
		SayInChat("Bitch I flex Ric Flair, damn");
	}
	if (lyrics == 357) {
		SayInChat("All they do is stare, ouu");
	}
	if (lyrics == 370) {
		SayInChat("Lil Pump really don't care, no");
	}
	if (lyrics == 387) {
		SayInChat("Told a bih lil pump yeah, ouu");
	}
	if (lyrics == 404) {
		SayInChat("Everybody know I'm up next, yeah");
	}
	if (lyrics == 421) {
		SayInChat("Everybody know I flex, ouu");
	}
	if (lyrics == 438) {
		SayInChat("I run through these checks, ouu");
	}
	if (lyrics == 455) {
		SayInChat("I flex on my ex, ouu");
	}
	if (lyrics == 472) {
		SayInChat("Left 4 bands in the trap (trap)");
	}
	if (lyrics == 489) {
		SayInChat("Left 4 bands in the trap (four)");
	}
	if (lyrics == 506) {
		SayInChat("Smokin' on loud pack, ouu");
	}
	if (lyrics == 523) {
		SayInChat("Smokin' on loud pack, ouu");
	}
	if (lyrics == 540) {
		SayInChat("Move them bricks to the bando like, ouu");
	}
	if (lyrics == 556) {
		SayInChat("Move them bricks to the bando like, ouu");
	}
	if (lyrics == 573) {
		SayInChat("Diamond on my wrist and it look like glue");
	}
	if (lyrics == 590) {
		SayInChat("Diamond on my wrist and it look like glue");
	}
	if (lyrics == 607) {
		SayInChat("Walk in the club and I flex like ouu");
	}
	if (lyrics == 624) {
		SayInChat("Walk in the club and I flex like ouu");
	}
	if (lyrics == 641) {
		SayInChat("Maison Margiela all over my shoe");
	}
	if (lyrics == 658) {
		SayInChat("Maison Margiela all over my shoe");
	}
	if (lyrics == 675) {
		SayInChat("Pull up swerving in a coupe");
	}
	if (lyrics == 691) {
		SayInChat("Pull up swerving in a coupe");
	}
	if (lyrics == 709) {
		SayInChat("Pull up swerving in a coupe");
	}
	if (lyrics == 726) {
		SayInChat("Pull up swerving in a coupe");
	}
	if (lyrics == 742) {
		SayInChat("Move them bricks to the bando like, ouu");
	}
	if (lyrics == 759) {
		SayInChat("Diamond on my wrist and it look like glue");
	}
	if (lyrics == 776) {
		SayInChat("Walk in the club and I flex like, ouu");
	}
	if (lyrics == 793) {
		SayInChat("Maison Margiela all over my shoe");
	}

	lyrics++;

	start_t = clock();
}

void CMiscHacks::ChatSpam2ndHand()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.089)
		return;

	// Verse 1
	if (lyrics == 0) {
		SayInChat("All I see is scrubs");
	}
	if (lyrics == 20) {
		SayInChat("Face mean mugged");
	}
	if (lyrics == 39) {
		SayInChat("On a couple drugs");
	}
	if (lyrics == 55) {
		SayInChat("Smoke with migo plug");
	}
	if (lyrics == 73) {
		SayInChat("FTP my blood");
	}
	if (lyrics == 90) {
		SayInChat("Grey the fucking gang");
	}
	if (lyrics == 108) {
		SayInChat("Boy, stay in your lane");
	}

	// Verse 2
	if (lyrics == 120) {
		SayInChat("Never high, just maintain");
	}
	if (lyrics == 139) {
		SayInChat("Hate this fame, hate this game");
	}
	if (lyrics == 156) {
		SayInChat("Take these shoes, last a day");
	}
	if (lyrics == 174) {
		SayInChat("Oil flowing through my veins");
	}
	if (lyrics == 190) {
		SayInChat("Lil Remains, that make that stain");
	}
	if (lyrics == 207) {
		SayInChat("Y'all look good in cellophane");
	}
	if (lyrics == 224) {
		SayInChat("Mane, insane, real blade");
	}
	if (lyrics == 244) {
		SayInChat("Swing it through your fucking brain");
	}
	if (lyrics == 257) {
		SayInChat("I said fuck 'em, we got a problem?");
	}
	if (lyrics == 288) {
		SayInChat("Fuck you gonna do with it? I think nada");
	}
	if (lyrics == 313) {
		SayInChat("Just a persona (okay), and I'm Nirvana (Cobain)");
	}
	if (lyrics == 348) {
		SayInChat("Popping xans to make you mad, I know you want some");
	}
	if (lyrics == 382) {
		SayInChat("Rolling rolling rolling rolling rolling rolling rolling");
	}
	if (lyrics == 413) {
		SayInChat("Got an ounce of the coke, white snow floating");
	}
	if (lyrics == 447) {
		SayInChat("River road shawty, Yung Christ, one chosen");
	}
	if (lyrics == 480) {
		SayInChat("Slick tote the AK-47 stolen, woadie joscing");
	}

	// Verse 3
	if (lyrics == 517) {
		SayInChat("All of y'all are over-saturating 'Preme and Bape");
	}
	if (lyrics == 547) {
		SayInChat("Bitch it's FTP all fucking day, all y'all look the fucking same");
	}
	if (lyrics == 568) {
		SayInChat("Fuck the fame, fuck a lane");
	}
	if (lyrics == 582) {
		SayInChat("Bitch I'm bout to swerve into the toll booth and scrape the change");
	}
	if (lyrics == 622) {
		SayInChat("Bring out your dead and blaming it on Yung Plague");
	}
	if (lyrics == 649) {
		SayInChat("Bitch I pull up with the K");
	}
	if (lyrics == 667) {
		SayInChat("K9 by my right leg 'till I say OK");
	}
	if (lyrics == 700) {
		SayInChat("K9 on his hind legs ripping off your face");
	}
	if (lyrics == 735) {
		SayInChat("All I gotta say is G*59 until the grave");
	}
	if (lyrics == 763) {
		SayInChat("One color (one color)");
	}
	if (lyrics == 781) {
		SayInChat("Two numbers (two numbers)");
	}
	if (lyrics == 796) {
		SayInChat("Three fingers in the sky amongst the thunder");
	}
	if (lyrics == 828) {
		SayInChat("Fuck the others I'ma");
	}
	if (lyrics == 845) {
		SayInChat("Bring the ruckus");
	}
	if (lyrics == 860) {
		SayInChat("Fuck your bitch, she swears she suffers under the covers");
	}
	if (lyrics == 888) {
		SayInChat("I'ma-I'ma-I'ma hell hound, hell bound, shawty wanna die");
	}
	if (lyrics == 941) {
		SayInChat("I'ma sell pounds, sell pounds, shawty said she might");
	}
	if (lyrics == 973) {
		SayInChat("Let them bells pound, bells pound, shawty wanna cry");
	}
	if (lyrics == 1007) {
		SayInChat("I'm like fuck it every night");
	}
	if (lyrics == 1022) {
		SayInChat("Keep turning black, I close my eyes");
	}

	lyrics++;

	start_t = clock();
}

void CMiscHacks::Fakelag(CUserCmd *pCmd, bool &bSendPacket)
{
	int iChoke = Menu::Window.MiscTab.FakeLagChoke.GetValue();

	static int iFakeLag = -1;
	iFakeLag++;

	if (iFakeLag <= iChoke && iFakeLag > -1)
	{
		bSendPacket = false;
	}
	else
	{
		bSendPacket = true;
		iFakeLag = -1;
	}
}
