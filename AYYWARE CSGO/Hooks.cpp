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

#include "Hooks.h"
#include "Hacks.h"
#include "Chams.h"
#include "NadePredict.h"
#include "Menu.h"
#include <intrin.h>
#include "Controls.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "MiscHacks.h"
#include "CRC32.h"
#include "EdgyLagComp.h"
#include "Resolver.h"
#include "Utilities.h"
#pragma comment(lib, "Winmm.lib")
#include <string>;;;;;;;;;;;;;;;

DamageEventListener* DamageListener;
DamageEventListener* DeathListener;
DamageEventListener* RoundStartListener;

int Skin;
extern float AAlineLBYAngle;
extern float AAlineRealAngle;
extern float AAlineFakeAngle;
extern int Shooted;
extern int xmas;

Vector LastAngleAA;
#define STUDIO_NONE						0x00000000
#define STUDIO_RENDER					0x00000001
#define STUDIO_VIEWXFORMATTACHMENTS		0x00000002
#define STUDIO_DRAWTRANSLUCENTSUBMODELS 0x00000004
#define STUDIO_TWOPASS					0x00000008
#define STUDIO_STATIC_LIGHTING			0x00000010
#define STUDIO_WIREFRAME				0x00000020
#define STUDIO_ITEM_BLINK				0x00000040
#define STUDIO_NOSHADOWS				0x00000080
#define STUDIO_WIREFRAME_VCOLLIDE		0x00000100
#define STUDIO_NOLIGHTING_OR_CUBEMAP	0x00000200
#define STUDIO_SKIP_FLEXES				0x00000400
#define STUDIO_DONOTMODIFYSTENCILSTATE	0x00000800	// TERROR

// Not a studio flag, but used to flag model as a non-sorting brush model
#define STUDIO_TRANSPARENCY				0x80000000

// Not a studio flag, but used to flag model as using shadow depth material override
#define STUDIO_SHADOWDEPTHTEXTURE		0x40000000

// Not a studio flag, but used to flag model as doing custom rendering into shadow texture
#define STUDIO_SHADOWTEXTURE			0x20000000

#define STUDIO_SKIP_DECALS				0x10000000
#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))

// Funtion Typedefs
typedef void(__thiscall* DrawModelEx_)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4*);
typedef void(__thiscall* PaintTraverse_)(PVOID, unsigned int, bool, bool);
typedef bool(__thiscall* InPrediction_)(PVOID);
typedef void(__stdcall *FrameStageNotifyFn)(ClientFrameStage_t);
typedef void(__thiscall* RenderViewFn)(void*, CViewSetup&, CViewSetup&, int, int);

using OverrideViewFn = void(__fastcall*)(void*, void*, CViewSetup*);
typedef float(__stdcall *oGetViewModelFOV)();

extern int c4_defuse;
extern int pc;
float realtime;

// Function Pointers to the originals
PaintTraverse_ oPaintTraverse;
DrawModelEx_ oDrawModelExecute;
FrameStageNotifyFn oFrameStageNotify;
OverrideViewFn oOverrideView;
RenderViewFn oRenderView;

// Hook function prototypes
void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
bool __stdcall Hooked_InPrediction();
void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld);
bool __stdcall CreateMoveClient_Hooked(/*void* self, int edx,*/ float frametime, CUserCmd* pCmd);
void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage);
void __fastcall Hooked_OverrideView(void* ecx, void* edx, CViewSetup* pSetup);
float __stdcall GGetViewModelFOV();
void __fastcall Hooked_RenderView(void* ecx, void* edx, CViewSetup &setup, CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw);

std::string GetTimeString()
{
	time_t current_time;
	struct tm *time_info;
	static char timeString[10];
	time(&current_time);
	time_info = localtime(&current_time);
	strftime(timeString, sizeof(timeString), "%X", time_info);
	return timeString;
}

// VMT Managers
namespace Hooks
{
	// VMT Managers
	Utilities::Memory::VMTManager VMTPanel; // Hooking drawing functions
	Utilities::Memory::VMTManager VMTClient; // Maybe CreateMove
	Utilities::Memory::VMTManager VMTClientMode; // CreateMove for functionality
	Utilities::Memory::VMTManager VMTModelRender; // DrawModelEx for chams
	Utilities::Memory::VMTManager VMTPrediction; // InPrediction for no vis recoil
	Utilities::Memory::VMTManager VMTPlaySound; // Autoaccept 
	Utilities::Memory::VMTManager VMTRenderView;
};

/*// Initialise all our hooks
void Hooks::Initialise()
{
// Panel hooks for drawing to the screen via surface functions
VMTPanel.Initialise((DWORD*)Interfaces::Panels);
oPaintTraverse = (PaintTraverse_)VMTPanel.HookMethod((DWORD)&PaintTraverse_Hooked, Offsets::VMT::Panel_PaintTraverse);
//Utilities::Log("Paint Traverse Hooked");

// No Visual Recoil
VMTPrediction.Initialise((DWORD*)Interfaces::Prediction);
VMTPrediction.HookMethod((DWORD)&Hooked_InPrediction, 14);
//Utilities::Log("InPrediction Hooked");

// Chams
VMTModelRender.Initialise((DWORD*)Interfaces::ModelRender);
oDrawModelExecute = (DrawModelEx_)VMTModelRender.HookMethod((DWORD)&Hooked_DrawModelExecute, Offsets::VMT::ModelRender_DrawModelExecute);
//Utilities::Log("DrawModelExecute Hooked");

// Setup ClientMode Hooks
//VMTClientMode.Initialise((DWORD*)Interfaces::ClientMode);
//VMTClientMode.HookMethod((DWORD)&CreateMoveClient_Hooked, 24);
//Utilities::Log("ClientMode CreateMove Hooked");

// Setup client hooks
VMTClient.Initialise((DWORD*)Interfaces::Client);
oCreateMove = (CreateMoveFn)VMTClient.HookMethod((DWORD)&hkCreateMove, 21);
}*/

static float saveLastHeadshotFloat[65];
static float saveLastBaimFloat[65];
static float saveLastBaim30Float[65];

static float saveLastBaim10Float[65];

int hitmarkertime = 0;

static int hittedLogHits[65];
static int missedLogHits[65];

void hitmarker(IGameEvent* pEvent)
{
	if (Menu::Window.SettingsTab.OtherSettingsHitmarkerSound.GetState())
	{
		int attackerid = pEvent->GetInt("attacker");
		int userid = pEvent->GetInt("userid");
		int dmg = pEvent->GetInt("dmg_health");
		int hitgroup = pEvent->GetInt("hitgroup");

		IClientEntity* pEntity = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetPlayerForUserID(userid));
		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		if (Interfaces::Engine->GetPlayerForUserID(attackerid) == Interfaces::Engine->GetLocalPlayer())
		{
			hitmarkertime = 255;
			PlaySound("C:\\hitmarker.wav", NULL, SND_FILENAME | SND_ASYNC);

			if (!pEntity || !pLocal || pEntity == pLocal)
				return;

			hittedLogHits[pEntity->GetIndex()]++;

			if (pEntity->GetVelocity().Length2D() == 0) {
				if (hitgroup == 1) {
					saveLastHeadshotFloat[pEntity->GetIndex()] = pEntity->GetEyeAngles().y;

				}
				else if (dmg >= 50) {
					saveLastBaimFloat[pEntity->GetIndex()] = pEntity->GetEyeAngles().y;
				}
				else if (dmg >= 30) {
					saveLastBaim30Float[pEntity->GetIndex()] = pEntity->GetEyeAngles().y;
				}
			}
		}
	}
}

int More = 0;

void deathplayer(IGameEvent* pEvent)
{
	int attackerid = pEvent->GetInt("attacker");
	int entityid = Interfaces::Engine->GetPlayerForUserID(attackerid);

	if (entityid == Interfaces::Engine->GetLocalPlayer())
	{
		int nUserID = pEvent->GetInt("attacker");
		int nDead = pEvent->GetInt("userid");
		if (nUserID || nDead)
		{
			bool headshot = pEvent->GetBool("headshot");
			if (Menu::Window.MiscTab.OtherKillSpam.GetIndex() > 0)
			{
				Menu::Window.MiscTab.OtherChatSpam.SelectIndex(0);
				Menu::Window.MiscTab.OtherLyricsSpam.SelectIndex(0);
				if (headshot)
				{
					if (Menu::Window.MiscTab.OtherKillSpam.GetIndex() == 1) {
						static clock_t start_t = clock();
						int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
						double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
						if (timeSoFar < 0.85)
							return;

							SayInChat("1");

						/*int m_iRand = rand() % 4 + 1;

						if (m_iRand == 1) {
							SayInChat("INSERTGANG.XYZ > ALL");
						}
						if (m_iRand == 2) {
							SayInChat("INSERTGANG.XYZ > NEVER RQ");
						}
						if (m_iRand == 3) {
							SayInChat("INSERTGANG.XYZ > YOUR PASTA");
						}*/

						start_t = clock();
					}
					if (Menu::Window.MiscTab.OtherKillSpam.GetIndex() == 2) {
						static clock_t start_t = clock();
						int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
						double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
						if (timeSoFar < 0.85)
							return;

						if (More == 0) {
							SayInChat("ha u get tapped so hard :PP");
						}
						if (More == 1) {
							SayInChat("Mad?");
							More = -1;
						}
						More++;

						start_t = clock();
					}
					if (Menu::Window.MiscTab.OtherKillSpam.GetIndex() == 3) {
						static clock_t start_t = clock();
						int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
						double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
						if (timeSoFar < 0.85)
							return;

						SayInChat("One Tap");

						/*int m_iRand = rand() % 4 + 1;

						if (m_iRand == 1) {
						SayInChat("INSERTGANG.XYZ > ALL");
						}
						if (m_iRand == 2) {
						SayInChat("INSERTGANG.XYZ > NEVER RQ");
						}
						if (m_iRand == 3) {
						SayInChat("INSERTGANG.XYZ > YOUR PASTA");
						}*/

						start_t = clock();
					}
				}
				else
				{
					if (Menu::Window.MiscTab.OtherKillSpam.GetIndex() == 2) {
						static clock_t start_t = clock();
						int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
						double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
						if (timeSoFar < 0.85)
							return;

						if (More == 0) {
							SayInChat("1");
						}
						if (More == 1) {
							SayInChat("1");
						}
						if (More == 2) {
							SayInChat("1");
							More = -1;
						}
						More++;

						start_t = clock();
					}
					if (Menu::Window.MiscTab.OtherKillSpam.GetIndex() == 3) {
						static clock_t start_t = clock();
						int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
						double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
						if (timeSoFar < 0.85)
							return;

						SayInChat("Sry, my mouse doesn't allow me flick so hard.");

						start_t = clock();
					}
				}
			}
		}
	}
}

void roundstart(IGameEvent* pEvent)
{
	Shooted = 0;
	if (Menu::Window.SkinchangerTab.SkinEnable.GetState()) {
		Skin = 1;
	}
}

// Undo our hooks
void Hooks::UndoHooks()
{
	VMTPanel.RestoreOriginal();
	VMTPrediction.RestoreOriginal();
	VMTModelRender.RestoreOriginal();
	VMTClientMode.RestoreOriginal();
}

// Initialise all our hooks
void Hooks::Initialise()
{
	// Panel hooks for drawing to the screen via surface functions
	VMTPanel.Initialise((DWORD*)Interfaces::Panels);
	oPaintTraverse = (PaintTraverse_)VMTPanel.HookMethod((DWORD)&PaintTraverse_Hooked, Offsets::VMT::Panel_PaintTraverse);
	//Utilities::Log("Paint Traverse Hooked");

	// No Visual Recoi	l
	VMTPrediction.Initialise((DWORD*)Interfaces::Prediction);
	VMTPrediction.HookMethod((DWORD)&Hooked_InPrediction, 14);
	//Utilities::Log("InPrediction Hooked");

	// Chams
	VMTModelRender.Initialise((DWORD*)Interfaces::ModelRender);
	oDrawModelExecute = (DrawModelEx_)VMTModelRender.HookMethod((DWORD)&Hooked_DrawModelExecute, Offsets::VMT::ModelRender_DrawModelExecute);
	//Utilities::Log("DrawModelExecute Hooked");

	// Setup ClientMode Hooks
	VMTClientMode.Initialise((DWORD*)Interfaces::ClientMode);
	VMTClientMode.HookMethod((DWORD)CreateMoveClient_Hooked, 24);

	oOverrideView = (OverrideViewFn)VMTClientMode.HookMethod((DWORD)&Hooked_OverrideView, 18);
	VMTClientMode.HookMethod((DWORD)&GGetViewModelFOV, 35);

	// Setup client hooks
	VMTClient.Initialise((DWORD*)Interfaces::Client);
	oFrameStageNotify = (FrameStageNotifyFn)VMTClient.HookMethod((DWORD)&Hooked_FrameStageNotify, 36);

	DamageListener = new DamageEventListener(hitmarker);
	// ...In injection thread
	Interfaces::GameEventManager->AddListener(DamageListener, "player_hurt", false);

	DeathListener = new DamageEventListener(deathplayer);
	// ...In injection thread
	Interfaces::GameEventManager->AddListener(DeathListener, "player_death", false);

	RoundStartListener = new DamageEventListener(roundstart);
	// ...In injection thread
	Interfaces::GameEventManager->AddListener(RoundStartListener, "round_start", false);

}

void MovementCorrection(CUserCmd* pCmd)
{

}

//---------------------------------------------------------------------------------------------------------
//                                         Hooked Functions
//---------------------------------------------------------------------------------------------------------

