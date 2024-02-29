#include <string>
#include <thread>
#include <future>
#include <iostream>

#include "Cheats.h"
#include "Render.hpp"
#include "MenuConfig.hpp"

#include "Utils/Initial/Init.h"

#include "Features/ESP.h"
#include "Features/GUI.h"
#include "Features/RCS.H"
#include "Features/BombTimer.h"
#include "Features/SpectatorList.h"


int PreviousTotalHits = 0;


// i will add a image to this to make it so it shows the map detail - Phillip
void Cheats::RadarSetting(Base_Radar& Radar){
	// Radar window
	ImGui::SetNextWindowBgAlpha(0.7);
	ImGui::Begin("Radar", 0, ImGuiWindowFlags_NoResize);
	ImGui::SetWindowSize({ RadarCFG::RadarRange * 2,RadarCFG::RadarRange * 2 });
	
	// Radar.SetPos({ Gui.Window.Size.x / 2,Gui.Window.Size.y / 2 });
	Radar.SetDrawList(ImGui::GetWindowDrawList());
	Radar.SetPos({ ImGui::GetWindowPos().x + RadarCFG::RadarRange, ImGui::GetWindowPos().y + RadarCFG::RadarRange });
	Radar.SetProportion(RadarCFG::Proportion);
	Radar.SetRange(RadarCFG::RadarRange);
	Radar.SetSize(RadarCFG::RadarRange * 2);
	Radar.SetCrossColor(RadarCFG::RadarCrossLineColor);

	Radar.ArcArrowSize *= RadarCFG::RadarPointSizeProportion;
	Radar.ArrowSize *= RadarCFG::RadarPointSizeProportion;
	Radar.CircleSize *= RadarCFG::RadarPointSizeProportion;

	Radar.ShowCrossLine = RadarCFG::ShowRadarCrossLine;
	Radar.Opened = true;
}


