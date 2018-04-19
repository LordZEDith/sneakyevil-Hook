/*
Rest In Peace ApocalypseCheats
*/

#pragma once

#include "GUI.h"
#include "Controls.h"

class CRageBotTab : public CTab
{
public:
	void Setup();

	// Master Switch
	CLabel ActiveLabel;
	CCheckBox Active;

	// Aimbot Settings
	CGroupBox AimbotGroup;
	CCheckBox AimbotEnable;
	CCheckBox AimbotAutoFire;
	CSlider	  AimbotFov;
	CCheckBox AimbotSilentAim;
	CCheckBox AimbotPerfectSilentAim;
	CCheckBox AimbotAutoPistol;
	CCheckBox AWPAtBody;
	CCheckBox HitChanceAuto;
	CCheckBox AutoRevolver;
	CCheckBox AimbotAimStep;
	CSlider   BaimIfUnderXHealth;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CKeyBind  AimbotStopKey;

	// Target Selection Settings
	CGroupBox TargetGroup;
	CComboBox TargetSelection;
	CCheckBox TargetFriendlyFire;
	CComboBox TargetHitbox;
	CComboBox TargetHitscan;
	CCheckBox EnginePrediction;
	CCheckBox TargetMultipoint;
	CSlider   TargetPointscale;
	CSlider AimbotAutoPistolD;
	// Accuracy Settings
	CGroupBox AccuracyGroup;
	CCheckBox AccuracyRecoil;
	CCheckBox AccuracyAutoWall;
	CSlider	  AccuracyMinimumDamage;
	CCheckBox AccuracyAutoStop;
	CCheckBox AccuracyAutoCrouch;
	CCheckBox AccuracyAutoScope;
	CSlider   AccuracyHitchance;
	CCheckBox AccuracyPVS;
	CComboBox AccuracyPrediction;
	CCheckBox AdvancedResolver;
	CComboBox AccuracyResolver;
	CCheckBox AccuracyPositionAdjustment;
	CSlider   AccuracySmart;

	// Anti-Aim Settings
	CGroupBox AntiAimGroup;
	CCheckBox AntiAimEnable;
	CComboBox AntiAimPitch;
	CComboBox AntiAimYaw;
	CComboBox AntiAimYawMoving;
	CComboBox AntiAimYawJumping;
	CComboBox AntiAimZeta;
	CSlider	  CustomPitch;
	CKeyBind  AntiAimYawSwitch;
	CCheckBox AntiAimEdge;
	CCheckBox AntiAimDrawLines;
	CSlider	  AntiAimOffset;
	CCheckBox AntiAimKnife;
	CCheckBox AntiResolver;
	CSlider   AntiAimSpeed;
	CCheckBox AntiAimTarget;

};

class CLegitBotTab : public CTab
{
public:
	void Setup();

	// Master Switch
	CLabel ActiveLabel;
	CCheckBox Active;

	// Aimbot Settings
	CGroupBox AimbotGroup;
	CCheckBox AimbotEnable;
	CCheckBox AimbotAutoFire;
	CCheckBox AimbotFriendlyFire;
	CCheckBox AimbotSmokeCheck;
	//CCheckBox AimbotKeyPress;
	//CKeyBind  AimbotKeyBind;
	CCheckBox AimbotAutoPistol;
	CCheckBox LegitBackTracking;
	CSlider	  LegitBackTrackingTick;
	CSlider   AimbotInaccuracy;
	CKeyBind  AimbotDisableVis;

	// Main
	CGroupBox TriggerGroup;
	CCheckBox TriggerEnable;
	CCheckBox TriggerKeyPress;
	CKeyBind  TriggerKeyBind;
	CSlider  TriggerDelay;