void SetClanTag(const char* tag, const char* name)//190% paste
{
	static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(((DWORD)Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15\x00\x00\x00\x00\x6A\x24\x8B\xC8\x8B\x30", "xxxxxxxxx????xxxxxx")));
	pSetClanTag(tag, name);
}
void NoClantag()
{
	SetClanTag("", "");
}

void ClanTag()
{
	static int counter = 0;
	switch (Menu::Window.MiscTab.OtherClantag.GetIndex())
	{
	case 0:
		SetClanTag("", "");
		break;
	case 1:
	{
		static int motion = 0;
		int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * 3;

		if (Menu::Window.SettingsTab.ClanTagType.GetIndex() == 0)
		{
			SetClanTag(Menu::Window.SettingsTab.ClanTagText.getText().c_str(), Menu::Window.SettingsTab.ClanTagText.getText().c_str());
		}
		if (Menu::Window.SettingsTab.ClanTagType.GetIndex() == 1)
		{
			if (counter % 48 == 0)
				motion++;
			int value = ServerTime % 4;
			switch (value) {
			case 0:SetClanTag("", ""); break;
			case 1:SetClanTag(Menu::Window.SettingsTab.ClanTagText.getText().c_str(), Menu::Window.SettingsTab.ClanTagText.getText().c_str()); break;
			case 2:SetClanTag(Menu::Window.SettingsTab.ClanTagText.getText().c_str(), Menu::Window.SettingsTab.ClanTagText.getText().c_str()); break;
			case 3:SetClanTag("", ""); break;
			}
			counter++;
		}
	}
	break;
	case 2:
	{
		static int motion = 0;
		int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * 2.5;

		if (counter % 48 == 0)
			motion++;
		int value = ServerTime % 29;
		switch (value) {
		case 0:SetClanTag("                ", "sneakyevil.win"); break;
		case 1:SetClanTag("               s", "sneakyevil.win"); break;
		case 2:SetClanTag("              sn", "sneakyevil.win"); break;
		case 3:SetClanTag("             sne", "sneakyevil.win"); break;
		case 4:SetClanTag("            snea", "sneakyevil.win"); break;
		case 5:SetClanTag("           sneak", "sneakyevil.win"); break;
		case 6:SetClanTag("          sneaky", "sneakyevil.win"); break;
		case 7:SetClanTag("         sneakye", "sneakyevil.win"); break;
		case 8:SetClanTag("        sneakyev", "sneakyevil.win"); break;
		case 9:SetClanTag("       sneakyevi", "sneakyevil.win"); break;
		case 10:SetClanTag("     sneakyevil", "sneakyevil.win"); break;
		case 11:SetClanTag("    sneakyevil.", "sneakyevil.win"); break;
		case 12:SetClanTag("   sneakyevil.w", "sneakyevil.win"); break;
		case 13:SetClanTag("  sneakyevil.wi", "sneakyevil.win"); break;
		case 14:SetClanTag(" sneakyevil.win", "sneakyevil.win"); break;
		case 15:SetClanTag("sneakyevil.win ", "sneakyevil.win"); break;
		case 16:SetClanTag("neakyevil.win  ", "sneakyevil.win"); break;
		case 17:SetClanTag("eakyevil.win   ", "sneakyevil.win"); break;
		case 18:SetClanTag("akyevil.win    ", "sneakyevil.win"); break;
		case 19:SetClanTag("kyevil.win     ", "sneakyevil.win"); break;
		case 20:SetClanTag("yevil.win      ", "sneakyevil.win"); break;
		case 21:SetClanTag("evil.win       ", "sneakyevil.win"); break;
		case 22:SetClanTag("vil.win        ", "sneakyevil.win"); break;
		case 23:SetClanTag("il.win         ", "sneakyevil.win"); break;
		case 24:SetClanTag("l.win          ", "sneakyevil.win"); break;
		case 25:SetClanTag(".win           ", "sneakyevil.win"); break;
		case 26:SetClanTag("win            ", "sneakyevil.win"); break;
		case 27:SetClanTag("in             ", "sneakyevil.win"); break;
		case 28:SetClanTag("n              ", "sneakyevil.win"); break;
		case 29:SetClanTag("               ", "sneakyevil.win"); break;
		}
		counter++;
	}
	break;
	case 3:
	{
		static int motion = 0;
		int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * 2.5;

		if (counter % 68 == 0)
			motion++;
		int value = ServerTime % 67;
		switch (value) {
		case 0:SetClanTag("                ", "sneakyevil.xyz"); break;
		case 1:SetClanTag("#               ", "sneakyevil.xyz"); break;
		case 2:SetClanTag("&               ", "sneakyevil.xyz"); break;
		case 3:SetClanTag("@               ", "sneakyevil.xyz"); break;
		case 4:SetClanTag("s               ", "sneakyevil.xyz"); break;
		case 5:SetClanTag("s#              ", "sneakyevil.xyz"); break;
		case 6:SetClanTag("s&              ", "sneakyevil.xyz"); break;
		case 7:SetClanTag("s@              ", "sneakyevil.xyz"); break;
		case 8:SetClanTag("sn              ", "sneakyevil.xyz"); break;
		case 9:SetClanTag("sn#             ", "sneakyevil.xyz"); break;
		case 10:SetClanTag("sn&            ", "sneakyevil.xyz"); break;
		case 11:SetClanTag("sn@            ", "sneakyevil.xyz"); break;
		case 12:SetClanTag("sne            ", "sneakyevil.xyz"); break;
		case 13:SetClanTag("sne#           ", "sneakyevil.xyz"); break;
		case 14:SetClanTag("sne&           ", "sneakyevil.xyz"); break;
		case 15:SetClanTag("sne@           ", "sneakyevil.xyz"); break;
		case 16:SetClanTag("snea           ", "sneakyevil.xyz"); break;
		case 17:SetClanTag("snea#          ", "sneakyevil.xyz"); break;
		case 18:SetClanTag("snea&          ", "sneakyevil.xyz"); break;
		case 19:SetClanTag("snea@          ", "sneakyevil.xyz"); break;
		case 20:SetClanTag("sneak          ", "sneakyevil.xyz"); break;
		case 21:SetClanTag("sneak#         ", "sneakyevil.xyz"); break;
		case 22:SetClanTag("sneak&         ", "sneakyevil.xyz"); break;
		case 23:SetClanTag("sneak@         ", "sneakyevil.xyz"); break;
		case 24:SetClanTag("sneaky         ", "sneakyevil.xyz"); break;
		case 25:SetClanTag("sneaky#        ", "sneakyevil.xyz"); break;
		case 26:SetClanTag("sneaky&        ", "sneakyevil.xyz"); break;
		case 27:SetClanTag("sneaky@        ", "sneakyevil.xyz"); break;
		case 28:SetClanTag("sneakye        ", "sneakyevil.xyz"); break;
		case 29:SetClanTag("sneakye#       ", "sneakyevil.xyz"); break;
		case 30:SetClanTag("sneakye&       ", "sneakyevil.xyz"); break;
		case 31:SetClanTag("sneakye@       ", "sneakyevil.xyz"); break;
		case 32:SetClanTag("sneakyev       ", "sneakyevil.xyz"); break;
		case 33:SetClanTag("sneakyev#      ", "sneakyevil.xyz"); break;
		case 34:SetClanTag("sneakyev&      ", "sneakyevil.xyz"); break;
		case 35:SetClanTag("sneakyev@      ", "sneakyevil.xyz"); break;
		case 36:SetClanTag("sneakyevi      ", "sneakyevil.xyz"); break;
		case 37:SetClanTag("sneakyevi#     ", "sneakyevil.xyz"); break;
		case 38:SetClanTag("sneakyevi&     ", "sneakyevil.xyz"); break;
		case 39:SetClanTag("sneakyevi@     ", "sneakyevil.xyz"); break;
		case 40:SetClanTag("sneakyevil     ", "sneakyevil.xyz"); break;
		case 41:SetClanTag("sneakyevil#    ", "sneakyevil.xyz"); break;
		case 42:SetClanTag("sneakyevil&    ", "sneakyevil.xyz"); break;
		case 43:SetClanTag("sneakyevil@    ", "sneakyevil.xyz"); break;
		case 44:SetClanTag("sneakyevil.    ", "sneakyevil.xyz"); break;
		case 45:SetClanTag("sneakyevil.#   ", "sneakyevil.xyz"); break;
		case 46:SetClanTag("sneakyevil.&   ", "sneakyevil.xyz"); break;
		case 47:SetClanTag("sneakyevil.@   ", "sneakyevil.xyz"); break;
		case 48:SetClanTag("sneakyevil.x   ", "sneakyevil.xyz"); break;
		case 49:SetClanTag("sneakyevil.x#  ", "sneakyevil.xyz"); break;
		case 50:SetClanTag("sneakyevil.x&  ", "sneakyevil.xyz"); break;
		case 51:SetClanTag("sneakyevil.x@  ", "sneakyevil.xyz"); break;
		case 52:SetClanTag("sneakyevil.xy  ", "sneakyevil.xyz"); break;
		case 53:SetClanTag("sneakyevil.xy# ", "sneakyevil.xyz"); break;
		case 54:SetClanTag("sneakyevil.xy& ", "sneakyevil.xyz"); break;
		case 55:SetClanTag("sneakyevil.xy@ ", "sneakyevil.xyz"); break;
		case 56:SetClanTag("sneakyevil.xyz ", "sneakyevil.xyz"); break;
		case 57:SetClanTag("sneakyevil.xyz ", "sneakyevil.xyz"); break;
		case 58:SetClanTag("sneakyevil.xyz ", "sneakyevil.xyz"); break;
		case 59:SetClanTag("sneakyevil.xyz ", "sneakyevil.xyz"); break;
		case 60:SetClanTag("sneakyevil.xyz ", "sneakyevil.xyz"); break;
		case 61:SetClanTag("sneakyevil.xyz ", "sneakyevil.xyz"); break;
		case 62:SetClanTag("############## ", "sneakyevil.xyz"); break;
		case 63:SetClanTag("&&&&&&&&&&&&&& ", "sneakyevil.xyz"); break;
		case 64:SetClanTag("@@@@@@@@@@@@@@ ", "sneakyevil.xyz"); break;
		case 65:SetClanTag("                ", "sneakyevil.xyz"); break;
		case 66:SetClanTag("                ", "sneakyevil.xyz"); break;
		case 67:SetClanTag("                ", "sneakyevil.xyz"); break;
		}
		counter++;
	}
	break;
	case 4:
	{
		static int motion = 0;
		int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * 3;

		if (counter % 90 == 0)
			motion++;
		int value = ServerTime % 89;
		switch (value) {
		case 0:SetClanTag("               ", "sneakyevil Hook"); break;
		case 1:SetClanTag("              /", "sneakyevil Hook"); break;
		case 2:SetClanTag("              <", "sneakyevil Hook"); break;
		case 3:SetClanTag("              >", "sneakyevil Hook"); break;
		case 4:SetClanTag("              S", "sneakyevil Hook"); break;
		case 5:SetClanTag("             S ", "sneakyevil Hook"); break;
		case 6:SetClanTag("             S/", "sneakyevil Hook"); break;
		case 7:SetClanTag("             S<", "sneakyevil Hook"); break;
		case 8:SetClanTag("             S>", "sneakyevil Hook"); break;
		case 9:SetClanTag("             SN", "sneakyevil Hook"); break;
		case 10:SetClanTag("            SN ", "sneakyevil Hook"); break;
		case 11:SetClanTag("            SN/", "sneakyevil Hook"); break;
		case 12:SetClanTag("            SN<", "sneakyevil Hook"); break;
		case 13:SetClanTag("            SN>", "sneakyevil Hook"); break;
		case 14:SetClanTag("            SNE", "sneakyevil Hook"); break;
		case 15:SetClanTag("           SNE ", "sneakyevil Hook"); break;
		case 16:SetClanTag("           SNE/", "sneakyevil Hook"); break;
		case 17:SetClanTag("           SNE<", "sneakyevil Hook"); break;
		case 18:SetClanTag("           SNE>", "sneakyevil Hook"); break;
		case 19:SetClanTag("           SNEA", "sneakyevil Hook"); break;
		case 20:SetClanTag("          SNEA ", "sneakyevil Hook"); break;
		case 21:SetClanTag("          SNEA/", "sneakyevil Hook"); break;
		case 22:SetClanTag("          SNEA<", "sneakyevil Hook"); break;
		case 23:SetClanTag("          SNEA>", "sneakyevil Hook"); break;
		case 24:SetClanTag("          SNEAK", "sneakyevil Hook"); break;
		case 25:SetClanTag("         SNEAK ", "sneakyevil Hook"); break;
		case 26:SetClanTag("         SNEAK/", "sneakyevil Hook"); break;
		case 27:SetClanTag("         SNEAK<", "sneakyevil Hook"); break;
		case 28:SetClanTag("         SNEAK>", "sneakyevil Hook"); break;
		case 29:SetClanTag("         SNEAKY", "sneakyevil Hook"); break;
		case 30:SetClanTag("        SNEAKY ", "sneakyevil Hook"); break;
		case 31:SetClanTag("        SNEAKY/", "sneakyevil Hook"); break;
		case 32:SetClanTag("        SNEAKY<", "sneakyevil Hook"); break;
		case 33:SetClanTag("        SNEAKY>", "sneakyevil Hook"); break;
		case 34:SetClanTag("        SNEAKYE", "sneakyevil Hook"); break;
		case 35:SetClanTag("       SNEAKYE ", "sneakyevil Hook"); break;
		case 36:SetClanTag("       SNEAKYE/", "sneakyevil Hook"); break;
		case 37:SetClanTag("       SNEAKYE<", "sneakyevil Hook"); break;
		case 38:SetClanTag("       SNEAKYE>", "sneakyevil Hook"); break;
		case 39:SetClanTag("       SNEAKYEV", "sneakyevil Hook"); break;
		case 40:SetClanTag("      SNEAKYEV ", "sneakyevil Hook"); break;
		case 41:SetClanTag("      SNEAKYEV/", "sneakyevil Hook"); break;
		case 42:SetClanTag("      SNEAKYEV<", "sneakyevil Hook"); break;
		case 43:SetClanTag("      SNEAKYEV>", "sneakyevil Hook"); break;
		case 44:SetClanTag("      SNEAKYEVI", "sneakyevil Hook"); break;
		case 45:SetClanTag("     SNEAKYEVI ", "sneakyevil Hook"); break;
		case 46:SetClanTag("     SNEAKYEVI/", "sneakyevil Hook"); break;
		case 47:SetClanTag("     SNEAKYEVI<", "sneakyevil Hook"); break;
		case 48:SetClanTag("     SNEAKYEVI>", "sneakyevil Hook"); break;
		case 49:SetClanTag("     SNEAKYEVIL", "sneakyevil Hook"); break;
		case 50:SetClanTag("    SNEAKYEVIL ", "sneakyevil Hook"); break;
		case 51:SetClanTag("    SNEAKYEVIL/", "sneakyevil Hook"); break;
		case 52:SetClanTag("    SNEAKYEVIL<", "sneakyevil Hook"); break;
		case 53:SetClanTag("    SNEAKYEVIL>", "sneakyevil Hook"); break;
		case 54:SetClanTag("    SNEAKYEVILH", "sneakyevil Hook"); break;
		case 55:SetClanTag("   SNEAKYEVILH ", "sneakyevil Hook"); break;
		case 56:SetClanTag("   SNEAKYEVILH/", "sneakyevil Hook"); break;
		case 57:SetClanTag("   SNEAKYEVILH<", "sneakyevil Hook"); break;
		case 58:SetClanTag("   SNEAKYEVILH>", "sneakyevil Hook"); break;
		case 59:SetClanTag("   SNEAKYEVILHO", "sneakyevil Hook"); break;
		case 61:SetClanTag("  SNEAKYEVILHO ", "sneakyevil Hook"); break;
		case 62:SetClanTag("  SNEAKYEVILHO/", "sneakyevil Hook"); break;
		case 63:SetClanTag("  SNEAKYEVILHO<", "sneakyevil Hook"); break;
		case 64:SetClanTag("  SNEAKYEVILHO>", "sneakyevil Hook"); break;
		case 65:SetClanTag("  SNEAKYEVILHOO", "sneakyevil Hook"); break;
		case 66:SetClanTag(" SNEAKYEVILHOO ", "sneakyevil Hook"); break;
		case 67:SetClanTag(" SNEAKYEVILHOO/", "sneakyevil Hook"); break;
		case 68:SetClanTag(" SNEAKYEVILHOO<", "sneakyevil Hook"); break;
		case 69:SetClanTag(" SNEAKYEVILHOO>", "sneakyevil Hook"); break;
		case 70:SetClanTag(" SNEAKYEVILHOOK", "sneakyevil Hook"); break;
		case 71:SetClanTag("<SNEAKYEVILHOOK", "sneakyevil Hook"); break;
		case 72:SetClanTag("<SNEAKYEVILHOOK", "sneakyevil Hook"); break;
		case 73:SetClanTag("SNEAKYEVILHOOK>", "sneakyevil Hook"); break;
		case 74:SetClanTag("SNEAKYEVILHOOK>", "sneakyevil Hook"); break;
		case 75:SetClanTag("SNEAKYEVILHOOK ", "sneakyevil Hook"); break;
		case 76:SetClanTag("NEAKYEVILHOOK  ", "sneakyevil Hook"); break;
		case 77:SetClanTag("EAKYEVILHOOK   ", "sneakyevil Hook"); break;
		case 78:SetClanTag("AKYEVILHOOK    ", "sneakyevil Hook"); break;
		case 79:SetClanTag("KYEVILHOOK     ", "sneakyevil Hook"); break;
		case 80:SetClanTag("YEVILHOOK      ", "sneakyevil Hook"); break;
		case 81:SetClanTag("EVILHOOK       ", "sneakyevil Hook"); break;
		case 82:SetClanTag("VILHOOK        ", "sneakyevil Hook"); break;
		case 83:SetClanTag("ILHOOK         ", "sneakyevil Hook"); break;
		case 84:SetClanTag("LHOOK          ", "sneakyevil Hook"); break;
		case 85:SetClanTag("HOOK           ", "sneakyevil Hook"); break;
		case 86:SetClanTag("OOK            ", "sneakyevil Hook"); break;
		case 87:SetClanTag("OK             ", "sneakyevil Hook"); break;
		case 88:SetClanTag("K              ", "sneakyevil Hook"); break;
		case 89:SetClanTag("               ", "sneakyevil Hook"); break;
		}
		counter++;
	}
	break;
	case 5:
	{
		static int motion = 0;
		int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * 3;

		if (counter % 48 == 0)
			motion++;
		int value = ServerTime % 6;
		switch (value) {
		case 0:SetClanTag("                  ", "Commend Me >"); break;
		case 1:SetClanTag("   Commend Me >   ", "Commend Me >"); break;
		case 2:SetClanTag("   Commend Me >>  ", "Commend Me >"); break;
		case 3:SetClanTag("   Commend Me >>> ", "Commend Me >"); break;
		case 4:SetClanTag("   Commend Me >>  ", "Commend Me >"); break;
		case 5:SetClanTag("   Commend Me >   ", "Commend Me >"); break;
		case 6:SetClanTag("                  ", "Commend Me >"); break;
		}
		counter++;
	}
	break;
	case 6:
	{
		static int motion = 0;
		int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * 3;

		if (counter % 48 == 0)
			motion++;
		int value = ServerTime % 17;
		switch (value) {
		case 0:SetClanTag("          ", "skeet.cc"); break;
		case 1:SetClanTag("         s", "skeet.cc"); break;
		case 2:SetClanTag("        sk", "skeet.cc"); break;
		case 3:SetClanTag("       ske", "skeet.cc"); break;
		case 4:SetClanTag("      skee", "skeet.cc"); break;
		case 5:SetClanTag("     skeet", "skeet.cc"); break;
		case 6:SetClanTag("    skeet.", "skeet.cc"); break;
		case 7:SetClanTag("   skeet.c", "skeet.cc"); break;
		case 8:SetClanTag(" skeet.cc", "skeet.cc"); break;
		case 9:SetClanTag("skeet.cc ", "skeet.cc"); break;
		case 10:SetClanTag("keet.cc  ", "skeet.cc"); break;
		case 11:SetClanTag("eet.cc   ", "skeet.cc"); break;
		case 12:SetClanTag("et.cc    ", "skeet.cc"); break;
		case 13:SetClanTag("t.cc     ", "skeet.cc"); break;
		case 14:SetClanTag(".cc      ", "skeet.cc"); break;
		case 15:SetClanTag("cc       ", "skeet.cc"); break;
		case 16:SetClanTag("c        ", "skeet.cc"); break;
		case 17:SetClanTag("         ", "skeet.cc"); break;
		}
		counter++;
	}
	break;
	case 7:
	{
		static int motion = 0;
		int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * 3;

		if (counter % 48 == 0)
			motion++;
		int value = ServerTime % 39;
		switch (value) {
		case 0:SetClanTag("          ", "skeet.cc"); break;
		case 1:SetClanTag("          ", "skeet.cc"); break;
		case 2:SetClanTag("         g", "skeet.cc"); break;
		case 3:SetClanTag("         g", "skeet.cc"); break;
		case 4:SetClanTag("        ga", "skeet.cc"); break;
		case 5:SetClanTag("        ga", "skeet.cc"); break;
		case 6:SetClanTag("       gam", "skeet.cc"); break;
		case 7:SetClanTag("       gam", "skeet.cc"); break;
		case 8:SetClanTag("      game", "skeet.cc"); break;
		case 9:SetClanTag("      game", "skeet.cc"); break;
		case 10:SetClanTag("     games", "skeet.cc"); break;
		case 11:SetClanTag("     games", "skeet.cc"); break;
		case 12:SetClanTag("    gamese", "skeet.cc"); break;
		case 13:SetClanTag("    gamese", "skeet.cc"); break;
		case 14:SetClanTag("   gamesen", "skeet.cc"); break;
		case 15:SetClanTag("   gamesen", "skeet.cc"); break;
		case 16:SetClanTag("  gamesens", "skeet.cc"); break;
		case 17:SetClanTag("  gamesens", "skeet.cc"); break;
		case 18:SetClanTag(" gamsesense", "skeet.cc"); break;
		case 19:SetClanTag(" gamsesense", "skeet.cc"); break;
		case 20:SetClanTag("gamesense ", "skeet.cc"); break;
		case 21:SetClanTag("gamesense ", "skeet.cc"); break;
		case 22:SetClanTag("amesense  ", "skeet.cc"); break;
		case 23:SetClanTag("amesense  ", "skeet.cc"); break;
		case 24:SetClanTag("mesense   ", "skeet.cc"); break;
		case 25:SetClanTag("mesense   ", "skeet.cc"); break;
		case 26:SetClanTag("esense    ", "skeet.cc"); break;
		case 27:SetClanTag("esense    ", "skeet.cc"); break;
		case 28:SetClanTag("sense     ", "skeet.cc"); break;
		case 29:SetClanTag("sense     ", "skeet.cc"); break;
		case 30:SetClanTag("ense      ", "skeet.cc"); break;
		case 31:SetClanTag("ense      ", "skeet.cc"); break;
		case 32:SetClanTag("nse       ", "skeet.cc"); break;
		case 33:SetClanTag("nse       ", "skeet.cc"); break;
		case 34:SetClanTag("se        ", "skeet.cc"); break;
		case 35:SetClanTag("se        ", "skeet.cc"); break;
		case 36:SetClanTag("e         ", "skeet.cc"); break;
		case 37:SetClanTag("e         ", "skeet.cc"); break;
		case 38:SetClanTag("          ", "skeet.cc"); break;
		case 39:SetClanTag("          ", "skeet.cc"); break;
		}
		counter++;
	}
	break;
	case 8:
		SetClanTag("[VALV\xE1\xB4\xB1]", "Valve");
		break;
	case 9:
	{
		static int motion = 0;
		int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * 3;

		char fffff[64];
		sprintf_s(fffff, "[%s]", GetTimeString().c_str());
		if (counter % 48 == 0)
			motion++;
		int value = ServerTime % 1;
		switch (value) {
		case 0:SetClanTag(fffff, "Real Time"); break;
		case 1:SetClanTag(fffff, "Real Time"); break;
		}
		counter++;
	}
	break;
	}
}
HooksXD lmao;
int HooksXD::night;
int HooksXD::night2;
#include "Entities.h"
void HooksXD::XD3()
{
	CSGOClassID::CMaterialModifyControl;
	//int HooksXD::night;
	//int HooksXD::night2;
	 
	 HooksXD::night = 1;
	 HooksXD::night2 = 1;
};

bool __stdcall CreateMoveClient_Hooked(/*void* self, int edx,*/ float frametime, CUserCmd* pCmd)
{
	
	if (!pCmd->command_number)
		return true;

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{

		PVOID pebp;
		__asm mov pebp, ebp;
		bool* pbSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);
		bool& bSendPacket = *pbSendPacket;

		if (Menu::Window.MiscTab.OtherClantag.GetIndex() > 0)
			ClanTag();

		//	CUserCmd* cmdlist = *(CUserCmd**)((DWORD)Interfaces::pInput + 0xEC);
		//	CUserCmd* pCmd = &cmdlist[sequence_number % 150];
		grenade_prediction::instance().Tick(pCmd->buttons);

		// Backup for safety
		Vector origView = pCmd->viewangles;
		Vector viewforward, viewright, viewup, aimforward, aimright, aimup;
		Vector qAimAngles;
		qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
		AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);

		// Do da hacks
		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && pLocal && pLocal->IsAlive())
			Hacks::MoveHacks(pCmd, bSendPacket);
		backtracking->legitBackTrack(pCmd, pLocal);

		//Movement Fix
		//GameUtils::CL_FixMove(pCmd, origView);
		qAimAngles.Init(0.0f, GetAutostrafeView().y, 0.0f); // if pCmd->viewangles.x > 89, set pCmd->viewangles.x instead of 0.0f on first
		AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);
		qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
		AngleVectors(qAimAngles, &aimforward, &aimright, &aimup);
		Vector vForwardNorm;		Normalize(viewforward, vForwardNorm);
		Vector vRightNorm;			Normalize(viewright, vRightNorm);
		Vector vUpNorm;				Normalize(viewup, vUpNorm);

		// Original shit for movement correction
		float forward = pCmd->forwardmove;
		float right = pCmd->sidemove;
		float up = pCmd->upmove;
		if (forward > 450) forward = 450;
		if (right > 450) right = 450;
		if (up > 450) up = 450;
		if (forward < -450) forward = -450;
		if (right < -450) right = -450;
		if (up < -450) up = -450;
		pCmd->forwardmove = DotProduct(forward * vForwardNorm, aimforward) + DotProduct(right * vRightNorm, aimforward) + DotProduct(up * vUpNorm, aimforward);
		pCmd->sidemove = DotProduct(forward * vForwardNorm, aimright) + DotProduct(right * vRightNorm, aimright) + DotProduct(up * vUpNorm, aimright);
		pCmd->upmove = DotProduct(forward * vForwardNorm, aimup) + DotProduct(right * vRightNorm, aimup) + DotProduct(up * vUpNorm, aimup);

		// Angle normalisation
		if (Menu::Window.MiscTab.OtherSafeMode.GetState())
		{
			GameUtils::NormaliseViewAngle(pCmd->viewangles);


			if (pCmd->viewangles.z != 0.0f)
			{
				pCmd->viewangles.z = 0.00;
			}

			if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
			{
				Utilities::Log("Having to re-normalise!");
				GameUtils::NormaliseViewAngle(pCmd->viewangles);
				if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
				{
					pCmd->viewangles = origView;
					pCmd->sidemove = right;
					pCmd->forwardmove = forward;
				}
			}
		}

		if (pCmd->viewangles.x > 90)
		{
			pCmd->forwardmove = -pCmd->forwardmove;
		}

		if (pCmd->viewangles.x < -90)
		{
			pCmd->forwardmove = -pCmd->forwardmove;
		}

		if (bSendPacket == true) {
			AAlineRealAngle = pCmd->viewangles.y;
		}
		else {
			AAlineFakeAngle = pCmd->viewangles.y;
		}

		if (bSendPacket)
			LastAngleAA = pCmd->viewangles;
	}
	static bool nlag1 = false;
	static bool nlag2 = false;
	if (Menu::Window.VisualsTab.OtherNoSky.GetState())
	{
		if (!nlag1)
		{
			ConVar* NoSkybox = Interfaces::CVar->FindVar("sv_skyname"); /*No-Skybox*/
			*(float*)((DWORD)&NoSkybox->fnChangeCallback + 0xC) = NULL;
			NoSkybox->SetValue("sky_l4d_rural02_ldr");
			nlag1 = true;
			nlag2 = false;
		}
		
	}
	if (nlag1)
	{
		if (!Menu::Window.VisualsTab.OtherNoSky.GetState())
		{
			if (!nlag2)
			{
				ConVar* NoSkybox = Interfaces::CVar->FindVar("sv_skyname"); /*No-Skybox*/
				*(float*)((DWORD)&NoSkybox->fnChangeCallback + 0xC) = NULL;
				NoSkybox->SetValue("jungle");
				nlag2 = true;
				nlag1 = false;
			}

		}
	}
	static bool nlag3 = false;
	static bool nlag4 = false;
	if (Menu::Window.VisualsTab.OtherAsus.GetState())
	{
		if (!nlag3)
		{
			Interfaces::Engine->ClientCmd_Unrestricted("mat_postprocess_enable 0");
			nlag3 = true;
			nlag4 = false;
		}

	}
	if (nlag3)
	{
		if (!Menu::Window.VisualsTab.OtherAsus.GetState())
		{
			if (!nlag4)
			{
				Interfaces::Engine->ClientCmd_Unrestricted("mat_postprocess_enable 1");
				nlag4 = true;
				nlag3 = false;
			}

		}
	}
	return false;
}

