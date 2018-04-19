#include "Resolver.h"

int Globals::Shots = 0;
bool Globals::change;
CUserCmd* Globals::UserCmd;
int Globals::TargetID;
int resolvermode;
int Shooted;
IClientEntity* Globals::Target;

/*void PitchCorrection()
{
	CUserCmd* pCmd;
	for (int i = 0; i < Interfaces::Engine->GetMaxClients(); ++i)
	{
		IClientEntity* pLocal = hackManager.pLocal();
		IClientEntity *player = (IClientEntity*)Interfaces::EntList->GetClientEntity(i);
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

		if (!player || player->IsDormant() || player->GetHealth() < 1 || (DWORD)player == (DWORD)pLocal)
			continue;

		if (!player)
			continue;

		if (pLocal)
			continue;

		if (pLocal && player && pLocal->IsAlive())
		{
			if (Menu::Window.RageBotTab.AdvancedResolver.GetState())
			{
				Vector* eyeAngles = pEntity->GetEyeAnglesPointer();
				if (eyeAngles->x < -179.f) eyeAngles->x += 360.f;
				else if (eyeAngles->x > 90.0 || eyeAngles->x < -90.0) eyeAngles->x = 89.f;
				else if (eyeAngles->x > 89.0 && eyeAngles->x < 91.0) eyeAngles->x -= 90.f;
				else if (eyeAngles->x > 179.0 && eyeAngles->x < 181.0) eyeAngles->x -= 180;
				else if (eyeAngles->x > -179.0 && eyeAngles->x < -181.0) eyeAngles->x += 180;
				else if (fabs(eyeAngles->x) == 0) eyeAngles->x = std::copysign(89.0f, eyeAngles->x);
			}
		}
	}
}*/