	// Main
	CGroupBox WeaponMainGroup;
	CSlider   WeaponMainSpeed;
	CSlider   WeaponMainFoV;
	CSlider   WeaponMainRecoil;
	CCheckBox WeaponMainPSilent;
	CSlider   WeaponMainInacc;
	CComboBox WeaponMainHitbox;
	CComboBox WeaponMainSecHitbox;
	CKeyBind  WeaponMainKey;

	// Pistol
	CGroupBox WeaponPistGroup;
	CSlider   WeaponPistSpeed;
	CSlider   WeaponPistFoV;
	CSlider WeaponPistRecoil;
	CCheckBox WeaponPistPSilent;
	CSlider   WeaponPistInacc;
	CComboBox WeaponPistHitbox;
	CComboBox WeaponPistSecHitbox;
	CKeyBind  WeaponPistKey;

	// Sniper
	CCheckBox WeaponSnipEnable;
	CGroupBox WeaponSnipGroup;
	CSlider   WeaponSnipSpeed;
	CSlider   WeaponSnipFoV;
	CSlider WeaponSnipRecoil;
	CCheckBox WeaponSnipPSilent;
	CSlider   WeaponSnipInacc;
	CComboBox WeaponSnipHitbox;
	CComboBox WeaponSnipSecHitbox;
	CComboBox WeaponSnipScoutHitbox;
	CKeyBind  WeaponSnipKey;

	CGroupBox TriggerFilterGroup;

	CCheckBox TriggerHead;
	CCheckBox TriggerChest;
	CCheckBox TriggerStomach;
	CCheckBox TriggerArms;
	CCheckBox TriggerLegs;
	CCheckBox TriggerTeammates;
	CCheckBox TriggerRecoil;
	//More
	CGroupBox AimMoreGroup;
	CCheckBox FoVRadius;
	CCheckBox WeaponConfig;
	CComboBox WeaponConf;
};

class CVisualTab : public CTab
{
public:
	void Setup();

	// Master Switch
	CLabel ActiveLabel;
	CCheckBox Active;

	// Options Settings
	CGroupBox OptionsGroup;
	CComboBox OptionsBox;
	CCheckBox OptionsBoxFill;
	CCheckBox OptionsBoxFillColor;
	CCheckBox OptionsName;
	CComboBox OptionsHealth;
	CComboBox OptionsArmor;
	CCheckBox OptionsDistance;
	CComboBox OptionsWeapon;
	CCheckBox OptionsInfo;
	CCheckBox OptionsHelmet;
	CCheckBox OptionsKit;
	CCheckBox OptionsDefuse;
	CCheckBox Barrels;
	CCheckBox OtherWireframe;
	CCheckBox NadePred;
	CCheckBox OptionsGlow;
	CComboBox OptionsChams;
	CComboBox OptionsChamsModel;
	CCheckBox OptionsChamsXQZ;
	CComboBox OptionsFriendly;
	CComboBox OptionsFriendly2;
	CCheckBox OptionsNadePrediction;
	CCheckBox OptionsSkeleton;
	CCheckBox OptionsVisibleOnly;
	CCheckBox Grenades;
	CCheckBox OptionsAimSpot;
	CCheckBox OptionsCompRank;
	CCheckBox OptionsCompWins;
	CComboBox OptionsDefusing;
	CCheckBox OptionsMoney;
	CCheckBox OptionsLBY;
	CCheckBox OtherAsus;

	// Filters Settings
	CGroupBox FiltersGroup;
	CCheckBox FiltersAll;
	CCheckBox FiltersPlayers;
	CCheckBox FiltersEnemiesOnly;
	CCheckBox FiltersWeapons;
	CComboBox FiltersNades;
	CCheckBox FiltersChickens;
	CCheckBox FiltersGrenadePred;
	CCheckBox FiltersC4;
	CCheckBox FiltersDead;