// Paint Traverse Hooked function
void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!strcmp(("HudZoom"), Interfaces::Panels->GetName(vguiPanel)) && Menu::Window.VisualsTab.OtherNoScope.GetState() > 0 && pLocal->IsScoped() && Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && pLocal->IsAlive())
	{
		return;
	}

	oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);

	static unsigned int FocusOverlayPanel = 0;
	static bool FoundPanel = false;

	if (!FoundPanel)
	{
		PCHAR szPanelName = (PCHAR)Interfaces::Panels->GetName(vguiPanel);
		if (strstr(szPanelName, "MatSystemTopPanel"))
		{
			FocusOverlayPanel = vguiPanel;
			FoundPanel = true;
		}
	}
	else if (FocusOverlayPanel == vguiPanel)
	{
		if (Menu::Window.MiscTab.OtherWatermark.GetState())
		{
			RECT scrn = Render::GetViewport();
			RECT TextSize = Render::GetTextSize(Render::Fonts::MenuBold, "");
			char aaaaa[64];
			sprintf_s(aaaaa, "Build Date: %s", __DATE__);
			Render::Text(5, 11, Color(Menu::Window.ColorTab.MenuInnerR.GetValue(), Menu::Window.ColorTab.MenuInnerG.GetValue(), Menu::Window.ColorTab.MenuInnerB.GetValue(), 255), Render::Fonts::MenuBold, aaaaa);
			Render::Text(5, 1, Color(Menu::Window.ColorTab.MenuInnerR.GetValue(), Menu::Window.ColorTab.MenuInnerG.GetValue(), Menu::Window.ColorTab.MenuInnerB.GetValue(), 255), Render::Fonts::MenuBold, "sneakyevil Hook | Welcome Paster :)");
		}
		if (Menu::Window.MiscTab.OtherClock.GetState())
		{
			RECT scrn = Render::GetViewport();
			RECT TextSize = Render::GetTextSize(Render::Fonts::MenuBold, "");
			char bbbbb[64];
			sprintf_s(bbbbb, "Current Time: %s", GetTimeString().c_str());
			if (Menu::Window.MiscTab.OtherWatermark.GetState())
			{
				Render::Text(5, 21, Color(Menu::Window.ColorTab.MenuInnerR.GetValue(), Menu::Window.ColorTab.MenuInnerG.GetValue(), Menu::Window.ColorTab.MenuInnerB.GetValue(), 255), Render::Fonts::MenuBold, bbbbb);
			}
			else
			{
				Render::Text(5, 1, Color(Menu::Window.ColorTab.MenuInnerR.GetValue(), Menu::Window.ColorTab.MenuInnerG.GetValue(), Menu::Window.ColorTab.MenuInnerB.GetValue(), 255), Render::Fonts::MenuBold, bbbbb);
			}
		}
		if (Menu::Window.VisualsTab.OptionsDefusing.GetIndex() == 2)
		{
			if (c4_defuse == 1)
			{
				RECT scrn = Render::GetViewport();
				RECT TextSize = Render::GetTextSize(Render::Fonts::MenuBold, "");
				if (Menu::Window.MiscTab.OtherWatermark.GetState() && Menu::Window.MiscTab.OtherClock.GetState())
				{
					Render::Text(5, 31, Color(255, 0, 0, 255), Render::Fonts::MenuBold, "Someone is defusing");
				}
				else if (Menu::Window.MiscTab.OtherWatermark.GetState() && !Menu::Window.MiscTab.OtherClock.GetState())
				{
					Render::Text(5, 21, Color(255, 0, 0, 255), Render::Fonts::MenuBold, "Someone is defusing");
				}
				else if(!Menu::Window.MiscTab.OtherWatermark.GetState() && Menu::Window.MiscTab.OtherClock.GetState())
				{
					Render::Text(5, 11, Color(255, 0, 0, 255), Render::Fonts::MenuBold, "Someone is defusing");
				}
				else
				{
					Render::Text(5, 1, Color(255, 0, 0, 255), Render::Fonts::MenuBold, "Someone is defusing");
				}
			}
		}
		if (Menu::Window.SettingsTab.OtherSettingsDebug.GetState())
		{
			CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
			float flCone = pWeapon->GetInaccuracy();
			float flSpread = pWeapon->GetWeaponSpread();

			float flRadius = (flCone + flSpread) * 500.0f;

			CUserCmd* cmdlist = *(CUserCmd**)((DWORD)Interfaces::pInput + 0xEC);
			CUserCmd* pCmd = cmdlist;
			IClientEntity* localplayer = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

			if (localplayer->IsAlive()) {
				char aaaaa[64];
				sprintf_s(aaaaa, "Spread: %d", flRadius);
				Render::Text(5, 95, Color(Menu::Window.ColorTab.MenuInnerR.GetValue(), Menu::Window.ColorTab.MenuInnerG.GetValue(), Menu::Window.ColorTab.MenuInnerB.GetValue(), 255), Render::Fonts::MenuBold, aaaaa);

				Render::Text(5, 110, Color(Menu::Window.ColorTab.MenuInnerR.GetValue(), Menu::Window.ColorTab.MenuInnerG.GetValue(), Menu::Window.ColorTab.MenuInnerB.GetValue(), 255), Render::Fonts::MenuBold, "LBY:");
				if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
				{
					if (pCmd->viewangles.y - *localplayer->GetLowerBodyYawTarget() >= -35 && pCmd->viewangles.y - *localplayer->GetLowerBodyYawTarget() <= 35)
						Render::Text(5, 125, Color(255, 0, 30, 255), Render::Fonts::MenuBold, "BROKEN");
					else
						Render::Text(5, 125, Color(0, 255, 0, 255), Render::Fonts::MenuBold, "SAFE");
				}
			}
		}
		if (Menu::Window.VisualsTab.OtherLBYDraw.GetState())
		{
			CUserCmd* cmdlist = *(CUserCmd**)((DWORD)Interfaces::pInput + 0xEC);
			CUserCmd* pCmd = cmdlist;

			IClientEntity* localplayer = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
			RECT TextSize = Render::GetTextSize(Render::Fonts::LBY, "LBY");
			RECT scrn = Render::GetViewport();
			if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
				if (pCmd->viewangles.y - *localplayer->GetLowerBodyYawTarget() >= -35 && pCmd->viewangles.y - *localplayer->GetLowerBodyYawTarget() <= 35)
					Render::Text(10, scrn.bottom - 60, Color(255, 0, 30, 255), Render::Fonts::LBY, "LBY");
				else
					Render::Text(10, scrn.bottom - 60, Color(0, 255, 0, 255), Render::Fonts::LBY, "LBY");
		}
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
			Hacks::DrawHacks();

		// Update and draw the menu
		Menu::DoUIFrame();
	}
}

// InPrediction Hooked Function
bool __stdcall Hooked_InPrediction()
{
	bool result;
	static InPrediction_ origFunc = (InPrediction_)Hooks::VMTPrediction.GetOriginalFunction(14);
	static DWORD *ecxVal = Interfaces::Prediction;
	result = origFunc(ecxVal);

	// If we are in the right place where the player view is calculated
	// Calculate the change in the view and get rid of it
	if (Menu::Window.VisualsTab.OtherNoVisualRecoil.GetState() && (DWORD)(_ReturnAddress()) == Offsets::Functions::dwCalcPlayerView)
	{
		IClientEntity* pLocalEntity = NULL;

		float* m_LocalViewAngles = NULL;

		__asm
		{
			MOV pLocalEntity, ESI
			MOV m_LocalViewAngles, EBX
		}

		Vector viewPunch = pLocalEntity->localPlayerExclusive()->GetViewPunchAngle();
		Vector aimPunch = pLocalEntity->localPlayerExclusive()->GetAimPunchAngle();

		m_LocalViewAngles[0] -= (viewPunch[0] + (aimPunch[0] * 2 * 0.4499999f));
		m_LocalViewAngles[1] -= (viewPunch[1] + (aimPunch[1] * 2 * 0.4499999f));
		m_LocalViewAngles[2] -= (viewPunch[2] + (aimPunch[2] * 2 * 0.4499999f));
		return true;
	}

	return result;
}