void R::Resolve()
{
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	
	for (int i = 0; i < Interfaces::Engine->GetMaxClients(); ++i)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

		if (!pEntity || pEntity->IsDormant() || !pEntity->IsAlive())
			continue;

		if (pEntity->GetTeamNum() == pLocal->GetTeamNum() || !pLocal->IsAlive())
			continue;

		Vector* eyeAngles = pEntity->GetEyeAnglesPointer();

		if (Menu::Window.RageBotTab.AccuracyResolver.GetIndex() >= 1)
		{
			if (pEntity != Globals::Target) continue;

			Vector AimAngle;
			CalcAngleYawOnly(pEntity->GetEyePosition(), pLocal->GetAbsOrigin2(), AimAngle);
			NormalizeVector(AimAngle);
			static float StoredYaw = 0;
			static bool bLowerBodyIsUpdated = false;
			if (pEntity->GetLowerBodyYaw() != StoredYaw) bLowerBodyIsUpdated = true;
			else bLowerBodyIsUpdated = false;
			if (bLowerBodyIsUpdated) StoredYaw = pEntity->GetLowerBodyYaw();
			if (Menu::Window.RageBotTab.AccuracyResolver.GetIndex() == 1)
			{
				if (pEntity->GetFlags() & FL_ONGROUND && pEntity->GetVelocity().Length2D() != 0) eyeAngles->y = pEntity->GetLowerBodyYaw();
				else
				{
					if (Menu::Window.RageBotTab.AccuracyResolver.GetIndex() == 1 || Menu::Window.RageBotTab.AccuracyResolver.GetIndex() == 2)
					{
						int num = Globals::Shots % 5;
						switch (num) {
						case 0:eyeAngles->y = AimAngle.y - 0; break;
						case 1:eyeAngles->y = AimAngle.y - 15; break;
						case 2:eyeAngles->y = AimAngle.y + 40; break;
						case 3:eyeAngles->y = AimAngle.y - 40; break;
						case 4:eyeAngles->y = AimAngle.y + 15; break;
						}
					}
				}
			}
			if (Menu::Window.RageBotTab.AccuracyResolver.GetIndex() == 2) //sneakyevil pResolver
			{
				bool isFakeHeading = false;
				bool bLowerBodyUpdated = false;
				bool IsBreakingLBY = false;
				static bool isMoving;
				float PlayerIsMoving = abs(pEntity->GetVelocity().Length());
				if (PlayerIsMoving > 0.1) isMoving = true;
				else if (PlayerIsMoving <= 0.1) isMoving = false;
				auto FYaw = pEntity->GetLowerBodyYaw();
				float bodyeyedelta = fabs(pEntity->GetEyeAngles().y - pEntity->GetLowerBodyYaw());
				if (pEntity->GetEyeAngles().y - pEntity->GetLowerBodyYaw() > 35)
				{
					isFakeHeading = true;
				}
				else
				{
					isFakeHeading = false;
				}
				if (bLowerBodyIsUpdated != pEntity->GetLowerBodyYaw())
				{
					bLowerBodyUpdated = true;
				}
				else
				{
					bLowerBodyUpdated = false;
				}
				if (isMoving || bLowerBodyUpdated)
				{
					resolvermode = 3;

					eyeAngles->y = FYaw;
					StoredYaw = pEntity->GetEyeAngles().y;

					IsBreakingLBY = false;
				}
				else
				{
					resolvermode = 1;

					if (bodyeyedelta == 0.f && pEntity->GetVelocity().Length2D() >= 0 && pEntity->GetVelocity().Length2D() < 36)
					{
						eyeAngles->y = StoredYaw;
						IsBreakingLBY = true;
					}
					else
					{
						IsBreakingLBY = false;
					}
				}
				if (IsBreakingLBY)
				{
					resolvermode = 2;

					eyeAngles->y = StoredYaw;

					switch (Globals::Shots % 3)
					{
					case 1:eyeAngles->y = 180; break;
					case 2:eyeAngles->y = pEntity->GetLowerBodyYaw() + rand() % 180;
					case 3:eyeAngles->y = pEntity->GetLowerBodyYaw() + pEntity->GetEyeAngles().y + rand() % 35;
					}
				}
				else if (!IsBreakingLBY && !isFakeHeading)
				{
					resolvermode = 5;

					eyeAngles->y = pEntity->GetLowerBodyYaw();

					switch (Globals::Shots % 4)
					{
					case 1:eyeAngles->y = 45 + rand() % 180;
					case 2:eyeAngles->y = StoredYaw + rand() % 90;
					case 3:eyeAngles->y = 180 + rand() % 90;
					case 4:eyeAngles->y = StoredYaw + pEntity->GetEyeAngles().y + rand() % 45;
					}
				}
				else if (isFakeHeading)
				{
					resolvermode = 4;

					eyeAngles->y = pEntity->GetLowerBodyYaw() - pEntity->GetEyeAngles().y;

					switch (Globals::Shots % 2)
					{
					case 1:eyeAngles->y = pEntity->GetLowerBodyYaw() + rand() % 90; break;
					case 2:eyeAngles->y = pEntity->GetEyeAngles().y + rand() % 360; break;
					}
				}
				else
				{
					resolvermode = 1;

					eyeAngles->y = pEntity->GetLowerBodyYaw() + rand() % 180;

					switch (Globals::Shots % 13)
					{
					case 1:eyeAngles->y = 180; break;
					case 2:eyeAngles->y = pEntity->GetLowerBodyYaw() + rand() % 180;
					case 3:eyeAngles->y = pEntity->GetLowerBodyYaw() + pEntity->GetEyeAngles().y + rand() % 35;
					case 4:eyeAngles->y = 45 + rand() % 180;
					case 5:eyeAngles->y = StoredYaw + rand() % 90;
					case 6:eyeAngles->y = 180 + rand() % 90;
					case 7:eyeAngles->y = StoredYaw + pEntity->GetEyeAngles().y + rand() % 45;
					case 8:eyeAngles->y = pEntity->GetLowerBodyYaw(); break;
					case 9:eyeAngles->y = pEntity->GetLowerBodyYaw() + rand() % 90; break;
					case 10:eyeAngles->y = pEntity->GetLowerBodyYaw() + rand() % 180; break;
					case 11:eyeAngles->y = StoredYaw + rand() % 45; break;
					case 12:eyeAngles->y = pEntity->GetLowerBodyYaw() + rand() % 90; break;
					case 13:eyeAngles->y = pEntity->GetEyeAngles().y + rand() % 360; break;
					}
				}
			}
			if (Menu::Window.RageBotTab.AccuracyResolver.GetIndex() == 6)
			{
				Vector* EyeYaw = pEntity->GetEyeAnglesPointer();
				static bool isMoving;
				float PlayerIsMoving = abs(pEntity->GetVelocity().Length());
				if (PlayerIsMoving > 0.1) isMoving = true;
				else if (PlayerIsMoving <= 0.1) isMoving = false;

				static float StoredLBY = 0.0f;
				static bool bLowerBodyIsUpdated;
				if (pEntity->GetLowerBodyYaw() != StoredLBY) bLowerBodyIsUpdated = true;
				else bLowerBodyIsUpdated = false;

				float bodyeyedelta = pEntity->GetEyeAngles().y - pEntity->GetLowerBodyYaw();

				if (bLowerBodyIsUpdated || isMoving || fabsf(bodyeyedelta) >= 35.0f)
				{
					EyeYaw->y = pEntity->GetLowerBodyYaw();
					StoredLBY = pEntity->GetLowerBodyYaw();
				}

				else
				{
					if (fabsf(bodyeyedelta) < 35.0f && fabsf(bodyeyedelta) > 0.0f)
						EyeYaw->y = pEntity->GetLowerBodyYaw() + bodyeyedelta;
				}
			}
		}
		/*if (Menu::Window.RageBotTab.AccuracyResolver.GetIndex() == 3)
		{
			if (pEntity->GetVelocity().Length2D() >= 0.9)
			{
				eyeAngles->y = pEntity->GetLowerBodyYaw();
			}
			else if (!pEntity->GetFlags() & FL_ONGROUND)
			{
				switch (Globals::Shots % 1)
				{
				case 1:
					eyeAngles->y = -30 + rand() % 45 - rand() % 51;
					break;
				case 2:
					eyeAngles->y = pEntity->GetLowerBodyYaw() + 20 - rand() % 35;
					break;
				case 3:
					eyeAngles->y = pEntity->GetLowerBodyYaw() + 45 % 30 - rand() % 90;
					break;
				case 4:
					eyeAngles->y = pEntity->GetLowerBodyYaw() - 90 - rand() % 90 - rand() % 40;
					break;
				case 5:
					eyeAngles->y = -160 - rand() % 90 - rand() % 40;
					break;
				}
			}
			else
			{
				eyeAngles->y = rand() % 180 - rand() % 45;
			}
		}
		if (Menu::Window.RageBotTab.AccuracyResolver.GetIndex() == 4)
		{
			eyeAngles->y = 179.f;
			if (!pEntity->GetVelocity().Length2D() >= 0.9)
			{
				eyeAngles->y = pEntity->GetLowerBodyYaw() - 25.f + rand() % 15 - rand() % 19;
			}
			else if (!pEntity->GetFlags() & FL_ONGROUND)
			{
				eyeAngles->y = 175.f + rand() % 40 - rand() % 41;
			}
			else if (pEntity->GetFlags() & FL_ONGROUND)
			{
				eyeAngles->y = pEntity->GetLowerBodyYaw() - 125.f - rand() % 40 + rand() % 39;
			}
		}*/
	}
}