	// Other Settings
	CGroupBox OtherGroup;
	CCheckBox OtherCrosshair;
	CCheckBox OtherSpread;
	CComboBox OtherRecoilCrosshair;
	CCheckBox OtherRadar;
	CCheckBox OtherNoVisualRecoil;
	CCheckBox OtherNoSky; 
	CCheckBox OtherNoFlash; 
	CSlider   OtherNoFlashVal;
	CCheckBox OtherGlow;
	CCheckBox OptionsScoped;
	CCheckBox NightMode;
	CCheckBox OtherNoSmoke;
	CCheckBox OtherFovDraw;
	CCheckBox OtherLBYDraw;
	CCheckBox OtherNightMode;
	CCheckBox OtherNoPost;
	CCheckBox OtherAsusWalls;
	CCheckBox OtherGrenadePredict;
	CCheckBox OtherBackTracking;
	CComboBox OtherNoHands;
	CComboBox OtherWeapons;
	CSlider OtherViewmodelFOV;
	CSlider OtherFOV;
	CCheckBox OtherNoScope;


};

class CMiscTab : public CTab
{
public:
	void Setup();

	// Knife Changer
	/*CGroupBox KnifeGroup;
	CCheckBox KnifeEnable;
	CComboBox KnifeModel;
	CComboBox KnifeSkin;
	CButton   KnifeApply;*/

	// Other Settings
	CGroupBox OtherGroup;
	CCheckBox OtherAutoJump;
	CCheckBox OtherEdgeJump;
	CComboBox OtherAutoStrafe;
	CCheckBox OtherSafeMode;
	CComboBox OtherNameSpam;
	CComboBox OtherNameSteal;
	CComboBox OtherChatSpam;
	CComboBox OtherKillSpam;
	CComboBox OtherLyricsSpam;
	CComboBox OtherRadioSpam;
	CCheckBox OtherTeamChat;
	CSlider	  OtherChatDelay;
	CKeyBind  OtherAirStuck;
	CKeyBind  OtherSlowMotion;
	CKeyBind  OtherFakeWalk;
	CKeyBind  OtherJumpThrow;
	CKeyBind  OtherLagSwitch;
	CCheckBox OtherSpectators;
	CComboBox OtherThirdperson;
	CCheckBox OtherSkinChanger;
	CCheckBox OtherWatermark;
	CCheckBox OtherClock;
	CCheckBox OtherCircleStrafe;
	CKeyBind OtherCircleStrafeKey;
	CCheckBox OtherHitmarker;
	CCheckBox OtherAutoCounterStafe;
	CCheckBox OtherAutoAccept;
	CKeyBind  OtherAutoCounterStafeKey;
	CCheckBox OtherWalkbot;
	CComboBox OtherClantag;
	CCheckBox DisableAll;
	CGroupBox MPGroup;
	CComboBox MAC10Skin;
	CComboBox P90Skin;
	CComboBox UMP45Skin;
	CComboBox BIZONSkin;
	CComboBox MP7Skin;
	CComboBox MP9Skin;
	
	CButton NApply;
	// Fake Lag Settings
	CGroupBox FakeLagGroup;
	CCheckBox FakeLagEnable;
	CSlider  FakeLagChoke;
	CSlider  FakeLagSend;

	CCheckBox OtherCheats;
	//CCheckBox FakeLagWhileShooting;
};

class CColorTab : public CTab
{
public:
	void Setup();

	//Groups
	CSlider CharmsA;
	CSlider  CrossVisColorR;
	CSlider  CrossVisColorG;
	CSlider  CrossVisColorB;