// DrawModelExec for chams and shit
void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld)
{
	Color color;
	float flColor[3] = { 0.f };
	static IMaterial* CoveredLit = CreateMaterial(true);
	static IMaterial* OpenLit = CreateMaterial(false);
	static IMaterial* CoveredFlat = CreateMaterial(true, false);
	static IMaterial* OpenFlat = CreateMaterial(false, false);
	bool DontDraw = false;

	const char* ModelName = Interfaces::ModelInfo->GetModelName((model_t*)pInfo.pModel);
	IClientEntity* pModelEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(pInfo.entity_index);
	IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Menu::Window.VisualsTab.Active.GetState())
	{
		// Player Chams
		int ChamsStyle = Menu::Window.VisualsTab.OptionsChams.GetIndex();
		int HandsStyle = Menu::Window.VisualsTab.OtherNoHands.GetIndex();
		int WeaponsStyle = Menu::Window.VisualsTab.OtherWeapons.GetIndex();


		//MADE BY CyclesProgramming
		//

		//terrorist red
		float TR = Menu::Window.ColorTab.TVisColorR.GetValue();
		float TNVR = Menu::Window.ColorTab.TNVisColorR.GetValue();
		//terrorist green
		float TG = Menu::Window.ColorTab.TVisColorG.GetValue();
		float TNVG = Menu::Window.ColorTab.TNVisColorG.GetValue();
		//terrorist green
		float TB = Menu::Window.ColorTab.TVisColorB.GetValue();
		float TNVB = Menu::Window.ColorTab.TNVisColorB.GetValue();

		//ct red
		float CTR = Menu::Window.ColorTab.CTVisColorR.GetValue();
		float CTNVR = Menu::Window.ColorTab.CTNVisColorR.GetValue();
		//ct green
		float CTG = Menu::Window.ColorTab.CTVisColorG.GetValue();
		float CTNVG = Menu::Window.ColorTab.CTNVisColorG.GetValue();
		//ct blue
		float CTB = Menu::Window.ColorTab.CTVisColorB.GetValue();
		float CTNVB = Menu::Window.ColorTab.CTNVisColorB.GetValue();

		if (ChamsStyle != 0 && ChamsStyle != 3 && Menu::Window.VisualsTab.FiltersPlayers.GetState() && strstr(ModelName, "models/player"))
		{
			if (pLocal/* && (!Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() || pModelEntity->GetTeamNum() != pLocal->GetTeamNum())*/)
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;

				IClientEntity* pModelEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(pInfo.entity_index);
				if (pModelEntity)
				{
					IClientEntity *local = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
					if (local)
					{
						if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0 /*&& pModelEntity->GetTeamNum() != local->GetTeamNum()*/)
						{
							int alpharaw = Menu::Window.ColorTab.CharmsA.GetValue();

							float alpha2 = alpharaw / 255;
							float alpha;

							if (pModelEntity->HasGunGameImmunity())
								alpha2 = 0.5f;

							if (Menu::Window.VisualsTab.OptionsFriendly.GetIndex() == 1)
							{
								if (hackManager.pLocal()->GetTeamNum() == 2)
								{
									Menu::Window.ColorTab.TVisColorR.SetValue(255);
									Menu::Window.ColorTab.TVisColorG.SetValue(0);
									Menu::Window.ColorTab.TVisColorB.SetValue(0);
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 0 || Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 1)
									{
										Menu::Window.ColorTab.TNVisColorR.SetValue(255);
										Menu::Window.ColorTab.TNVisColorG.SetValue(0);
										Menu::Window.ColorTab.TNVisColorB.SetValue(0);
									}
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 2)
									{
										Menu::Window.ColorTab.TNVisColorR.SetValue(0);
										Menu::Window.ColorTab.TNVisColorG.SetValue(255);
										Menu::Window.ColorTab.TNVisColorB.SetValue(0);
									}

									Menu::Window.ColorTab.CTVisColorR.SetValue(0);
									Menu::Window.ColorTab.CTVisColorG.SetValue(255);
									Menu::Window.ColorTab.CTVisColorB.SetValue(0);
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 0 || Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 1)
									{
										Menu::Window.ColorTab.CTNVisColorR.SetValue(0);
										Menu::Window.ColorTab.CTNVisColorG.SetValue(255);
										Menu::Window.ColorTab.CTNVisColorB.SetValue(0);
									}
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 2)
									{
										Menu::Window.ColorTab.CTNVisColorR.SetValue(255);
										Menu::Window.ColorTab.CTNVisColorG.SetValue(0);
										Menu::Window.ColorTab.CTNVisColorB.SetValue(0);
									}

								}
								else
								{
									Menu::Window.ColorTab.TVisColorR.SetValue(0);
									Menu::Window.ColorTab.TVisColorG.SetValue(255);
									Menu::Window.ColorTab.TVisColorB.SetValue(0);
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 0 || Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 1)
									{
										Menu::Window.ColorTab.TNVisColorR.SetValue(0);
										Menu::Window.ColorTab.TNVisColorG.SetValue(255);
										Menu::Window.ColorTab.TNVisColorB.SetValue(0);
									}
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 2)
									{
										Menu::Window.ColorTab.TNVisColorR.SetValue(255);
										Menu::Window.ColorTab.TNVisColorG.SetValue(0);
										Menu::Window.ColorTab.TNVisColorB.SetValue(0);
									}

									Menu::Window.ColorTab.CTVisColorR.SetValue(255);
									Menu::Window.ColorTab.CTVisColorG.SetValue(0);
									Menu::Window.ColorTab.CTVisColorB.SetValue(0);
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 0 || Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 1)
									{
										Menu::Window.ColorTab.CTNVisColorR.SetValue(255);
										Menu::Window.ColorTab.CTNVisColorG.SetValue(0);
										Menu::Window.ColorTab.CTNVisColorB.SetValue(0);
									}
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 2)
									{
										Menu::Window.ColorTab.CTNVisColorR.SetValue(0);
										Menu::Window.ColorTab.CTNVisColorG.SetValue(255);
										Menu::Window.ColorTab.CTNVisColorB.SetValue(0);
									}

								}
							}
							if (Menu::Window.VisualsTab.OptionsFriendly.GetIndex() == 2)
							{
								if (hackManager.pLocal()->GetTeamNum() == 2)
								{
									Menu::Window.ColorTab.TVisColorR.SetValue(0);
									Menu::Window.ColorTab.TVisColorG.SetValue(255);
									Menu::Window.ColorTab.TVisColorB.SetValue(0);
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 0 || Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 1)
									{
										Menu::Window.ColorTab.TNVisColorR.SetValue(0);
										Menu::Window.ColorTab.TNVisColorG.SetValue(255);
										Menu::Window.ColorTab.TNVisColorB.SetValue(0);
									}
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 2)
									{
										Menu::Window.ColorTab.TNVisColorR.SetValue(255);
										Menu::Window.ColorTab.TNVisColorG.SetValue(0);
										Menu::Window.ColorTab.TNVisColorB.SetValue(0);
									}

									Menu::Window.ColorTab.CTVisColorR.SetValue(255);
									Menu::Window.ColorTab.CTVisColorG.SetValue(0);
									Menu::Window.ColorTab.CTVisColorB.SetValue(0);
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 0 || Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 1)
									{
										Menu::Window.ColorTab.CTNVisColorR.SetValue(255);
										Menu::Window.ColorTab.CTNVisColorG.SetValue(0);
										Menu::Window.ColorTab.CTNVisColorB.SetValue(0);
									}
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 2)
									{
										Menu::Window.ColorTab.CTNVisColorR.SetValue(0);
										Menu::Window.ColorTab.CTNVisColorG.SetValue(255);
										Menu::Window.ColorTab.CTNVisColorB.SetValue(0);
									}
								}
								else
								{
									Menu::Window.ColorTab.TVisColorR.SetValue(255);
									Menu::Window.ColorTab.TVisColorG.SetValue(0);
									Menu::Window.ColorTab.TVisColorB.SetValue(0);
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 1)
									{
										Menu::Window.ColorTab.TNVisColorR.SetValue(255);
										Menu::Window.ColorTab.TNVisColorG.SetValue(0);
										Menu::Window.ColorTab.TNVisColorB.SetValue(0);
									}
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 2)
									{
										Menu::Window.ColorTab.TNVisColorR.SetValue(0);
										Menu::Window.ColorTab.TNVisColorG.SetValue(255);
										Menu::Window.ColorTab.TNVisColorB.SetValue(0);
									}

									Menu::Window.ColorTab.CTVisColorR.SetValue(0);
									Menu::Window.ColorTab.CTVisColorG.SetValue(255);
									Menu::Window.ColorTab.CTVisColorB.SetValue(0);
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 1)
									{
										Menu::Window.ColorTab.CTNVisColorR.SetValue(0);
										Menu::Window.ColorTab.CTNVisColorG.SetValue(255);
										Menu::Window.ColorTab.CTNVisColorB.SetValue(0);
									}
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 2)
									{
										Menu::Window.ColorTab.CTNVisColorR.SetValue(255);
										Menu::Window.ColorTab.CTNVisColorG.SetValue(0);
										Menu::Window.ColorTab.CTNVisColorB.SetValue(0);
									}
								}
							}

							if (Menu::Window.VisualsTab.OptionsFriendly.GetIndex() == 3)
							{
								if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 3)
								{
									if (hackManager.pLocal()->GetTeamNum() == 2)
									{
										Menu::Window.ColorTab.TVisColorR.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMVISR.GetValue());
										Menu::Window.ColorTab.TVisColorG.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMVISG.GetValue());
										Menu::Window.ColorTab.TVisColorB.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMVISB.GetValue());
										Menu::Window.ColorTab.TNVisColorR.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMNOVISR.GetValue());
										Menu::Window.ColorTab.TNVisColorG.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMNOVISG.GetValue());
										Menu::Window.ColorTab.TNVisColorB.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMNOVISB.GetValue());

										Menu::Window.ColorTab.CTVisColorR.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYVISR.GetValue());
										Menu::Window.ColorTab.CTVisColorG.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYVISG.GetValue());
										Menu::Window.ColorTab.CTVisColorB.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYVISB.GetValue());
										Menu::Window.ColorTab.CTNVisColorR.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYNOVISR.GetValue());
										Menu::Window.ColorTab.CTNVisColorG.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYNOVISG.GetValue());
										Menu::Window.ColorTab.CTNVisColorB.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYNOVISB.GetValue());
									}
									else
									{
										Menu::Window.ColorTab.TVisColorR.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYVISR.GetValue());
										Menu::Window.ColorTab.TVisColorG.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYVISG.GetValue());
										Menu::Window.ColorTab.TVisColorB.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYVISB.GetValue());
										Menu::Window.ColorTab.TNVisColorR.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYNOVISR.GetValue());
										Menu::Window.ColorTab.TNVisColorG.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYNOVISG.GetValue());
										Menu::Window.ColorTab.TNVisColorB.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYNOVISB.GetValue());

										Menu::Window.ColorTab.CTVisColorR.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMVISR.GetValue());
										Menu::Window.ColorTab.CTVisColorG.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMVISG.GetValue());
										Menu::Window.ColorTab.CTVisColorB.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMVISB.GetValue());
										Menu::Window.ColorTab.CTNVisColorR.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMNOVISR.GetValue());
										Menu::Window.ColorTab.CTNVisColorG.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMNOVISG.GetValue());
										Menu::Window.ColorTab.CTNVisColorB.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMNOVISB.GetValue());
									}
								}
							}


							if (pModelEntity->GetTeamNum() == 2)
							{
								flColor[0] = TNVR / 255.f;
								flColor[1] = TNVG / 255.f;
								flColor[2] = TNVB / 255.f;
								if (ChamsStyle == 3 || ChamsStyle == 4)
								{
									alpha = 0;
								}
								else
								{
									alpha = alpha2;
								}

							}
							else
							{
								flColor[0] = CTNVR / 255.f;
								flColor[1] = CTNVG / 255.f;
								flColor[2] = CTNVB / 255.f;
								if (ChamsStyle == 3 || ChamsStyle == 4)
								{
									alpha = 0;
								}
								else
								{
									alpha = alpha2;
								}
							}

							Interfaces::RenderView->SetColorModulation(flColor);
							Interfaces::RenderView->SetBlend(alpha);
							Interfaces::ModelRender->ForcedMaterialOverride(covered);
							oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

							if (pModelEntity->GetTeamNum() == 2)
							{
								flColor[0] = TR / 255.f;
								flColor[1] = TG / 255.f;
								flColor[2] = TB / 255.f;
							}
							else
							{
								flColor[0] = CTR / 255.f;
								flColor[1] = CTG / 255.f;
								flColor[2] = CTB / 255.f;
							}

							Interfaces::RenderView->SetColorModulation(flColor);
							Interfaces::RenderView->SetBlend(alpha);
							Interfaces::ModelRender->ForcedMaterialOverride(open);
						}
						else
						{
							color.SetColor(255, 255, 255, 255);
							ForceMaterial(color, open);
						}
					}
				}
			}
		}
		else if (ChamsStyle >= 3 && Menu::Window.VisualsTab.FiltersPlayers.GetState() && strstr(ModelName, "models/player"))
		{
			if (pLocal/* && (!Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() || pModelEntity->GetTeamNum() != pLocal->GetTeamNum())*/)
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;

				IClientEntity* pModelEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(pInfo.entity_index);
				if (pModelEntity)
				{
					IClientEntity *local = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
					if (local)
					{
						if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0 /*&& pModelEntity->GetTeamNum() != local->GetTeamNum()*/)
						{
							int alpharaw = Menu::Window.ColorTab.CharmsA.GetValue();

							float alpha2 = alpharaw / 255;
							float alpha;

							if (pModelEntity->HasGunGameImmunity())
								alpha2 = 0.5f;

							if (Menu::Window.VisualsTab.OptionsFriendly.GetIndex() == 1)
							{
								if (hackManager.pLocal()->GetTeamNum() == 2)
								{
									Menu::Window.ColorTab.TVisColorR.SetValue(255);
									Menu::Window.ColorTab.TVisColorG.SetValue(0);
									Menu::Window.ColorTab.TVisColorB.SetValue(0);
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 0 || Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 1)
									{
										Menu::Window.ColorTab.TNVisColorR.SetValue(255);
										Menu::Window.ColorTab.TNVisColorG.SetValue(0);
										Menu::Window.ColorTab.TNVisColorB.SetValue(0);
									}
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 2)
									{
										Menu::Window.ColorTab.TNVisColorR.SetValue(0);
										Menu::Window.ColorTab.TNVisColorG.SetValue(255);
										Menu::Window.ColorTab.TNVisColorB.SetValue(0);
									}

									Menu::Window.ColorTab.CTVisColorR.SetValue(0);
									Menu::Window.ColorTab.CTVisColorG.SetValue(255);
									Menu::Window.ColorTab.CTVisColorB.SetValue(0);
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 0 || Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 1)
									{
										Menu::Window.ColorTab.CTNVisColorR.SetValue(0);
										Menu::Window.ColorTab.CTNVisColorG.SetValue(255);
										Menu::Window.ColorTab.CTNVisColorB.SetValue(0);
									}
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 2)
									{
										Menu::Window.ColorTab.CTNVisColorR.SetValue(255);
										Menu::Window.ColorTab.CTNVisColorG.SetValue(0);
										Menu::Window.ColorTab.CTNVisColorB.SetValue(0);
									}

								}
								else
								{
									Menu::Window.ColorTab.TVisColorR.SetValue(0);
									Menu::Window.ColorTab.TVisColorG.SetValue(255);
									Menu::Window.ColorTab.TVisColorB.SetValue(0);
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 0 || Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 1)
									{
										Menu::Window.ColorTab.TNVisColorR.SetValue(0);
										Menu::Window.ColorTab.TNVisColorG.SetValue(255);
										Menu::Window.ColorTab.TNVisColorB.SetValue(0);
									}
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 2)
									{
										Menu::Window.ColorTab.TNVisColorR.SetValue(255);
										Menu::Window.ColorTab.TNVisColorG.SetValue(0);
										Menu::Window.ColorTab.TNVisColorB.SetValue(0);
									}

									Menu::Window.ColorTab.CTVisColorR.SetValue(255);
									Menu::Window.ColorTab.CTVisColorG.SetValue(0);
									Menu::Window.ColorTab.CTVisColorB.SetValue(0);
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 0 || Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 1)
									{
										Menu::Window.ColorTab.CTNVisColorR.SetValue(255);
										Menu::Window.ColorTab.CTNVisColorG.SetValue(0);
										Menu::Window.ColorTab.CTNVisColorB.SetValue(0);
									}
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 2)
									{
										Menu::Window.ColorTab.CTNVisColorR.SetValue(0);
										Menu::Window.ColorTab.CTNVisColorG.SetValue(255);
										Menu::Window.ColorTab.CTNVisColorB.SetValue(0);
									}

								}
							}
							if (Menu::Window.VisualsTab.OptionsFriendly.GetIndex() == 2)
							{
								if (hackManager.pLocal()->GetTeamNum() == 2)
								{
									Menu::Window.ColorTab.TVisColorR.SetValue(0);
									Menu::Window.ColorTab.TVisColorG.SetValue(255);
									Menu::Window.ColorTab.TVisColorB.SetValue(0);
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 0 || Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 1)
									{
										Menu::Window.ColorTab.TNVisColorR.SetValue(0);
										Menu::Window.ColorTab.TNVisColorG.SetValue(255);
										Menu::Window.ColorTab.TNVisColorB.SetValue(0);
									}
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 2)
									{
										Menu::Window.ColorTab.TNVisColorR.SetValue(255);
										Menu::Window.ColorTab.TNVisColorG.SetValue(0);
										Menu::Window.ColorTab.TNVisColorB.SetValue(0);
									}

									Menu::Window.ColorTab.CTVisColorR.SetValue(255);
									Menu::Window.ColorTab.CTVisColorG.SetValue(0);
									Menu::Window.ColorTab.CTVisColorB.SetValue(0);
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 0 || Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 1)
									{
										Menu::Window.ColorTab.CTNVisColorR.SetValue(255);
										Menu::Window.ColorTab.CTNVisColorG.SetValue(0);
										Menu::Window.ColorTab.CTNVisColorB.SetValue(0);
									}
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 2)
									{
										Menu::Window.ColorTab.CTNVisColorR.SetValue(0);
										Menu::Window.ColorTab.CTNVisColorG.SetValue(255);
										Menu::Window.ColorTab.CTNVisColorB.SetValue(0);
									}
								}
								else
								{
									Menu::Window.ColorTab.TVisColorR.SetValue(255);
									Menu::Window.ColorTab.TVisColorG.SetValue(0);
									Menu::Window.ColorTab.TVisColorB.SetValue(0);
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 1)
									{
										Menu::Window.ColorTab.TNVisColorR.SetValue(255);
										Menu::Window.ColorTab.TNVisColorG.SetValue(0);
										Menu::Window.ColorTab.TNVisColorB.SetValue(0);
									}
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 2)
									{
										Menu::Window.ColorTab.TNVisColorR.SetValue(0);
										Menu::Window.ColorTab.TNVisColorG.SetValue(255);
										Menu::Window.ColorTab.TNVisColorB.SetValue(0);
									}

									Menu::Window.ColorTab.CTVisColorR.SetValue(0);
									Menu::Window.ColorTab.CTVisColorG.SetValue(255);
									Menu::Window.ColorTab.CTVisColorB.SetValue(0);
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 1)
									{
										Menu::Window.ColorTab.CTNVisColorR.SetValue(0);
										Menu::Window.ColorTab.CTNVisColorG.SetValue(255);
										Menu::Window.ColorTab.CTNVisColorB.SetValue(0);
									}
									if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 2)
									{
										Menu::Window.ColorTab.CTNVisColorR.SetValue(255);
										Menu::Window.ColorTab.CTNVisColorG.SetValue(0);
										Menu::Window.ColorTab.CTNVisColorB.SetValue(0);
									}
								}
							}

							if (Menu::Window.VisualsTab.OptionsFriendly.GetIndex() == 3)
							{
								if (Menu::Window.VisualsTab.OptionsFriendly2.GetIndex() == 3)
								{
									if (hackManager.pLocal()->GetTeamNum() == 2)
									{
										Menu::Window.ColorTab.TVisColorR.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMVISR.GetValue());
										Menu::Window.ColorTab.TVisColorG.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMVISG.GetValue());
										Menu::Window.ColorTab.TVisColorB.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMVISB.GetValue());
										Menu::Window.ColorTab.TNVisColorR.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMNOVISR.GetValue());
										Menu::Window.ColorTab.TNVisColorG.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMNOVISG.GetValue());
										Menu::Window.ColorTab.TNVisColorB.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMNOVISB.GetValue());

										Menu::Window.ColorTab.CTVisColorR.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYVISR.GetValue());
										Menu::Window.ColorTab.CTVisColorG.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYVISG.GetValue());
										Menu::Window.ColorTab.CTVisColorB.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYVISB.GetValue());
										Menu::Window.ColorTab.CTNVisColorR.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYNOVISR.GetValue());
										Menu::Window.ColorTab.CTNVisColorG.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYNOVISG.GetValue());
										Menu::Window.ColorTab.CTNVisColorB.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYNOVISB.GetValue());
									}
									else
									{
										Menu::Window.ColorTab.TVisColorR.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYVISR.GetValue());
										Menu::Window.ColorTab.TVisColorG.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYVISG.GetValue());
										Menu::Window.ColorTab.TVisColorB.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYVISB.GetValue());
										Menu::Window.ColorTab.TNVisColorR.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYNOVISR.GetValue());
										Menu::Window.ColorTab.TNVisColorG.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYNOVISG.GetValue());
										Menu::Window.ColorTab.TNVisColorB.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorENEMYNOVISB.GetValue());

										Menu::Window.ColorTab.CTVisColorR.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMVISR.GetValue());
										Menu::Window.ColorTab.CTVisColorG.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMVISG.GetValue());
										Menu::Window.ColorTab.CTVisColorB.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMVISB.GetValue());
										Menu::Window.ColorTab.CTNVisColorR.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMNOVISR.GetValue());
										Menu::Window.ColorTab.CTNVisColorG.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMNOVISG.GetValue());
										Menu::Window.ColorTab.CTNVisColorB.SetValue(Menu::Window.OtherSettingsTab.customfriendlycolorTEAMNOVISB.GetValue());
									}
								}
							}

							if (pModelEntity->GetTeamNum() == 2)
							{
								flColor[0] = TNVR / 255.f;
								flColor[1] = TNVG / 255.f;
								flColor[2] = TNVB / 255.f;
								alpha = alpha2;
							}
							else
							{
								flColor[0] = CTNVR / 255.f;
								flColor[1] = CTNVG / 255.f;
								flColor[2] = CTNVB / 255.f;
								alpha = alpha2;
							}

							Interfaces::RenderView->SetColorModulation(flColor);
							Interfaces::RenderView->SetBlend(alpha);
							if (ChamsStyle == 3) {
								covered->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
								covered->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
							}
							Interfaces::ModelRender->ForcedMaterialOverride(covered);
							oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

							if (pModelEntity->GetTeamNum() == 2)
							{
								flColor[0] = TR / 255.f;
								flColor[1] = TG / 255.f;
								flColor[2] = TB / 255.f;
							}
							else
							{
								flColor[0] = CTR / 255.f;
								flColor[1] = CTG / 255.f;
								flColor[2] = CTB / 255.f;
							}

							Interfaces::RenderView->SetColorModulation(flColor);
							Interfaces::RenderView->SetBlend(alpha);
							Interfaces::ModelRender->ForcedMaterialOverride(open);
						}
						else
						{
							color.SetColor(255, 255, 255, 255);
							ForceMaterial(color, open);
						}
					}
				}
			}
		}
		else if (HandsStyle == 0)
		{
			Interfaces::RenderView->SetBlend(1);
		}
		else if (HandsStyle != 0 && strstr(ModelName, "arms"))
		{
			if (HandsStyle == 1)
			{
				DontDraw = true;
			}
			else if (HandsStyle == 2)
			{
				Interfaces::RenderView->SetBlend(0.3);
			}
			else if (HandsStyle == 3)
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
				if (pLocal)
				{
					if (pLocal->IsAlive())
					{
						int alpha = 255;

						if (pLocal->GetTeamNum() == 2)
							color.SetColor(255, 0, 0, alpha);
						else
							color.SetColor(0, 0, 255, alpha);

						ForceMaterial(color, OpenLit);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

						if (pLocal->GetTeamNum() == 2)
							color.SetColor(255, 0, 0, alpha);
						else
							color.SetColor(0, 0, 255, alpha);
					}
					else
					{
						color.SetColor(255, 255, 255, 255);
					}

					ForceMaterial(color, open);
				}
			}
			else if (HandsStyle == 4)
			{
				static int counter = 0;
				static float colors[3] = { 1.f, 0.f, 0.f };

				if (colors[counter] >= 1.0f)
				{
					colors[counter] = 1.0f;
					counter += 1;
					if (counter > 2)
						counter = 0;
				}
				else
				{
					int prev = counter - 1;
					if (prev < 0) prev = 2;
					colors[prev] -= 0.05f;
					colors[counter] += 0.05f;
				}

				Interfaces::RenderView->SetColorModulation(colors);
				Interfaces::RenderView->SetBlend(0.3);
				Interfaces::ModelRender->ForcedMaterialOverride(OpenLit);
			}
			else
			{
				static int counter = 0;
				static float colors[3] = { 1.f, 0.f, 0.f };

				if (colors[counter] >= 1.0f)
				{
					colors[counter] = 1.0f;
					counter += 1;
					if (counter > 2)
						counter = 0;
				}
				else
				{
					int prev = counter - 1;
					if (prev < 0) prev = 2;
					colors[prev] -= 0.05f;
					colors[counter] += 0.05f;
				}

				Interfaces::RenderView->SetColorModulation(colors);
				Interfaces::RenderView->SetBlend(1);
				Interfaces::ModelRender->ForcedMaterialOverride(OpenLit);
			}
		}
		else if (WeaponsStyle != 0 && strstr(ModelName, "weapons/v"))
		{
			if (WeaponsStyle == 1)
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
				if (pLocal)
				{
					if (pLocal->IsAlive())
					{
						int alpha = 255;
						color.SetColor(255, 0, 0, alpha);

						ForceMaterial(color, OpenLit);
						OpenLit->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
					}
					else
					{
						color.SetColor(255, 255, 255, 255);
					}

					ForceMaterial(color, open);
				}
			}
			else if (WeaponsStyle == 2)
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
				if (pLocal)
				{
					if (pLocal->IsAlive())
					{
						int alpha = 255;
						color.SetColor(0, 0, 255, alpha);

						ForceMaterial(color, OpenLit);
						OpenLit->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
					}
					else
					{
						color.SetColor(255, 255, 255, 255);
					}

					ForceMaterial(color, open);
				}
			}
			else if (WeaponsStyle == 3)
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
				if (pLocal)
				{
					if (pLocal->IsAlive())
					{
						int alpha = 255;
						color.SetColor(0, 255, 0, alpha);

						ForceMaterial(color, OpenLit);
						OpenLit->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
					}
					else
					{
						color.SetColor(255, 255, 255, 255);
					}

					ForceMaterial(color, open);
				}
			}
			else if (WeaponsStyle == 4)
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
				if (pLocal)
				{
					if (pLocal->IsAlive())
					{
						int alpha = 255;
						color.SetColor(255, 255, 0, alpha);

						ForceMaterial(color, OpenLit);
						OpenLit->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
					}
					else
					{
						color.SetColor(255, 255, 255, 255);
					}

					ForceMaterial(color, open);
				}
			}
			else if (WeaponsStyle == 5)
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
				if (pLocal)
				{
					if (pLocal->IsAlive())
					{
						int alpha = 255;
						color.SetColor(128, 0, 128, alpha);

						ForceMaterial(color, OpenLit);
						OpenLit->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
					}
					else
					{
						color.SetColor(255, 255, 255, 255);
					}

					ForceMaterial(color, open);
				}
			}
			if (WeaponsStyle == 6)
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
				if (pLocal)
				{
					if (pLocal->IsAlive())
					{
						color.SetColor(255, 0, 0, 50);

						ForceMaterial(color, OpenLit);
						OpenLit->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
					}
					else
					{
						color.SetColor(255, 255, 255, 50);
					}

					ForceMaterial(color, open);
				}
			}
			if (WeaponsStyle == 7)
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
				if (pLocal)
				{
					if (pLocal->IsAlive())
					{
						color.SetColor(0, 0, 255, 50);

						ForceMaterial(color, OpenLit);
						OpenLit->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
					}
					else
					{
						color.SetColor(255, 255, 255, 50);
					}

					ForceMaterial(color, open);
				}
			}
			if (WeaponsStyle == 8)
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
				if (pLocal)
				{
					if (pLocal->IsAlive())
					{
						color.SetColor(0, 255, 0, 50);

						ForceMaterial(color, OpenLit);
						OpenLit->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
					}
					else
					{
						color.SetColor(255, 255, 255, 50);
					}

					ForceMaterial(color, open);
				}
			}
			if (WeaponsStyle == 9)
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
				if (pLocal)
				{
					if (pLocal->IsAlive())
					{
						color.SetColor(255, 255, 0, 50);

						ForceMaterial(color, OpenLit);
						OpenLit->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
					}
					else
					{
						color.SetColor(255, 255, 255, 50);
					}

					ForceMaterial(color, open);
				}
			}
			if (WeaponsStyle == 10)
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
				if (pLocal)
				{
					if (pLocal->IsAlive())
					{
						color.SetColor(128, 0, 0, 50);

						ForceMaterial(color, OpenLit);
						OpenLit->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
					}
					else
					{
						color.SetColor(255, 255, 255, 50);
					}

					ForceMaterial(color, open);
				}
			}
			if (WeaponsStyle == 11)
			{
				static int counter = 0;
				static float colors[3] = { 1.f, 0.f, 0.f };

				if (colors[counter] >= 1.0f)
				{
					colors[counter] = 1.0f;
					counter += 1;
					if (counter > 2)
						counter = 0;
				}
				else
				{
					int prev = counter - 1;
					if (prev < 0) prev = 2;
					colors[prev] -= 0.01f;
					colors[counter] += 0.01f;
				}

				Interfaces::RenderView->SetColorModulation(colors);
				Interfaces::RenderView->SetBlend(1);
				OpenLit->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
				Interfaces::ModelRender->ForcedMaterialOverride(OpenLit);
			}
			if (WeaponsStyle == 12)
			{
				static int counter = 0;
				static float colors[3] = { 1.f, 0.f, 0.f };

				if (colors[counter] >= 1.0f)
				{
					colors[counter] = 1.0f;
					counter += 1;
					if (counter > 2)
						counter = 0;
				}
				else
				{
					int prev = counter - 1;
					if (prev < 0) prev = 2;
					colors[prev] -= 0.01f;
					colors[counter] += 0.01f;
				}

				Interfaces::RenderView->SetColorModulation(colors);
				Interfaces::RenderView->SetBlend(1);
				OpenLit->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
				Interfaces::ModelRender->ForcedMaterialOverride(OpenLit);
			}
		}
		else if (ChamsStyle != 0 && Menu::Window.VisualsTab.FiltersWeapons.GetState() && strstr(ModelName, "_dropped.mdl"))
		{
			IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
			color.SetColor(255, 255, 255, 255);
			ForceMaterial(color, covered);
		}
	}


	if (!DontDraw)
		oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	Interfaces::ModelRender->ForcedMaterialOverride(NULL);
}

