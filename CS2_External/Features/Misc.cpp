#include "Misc.h"

unsigned char* sounds[] = { neverlose_sound, skeet_sound, Bell, Bubble, Cod};

namespace Misc
{
	bool aKeyPressed = false;
	bool dKeyPressed = false;
	bool wKeyPressed = false;
	bool sKeyPressed = false;

	

	void HitSound(const CEntity& aLocalPlayer, int& PreviousTotalHits) noexcept {
		char buffer[256] = "";

		uintptr_t pBulletServices, totalHits;
		ProcessMgr.ReadMemory(aLocalPlayer.Pawn.Address + Offset::Pawn.BulletServices, pBulletServices);
		ProcessMgr.ReadMemory(pBulletServices + Offset::Pawn.TotalHit, totalHits);

		ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 cursorPos = ImGui::GetCursorScreenPos();

		if (totalHits != PreviousTotalHits) {
			if (totalHits == 0 && PreviousTotalHits != 0) {}
			else {
				if (MiscCFG::HitSound && MiscCFG::hitsound_type >= 0 && MiscCFG::hitsound_type <= 5) {
					PlaySoundA(reinterpret_cast<char*>(sounds[MiscCFG::hitsound_type]), NULL, SND_ASYNC | SND_MEMORY);
				}
				PreviousTotalHits = totalHits;
			}
		}
	}

	/*
	void FlashImmunity(const CEntity& aLocalPlayer) noexcept
	{
		if (MenuConfig::SafeMode)
			return;

		float MaxAlpha = 255.f - MiscCFG::FlashImmunity;
		ProcessMgr.WriteMemory(aLocalPlayer.Pawn.Address + Offset::Pawn.flFlashMaxAlpha, MaxAlpha);
	}
	*/

	void FastStop() noexcept
	{
		if (!MiscCFG::FastStop)
			return;
		// Disable when bhopping
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			return;
		// Disable when slow walking
		if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
			return;

		Misc::StopKeyEvent('A', &aKeyPressed, 'D', 50.f);
		Misc::StopKeyEvent('D', &dKeyPressed, 'A', 50.f);
		Misc::StopKeyEvent('W', &wKeyPressed, 'S', 50.f);
		Misc::StopKeyEvent('S', &sKeyPressed, 'W', 50.f);
	}

	/*
	void NadeManager(CGame Game) noexcept
	{
		if (MenuConfig::SafeMode)
			return;

		std::vector<std::string> EntityNames = {
		"smokegrenade_projectile", "weapon_glock", "weapon_smokegrenade", "basemodelentity",
		"c_cs_player_for_precache", "info_particle_system", "prop_dynamic", "post_processing_volume",
		"env_player_visibility", "team_intro_terrorist", "c_cs_observer_for_precache",
		"team_intro_counterterrorist", "point_camera", "sky_camera", "env_sky", "team_select_terrorist",
		"team_select_counterterrorist", "point_camera", "func_bomb_target", "env_cubemap_fog",
		"csgo_viewmodel", "cs_minimap_boundary", "cs_gamerules", "cs_player_manager", "vote_controller",
		"weapon_incgrenade", "molotov_projectile"
		};

		if (!MiscCFG::NoSmoke && !MiscCFG::SmokeColored)
			return;

		for (int i_smoke = 64; i_smoke < 1024; i_smoke++) {
			uintptr_t SmokeEntity = GetSmokeEntity(i_smoke, Game.GetEntityListEntry());

			uintptr_t ent_base, addr;
			ProcessMgr.ReadMemory<uintptr_t>(SmokeEntity, ent_base);

			// No Smoke
			if (MiscCFG::NoSmoke)
			{
				bool begin = false;
				int uf = 0;

				ProcessMgr.WriteMemory<bool>(ent_base + Offset::SmokeGrenadeProjectile.bDidSmokeEffect, begin);
				ProcessMgr.WriteMemory<bool>(ent_base + Offset::SmokeGrenadeProjectile.bSmokeEffectSpawned, begin);
				ProcessMgr.WriteMemory<int>(ent_base + Offset::SmokeGrenadeProjectile.nSmokeEffectTickBegin, uf);
			}

			// Smoke Color
			if (MiscCFG::SmokeColored || MiscCFG::FireColored)
			{
				char toread[32];
				std::string classname;
				Vector3 COLOR = { MiscCFG::SmokeColor.Value.x, MiscCFG::SmokeColor.Value.y ,MiscCFG::SmokeColor.Value.z };
				ProcessMgr.ReadMemory<uintptr_t>(ent_base + 0x10, addr);
				ProcessMgr.ReadMemory<uintptr_t>(addr + 0x20, addr);
				ProcessMgr.ReadMemory<char[32]>(addr, toread);
				classname = toread;

				
				* Filter id to find id
				if (std::find(EntityNames.begin(), EntityNames.end(), classname) == EntityNames.end())
					std::cout << classname << std::endl;
				

				if (classname == "smokegrenade_projectile")
				{
					if (MiscCFG::SmokeColored)
						ProcessMgr.WriteMemory<Vector3>(ent_base + Offset::SmokeGrenadeProjectile.vSmokeColor, COLOR);
				}
				 Disabled
				if (classname == "molotov_projectile")
				{
					Vector3 FireColor = { 0,0,0 };
					ProcessMgr.ReadMemory<Vector3>(ent_base + 0x112C, FireColor);
					std::cout << FireColor.x << ", " << FireColor.y << ", " << FireColor.z << std::endl;
						
				}
				
			}
		}
	}
	
	void RadarHack(const CEntity& EntityList) noexcept
	{
		if (MenuConfig::SafeMode)
			return;

		if (!MiscCFG::RadarHack)
			return;

		bool SpottedStatus = true;
		ProcessMgr.WriteMemory(EntityList.Pawn.Address + Offset::Pawn.bSpottedByMask, SpottedStatus);
	}
	*/