void Cheats::Run()
{	
	// Show menu
	static DWORD lastTick = 0; 
	DWORD currentTick = GetTickCount(); 
	if ((GetAsyncKeyState(VK_INSERT) & 0x8000) && currentTick - lastTick >= 150) {
		MenuConfig::ShowMenu = !MenuConfig::ShowMenu;
		lastTick = currentTick;
	}
	if (MenuConfig::ShowMenu)
	{
		GUI::NewGui();
	}

	if (!Init::Client::isGameWindowActive() && !MenuConfig::ShowMenu)
		return;

	Misc::NightMode();

	// Update matrix
	if(!ProcessMgr.ReadMemory(gGame.GetMatrixAddress(), gGame.View.Matrix,64))
		return;

	// Update EntityList Entry
	gGame.UpdateEntityListEntry();

	DWORD64 LocalControllerAddress = 0;
	DWORD64 LocalPawnAddress = 0;

	if (!ProcessMgr.ReadMemory(gGame.GetLocalControllerAddress(), LocalControllerAddress))
		return;
	if (!ProcessMgr.ReadMemory(gGame.GetLocalPawnAddress(), LocalPawnAddress))
		return;

	// LocalEntity
	CEntity LocalEntity, ServerEntity;
	static int LocalPlayerControllerIndex = 1;
	LocalEntity.UpdateClientData();
	if (!LocalEntity.UpdateController(LocalControllerAddress))
		return;
	if (!LocalEntity.UpdatePawn(LocalPawnAddress) && !MiscCFG::WorkInSpec)
		return;



	const char* weaponGroupNames[] = { "Knife", "Rifle", "Pistol", "Heavy Pistol", "Sniper", "Smg","Shotgun", "Grenade","Unknown", };
	int weaponGroup = GetWeaponGroup(LocalEntity.Pawn.WeaponName);
	// broken weapon configs -Phillip
	/*
    std::unordered_map<int, std::pair<float, float>> weaponSettings = {
        {0, {0, 0}},// Knife
        {1, {Config::Raimfov, MenuConfig::Raimsmooth}},      // Rifle
        {3, {Config::Paimfov, MenuConfig::Paimsmooth}},      // Pistol
        {4, {Config::Hpaimfov, MenuConfig::Hpaimsmooth}},    // Heavy Pistol
        {5, {Config::Saimfov, MenuConfig::Saimsmooth}},      // Sniper
        {6, {Config::SMGaimfov, MenuConfig::SMGaimsmooth}}   // SMG
    };

	 auto it = weaponSettings.find(weaponGroup);
	if (it != weaponSettings.end()) {
		AimControl::AimFov = it->second.first;
		AimControl::Smooth = it->second.second;
	}
	else {
		AimControl::AimFov = MenuConfig::aimfov;
		AimControl::Smooth = MenuConfig::aimsmooth;
	}

	*/
	// HealthBar Map
	static std::map<DWORD64, ESP::HealthBar> HealthBarMap;

	// AimBot data
	float DistanceToSight = 0;
	float MaxAimDistance = 100000;
	Vec3  HeadPos{ 0,0,0 };
	Vec2  Angles{ 0,0 };
	std::vector<Vec3> AimPosList;

	// Radar Data
	Base_Radar Radar;
	if (RadarCFG::ShowRadar)
		RadarSetting(Radar);

	for (int i = 0; i < 64; i++)
	{
		CEntity Entity;
		DWORD64 EntityAddress = 0;
		if (!ProcessMgr.ReadMemory<DWORD64>(gGame.GetEntityListEntry() + (i + 1) * 0x78, EntityAddress))
			continue;
		if (EntityAddress == LocalEntity.Controller.Address)
		{
			LocalPlayerControllerIndex = i;
			continue;
		}
		if (!Entity.UpdateController(EntityAddress))
			continue;
		if (!Entity.UpdatePawn(Entity.Pawn.Address))
			continue;

		/*
		std::vector<std::string> spectators;
		if (MiscCFG::SpecList && !MenuConfig::ShowMenu)
		{
			int spectatorCount = 0;
			uint32_t m_hPawn;
			uintptr_t pCSPlayerPawn, m_pObserverServices;
			ProcessMgr.ReadMemory<uint32_t>(Entity.Controller.Address + 0x5BC, m_hPawn);
			ProcessMgr.ReadMemory<uintptr_t>(gGame.GetEntityListEntry() + 120 * (m_hPawn & 0x1FF), pCSPlayerPawn);
			ProcessMgr.ReadMemory<uintptr_t>(pCSPlayerPawn + Offset::PlayerController.m_pObserverServices, m_pObserverServices);

			if (m_pObserverServices)
			{
				uint32_t m_hObserverTarget;
				uintptr_t list_entry, pController;
				ProcessMgr.ReadMemory<uint32_t>(m_pObserverServices + Offset::PlayerController.m_hObserverTarget, m_hObserverTarget);
				ProcessMgr.ReadMemory<uintptr_t>(EntityAddress + 0x8 * ((m_hObserverTarget & 0x7FFF) >> 9) + 0x10, list_entry);
				ProcessMgr.ReadMemory<uintptr_t>(gGame.GetEntityListEntry() + 120 * (m_hObserverTarget & 0x1FF), pController);

				if (pController == LocalEntity.Pawn.Address)
				{
					spectators.push_back(Entity.Controller.PlayerName);
				}
			}
		}
		*/
		// It not work rn.
		// SpecList::SpectatorWindowList(spectators);
		if (MenuConfig::TeamCheck && Entity.Controller.TeamID == LocalEntity.Controller.TeamID)
			continue;

		if (MiscCFG::PlayerList)
			Misc::PlayerList(Entity, LocalEntity);

		if (!Entity.IsAlive())
			continue;
//		if (MenuConfig::VisibleCheck && (!Entity.Pawn.bSpottedByMask > 0))
//			continue;

		// Add entity to radar
		if (RadarCFG::ShowRadar)
			Radar.AddPoint(LocalEntity.Pawn.Pos, LocalEntity.Pawn.ViewAngle.y, Entity.Pawn.Pos, ImColor(237, 85, 106, 200), RadarCFG::RadarType, Entity.Pawn.ViewAngle.y);
		
		//Misc::RadarHack(Entity);

		if (!Entity.IsInScreen())
			continue;


		//update Bone select
		if (AimControl::HitboxList.size() != 0)
		{
			for (int i = 0; i < AimControl::HitboxList.size(); i++)
			{
				Vec3 TempPos;
				DistanceToSight = Entity.GetBone().BonePosList[AimControl::HitboxList[i]].ScreenPos.DistanceTo({ Gui.Window.Size.x / 2,Gui.Window.Size.y / 2 });

				if (DistanceToSight < MaxAimDistance)
				{
					MaxAimDistance = DistanceToSight;

					if (!MenuConfig::VisibleCheck ||
						Entity.Pawn.bSpottedByMask & (DWORD64(1) << (LocalPlayerControllerIndex)) ||
						LocalEntity.Pawn.bSpottedByMask & (DWORD64(1) << (i)))
					{
						TempPos = Entity.GetBone().BonePosList[AimControl::HitboxList[i]].Pos;
						if (AimControl::HitboxList[i] == BONEINDEX::head)
							TempPos.z -= 1.f;

						AimPosList.push_back(TempPos);
					}
				}
			}
		}

		// this shit is so ass ima recode it 
		if (ESPConfig::ESPenabled){
			ImVec4 Rect = ESP::Get2DBox(Entity);

			int distance = static_cast<int>(Entity.Pawn.Pos.DistanceTo(LocalEntity.Pawn.Pos) / 100);

			if (ESPConfig::RenderDistance == 0 || (distance <= ESPConfig::RenderDistance && ESPConfig::RenderDistance > 0)){
				ESP::RenderPlayerESP(LocalEntity, Entity, Rect, LocalPlayerControllerIndex, i);
				// Draw HealthBar
				if (ESPConfig::ShowHealthBar)
					ESP::DrawHealthBar(EntityAddress, 100, Entity.Pawn.Health, { Rect.x - 6.f,Rect.y }, { 4 ,Rect.w }, ESPConfig::HealthbarMax);
				
				// Draw Ammo
				// When player is using knife or nade, Ammo = -1.
				if (ESPConfig::AmmoBar ) 
					ESP::DrawAmmoBar(EntityAddress, Entity.Pawn.MaxAmmo, Entity.Pawn.Ammo,{ Rect.x, Rect.y + Rect.w + 2 }, { Rect.z, 4 }, ESPConfig::AmmoColor);
				if (ESPConfig::ArmorBar && Entity.Pawn.Armor > 0) 
					ESP::DrawArmorBar(EntityAddress, 100, Entity.Pawn.Armor, { Rect.x + Rect.z + 2, Rect.y }, { 4.f, Rect.w }, ESPConfig::ArmourColor);
				
			}
		}
		// SpecList::GetSpectatorList(Entity, LocalEntity, EntityAddress);
	}
	
	// Radar render
	if(RadarCFG::ShowRadar)
	{
		Radar.Render();
		ImGui::End();
	}

	// TriggerBot
	if (MenuConfig::TriggerBot && (GetAsyncKeyState(TriggerBot::HotKey) || MenuConfig::TriggerAlways))
		TriggerBot::Run(LocalEntity);	

	Misc::HitSound(LocalEntity, PreviousTotalHits);
	//Misc::FlashImmunity(LocalEntity);
	Misc::FastStop();
	//Misc::NadeManager(gGame);
	// CrossHair
	TriggerBot::TargetCheck(LocalEntity);
	//Misc::AirCheck(LocalEntity);
	if (CrosshairsCFG::ShowCrossHair) {
		if (!LocalEntity.Pawn.Scoped) {
			const auto crosshairColor = CrosshairsCFG::isAim && CrosshairsCFG::Targeting ? CrosshairsCFG::TargetedColor : CrosshairsCFG::CrossHairColor;
			Render::DrawCrossHair(ImGui::GetBackgroundDrawList(), ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), ImGui::ColorConvertFloat4ToU32(crosshairColor));
		}
	}

	bmb::RenderWindow();
	
	// Aimbot
	if (MenuConfig::AimBot)
	{
		if (ESPConfig::DrawFov) Render::DrawFovCircle(LocalEntity);

			if (GetAsyncKeyState(AimControl::HotKey)){
				if (AimPosList.size() != 0){
						AimControl::AimBot(LocalEntity, LocalEntity.Pawn.CameraPos, AimPosList);
				}
			}
		
	}
	if (AimControl::HasTarget == false || AimPosList.size() != 0 || !MenuConfig::AimBot && !weaponGroup == 7 || 2)
		RCS::RecoilControl(LocalEntity);
	//AimControl::AutoPistol(weaponGroup);

}