std::vector<const char*> smoke_materials = {
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	"particle/vistasmokev1/vistasmokev1_fire",
};

// Hooked FrameStageNotify for removing visual recoil
void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage)
{
	DWORD eyeangles = NetVar.GetNetVar(0xBFEA4E7B);
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && curStage == FRAME_RENDER_START)
	{

		if (pLocal->IsAlive())
		{
			if (*(bool*)((DWORD)Interfaces::pInput + 0xA5))
				*(Vector*)((DWORD)pLocal + 0x31C8) = LastAngleAA;
		}

		if (Menu::Window.MiscTab.OtherThirdperson.GetIndex() > 0 || Menu::Window.SettingsTab.OtherSettingsImpacts.GetIndex() > 0 || Menu::Window.MiscTab.OtherSafeMode.GetState() || Menu::Window.VisualsTab.OtherAsus.GetState())
		{
			static bool rekt = false;
			if (!rekt)
			{
				ConVar* sv_cheats = Interfaces::CVar->FindVar("sv_cheats");
				SpoofedConvar* sv_cheats_spoofed = new SpoofedConvar(sv_cheats);
				sv_cheats_spoofed->SetInt(1);
				rekt = true;
			}
		}

		if (Menu::Window.SettingsTab.OtherSettingsImpacts.GetIndex() > 0)
		{
			if (Menu::Window.SettingsTab.OtherSettingsAntiUntrusted.GetState()) {
				Menu::Window.SettingsTab.OtherSettingsImpacts.SelectIndex(0);
				Beep(500, 1000);
			}
			else {
				if (Menu::Window.SettingsTab.OtherSettingsImpacts.GetIndex() == 1)
				{
					ConVar* sv_showimpacts = Interfaces::CVar->FindVar("sv_showimpacts");
					SpoofedConvar* sv_showimpacts_spoofed = new SpoofedConvar(sv_showimpacts);
					sv_showimpacts_spoofed->SetInt(2);
				}
				if (Menu::Window.SettingsTab.OtherSettingsImpacts.GetIndex() == 2)
				{
					ConVar* sv_showimpacts = Interfaces::CVar->FindVar("sv_showimpacts");
					SpoofedConvar* sv_showimpacts_spoofed = new SpoofedConvar(sv_showimpacts);
					sv_showimpacts_spoofed->SetInt(3);
				}
				if (Menu::Window.SettingsTab.OtherSettingsImpacts.GetIndex() == 3)
				{
					ConVar* sv_showimpacts = Interfaces::CVar->FindVar("sv_showimpacts");
					SpoofedConvar* sv_showimpacts_spoofed = new SpoofedConvar(sv_showimpacts);
					sv_showimpacts_spoofed->SetInt(1);
				}
			}
		}

		static bool TP = false;
		if (Menu::Window.MiscTab.OtherThirdperson.GetIndex() > 0 && pLocal->IsAlive())
		{
			if (!TP) {
				Interfaces::Engine->ClientCmd_Unrestricted("thirdperson");
				TP = true;
			}
		}
		if (Menu::Window.MiscTab.OtherThirdperson.GetIndex() == 2)
		{
			CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
			if (GameUtils::IsG(pWeapon)) {
				Interfaces::Engine->ClientCmd_Unrestricted("firstperson");
				TP = false;
			}
		}
		if (Menu::Window.MiscTab.OtherThirdperson.GetIndex() > 0 && pLocal->IsAlive() == 0)
		{
			if (TP) {
				Interfaces::Engine->ClientCmd_Unrestricted("firstperson");
				TP = false;
			}
		}
		if (Menu::Window.MiscTab.OtherThirdperson.GetIndex() == 0)
		{
			if (TP) {
				Interfaces::Engine->ClientCmd_Unrestricted("firstperson");
				TP = false;
			}
		}

		if (Menu::Window.RageBotTab.AccuracyPVS.GetState())
		{
			for (int i = 1; i < Interfaces::Globals->maxClients; i++)
			{
				if (i == Interfaces::Engine->GetLocalPlayer()) continue;
				IClientEntity* pCurEntity = Interfaces::EntList->GetClientEntity(i);
				if (!pCurEntity) continue;
				*(int*)((uintptr_t)pCurEntity + 0xA30) = Interfaces::Globals->framecount;
				*(int*)((uintptr_t)pCurEntity + 0xA28) = 0;
			}
		}

		if (curStage == FRAME_RENDER_START)
		{
			for (int i = 1; i <= Interfaces::Globals->maxClients; i++)
			{
				if (i == Interfaces::Engine->GetLocalPlayer()) continue;

				IClientEntity* pCurEntity = Interfaces::EntList->GetClientEntity(i);
				if (!pCurEntity) continue;

				*(int*)((uintptr_t)pCurEntity + 0xA30) = Interfaces::Globals->framecount; //we'll skip occlusion checks now
				*(int*)((uintptr_t)pCurEntity + 0xA28) = 0;//clear occlusion flags
			}
			for (auto matName : smoke_materials)
			{
				IMaterial* mat = Interfaces::MaterialSystem->FindMaterial(matName, "Other textures");
				mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, Menu::Window.VisualsTab.OtherNoSmoke.GetState());
			}
		}
	}

	if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
		//Utilities::Log("APPLY SKIN APPLY SKIN");
		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		int iBayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
		int iButterfly = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
		int iFlip = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
		int iGut = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
		int iKarambit = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
		int iM9Bayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
		int iHuntsman = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
		int iFalchion = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
		int iDagger = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
		int iBowie = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");

		int iGunGame = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gg.mdl");

		for (int i = 0; i <= Interfaces::EntList->GetHighestEntityIndex(); i++) // CHANGE
		{
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

			if (pEntity)
			{
				ULONG hOwnerEntity = *(PULONG)((DWORD)pEntity + 0x148);

				IClientEntity* pOwner = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)hOwnerEntity);

				if (pOwner)
				{
					if (pOwner == pLocal)
					{
						std::string sWeapon = Interfaces::ModelInfo->GetModelName(pEntity->GetModel());

						auto weps = pLocal->Weapons();
						for (size_t i = 0; weps[i] != nullptr; i++) {
							auto pWeapons = reinterpret_cast<CBaseCombatWeapon*>(Interfaces::EntList->GetClientEntityFromHandle(weps[i]));

							if (sWeapon.find("c4_planted", 0) != std::string::npos)
								continue;

							if (sWeapon.find("thrown", 0) != std::string::npos)
								continue;

							if (sWeapon.find("smokegrenade", 0) != std::string::npos)
								continue;

							if (sWeapon.find("flashbang", 0) != std::string::npos)
								continue;

							if (sWeapon.find("fraggrenade", 0) != std::string::npos)
								continue;

							if (sWeapon.find("molotov", 0) != std::string::npos)
								continue;

							if (sWeapon.find("decoy", 0) != std::string::npos)
								continue;

							if (sWeapon.find("incendiarygrenade", 0) != std::string::npos)
								continue;

							if (sWeapon.find("ied", 0) != std::string::npos)
								continue;

							if (sWeapon.find("w_eq_", 0) != std::string::npos)
								continue;

							CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)pEntity;
	
							ClientClass *pClass = Interfaces::Client->GetAllClasses();


							if (Menu::Window.SkinchangerTab.SkinEnable.GetState() && pLocal->IsAlive())
							{
								int Model = Menu::Window.SkinchangerTab.KnifeModel.GetIndex();

								int M41S = Menu::Window.SkinchangerTab.M41SSkin.GetIndex();
								int M4A4 = Menu::Window.SkinchangerTab.M4A4Skin.GetIndex();
								int AK47 = Menu::Window.SkinchangerTab.AK47Skin.GetIndex();
								int AWP = Menu::Window.SkinchangerTab.AWPSkin.GetIndex();
								int GLOCK = Menu::Window.SkinchangerTab.GLOCKSkin.GetIndex();
								int USPS = Menu::Window.SkinchangerTab.USPSSkin.GetIndex();
								int DEAGLE = Menu::Window.SkinchangerTab.DEAGLESkin.GetIndex();
								int FIVE7 = Menu::Window.SkinchangerTab.FIVESEVENSkin.GetIndex();
								int AUG = Menu::Window.SkinchangerTab.AUGSkin.GetIndex();
								int FAMAS = Menu::Window.SkinchangerTab.FAMASSkin.GetIndex();
								int G3SG1 = Menu::Window.SkinchangerTab.G3SG1Skin.GetIndex();
								int Galil = Menu::Window.SkinchangerTab.GALILSkin.GetIndex();
								int M249 = Menu::Window.SkinchangerTab.M249Skin.GetIndex();
								int MAC10 = Menu::Window.MiscTab.MAC10Skin.GetIndex();
								int P90 = Menu::Window.MiscTab.P90Skin.GetIndex();
								int UMP45 = Menu::Window.MiscTab.UMP45Skin.GetIndex();
								int XM1014 = Menu::Window.SkinchangerTab.XM1014Skin.GetIndex();
								int BIZON = Menu::Window.MiscTab.BIZONSkin.GetIndex();
								int MAG7 = Menu::Window.SkinchangerTab.MAG7Skin.GetIndex();
								int NEGEV = Menu::Window.SkinchangerTab.NEGEVSkin.GetIndex();
								int SAWEDOFF = Menu::Window.SkinchangerTab.SAWEDOFFSkin.GetIndex();
								int TEC9 = Menu::Window.SkinchangerTab.TECNINESkin.GetIndex();
								int P2000 = Menu::Window.SkinchangerTab.P2000Skin.GetIndex();
								int MP7 = Menu::Window.MiscTab.MP7Skin.GetIndex();
								int MP9 = Menu::Window.MiscTab.MP9Skin.GetIndex();
								int NOVA = Menu::Window.SkinchangerTab.NOVASkin.GetIndex();
								int P250 = Menu::Window.SkinchangerTab.P250Skin.GetIndex();
								int SCAR20 = Menu::Window.SkinchangerTab.SCAR20Skin.GetIndex();
								int SG553 = Menu::Window.SkinchangerTab.SG553Skin.GetIndex();
								int SSG08 = Menu::Window.SkinchangerTab.SSG08Skin.GetIndex();
								int Magnum = Menu::Window.SkinchangerTab.DEAGLESkin.GetIndex();
								int CZ = Menu::Window.SkinchangerTab.CZSkin.GetIndex();
								int dualies = CZ = Menu::Window.SkinchangerTab.DUALSSkin.GetIndex();


								int weapon = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();

								switch (weapon)
								{
								case 2:
									switch (dualies)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 28;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 43;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 46;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 47;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 153;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 190;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 249;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 220;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 336;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 261;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 276;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 307;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 330;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 447;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 450;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 491;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 528;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 544;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 658;
										break;
									}
									break;
								case 7: // AK47 
								{
									switch (AK47)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 341;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 14;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 44;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 172;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 180;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 394;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 300;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 226;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 282;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 302;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 316;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 340;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 380;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 422;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 456;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 474;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 490;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 506;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 524;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 600;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 639;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 656;
										break;
									case 22:
										*pWeapon->FallbackPaintKit() = 675;
										break;

									default:
										break;
									}
								}
								break;
								case 16: // M4A4
								{
									switch (M4A4)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 155;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 187;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 255;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 309;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 215;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 336;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 384;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 400;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 449;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 471;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 480;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 512;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 533;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 588;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 632;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 664;
										break;
									default:
										break;
									}
								}
								break;
								case 60: // M4A1
								{
									switch (M41S)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 60;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 430;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 77;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 254;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 189;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 301;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 217;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 257;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 321;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 326;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 360;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 383;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 440;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 445;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 497;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 548;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 587;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 631;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 644;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 663;
										break;
									default:
										break;
									}
								}
								break;
								case 9: // AWP
								{
									switch (AWP)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 174;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 344;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 84;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 30;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 51;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 72;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 181;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 259;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 395;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 212;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 227;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 251;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 279;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 424;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 446;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 451;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 475;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 525;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 640;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 662;
										break;
									
									default:
										break;
									}
								}
								break;
								case 61: // USP
								{
									switch (USPS)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 25;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 60;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 183;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 339;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 217;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 221;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 236;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 277;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 290;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 313;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 318;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 332;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 364;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 454;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 489;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 504;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 540;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 637;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 653;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 657;
										break;
									default:
										break;
									}
								}
								break;
								case 4: // Glock
								{
									switch (GLOCK)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 2;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 3;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 38;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 40;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 48;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 437;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 99;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 159;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 399;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 208;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 230;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 278;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 293;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 353;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 367;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 381;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 479;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 495;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 532;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 607;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 586;
										break;
									default:
										break;
									}
								}
								break;
								case 1: // Deagle
								{
									switch (DEAGLE)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 37;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 347;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 468;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 469;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 5;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 232;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 17;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 40;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 61;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 90;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 185;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 231;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 237;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 397;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 328;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 273;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 296;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 351;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 425;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 470;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 509;
										break;
									case 22:
										*pWeapon->FallbackPaintKit() = 527;
										break;
									case 23:
										*pWeapon->FallbackPaintKit() = 645;
										break;
									default:
										break;
									}
								}
								break;
								case 3: // Five Seven
								{
									switch (FIVE7)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 3;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 27;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 44;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 46;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 78;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 141;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 151;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 254;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 248;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 210;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 223;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 252;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 265;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 274;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 464;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 352;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 377;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 387;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 427;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 510;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 530;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 643;
										break;
									case 22:
										*pWeapon->FallbackPaintKit() = 660;
										break;
									default:
										break;
									}
								}
								break;
								case 8: // AUG
								{
									switch (AUG)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 9;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 33;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 280;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 305;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 375;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 442;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 444;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 455;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 507;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 541;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 601;
										break;
									default:
										break;
									}
								}
								break;
								case 10: // Famas
								{
									switch (FAMAS)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 22;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 47;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 92;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 429;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 154;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 178;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 194;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 244;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 218;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 260;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 288;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 371;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 477;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 492;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 529;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 604;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 626;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 659;
										break;
									default:
										break;
									}
								}
								break;
								case 11: // G3SG1
								{
									switch (G3SG1)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 8;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 6;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 27;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 46;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 72;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 74;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 147;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 170;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 195;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 229;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 294;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 465;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 464;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 382;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 438;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 493;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 511;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 545;
										break;
									default:
										break;
									}
								}
								break;
								case 13: // Galil
								{
									switch (Galil)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 5;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 22;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 83;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 428;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 76;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 119;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 398;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 192;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 308;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 216;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 237;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 241;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 264;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 297;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 379;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 460;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 478;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 494;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 546;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 647;
										break;
									case 22:
										*pWeapon->FallbackPaintKit() = 661;
										break;
									default:
										break;
									}
								}
								break;
								case 14: // M249
								{
									switch (M249)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 22;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 75;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 202;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 243;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 266;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 401;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 452;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 472;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 496;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 547;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 648;
										break;
									default:
										break;
									}
								}
								break;
								case 17: // Mac 10
								{
									switch (MAC10)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 101;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 3;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 32;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 5;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 17;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 38;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 433;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 98;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 157;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 188;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 337;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 246;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 284;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 310;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 333;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 343;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 372;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 402;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 498;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 534;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 651;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 665;
										break;
									default:
										break;
									}
								}
								break;
								case 19: // P90
								{
									switch (P90)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 342;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 20;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 22;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 100;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 67;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 111;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 124;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 156;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 234;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 169;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 175;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 182;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 244;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 228;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 283;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 311;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 335;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 359;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 486;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 516;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 636;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 669; //ayy lmao
										break;
									default:
										break;
									}
								}
								break;
								case 24: // UMP-45
								{
									switch (UMP45)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 37;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 5;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 15;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 17;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 436;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 70;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 93;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 169;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 175;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 193;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 392;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 281;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 333;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 362;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 441;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 488;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 556;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 652;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 672;
										break;
									default:
										break;
									}
								}
								break;
								case 25: // XM1014
								{
									switch (XM1014)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 166;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 238;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 27;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 42;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 96;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 95;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 135;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 151;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 169;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 205;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 240;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 251;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 393;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 320;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 314;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 348;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 370;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 407;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 505;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 521;
										break;
									case 22:
										*pWeapon->FallbackPaintKit() = 557;
										break;
									case 23:
										*pWeapon->FallbackPaintKit() = 654;
										break;
									default:
										break;
									}
								}
								break;
								case 26: // Bizon
								{
									switch (BIZON)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 13;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 164;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 25;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 27;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 70;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 148;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 149;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 159;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 171;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 203;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 224;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 236;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 267;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 306;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 323;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 349;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 376;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 457;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 459;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 508;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 526;
										break;
									case 22:
										*pWeapon->FallbackPaintKit() = 542;
										break;
									case 23:
										*pWeapon->FallbackPaintKit() = 641;
										break;
									case 24:
										*pWeapon->FallbackPaintKit() = 676;
										break;
									default:
										break;
									}
								}
								break;
								case 27: // Mag 7
								{
									switch (MAG7)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 462;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 34;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 32;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 100;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 39;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 431;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 99;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 171;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 177;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 198;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 291;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 385;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 473;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 499;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 535;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 666;
										break;

									default:
										break;
									}
								}
								break;
								case 28: // Negev
								{
									switch (NEGEV)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 28;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 432;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 157;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 201;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 240;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 285;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 298;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 317;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 355;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 369;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 483;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 514;
										break;
									default:
										break;
									}
								}
								break;
								case 29: // Sawed Off
								{
									switch (SAWEDOFF)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 345;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 5;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 22;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 30;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 83;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 38;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 41;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 434;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 119;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 171;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 204;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 405;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 246;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 250;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 390;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 256;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 323;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 458;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 459;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 517;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 552;
										break;
									case 22:
										*pWeapon->FallbackPaintKit() = 638;
										break;
									case 23:
										*pWeapon->FallbackPaintKit() = 655;
										break;

									default:
										break;
									}
								}
								break;
								case 30: // Tec 9
								{
									switch (TEC9)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 101;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 2;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 5;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 463;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 17;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 36;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 439;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 159;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 179;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 248;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 206;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 216;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 242;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 272;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 289;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 303;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 374;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 459;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 520;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 539;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 555;
										break;
									case 22:
										*pWeapon->FallbackPaintKit() = 614;
										break;
									case 23:
										*pWeapon->FallbackPaintKit() = 671;
										break;
									default:
										break;
									}
								}
								break;
								case 32: // P2000
								{
									switch (P2000)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 104;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 32;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 21;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 25;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 36;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 485;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 38;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 71;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 95;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 184;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 211;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 338;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 246;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 275;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 327;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 346;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 357;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 389;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 442;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 443;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 515;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 550;
										break;
									case 22:
										*pWeapon->FallbackPaintKit() = 591;
										break;
									case 23:
										*pWeapon->FallbackPaintKit() = 667;
										break;
									default:
										break;
									}
								}
								break;
								case 33: // MP7
								{
									switch (MP7)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 2;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 102;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 5;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 28;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 11;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 15;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 22;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 27;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 36;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 141;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 245;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 209;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 213;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 250;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 354;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 365;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 423;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 442;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 481;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 500;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 536;
										break;
									case 22:
										*pWeapon->FallbackPaintKit() = 649;
										break;
									default:
										break;
									}
								}
								break;
								case 34: // MP9
								{
									switch (MP9)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 482;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 27;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 33;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 100;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 39;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 61;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 148;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 141;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 199;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 329;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 262;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 366;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 368;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 386;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 403;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 448;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 549;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 609;
										break;
									default:
										break;
									}
								}
								break;
								case 35: // Nova
								{
									switch (NOVA)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 3;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 166;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 164;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 25;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 62;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 99;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 107;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 158;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 170;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 191;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 214;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 225;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 263;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 286;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 294;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 299;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 356;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 450;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 484;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 537;
										break;
									default:
										break;
									}
								}
								break;
								case 36: // P250
								{
									switch (P250)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 102;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 34;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 162;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 15;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 164;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 27;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 77;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 99;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 168;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 258;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 207;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 219;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 404;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 230;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 271;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 295;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 464;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 358;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 373;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 388;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 426;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 466;
										break;
									case 22:
										*pWeapon->FallbackPaintKit() = 467;
										break;
									case 23:
										*pWeapon->FallbackPaintKit() = 501;
										break;
									case 24:
										*pWeapon->FallbackPaintKit() = 551;
										break;
									case 25:
										*pWeapon->FallbackPaintKit() = 650;
										break;
									case 26:
										*pWeapon->FallbackPaintKit() = 668;
										break;
									case 27:
										*pWeapon->FallbackPaintKit() = 678;
										break;

									default:
										break;
									}
								}
								break;
								case 38: // Scar 20
								{
									switch (SCAR20)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 165;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 100;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 46;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 70;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 116;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 157;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 196;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 232;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 391;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 298;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 312;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 406;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 453;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 502;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 518;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 597;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 642;
										break;
									default:
										break;
									}
								}
								break;
								case 39: // SG553
								{
									switch (SG553)
									{
									case 1:
										*pWeapon->FallbackPaintKit() = 39;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 98;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 410;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 347;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 287;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 298;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 363;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 378;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 487;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 519;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 553;
										break;
									default:
										break;
									}
								}
								break;
								case 40: // SSG08
								{
									switch (SSG08)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 26;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 60;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 96;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 99;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 157;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 200;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 222;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 233;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 253;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 304;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 319;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 361;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 503;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 538;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 554;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 624;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 670;
										break;

									default:
										break;
									}
								}
								break;
								case 64: // Revolver
								{
									switch (Magnum)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 27;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 12;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 522;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 523;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 595;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									default:
										break;
									}
								}
								break;
								case 63: // CZ-75
								{
									switch (CZ)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 270;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 12;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 476;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 269;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 643;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 435;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 350;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 543;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 268;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 325;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 602;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 334;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 622;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 218;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 297;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 322;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 453;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 315;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;

									default:
										break;
									}
								}
								break;
								default:
									break;
								}




								if (pEntity->GetClientClass()->m_ClassID == (int)CSGOClassID::CKnife)
								{
									auto pCustomName1 = MakePtr(char*, pWeapon, 0x301C);


									if (Model == 0) // Bayonet
									{
										*pWeapon->ModelIndex() = iBayonet; // m_nModelIndex
										*pWeapon->ViewModelIndex() = iBayonet;
										*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 500;
										*pWeapon->m_AttributeManager()->m_Item()->EntityQuality() = 3;

										int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

										if (Skin == 0)
										{
											*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
										}
										else if (Skin == 1)
										{
											*pWeapon->FallbackPaintKit() = 12; // Crimson Web
										}
										else if (Skin == 2)
										{
											*pWeapon->FallbackPaintKit() = 27; // Bone Mask
										}
										else if (Skin == 3)
										{
											*pWeapon->FallbackPaintKit() = 38; // Fade
										}
										else if (Skin == 4)
										{
											*pWeapon->FallbackPaintKit() = 40; // Night
										}
										else if (Skin == 5)
										{
											*pWeapon->FallbackPaintKit() = 42; // Blue Steel
										}
										else if (Skin == 6)
										{
											*pWeapon->FallbackPaintKit() = 43; // Stained
										}
										else if (Skin == 7)
										{
											*pWeapon->FallbackPaintKit() = 44; // Case Hardened
										}
										else if (Skin == 8)
										{
											*pWeapon->FallbackPaintKit() = 59; // Slaughter
										}
										else if (Skin == 9)
										{
											*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
										}
										else if (Skin == 10)
										{
											*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
										}
										else if (Skin == 11)
										{
											*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
										}
										else if (Skin == 12)
										{
											*pWeapon->FallbackPaintKit() = 143; // Urban Masked
										}
										else if (Skin == 13)
										{
											*pWeapon->FallbackPaintKit() = 175; // Scorched
										}
										else if (Skin == 14)
										{
											*pWeapon->FallbackPaintKit() = 323; // Rust Coat
										}
										else if (Skin == 15)
										{
											*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
										}
										else if (Skin == 16)
										{
											*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
										}
										else if (Skin == 17)
										{
											*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
										}
										else if (Skin == 18)
										{
											*pWeapon->FallbackPaintKit() = 413; // Marble Fade
										}
										else if (Skin == 19)
										{
											*pWeapon->FallbackPaintKit() = 414; // Rust Coat
										}
										else if (Skin == 20)
										{
											*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
										}
										else if (Skin == 21)
										{
											*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
										}
										else if (Skin == 22)
										{
											*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
										}
										else if (Skin == 23)
										{
											*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
										}
										else if (Skin == 24)
										{
											*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
										}
										else if (Skin == 25)
										{
											*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
										}
										else if (Skin == 26)
										{
											*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
										}
										else if (Skin == 29)
										{
											*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
										}
										else if (Skin == 30)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
										}
										else if (Skin == 31)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
										}
										else if (Skin == 32)
										{
											*pWeapon->FallbackPaintKit() = 558; // Lore
										}
									}
									else if (Model == 1) // Bowie Knife
									{
										*pWeapon->ModelIndex() = iBowie; // m_nModelIndex
										*pWeapon->ViewModelIndex() = iBowie;
										*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 514;
										*pWeapon->m_AttributeManager()->m_Item()->EntityQuality() = 3;

										int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

										if (Skin == 0)
										{
											*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
										}
										else if (Skin == 1)
										{
											*pWeapon->FallbackPaintKit() = 12; // Crimson Web
										}
										else if (Skin == 2)
										{
											*pWeapon->FallbackPaintKit() = 27; // Bone Mask
										}
										else if (Skin == 3)
										{
											*pWeapon->FallbackPaintKit() = 38; // Fade
										}
										else if (Skin == 4)
										{
											*pWeapon->FallbackPaintKit() = 40; // Night
										}
										else if (Skin == 5)
										{
											*pWeapon->FallbackPaintKit() = 42; // Blue Steel
										}
										else if (Skin == 6)
										{
											*pWeapon->FallbackPaintKit() = 43; // Stained
										}
										else if (Skin == 7)
										{
											*pWeapon->FallbackPaintKit() = 44; // Case Hardened
										}
										else if (Skin == 8)
										{
											*pWeapon->FallbackPaintKit() = 59; // Slaughter
										}
										else if (Skin == 9)
										{
											*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
										}
										else if (Skin == 10)
										{
											*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
										}
										else if (Skin == 11)
										{
											*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
										}
										else if (Skin == 12)
										{
											*pWeapon->FallbackPaintKit() = 143; // Urban Masked
										}
										else if (Skin == 13)
										{
											*pWeapon->FallbackPaintKit() = 175; // Scorched
										}
										else if (Skin == 14)
										{
											*pWeapon->FallbackPaintKit() = 323; // Rust Coat
										}
										else if (Skin == 15)
										{
											*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
										}
										else if (Skin == 16)
										{
											*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
										}
										else if (Skin == 17)
										{
											*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
										}
										else if (Skin == 18)
										{
											*pWeapon->FallbackPaintKit() = 413; // Marble Fade
										}
										else if (Skin == 19)
										{
											*pWeapon->FallbackPaintKit() = 414; // Rust Coat
										}
										else if (Skin == 20)
										{
											*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
										}
										else if (Skin == 21)
										{
											*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
										}
										else if (Skin == 22)
										{
											*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
										}
										else if (Skin == 23)
										{
											*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
										}
										else if (Skin == 24)
										{
											*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
										}
										else if (Skin == 25)
										{
											*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
										}
										else if (Skin == 26)
										{
											*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
										}
										else if (Skin == 29)
										{
											*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
										}
										else if (Skin == 30)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
										}
										else if (Skin == 31)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
										}
										else if (Skin == 32)
										{
											*pWeapon->FallbackPaintKit() = 558; // Lore
										}

									}
									else if (Model == 2) // Butterfly Knife
									{
										*pWeapon->ModelIndex() = iButterfly; // m_nModelIndex
										*pWeapon->ViewModelIndex() = iButterfly;
										*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 515;
										*pWeapon->m_AttributeManager()->m_Item()->EntityQuality() = 3;

										int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

										if (Skin == 0)
										{
											*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
										}
										else if (Skin == 1)
										{
											*pWeapon->FallbackPaintKit() = 12; // Crimson Web
										}
										else if (Skin == 2)
										{
											*pWeapon->FallbackPaintKit() = 27; // Bone Mask
										}
										else if (Skin == 3)
										{
											*pWeapon->FallbackPaintKit() = 38; // Fade
										}
										else if (Skin == 4)
										{
											*pWeapon->FallbackPaintKit() = 40; // Night
										}
										else if (Skin == 5)
										{
											*pWeapon->FallbackPaintKit() = 42; // Blue Steel
										}
										else if (Skin == 6)
										{
											*pWeapon->FallbackPaintKit() = 43; // Stained
										}
										else if (Skin == 7)
										{
											*pWeapon->FallbackPaintKit() = 44; // Case Hardened
										}
										else if (Skin == 8)
										{
											*pWeapon->FallbackPaintKit() = 59; // Slaughter
										}
										else if (Skin == 9)
										{
											*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
										}
										else if (Skin == 10)
										{
											*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
										}
										else if (Skin == 11)
										{
											*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
										}
										else if (Skin == 12)
										{
											*pWeapon->FallbackPaintKit() = 143; // Urban Masked
										}
										else if (Skin == 13)
										{
											*pWeapon->FallbackPaintKit() = 175; // Scorched
										}
										else if (Skin == 14)
										{
											*pWeapon->FallbackPaintKit() = 323; // Rust Coat
										}
										else if (Skin == 15)
										{
											*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
										}
										else if (Skin == 16)
										{
											*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
										}
										else if (Skin == 17)
										{
											*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
										}
										else if (Skin == 18)
										{
											*pWeapon->FallbackPaintKit() = 413; // Marble Fade
										}
										else if (Skin == 19)
										{
											*pWeapon->FallbackPaintKit() = 414; // Rust Coat
										}
										else if (Skin == 20)
										{
											*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
										}
										else if (Skin == 21)
										{
											*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
										}
										else if (Skin == 22)
										{
											*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
										}
										else if (Skin == 23)
										{
											*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
										}
										else if (Skin == 24)
										{
											*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
										}
										else if (Skin == 25)
										{
											*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
										}
										else if (Skin == 26)
										{
											*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
										}
										else if (Skin == 29)
										{
											*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
										}
										else if (Skin == 30)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
										}
										else if (Skin == 31)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
										}
										else if (Skin == 32)
										{
											*pWeapon->FallbackPaintKit() = 558; // Lore
										}

									}
									else if (Model == 3) // Falchion Knife
									{
										*pWeapon->ModelIndex() = iFalchion; // m_nModelIndex
										*pWeapon->ViewModelIndex() = iFalchion;
										*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 512;
										*pWeapon->m_AttributeManager()->m_Item()->EntityQuality() = 3;

										int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

										if (Skin == 0)
										{
											*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
										}
										else if (Skin == 1)
										{
											*pWeapon->FallbackPaintKit() = 12; // Crimson Web
										}
										else if (Skin == 2)
										{
											*pWeapon->FallbackPaintKit() = 27; // Bone Mask
										}
										else if (Skin == 3)
										{
											*pWeapon->FallbackPaintKit() = 38; // Fade
										}
										else if (Skin == 4)
										{
											*pWeapon->FallbackPaintKit() = 40; // Night
										}
										else if (Skin == 5)
										{
											*pWeapon->FallbackPaintKit() = 42; // Blue Steel
										}
										else if (Skin == 6)
										{
											*pWeapon->FallbackPaintKit() = 43; // Stained
										}
										else if (Skin == 7)
										{
											*pWeapon->FallbackPaintKit() = 44; // Case Hardened
										}
										else if (Skin == 8)
										{
											*pWeapon->FallbackPaintKit() = 59; // Slaughter
										}
										else if (Skin == 9)
										{
											*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
										}
										else if (Skin == 10)
										{
											*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
										}
										else if (Skin == 11)
										{
											*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
										}
										else if (Skin == 12)
										{
											*pWeapon->FallbackPaintKit() = 143; // Urban Masked
										}
										else if (Skin == 13)
										{
											*pWeapon->FallbackPaintKit() = 175; // Scorched
										}
										else if (Skin == 14)
										{
											*pWeapon->FallbackPaintKit() = 323; // Rust Coat
										}
										else if (Skin == 15)
										{
											*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
										}
										else if (Skin == 16)
										{
											*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
										}
										else if (Skin == 17)
										{
											*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
										}
										else if (Skin == 18)
										{
											*pWeapon->FallbackPaintKit() = 413; // Marble Fade
										}
										else if (Skin == 19)
										{
											*pWeapon->FallbackPaintKit() = 414; // Rust Coat
										}
										else if (Skin == 20)
										{
											*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
										}
										else if (Skin == 21)
										{
											*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
										}
										else if (Skin == 22)
										{
											*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
										}
										else if (Skin == 23)
										{
											*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
										}
										else if (Skin == 24)
										{
											*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
										}
										else if (Skin == 25)
										{
											*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
										}
										else if (Skin == 26)
										{
											*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
										}
										else if (Skin == 29)
										{
											*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
										}
										else if (Skin == 30)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
										}
										else if (Skin == 31)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
										}
										else if (Skin == 32)
										{
											*pWeapon->FallbackPaintKit() = 558; // Lore
										}
									}
									else if (Model == 4) // Flip Knife
									{
										*pWeapon->ModelIndex() = iFlip; // m_nModelIndex
										*pWeapon->ViewModelIndex() = iFlip;
										*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 505;
										*pWeapon->m_AttributeManager()->m_Item()->EntityQuality() = 3;

										int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

										if (Skin == 0)
										{
											*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
										}
										else if (Skin == 1)
										{
											*pWeapon->FallbackPaintKit() = 12; // Crimson Web
										}
										else if (Skin == 2)
										{
											*pWeapon->FallbackPaintKit() = 27; // Bone Mask
										}
										else if (Skin == 3)
										{
											*pWeapon->FallbackPaintKit() = 38; // Fade
										}
										else if (Skin == 4)
										{
											*pWeapon->FallbackPaintKit() = 40; // Night
										}
										else if (Skin == 5)
										{
											*pWeapon->FallbackPaintKit() = 42; // Blue Steel
										}
										else if (Skin == 6)
										{
											*pWeapon->FallbackPaintKit() = 43; // Stained
										}
										else if (Skin == 7)
										{
											*pWeapon->FallbackPaintKit() = 44; // Case Hardened
										}
										else if (Skin == 8)
										{
											*pWeapon->FallbackPaintKit() = 59; // Slaughter
										}
										else if (Skin == 9)
										{
											*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
										}
										else if (Skin == 10)
										{
											*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
										}
										else if (Skin == 11)
										{
											*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
										}
										else if (Skin == 12)
										{
											*pWeapon->FallbackPaintKit() = 143; // Urban Masked
										}
										else if (Skin == 13)
										{
											*pWeapon->FallbackPaintKit() = 175; // Scorched
										}
										else if (Skin == 14)
										{
											*pWeapon->FallbackPaintKit() = 323; // Rust Coat
										}
										else if (Skin == 15)
										{
											*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
										}
										else if (Skin == 16)
										{
											*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
										}
										else if (Skin == 17)
										{
											*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
										}
										else if (Skin == 18)
										{
											*pWeapon->FallbackPaintKit() = 413; // Marble Fade
										}
										else if (Skin == 19)
										{
											*pWeapon->FallbackPaintKit() = 414; // Rust Coat
										}
										else if (Skin == 20)
										{
											*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
										}
										else if (Skin == 21)
										{
											*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
										}
										else if (Skin == 22)
										{
											*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
										}
										else if (Skin == 23)
										{
											*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
										}
										else if (Skin == 24)
										{
											*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
										}
										else if (Skin == 25)
										{
											*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
										}
										else if (Skin == 26)
										{
											*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
										}
										else if (Skin == 29)
										{
											*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
										}
										else if (Skin == 30)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
										}
										else if (Skin == 31)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
										}
										else if (Skin == 32)
										{
											*pWeapon->FallbackPaintKit() = 559; // Lore
										}

									}
									else if (Model == 5) // Gut Knife
									{
										*pWeapon->ModelIndex() = iGut; // m_nModelIndex
										*pWeapon->ViewModelIndex() = iGut;
										*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 506;
										*pWeapon->m_AttributeManager()->m_Item()->EntityQuality() = 3;

										int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

										if (Skin == 0)
										{
											*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
										}
										else if (Skin == 1)
										{
											*pWeapon->FallbackPaintKit() = 12; // Crimson Web
										}
										else if (Skin == 2)
										{
											*pWeapon->FallbackPaintKit() = 27; // Bone Mask
										}
										else if (Skin == 3)
										{
											*pWeapon->FallbackPaintKit() = 38; // Fade
										}
										else if (Skin == 4)
										{
											*pWeapon->FallbackPaintKit() = 40; // Night
										}
										else if (Skin == 5)
										{
											*pWeapon->FallbackPaintKit() = 42; // Blue Steel
										}
										else if (Skin == 6)
										{
											*pWeapon->FallbackPaintKit() = 43; // Stained
										}
										else if (Skin == 7)
										{
											*pWeapon->FallbackPaintKit() = 44; // Case Hardened
										}
										else if (Skin == 8)
										{
											*pWeapon->FallbackPaintKit() = 59; // Slaughter
										}
										else if (Skin == 9)
										{
											*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
										}
										else if (Skin == 10)
										{
											*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
										}
										else if (Skin == 11)
										{
											*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
										}
										else if (Skin == 12)
										{
											*pWeapon->FallbackPaintKit() = 143; // Urban Masked
										}
										else if (Skin == 13)
										{
											*pWeapon->FallbackPaintKit() = 175; // Scorched
										}
										else if (Skin == 14)
										{
											*pWeapon->FallbackPaintKit() = 323; // Rust Coat
										}
										else if (Skin == 15)
										{
											*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
										}
										else if (Skin == 16)
										{
											*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
										}
										else if (Skin == 17)
										{
											*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
										}
										else if (Skin == 18)
										{
											*pWeapon->FallbackPaintKit() = 413; // Marble Fade
										}
										else if (Skin == 19)
										{
											*pWeapon->FallbackPaintKit() = 414; // Rust Coat
										}
										else if (Skin == 20)
										{
											*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
										}
										else if (Skin == 21)
										{
											*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
										}
										else if (Skin == 22)
										{
											*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
										}
										else if (Skin == 23)
										{
											*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
										}
										else if (Skin == 24)
										{
											*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
										}
										else if (Skin == 25)
										{
											*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
										}
										else if (Skin == 26)
										{
											*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
										}
										else if (Skin == 29)
										{
											*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
										}
										else if (Skin == 30)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
										}
										else if (Skin == 31)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
										}
										else if (Skin == 32)
										{
											*pWeapon->FallbackPaintKit() = 560; // Lore
										}


									}
									else if (Model == 6) // Huntsman Knife
									{
										*pWeapon->ModelIndex() = iHuntsman; // m_nModelIndex
										*pWeapon->ViewModelIndex() = iHuntsman;
										*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 509;
										*pWeapon->m_AttributeManager()->m_Item()->EntityQuality() = 3;

										int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

										if (Skin == 0)
										{
											*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
										}
										else if (Skin == 1)
										{
											*pWeapon->FallbackPaintKit() = 12; // Crimson Web
										}
										else if (Skin == 2)
										{
											*pWeapon->FallbackPaintKit() = 27; // Bone Mask
										}
										else if (Skin == 3)
										{
											*pWeapon->FallbackPaintKit() = 38; // Fade
										}
										else if (Skin == 4)
										{
											*pWeapon->FallbackPaintKit() = 40; // Night
										}
										else if (Skin == 5)
										{
											*pWeapon->FallbackPaintKit() = 42; // Blue Steel
										}
										else if (Skin == 6)
										{
											*pWeapon->FallbackPaintKit() = 43; // Stained
										}
										else if (Skin == 7)
										{
											*pWeapon->FallbackPaintKit() = 44; // Case Hardened
										}
										else if (Skin == 8)
										{
											*pWeapon->FallbackPaintKit() = 59; // Slaughter
										}
										else if (Skin == 9)
										{
											*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
										}
										else if (Skin == 10)
										{
											*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
										}
										else if (Skin == 11)
										{
											*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
										}
										else if (Skin == 12)
										{
											*pWeapon->FallbackPaintKit() = 143; // Urban Masked
										}
										else if (Skin == 13)
										{
											*pWeapon->FallbackPaintKit() = 175; // Scorched
										}
										else if (Skin == 14)
										{
											*pWeapon->FallbackPaintKit() = 323; // Rust Coat
										}
										else if (Skin == 15)
										{
											*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
										}
										else if (Skin == 16)
										{
											*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
										}
										else if (Skin == 17)
										{
											*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
										}
										else if (Skin == 18)
										{
											*pWeapon->FallbackPaintKit() = 413; // Marble Fade
										}
										else if (Skin == 19)
										{
											*pWeapon->FallbackPaintKit() = 414; // Rust Coat
										}
										else if (Skin == 20)
										{
											*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
										}
										else if (Skin == 21)
										{
											*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
										}
										else if (Skin == 22)
										{
											*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
										}
										else if (Skin == 23)
										{
											*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
										}
										else if (Skin == 24)
										{
											*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
										}
										else if (Skin == 25)
										{
											*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
										}
										else if (Skin == 26)
										{
											*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
										}
										else if (Skin == 29)
										{
											*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
										}
										else if (Skin == 30)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
										}
										else if (Skin == 31)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
										}
										else if (Skin == 32)
										{
											*pWeapon->FallbackPaintKit() = 559; // Lore
										}


									}
									else if (Model == 7) // Karambit
									{
										*pWeapon->ModelIndex() = iKarambit; // m_nModelIndex
										*pWeapon->ViewModelIndex() = iKarambit;
										*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 507;
										*pWeapon->m_AttributeManager()->m_Item()->EntityQuality() = 3;

										int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

										if (Skin == 0)
										{
											*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
										}
										else if (Skin == 1)
										{
											*pWeapon->FallbackPaintKit() = 12; // Crimson Web
										}
										else if (Skin == 2)
										{
											*pWeapon->FallbackPaintKit() = 27; // Bone Mask
										}
										else if (Skin == 3)
										{
											*pWeapon->FallbackPaintKit() = 38; // Fade
										}
										else if (Skin == 4)
										{
											*pWeapon->FallbackPaintKit() = 40; // Night
										}
										else if (Skin == 5)
										{
											*pWeapon->FallbackPaintKit() = 42; // Blue Steel
										}
										else if (Skin == 6)
										{
											*pWeapon->FallbackPaintKit() = 43; // Stained
										}
										else if (Skin == 7)
										{
											*pWeapon->FallbackPaintKit() = 44; // Case Hardened
										}
										else if (Skin == 8)
										{
											*pWeapon->FallbackPaintKit() = 59; // Slaughter
										}
										else if (Skin == 9)
										{
											*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
										}
										else if (Skin == 10)
										{
											*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
										}
										else if (Skin == 11)
										{
											*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
										}
										else if (Skin == 12)
										{
											*pWeapon->FallbackPaintKit() = 143; // Urban Masked
										}
										else if (Skin == 13)
										{
											*pWeapon->FallbackPaintKit() = 175; // Scorched
										}
										else if (Skin == 14)
										{
											*pWeapon->FallbackPaintKit() = 323; // Rust Coat
										}
										else if (Skin == 15)
										{
											*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
										}
										else if (Skin == 16)
										{
											*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
										}
										else if (Skin == 17)
										{
											*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
										}
										else if (Skin == 18)
										{
											*pWeapon->FallbackPaintKit() = 413; // Marble Fade
										}
										else if (Skin == 19)
										{
											*pWeapon->FallbackPaintKit() = 414; // Rust Coat
										}
										else if (Skin == 20)
										{
											*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
										}
										else if (Skin == 21)
										{
											*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
										}
										else if (Skin == 22)
										{
											*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
										}
										else if (Skin == 23)
										{
											*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
										}
										else if (Skin == 24)
										{
											*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
										}
										else if (Skin == 25)
										{
											*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
										}
										else if (Skin == 26)
										{
											*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 570; // Doppler Phase 4
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 568; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
										}
										else if (Skin == 29)
										{
											*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
										}
										else if (Skin == 30)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
										}
										else if (Skin == 31)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
										}
										else if (Skin == 32)
										{
											*pWeapon->FallbackPaintKit() = 561; // Lore
										}

									}
									else if (Model == 8) // M9 Bayonet
									{
										*pWeapon->ModelIndex() = iM9Bayonet; // m_nModelIndex
										*pWeapon->ViewModelIndex() = iM9Bayonet;
										*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 508;
										*pWeapon->m_AttributeManager()->m_Item()->EntityQuality() = 3;

										int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

										if (Skin == 0)
										{
											*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
										}
										else if (Skin == 1)
										{
											*pWeapon->FallbackPaintKit() = 12; // Crimson Web
										}
										else if (Skin == 2)
										{
											*pWeapon->FallbackPaintKit() = 27; // Bone Mask
										}
										else if (Skin == 3)
										{
											*pWeapon->FallbackPaintKit() = 38; // Fade
										}
										else if (Skin == 4)
										{
											*pWeapon->FallbackPaintKit() = 40; // Night
										}
										else if (Skin == 5)
										{
											*pWeapon->FallbackPaintKit() = 42; // Blue Steel
										}
										else if (Skin == 6)
										{
											*pWeapon->FallbackPaintKit() = 43; // Stained
										}
										else if (Skin == 7)
										{
											*pWeapon->FallbackPaintKit() = 44; // Case Hardened
										}
										else if (Skin == 8)
										{
											*pWeapon->FallbackPaintKit() = 59; // Slaughter
										}
										else if (Skin == 9)
										{
											*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
										}
										else if (Skin == 10)
										{
											*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
										}
										else if (Skin == 11)
										{
											*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
										}
										else if (Skin == 12)
										{
											*pWeapon->FallbackPaintKit() = 143; // Urban Masked
										}
										else if (Skin == 13)
										{
											*pWeapon->FallbackPaintKit() = 175; // Scorched
										}
										else if (Skin == 14)
										{
											*pWeapon->FallbackPaintKit() = 323; // Rust Coat
										}
										else if (Skin == 15)
										{
											*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
										}
										else if (Skin == 16)
										{
											*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
										}
										else if (Skin == 17)
										{
											*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
										}
										else if (Skin == 18)
										{
											*pWeapon->FallbackPaintKit() = 413; // Marble Fade
										}
										else if (Skin == 19)
										{
											*pWeapon->FallbackPaintKit() = 414; // Rust Coat
										}
										else if (Skin == 20)
										{
											*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
										}
										else if (Skin == 21)
										{
											*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
										}
										else if (Skin == 22)
										{
											*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
										}
										else if (Skin == 23)
										{
											*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
										}
										else if (Skin == 24)
										{
											*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
										}
										else if (Skin == 25)
										{
											*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
										}
										else if (Skin == 26)
										{
											*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 570; // Doppler Phase 4
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 568; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
										}
										else if (Skin == 29)
										{
											*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
										}
										else if (Skin == 30)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
										}
										else if (Skin == 31)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
										}
										else if (Skin == 32)
										{
											*pWeapon->FallbackPaintKit() = 562; // Lore
										}

									}


									else if (Model == 9) // Shadow Daggers
									{
										*pWeapon->ModelIndex() = iDagger; // m_nModelIndex
										*pWeapon->ViewModelIndex() = iDagger;
										*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 516;
										*pWeapon->m_AttributeManager()->m_Item()->EntityQuality() = 3;

										int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

										if (Skin == 0)
										{
											*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
										}
										else if (Skin == 1)
										{
											*pWeapon->FallbackPaintKit() = 12; // Crimson Web
										}
										else if (Skin == 2)
										{
											*pWeapon->FallbackPaintKit() = 27; // Bone Mask
										}
										else if (Skin == 3)
										{
											*pWeapon->FallbackPaintKit() = 38; // Fade
										}
										else if (Skin == 4)
										{
											*pWeapon->FallbackPaintKit() = 40; // Night
										}
										else if (Skin == 5)
										{
											*pWeapon->FallbackPaintKit() = 42; // Blue Steel
										}
										else if (Skin == 6)
										{
											*pWeapon->FallbackPaintKit() = 43; // Stained
										}
										else if (Skin == 7)
										{
											*pWeapon->FallbackPaintKit() = 44; // Case Hardened
										}
										else if (Skin == 8)
										{
											*pWeapon->FallbackPaintKit() = 59; // Slaughter
										}
										else if (Skin == 9)
										{
											*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
										}
										else if (Skin == 10)
										{
											*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
										}
										else if (Skin == 11)
										{
											*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
										}
										else if (Skin == 12)
										{
											*pWeapon->FallbackPaintKit() = 143; // Urban Masked
										}
										else if (Skin == 13)
										{
											*pWeapon->FallbackPaintKit() = 175; // Scorched
										}
										else if (Skin == 14)
										{
											*pWeapon->FallbackPaintKit() = 323; // Rust Coat
										}
										else if (Skin == 15)
										{
											*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
										}
										else if (Skin == 16)
										{
											*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
										}
										else if (Skin == 17)
										{
											*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
										}
										else if (Skin == 18)
										{
											*pWeapon->FallbackPaintKit() = 413; // Marble Fade
										}
										else if (Skin == 19)
										{
											*pWeapon->FallbackPaintKit() = 414; // Rust Coat
										}
										else if (Skin == 20)
										{
											*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
										}
										else if (Skin == 21)
										{
											*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
										}
										else if (Skin == 22)
										{
											*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
										}
										else if (Skin == 23)
										{
											*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
										}
										else if (Skin == 24)
										{
											*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
										}
										else if (Skin == 25)
										{
											*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
										}
										else if (Skin == 26)
										{
											*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
										}
										else if (Skin == 29)
										{
											*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
										}
										else if (Skin == 30)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
										}
										else if (Skin == 31)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
										}
										else if (Skin == 32)
										{
											*pWeapon->FallbackPaintKit() = 561; // Lore
										}
									}
								}
							}

							*pWeapon->OwnerXuidLow() = 0;
							*pWeapon->OwnerXuidHigh() = 0;
							*pWeapon->FallbackWear() = 0.001f;
							*pWeapon->m_AttributeManager()->m_Item()->ItemIDHigh() = 1;

						}
					}
				}

			}
		}
		if (pLocal->IsAlive())
			R::Resolve();
	}

	oFrameStageNotify(curStage);
}