	void PlayerList(const CEntity& EntityList, const CEntity& LocalEntity) noexcept
	{
		if (!MiscCFG::PlayerList && !MenuConfig::ShowMenu)
			return;

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;

		ImGui::Begin("Playerlist", nullptr, flags);{
			// Begin the table
			const std::vector<std::string> headers = { "Name", "Health", "Money","Distance","Team" }; // i removed state cause it doesnt work right
			if (ImGui::BeginTable("PlayerMoney", static_cast<int>(headers.size()),
				ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit)) {

				for (const auto& header : headers) {
					ImGui::TableSetupColumn(header.c_str());
					
				}
				ImGui::TableHeadersRow();
				ImGui::TableNextRow();

				std::stringstream ss;
				ss << "$" << EntityList.Controller.Money;
				int distance = static_cast<int>(EntityList.Pawn.Pos.DistanceTo(LocalEntity.Pawn.Pos) / 100);
				std::string dis_str = std::to_string(distance) + "m";
				int teamID = EntityList.Controller.TeamID;
				const char* teamNames[] = { "", "Spec", "T", "CT" };
				const char* teamName = teamNames[teamID];

				//int aliveStatus = EntityList.Controller.AliveStatus;
				//const char* playerState = (aliveStatus == 1 ? "Alive" : "Dead");

				ImGui::TableNextColumn(); ImGui::Text("%s", EntityList.Controller.PlayerName.c_str());
				ImGui::TableNextColumn(); ImGui::Text("%d", EntityList.Pawn.Health);
				ImGui::TableNextColumn(); ImGui::TextColored(ImColor(0, 255, 0, 255), ss.str().c_str());
				ImGui::TableNextColumn(); ImGui::Text("%s", dis_str.c_str());
				ImGui::TableNextColumn(); ImGui::Text("%s", teamName);
				//ImGui::TableNextColumn(); ImGui::Text("%s", playerState); // doesnt work properly
				ImGui::EndTable();
			}
		}
		ImGui::End(); 
	}


	// @Phillip
	void NightMode() noexcept
	{
		if (!MiscCFG::NightMode)
			return;

		ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_Always);
		ImGui::SetNextWindowSize({ Gui.Window.Size.x, Gui.Window.Size.y }, ImGuiCond_Always);
		ImGui::SetNextWindowBgAlpha(0.f);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
		ImGui::Begin("##background", nullptr, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration);

		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();

		ImGui::GetBackgroundDrawList()->AddRectFilled(windowPos, ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y), IM_COL32(0, 0, 0, MiscCFG::NightModeAlpha));
		

		ImGui::End();
		ImGui::PopStyleColor();
			
	}
}
