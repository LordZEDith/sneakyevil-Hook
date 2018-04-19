/*
Rest In Peace ApocalypseCheats
*/

#pragma once

#include "Hacks.h"

Vector GetAutostrafeView();

class CMiscHacks : public CHack
{
public:
	void Init();
	void Draw();
	void RotateMovement(CUserCmd * pCmd, float rotation);
	bool CircularStrafe(CUserCmd * pCmd, Vector & OriginalView);
	void Move(CUserCmd *pCmd, bool &bSendPacket);
	void CustomName();
private:
	void AutoJump(CUserCmd *pCmd);
	void JumpThrow(CUserCmd *pCmd);
	void AutoJumpSMAC(CUserCmd *pCmd);
	void EdgeJump(CUserCmd *pCmd);
	void SlowMo(CUserCmd * pCmd);
	void Walkbot(CUserCmd *pCmd);
	void PerfectStrafe(CUserCmd *pCmd);
	void LegitStrafe(CUserCmd *pCmd);
	void RageStrafe(CUserCmd *pCmd);
	int CircleFactor = 0;
	void ChatSpamKrevetak();
	void ChatSpamInterwebz();
	void ChatSpamInterwebz2();
	void ChatSpamCrasher3();
	void ChatSpamCasual();
	void ChatSpamInterwebz3();
	void ChatSpamInterwebz4();
	void ChatSpamInterwebz5();
	void ChatSpamxde();
	void ChatSpamtomik();
	void ChatSpamName();
	void ChatSpamName2();
	void ChatSpamDisperseName();
	void ChatSpamPosition();
	void ChatSpamPos();
	void ChatSpamRegular();
	void ChatSpamRegular2();
	void ChatSpamAimwareChat();
	void ChatSpamEzFrags();
	void ChatSpamUnityhacks();
	void ChatSpamAimware2Chat();
	void ChatSpamRegular3();
	void ChatBombTimer();
	void ChatSpamTime();
	void ChatSpamAimwareName();
	void ChatSpamAimware2Name();
	void ChatSpamAimware3Name();
	void ChatSpamTestName();
	void RadioSpamRegular();
	void RadioSpamGetOut();
	void RadioSpamRandom();
	void ChatSpamReports();
	void ChatSpamCallout();
	void NoName();
	//void ChatSpamAchievement();
	//void NameDispersion();
	//void ChatSpamAchievement();
	void ChatSpamLilPump();
	void ChatSpam2ndHand();
	void Fakelag(CUserCmd *pCmd, bool &bSendPacket);

	//std::string GetNameChange();

	/*void ChatSpam();
	void Namechanger();

	std::string GetRegularSpam();
	std::string GetCustomSpam();
	std::string GetPlayerSpam();
	std::string GetSinglePlayerSpam();

	std::string GetNameChange();
	std::string GetNameCalloutChange();*/
};

char* const tuxlist[] =
{
	"AimTux owns me and all",
	"Your Windows p2c sucks my AimTux dry",
	"It's free as in FREEDOM!",
	"Tux only let me out so I could play this game, please be nice!",
	"Tux nutted but you keep sucken",
	">tfw no vac on Linux",
};
char* const ezfragslist[] =
{
	"Think you could do better? Not without www.EZFrags.co.uk",
	"If I was cheating, I'd use www.EZFrags.co.uk",
	"I'm not using www.EZFrags.co.uk, you're just bad",
	"Visit www.EZFrags.co.uk for the finest public & private CS:GO cheats",
	"Stop being a noob! Get good with www.EZFrags.co.uk",
	"You just got pwned by www.EZFrags.co.uk, the #1 CS:GO cheat",
};
char* const unitylist[] =
{
	"How much did you tag that wall for ? ?",
	"This server is VAC secured, no aimware users here.Buy Unityhacks.com",
	"Your Brett got delivered by Unityhacks.com",
	"I don't cheat, I am just smurfing with Unityhacks.com",
	"I can hear you crouching, you think i hack ? You are God damn right Son I use Unityhacks.com",
	"server cvar 'sv_rekt' changed to 1",
	"You think you can kill me ? You are Funny.Not without Unityhacks.com dude!",
	"I thought I put bots on hard, why are they on easy ?",
	"Unityhacks > all refund your copy paste trash",
	"Unityhacks.com OWNS ME AND ALL",
	"you guys buy accounts on ebay ? ? Go buy them on Unityhacks.com it's much cheaper",
	"You have the reaction time of a dead puppy.Buy Unityhacks.com to improve",
	"If I wanted to suicide, i'd jump from your ego to your elo, just get good and buy Unityhacks.com",
	"is that a decoy, or are you trying to shoot somebody ?",
	"Don't cry and say I'll get banned, it's just annoying. I wont get banned anyways with",
	"Unityhacks.com",
	"Nice $4750 decoy you just bought.Buy Unityhacks.com to finally hit your shots.",
	"Even Noah can't carry these animals without Unityhacks.com..",
};