void __fastcall Hooked_OverrideView(void* ecx, void* edx, CViewSetup* pSetup)
{
	IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		if (Menu::Window.VisualsTab.Active.GetState() && pLocal->IsAlive() && !pLocal->IsScoped())
		{
			if (pSetup->fov = 90)
				pSetup->fov = Menu::Window.VisualsTab.OtherFOV.GetValue();
		}

		grenade_prediction::instance().View(pSetup);
		oOverrideView(ecx, edx, pSetup);
	}

}

void GetViewModelFOV(float& fov)
{
	IClientEntity* localplayer = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{

		if (!localplayer)
			return;


		if (Menu::Window.VisualsTab.Active.GetState())
			fov += Menu::Window.VisualsTab.OtherViewmodelFOV.GetValue();
	}
}

float __stdcall GGetViewModelFOV()
{
	float fov = Hooks::VMTClientMode.GetMethod<oGetViewModelFOV>(35)();

	GetViewModelFOV(fov);

	return fov;
}

void __fastcall Hooked_RenderView(void* ecx, void* edx, CViewSetup &setup, CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw)
{
	static DWORD oRenderView = Hooks::VMTRenderView.GetOriginalFunction(6);

	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	__asm
	{
		PUSH whatToDraw
		PUSH nClearFlags
		PUSH hudViewSetup
		PUSH setup
		MOV ECX, ecx
		CALL oRenderView
	}
} //hooked for no reason yay