	CGroupBox ColorsGroup;
	CGroupBox CTNVisGroup;
	CGroupBox CTVisGroup;
	CGroupBox TNVisGroup;
	CGroupBox TVisGroup;
	//Menu Groups
	CGroupBox MenuGroup;
	CGroupBox MenuBar2;
	CGroupBox MenuInside;
	CGroupBox MenuBackround;
	//CT Not Visible
	CCheckBox CTNVisColorHP;
	CSlider  CTNVisColorR;
	CSlider  CTNVisColorG;
	CSlider  CTNVisColorB;
	//CT Visible
	CSlider  CTVisColorR;
	CSlider  CTVisColorG;
	CSlider  CTVisColorB;
	CCheckBox CTVisColorHP;
	//T Not Visible
	CSlider  TNVisColorR;
	CSlider  TNVisColorG;
	CSlider  TNVisColorB;
	//T Visible
	CSlider  TVisColorR;
	CSlider  TVisColorG;
	CSlider  TVisColorB;
	CCheckBox TVisColorHP;

	//Menu Colors

	
	CSlider  MenuBar1R;
	CSlider  MenuBar1G;
	CSlider  MenuBar1B;
	CSlider  MenuBar2R;
	CSlider  MenuBar2G;
	CSlider  MenuBar2B;
	//Inner
	CSlider  MenuInnerR;
	CSlider  MenuInnerG;
	CSlider  MenuInnerB;

	CSlider  MenuBGR;
	CSlider  MenuBGG;
	CSlider  MenuBGB;
	CSlider  MenuOpacity;
};

class CSkinchangerTab : public CTab
{
public:
	void Setup();

	// Knife Changer/Skin Changer
	CLabel    SkinActive;
	CCheckBox SkinEnable;
	CButton   SkinApply;

	// Knife
	CGroupBox KnifeGroup;
	CKeyBind  OtherSkinApply;
	CKeyBind  OtherSkinToggle;
	CComboBox KnifeModel;
	CComboBox KnifeSkin;

	// Pistols
	CGroupBox PistolGroup;
	CComboBox GLOCKSkin;
	CComboBox USPSSkin;
	CComboBox DEAGLESkin;
	CComboBox MAGNUMSkin;
	CComboBox DUALSSkin;
	CComboBox FIVESEVENSkin;
	CComboBox TECNINESkin;
	CComboBox P2000Skin;
	CComboBox P250Skin;
	CComboBox CZSkin;

	// MPs
	

	// Rifles
	CGroupBox Riflegroup;
	CComboBox M41SSkin;
	CComboBox M4A4Skin;
	CComboBox AK47Skin;
	CComboBox AUGSkin;
	CComboBox FAMASSkin;
	CComboBox GALILSkin;
	CComboBox SG553Skin;


	// Machineguns
	CGroupBox MachinegunsGroup;
	CComboBox NEGEVSkin;
	CComboBox M249Skin;

	// Snipers
	CGroupBox Snipergroup;
	CComboBox SCAR20Skin;
	CComboBox G3SG1Skin;
	CComboBox SSG08Skin;
	CComboBox AWPSkin;

	// Shotguns
	CGroupBox Shotgungroup;
	CComboBox MAG7Skin;
	CComboBox XM1014Skin;
	CComboBox SAWEDOFFSkin;
	CComboBox NOVASkin;

	// Skinsettings
	CGroupBox SkinsettingsGroup;
	CCheckBox StatTrakEnable;
	/*
*/
	
};

class CSettingsTab : public CTab
{
public:
	void Setup();
	CGroupBox ButtonGroup;
	CButton SaveButton;
	CButton LoadButton;
	CGroupBox SpecVisGroup;

	CGroupBox	SettingsHitChance;
	CSlider		SettingsHitChanceAuto;
	CSlider		SettingsHitChanceAWP;
	CSlider		SettingsHitChanceScout;
	CSlider		SettingsHitChanceRifles;
	CSlider		SettingsHitChanceMPs;
	CSlider		SettingsHitChancePistols;
	CSlider		SettingsHitChanceDeagle;
	CSlider		SettingsHitChanceZeus;

	CGroupBox	SettingsAALines;
	CSlider		SettingsAALinesRealR;
	CSlider		SettingsAALinesRealG;
	CSlider		SettingsAALinesRealB;
	CSlider		SettingsAALinesRealLenght;
	CSlider		SettingsAALinesFakeR;
	CSlider		SettingsAALinesFakeG;
	CSlider		SettingsAALinesFakeB;
	CSlider		SettingsAALinesFakeLenght;