BYTE bMoveData[0x200];

void Prediction(CUserCmd* pCmd, IClientEntity* LocalPlayer)
{
	if (Interfaces::MoveHelper && Menu::Window.RageBotTab.AimbotEnable.GetState() && Menu::Window.RageBotTab.AccuracyPrediction.GetIndex() == 1 && LocalPlayer->IsAlive())
	{
		float curtime = Interfaces::Globals->curtime;
		float frametime = Interfaces::Globals->frametime;
		int iFlags = LocalPlayer->GetFlags();

		Interfaces::Globals->curtime = (float)LocalPlayer->GetTickBase() * Interfaces::Globals->interval_per_tick * 1.05;
		Interfaces::Globals->frametime = Interfaces::Globals->interval_per_tick;

		Interfaces::MoveHelper->SetHost(LocalPlayer);

		Interfaces::GamePrediction->SetupMove(LocalPlayer, pCmd, nullptr, bMoveData);
		Interfaces::GameMovement->ProcessMovement(LocalPlayer, bMoveData);
		Interfaces::GamePrediction->FinishMove(LocalPlayer, pCmd, bMoveData);

		Interfaces::MoveHelper->SetHost(0);

		Interfaces::Globals->curtime = curtime;
		Interfaces::Globals->frametime = frametime;
		*LocalPlayer->GetPointerFlags() = iFlags;
	}
	if (Interfaces::MoveHelper && Menu::Window.RageBotTab.AimbotEnable.GetState() && Menu::Window.RageBotTab.AccuracyPrediction.GetIndex() == 2 && LocalPlayer->IsAlive())
	{
		float curtime = Interfaces::Globals->curtime;
		float frametime = Interfaces::Globals->frametime;
		int iFlags = LocalPlayer->GetFlags();

		Interfaces::Globals->curtime = (float)LocalPlayer->GetTickBase() * Interfaces::Globals->interval_per_tick;
		Interfaces::Globals->frametime = Interfaces::Globals->interval_per_tick;

		Interfaces::MoveHelper->SetHost(LocalPlayer);

		Interfaces::GamePrediction->SetupMove(LocalPlayer, pCmd, nullptr, bMoveData);
		Interfaces::GameMovement->ProcessMovement(LocalPlayer, bMoveData);
		Interfaces::GamePrediction->FinishMove(LocalPlayer, pCmd, bMoveData);

		Interfaces::MoveHelper->SetHost(0);

		Interfaces::Globals->curtime = curtime;
		Interfaces::Globals->frametime = frametime;
		*LocalPlayer->GetPointerFlags() = iFlags;
	}
	if (Interfaces::MoveHelper && Menu::Window.RageBotTab.AimbotEnable.GetState() && Menu::Window.RageBotTab.AccuracyPrediction.GetIndex() == 3 && LocalPlayer->IsAlive())
	{
		int EN1 = 0;
		int EN2 = 0;
		static clock_t start_t = clock();
		double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
		if (timeSoFar < 0.5)
			return;

		float curtime = Interfaces::Globals->curtime;
		float frametime = Interfaces::Globals->frametime;
		int iFlags = LocalPlayer->GetFlags();

		if (EN1 == 0 && EN2 == 0) {
			Interfaces::Globals->curtime = (float)LocalPlayer->GetTickBase() * Interfaces::Globals->interval_per_tick;
			Interfaces::Globals->frametime = Interfaces::Globals->interval_per_tick;
			EN1 = 1;
		}
		if (EN1 == 1 && EN2 == 1) {
			Interfaces::Globals->curtime = (float)LocalPlayer->GetTickBase() * Interfaces::Globals->interval_per_tick * 1.05;
			Interfaces::Globals->frametime = Interfaces::Globals->interval_per_tick;
			EN1 = 0;
			EN2 = -1;
		}

		Interfaces::MoveHelper->SetHost(LocalPlayer);

		Interfaces::GamePrediction->SetupMove(LocalPlayer, pCmd, nullptr, bMoveData);
		Interfaces::GameMovement->ProcessMovement(LocalPlayer, bMoveData);
		Interfaces::GamePrediction->FinishMove(LocalPlayer, pCmd, bMoveData);

		Interfaces::MoveHelper->SetHost(0);

		Interfaces::Globals->curtime = curtime;
		Interfaces::Globals->frametime = frametime;
		*LocalPlayer->GetPointerFlags() = iFlags;

		EN2++;
		start_t = clock();
	}
	if (Interfaces::MoveHelper && Menu::Window.RageBotTab.AimbotEnable.GetState() && Menu::Window.RageBotTab.AccuracyPrediction.GetIndex() == 4 && LocalPlayer->IsAlive())
	{
		int EN1 = 0;
		int EN2 = 0;
		static clock_t start_t = clock();
		double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
		if (timeSoFar < 0.5)
			return;

		float curtime = Interfaces::Globals->curtime;
		float frametime = Interfaces::Globals->frametime;
		int iFlags = LocalPlayer->GetFlags();

		if (EN1 == 0 && EN2 == 0) {
			Interfaces::Globals->curtime = (float)LocalPlayer->GetTickBase() * Interfaces::Globals->interval_per_tick / 2.5;
			Interfaces::Globals->frametime = Interfaces::Globals->interval_per_tick;
			EN1 = 1;
		}
		if (EN1 == 1 && EN2 == 1) {
			Interfaces::Globals->curtime = (float)LocalPlayer->GetTickBase() * Interfaces::Globals->interval_per_tick * 2.5;
			Interfaces::Globals->frametime = Interfaces::Globals->interval_per_tick;
			EN1 = 2;
			EN2 = 1;
		}
		if (EN1 == 2 && EN2 == 2) {
			Interfaces::Globals->curtime = (float)LocalPlayer->GetTickBase() * Interfaces::Globals->interval_per_tick / 5;
			Interfaces::Globals->frametime = Interfaces::Globals->interval_per_tick;
			EN1 = 3;
			EN2 = 2;
		}
		if (EN1 == 3 && EN2 == 3) {
			Interfaces::Globals->curtime = (float)LocalPlayer->GetTickBase() * Interfaces::Globals->interval_per_tick * 5;
			Interfaces::Globals->frametime = Interfaces::Globals->interval_per_tick;
			EN1 = 0;
			EN2 = -1;
		}

		Interfaces::MoveHelper->SetHost(LocalPlayer);

		Interfaces::GamePrediction->SetupMove(LocalPlayer, pCmd, nullptr, bMoveData);
		Interfaces::GameMovement->ProcessMovement(LocalPlayer, bMoveData);
		Interfaces::GamePrediction->FinishMove(LocalPlayer, pCmd, bMoveData);

		Interfaces::MoveHelper->SetHost(0);

		Interfaces::Globals->curtime = curtime;
		Interfaces::Globals->frametime = frametime;
		*LocalPlayer->GetPointerFlags() = iFlags;

		EN2++;
		start_t = clock();
	}
}