	CGroupBox	SettingsHitScan;
	CCheckBox	SettingsHitScanEnable;
	CComboBox	SettingsHitScanAuto;
	CComboBox	SettingsHitScanScout;
	CComboBox	SettingsHitScanRifles;

	CGroupBox	SettingsHitScan2;
	CComboBox	SettingsHitScan2MPs;
	CComboBox	SettingsHitScan2Pistols;
	CComboBox	SettingsHitScan2Deagle;
	CComboBox	SettingsHitScan2Other;

	CSlider  SpecVisGroupX;
	CSlider  SpecVisGroupY;

	CGroupBox Gloves;
	CCheckBox EnableGloves;
	CComboBox GloveSkin;
	CComboBox GloveModel;
	CButton   GlovesApply;
	//ClanTag
	CGroupBox  CustomClanTag;
	CTextField ClanTagText;
	CComboBox  ClanTagType;
	CButton    ClanTagApply;

	//Other
	CGroupBox	OtherSettings;
	CSlider		OtherSettingsNadePredColorR;
	CSlider		OtherSettingsNadePredColorG;
	CSlider		OtherSettingsNadePredColorB;
	CComboBox   OtherSettingsImpacts;
	CCheckBox	OtherSettingsMemeWalk;
	CCheckBox	OtherSettingsHitmarkerSound;
	CCheckBox	OtherSettingsAutoAccept;
	CSlider		OtherSettingsBruteAfterX;
	CCheckBox	OtherSettingsDebug;
	CCheckBox	OtherSettingsAntiUntrusted;
	//Options
	CComboBox SetNr;
	CButton Unload;
	CButton Laglobby;
	

	
};

class COtherSettingsTab : public CTab
{
public:
	void Setup();

	CGroupBox	CrosshairGroup;
	CComboBox	OtherCrosshairType;
	CSlider		OtherCrosshairR;
	CSlider		OtherCrosshairG;
	CSlider		OtherCrosshairB;
	CSlider		OtherCrosshairA;
	CSlider		OtherCrosshairRR;
	CSlider		OtherCrosshairGG;
	CSlider		OtherCrosshairBB;
	CSlider		OtherCrosshairAA;
	CSlider		OtherCrosshairCScale;

	CGroupBox	CustomFriendlyColor;
	CSlider		customfriendlycolorTEAMVISR;
	CSlider		customfriendlycolorTEAMVISG;
	CSlider		customfriendlycolorTEAMVISB;
	CSlider		customfriendlycolorTEAMNOVISR;
	CSlider		customfriendlycolorTEAMNOVISG;
	CSlider		customfriendlycolorTEAMNOVISB;
	CSlider		customfriendlycolorENEMYVISR;
	CSlider		customfriendlycolorENEMYVISG;
	CSlider		customfriendlycolorENEMYVISB;
	CSlider		customfriendlycolorENEMYNOVISR;
	CSlider		customfriendlycolorENEMYNOVISG;
	CSlider		customfriendlycolorENEMYNOVISB;

};
class ApocalypseWindow : public CWindow
{
public:
	void Setup();

	CRageBotTab RageBotTab;
	CLegitBotTab LegitBotTab;
	CVisualTab VisualsTab;
	CMiscTab MiscTab;
	CColorTab ColorTab;
	CSettingsTab SettingsTab;
	COtherSettingsTab OtherSettingsTab;
	CSkinchangerTab SkinchangerTab;

	CButton SaveButton;
	CButton LoadButton;
	CButton UnloadButton;
	CButton LaglobbyButton;
};

namespace Menu
{
	void SetupMenu();
	void DoUIFrame();

	extern ApocalypseWindow Window;
};
