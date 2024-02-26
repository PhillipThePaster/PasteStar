#pragma once
#include "..\MenuConfig.hpp"
#include "..\Render.hpp"
#include "..\Features\Aimbot\Legitbot.hpp"
#include "..\Radar\Radar.h"
#include "..\TriggerBot.h"

#include "..\Resources\Images.h"
#include "../Utils/keys.cpp"

ID3D11ShaderResourceView* PasteLogo = NULL;
int LogoW = 0, LogoH = 0;

// checkbox for hitbox
bool checkbox1 = true;


const char* Hitpos[] = { "Head", "Neck",  "Chest", "Stomach" , "Pelvis" };
const char* items[] = { "Distance", "Lethal",  "Scoped", "Defusing" , "Money" };
int WeaponTab = 0; // weapon configs shit -phillip

static void HotKey(int* k){
	static bool waitingforkey = false;
	if (waitingforkey == false) {
		if (ImGui::Button(KeyNames[*k]))
			waitingforkey = true;
	}
	else if (waitingforkey == true) {
		ImGui::Button("...");
		std::this_thread::sleep_for(std::chrono::milliseconds(70));
		for (auto& Key : KeyCodes){
			if (GetAsyncKeyState(Key)) {
				*k = Key;
				waitingforkey = false;
			}
		}
	}
}

namespace GUI{
	// bad code
	inline void InitHitboxList() {
		if (MenuConfig::HitboxUpdated)
			return;

		auto HitboxList = AimControl::HitboxList;

		for (int i = 0; i < 5; ++i) {
			auto it = std::find(HitboxList.begin(), HitboxList.end(), static_cast<BONEINDEX>(i));
			if (it != HitboxList.end())
				*(&checkbox1 + i) = true;
		}

		MenuConfig::HitboxUpdated = false;
	}

	void addHitbox(int BoneIndex) {
		AimControl::HitboxList.push_back(BoneIndex);
	}

	void removeHitbox(int BoneIndex) {
		AimControl::HitboxList.erase(std::remove(AimControl::HitboxList.begin(), AimControl::HitboxList.end(), BoneIndex), AimControl::HitboxList.end());
	}

	void LoadImages(){
		if (PasteLogo == NULL){
			// Updater::CheckForUpdates();
			Gui.LoadTextureFromMemory(Images::PasteLogo, sizeof Images::PasteLogo, &PasteLogo, &LogoW, &LogoH);
		}
	}

	static int tabs;
	static int subtabs; // when i add weapon configs -Phillip
	float dpi_scale = 1.f;
	ImVec2 pos;
	ImDrawList* draw;

	void decorations(){
		pos = ImGui::GetWindowPos();
		draw = ImGui::GetWindowDrawList();

		draw->AddRectFilled(ImVec2(pos), ImVec2(pos.x + 680, pos.y + 433), ImColor(13, 13, 13));
		draw->AddRectFilledMultiColor(ImVec2(pos), ImVec2(pos.x + 680, pos.y + 25), ImColor(23, 23, 23), ImColor(23, 23, 23), ImColor(13, 13, 13), ImColor(13, 13, 13));
		draw->AddRectFilled(ImVec2(pos.x + 14, pos.y + 30), ImVec2(pos.x + 670, pos.y + 423), ImColor(18, 18, 18));
		draw->AddRect(ImVec2(pos.x + 14, pos.y + 30), ImVec2(pos.x + 670, pos.y + 423), ImColor(5, 5, 5), 0.f, 0.f, 2.f);
		draw->AddRect(ImVec2(pos.x + 175, pos.y + 35), ImVec2(pos.x + 665, pos.y + 417), ImColor(5, 5, 5), 0, 0, 2.f);
		ImVec2 textSize = ImGui::CalcTextSize("PasteStar");
		ImVec2 textPos = ImVec2(pos.x + (680 - textSize.x) * 0.5f, pos.y + 15 - textSize.y * 0.5f); 
		draw->AddText(ImGui::GetFont(), ImGui::GetFontSize(), textPos, ImColor(255, 255, 255), "PasteStar");
		ImGui::SetCursorPos(ImVec2(22, 40));
	}


	void NewGui(){

		LoadImages();
		ImTextureID ImageID;
		ImVec2 LogoSize, LogoPos;

		float RadarPointSizeProportionMin = 0.8f, RadarPointSizeProportionMax = 2.f;
		float ProportionMin = 500.f, ProportionMax = 3300.f;
		float RadarRangeMin = 100.f, RadarRangeMax = 300.f;
		float AlphaMin = 0.f, AlphaMax = 1.f;

		ImageID = (void*)PasteLogo;
		LogoSize = ImVec2(LogoW - 160 , LogoH - 180);
		LogoPos = MenuConfig::WCS.LogoPos;
		MenuConfig::ButtonBorderColor = MenuConfig::WCS.BorderColor_Yellow;

		ImGui::Begin("PasteStar", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration); {
			ImGuiIO& io = ImGui::GetIO(); (void)io;

			ImVec4* colors = ImGui::GetStyle().Colors;
			ImGuiStyle& style = ImGui::GetStyle();
			style.FrameBorderSize = 1.0f;
			style.WindowRounding = 0.0f;
			style.ChildRounding = 0.0f;
			style.FrameRounding = 0.0f;
			style.PopupRounding = 0.0f;
			style.ScrollbarRounding = 0.0f;
			style.GrabRounding = 0.0f;
			style.TabRounding = 0.0f;

			style.WindowTitleAlign = { 0.0, 0.5 };
			style.ColorButtonPosition = ImGuiDir_Left;


			colors[ImGuiCol_CheckMark] = ImVec4(MenuConfig::Accent.Value.x, MenuConfig::Accent.Value.y, MenuConfig::Accent.Value.z, MenuConfig::Accent.Value.w);
			colors[ImGuiCol_Text] = { 1.0f, 1.0f, 1.0f, 1.00f };				//
			colors[ImGuiCol_TextDisabled] = { 0.25f, 0.25f, 0.25f, 1.00f };		//
			colors[ImGuiCol_WindowBg] = { 0.09f, 0.09f, 0.09f, 0.94f };			//
			colors[ImGuiCol_ChildBg] = { 0.11f, 0.11f, 0.11f, 0.00f };			//
			colors[ImGuiCol_PopupBg] = { 0.11f, 0.11f, 0.11f, 0.94f };			//
			colors[ImGuiCol_Border] = { 0.07f, 0.08f, 0.08f, 1.00f };
			colors[ImGuiCol_BorderShadow] = { 0.00f, 0.00f, 0.00f, 0.00f };
			colors[ImGuiCol_FrameBg] = { 0.35f, 0.35f, 0.35f, 0.54f };			//
			colors[ImGuiCol_FrameBgHovered] = { 0.31f, 0.29f, 0.27f, 1.00f };
			colors[ImGuiCol_FrameBgActive] = { 0.40f, 0.36f, 0.33f, 0.67f };
			colors[ImGuiCol_TitleBg] = { 0.15f, 0.15f, 0.15f, 0.9f };
			colors[ImGuiCol_TitleBgActive] = { 0.15f, 0.15f, 0.15f, 0.9f };		//
			colors[ImGuiCol_TitleBgCollapsed] = { 0.15f, 0.15f, 0.15f, 0.9f };
			colors[ImGuiCol_MenuBarBg] = { 0.18f, 0.18f, 0.18f, 0.94f };		//
			colors[ImGuiCol_ScrollbarBg] = { 0.00f, 0.00f, 0.00f, 0.16f };
			colors[ImGuiCol_ScrollbarGrab] = { 0.24f, 0.22f, 0.21f, 1.00f };
			colors[ImGuiCol_ScrollbarGrabHovered] = { 0.31f, 0.29f, 0.27f, 1.00f };
			colors[ImGuiCol_ScrollbarGrabActive] = { 0.40f, 0.36f, 0.33f, 1.00f };
			colors[ImGuiCol_SliderGrab] = { 0.8f, 0.8f, 0.8f, 1.0f };			//		
			colors[ImGuiCol_SliderGrabActive] = { 0.55f, 0.55f, 0.55f, 1.00f }; //
			colors[ImGuiCol_Button] = { 0.10f, 0.10f, 0.10f, 0.40f };			//
			colors[ImGuiCol_ButtonHovered] = { 0.15f, 0.15f, 0.15f, 0.62f };	//	
			colors[ImGuiCol_ButtonActive] = { 0.60f, 0.60f, 0.60f, 1.00f };		//
			colors[ImGuiCol_Header] = { MenuConfig::Accent.Value.x, MenuConfig::Accent.Value.y,MenuConfig::Accent.Value.z, 0.1f };			//
			colors[ImGuiCol_HeaderHovered] = { 0.25f, 0.25f, 0.25f, 0.80f };	//
			colors[ImGuiCol_HeaderActive] = { 0.42f, 0.42f, 0.42f, 1.00f };
			colors[ImGuiCol_Separator] = { 0.35f, 0.35f, 0.35f, 0.50f };		//
			colors[ImGuiCol_SeparatorHovered] = { 0.31f, 0.29f, 0.27f, 0.78f };
			colors[ImGuiCol_SeparatorActive] = { 0.40f, 0.36f, 0.33f, 1.00f };
			colors[ImGuiCol_ResizeGrip] = { 1.0f, 1.0f, 1.0f, 0.25f };			//
			colors[ImGuiCol_ResizeGripHovered] = { 1.00f, 1.0f, 1.0f, 0.4f };	//
			colors[ImGuiCol_ResizeGripActive] = { 1.00f, 1.00f, 1.0f, 0.95f };	//
			colors[ImGuiCol_Tab] = { 0.18f, 0.18f, 0.18f, 1.0f };				//
			colors[ImGuiCol_TabHovered] = { 0.58f, 0.58f, 0.58f, 0.80f };		//
			colors[ImGuiCol_TabActive] = { 0.6f, 0.60f, 0.60f, 1.00f };
			colors[ImGuiCol_TabUnfocused] = { 0.07f, 0.10f, 0.15f, 0.97f };
			colors[ImGuiCol_TabUnfocusedActive] = { 0.14f, 0.26f, 0.42f, 1.00f };
			colors[ImGuiCol_PlotLines] = { 0.66f, 0.60f, 0.52f, 1.00f };
			colors[ImGuiCol_PlotLinesHovered] = { 0.98f, 0.29f, 0.20f, 1.00f };
			colors[ImGuiCol_PlotHistogram] = { 0.60f, 0.59f, 0.10f, 1.00f };
			colors[ImGuiCol_PlotHistogramHovered] = { 0.72f, 0.73f, 0.15f, 1.00f };
			colors[ImGuiCol_TextSelectedBg] = { 0.27f, 0.52f, 0.53f, 0.35f };
			colors[ImGuiCol_DragDropTarget] = { 0.60f, 0.59f, 0.10f, 0.90f };
			colors[ImGuiCol_NavHighlight] = { 0.51f, 0.65f, 0.60f, 1.00f };
			colors[ImGuiCol_NavWindowingHighlight] = { 1.00f, 1.00f, 1.00f, 0.70f };
			colors[ImGuiCol_NavWindowingDimBg] = { 0.80f, 0.80f, 0.80f, 0.20f };
			colors[ImGuiCol_ModalWindowDimBg] = { 0.11f, 0.13f, 0.13f, 0.35f };

			ImFontConfig font_config;
			font_config.OversampleH = 1; //or 2 is the same
			font_config.OversampleV = 1;
			font_config.PixelSnapH = 1;


			static int x = 680 * dpi_scale, y = 433 * dpi_scale;
			pos = ImGui::GetWindowPos();
			draw = ImGui::GetWindowDrawList();

			ImGui::SetWindowSize(ImVec2(ImFloor(x * dpi_scale), ImFloor(y * dpi_scale)));

			decorations();
			ImGui::BeginGroup();
			ImGui::SetCursorPos(ImVec2(30.f, 35.f));
			ImGui::Image(ImageID, LogoSize);

			const char* tabNames[] = { "Aimbot", "Triggerbot", "Visuals", "Misc", "Settings" };

			for (int i = 0; i < 5; ++i) {
				if (ImGui::tab(tabNames[i], i == tabs))
					tabs = i;
			}

			ImGui::EndGroup();

			switch (tabs) {
			case 0:
				ImGui::SetCursorPos(ImVec2(175, 30));
				if (ImGui::BeginChild(2, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), ImGuiWindowFlags_ChildMenu, true)) {

					ImGui::BeginChild("Tab1", ImVec2(ImGui::GetContentRegionAvail().x / 2.02f, ImGui::GetContentRegionAvail().y / 1.2));
					{
						ImGui::SetCursorPos(ImVec2(0, 10));
						ImGui::BeginGroup(); {
							ImGui::Text("going to recode the full aimbot");
							ImGui::Checkbox("Aimbot", &MenuConfig::AimBot);
							ImGui::SameLine();
							HotKey(&AimControl::HotKey);
							ImGui::Checkbox("DrawFov", &ESPConfig::DrawFov);
							ImGui::SameLine();
							ImGui::ColorEdit4("##FovCircleColor", reinterpret_cast<float*>(&MenuConfig::FovCircleColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
							ImGui::Checkbox("Visible", &MenuConfig::VisibleCheck);
							ImGui::Checkbox("Scoped", &AimControl::ScopeOnly);
							ImGui::Checkbox("Flashed", &AimControl::IgnoreFlash);
							ImGui::MultiCombo("Hitbox", &AimControl::AimPos, Hitpos, 5);
							Gui.Slider("Fov", ImGuiDataType_Float, &AimControl::AimFov, 0.f, 60.f, "%.0f", MenuConfig::Accent, ImGuiSliderFlags_None);
							Gui.Slider("Smooth", ImGuiDataType_Float, &AimControl::Smooth, 1.f, 5.f, "%.1f", MenuConfig::Accent, ImGuiSliderFlags_None);

							BONEINDEX boneIndices[] = { BONEINDEX::head, BONEINDEX::neck_0, BONEINDEX::spine_1, BONEINDEX::spine_2, BONEINDEX::pelvis };

							for (int i = 0; i < sizeof(boneIndices) / sizeof(boneIndices[0]); ++i) {
								if (AimControl::AimPos & (1 << i)) {
									addHitbox(boneIndices[i]);
								}
								else {
									removeHitbox(boneIndices[i]);
								}
							}
						}
						ImGui::EndGroup();
					}
					ImGui::EndChild();
					ImGui::SameLine(0.f, 10.f);
					ImGui::BeginChild("Tab2", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 1.2)); {
						ImGui::SetCursorPos(ImVec2(10, 10));
						ImGui::BeginGroup(); {
							float recoilMin = 0.f, recoilMax = 2.f;
							ImGui::Text("Will be recoded");
							ImGui::Checkbox("Rcs", &MenuConfig::RCS);
							Gui.Slider("Yaw", ImGuiDataType_Float, &RCS::RCSScale.x, 0.f, 2.f, "%.2f", MenuConfig::Accent, ImGuiSliderFlags_None);
							Gui.Slider("Pitch", ImGuiDataType_Float, &RCS::RCSScale.y, 0.f, 2.f, "%.2f", MenuConfig::Accent, ImGuiSliderFlags_None);				
						}
						ImGui::EndGroup();
					}
					ImGui::EndChild();

				}ImGui::EndChild();

				break;
			case 1:
				ImGui::SetCursorPos(ImVec2(180, 30));
				ImGui::BeginChild("Tab1", ImVec2(235, 382));
				{
					ImGui::SetCursorPos(ImVec2(10, 10));
					ImGui::BeginGroup(); {
						ImGui::Checkbox("Triggerbot", &MenuConfig::TriggerBot);
						ImGui::SameLine();
						HotKey(&TriggerBot::HotKey);
						ImGui::Checkbox("Always", &MenuConfig::TriggerAlways); // will add a custom keybind system with this just to lazy -Phillip
						ImGui::Checkbox("Scoped", &TriggerBot::ScopeOnly);
						ImGui::Checkbox("Flashed", &TriggerBot::IgnoreFlash);
						Gui.Slider("Delay", ImGuiDataType_Float, &AimControl::AimFov, 0.f, 1000.f, "%.0f ms", MenuConfig::Accent, ImGuiSliderFlags_None);
					}
					ImGui::EndGroup();
				}
				ImGui::EndChild();
				ImGui::SameLine(0.f, 10.f);
				ImGui::BeginChild("Tab2", ImVec2(235, 382));
				{
					ImGui::SetCursorPos(ImVec2(10, 10));
					ImGui::BeginGroup(); {
					}
					ImGui::EndGroup();
				}
				ImGui::EndChild();
				break;
			case 2:
				ImGui::SetCursorPos(ImVec2(180, 30));
				ImGui::BeginChild("Tab1", ImVec2(235, 382));
				{
					ImGui::SetCursorPos(ImVec2(10, 10));
					ImGui::BeginGroup(); {
						ImGui::Checkbox("Box", &ESPConfig::ShowBoxESP);
						ImGui::SameLine();
						ImGui::ColorEdit4("##boxcolor", reinterpret_cast<float*>(&ESPConfig::BoxColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
						ImGui::Checkbox("Filled", &ESPConfig::FilledBox);
						ImGui::SameLine();
						ImGui::ColorEdit4("##Filledcolor", reinterpret_cast<float*>(&ESPConfig::FilledColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
						ImGui::Checkbox("Headcircle", &ESPConfig::ShowHeadBox);
						ImGui::SameLine();
						ImGui::ColorEdit4("##Headcolor", reinterpret_cast<float*>(&ESPConfig::HeadBoxColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
						ImGui::Checkbox("Skeleton", &ESPConfig::ShowBoneESP);
						ImGui::SameLine();
						ImGui::ColorEdit4("##Skeletoncolor", reinterpret_cast<float*>(&ESPConfig::BoneColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
						ImGui::Checkbox("Healthbar", &ESPConfig::ShowHealthBar);
						ImGui::Checkbox("Weapon", &ESPConfig::ShowWeaponESP);
						ImGui::Checkbox("Name", &ESPConfig::ShowPlayerName);
						ImGui::Checkbox("Armourbar", &ESPConfig::ArmorBar);
						ImGui::Checkbox("Ammobar", &ESPConfig::AmmoBar);
						ImGui::Checkbox("Snapline", &ESPConfig::ShowLineToEnemy);
						ImGui::SameLine();
						ImGui::ColorEdit4("##Snaplinecolor", reinterpret_cast<float*>(&ESPConfig::LineToEnemyColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
						ImGui::Checkbox("Eyeline", &ESPConfig::ShowEyeRay);
						ImGui::SameLine();
						ImGui::ColorEdit4("##Eyelinecolor", reinterpret_cast<float*>(&ESPConfig::EyeRayColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

						ImGui::MultiCombo("", &ESPConfig::selectedFlags, items, 5);
					}
					ImGui::EndGroup();
				}
				ImGui::EndChild();
				ImGui::SameLine(0.f, 10.f);
				ImGui::BeginChild("Tab2", ImVec2(235, 382));
				{
					ImGui::SetCursorPos(ImVec2(10, 10));
					ImGui::BeginGroup(); {
						ImGui::Checkbox("Visible", &ESPConfig::VisibleCheck);
						ImGui::Separator();
						ImGui::Checkbox("Radar", &RadarCFG::ShowRadar);
						Gui.Slider("Proportion", ImGuiDataType_Float, &RadarCFG::Proportion, 500.f, 3300.f, "%.0f", MenuConfig::Accent, ImGuiSliderFlags_None);
						Gui.Slider("Range", ImGuiDataType_Float, &RadarCFG::RadarRange, 100.f, 300.f, "%.0f", MenuConfig::Accent, ImGuiSliderFlags_None);
						ImGui::Separator();
						ImGui::Checkbox("Nightmode", &MiscCFG::NightMode);
						Gui.Slider("alpha", ImGuiDataType_Float, &MiscCFG::NightModeAlpha, 1.0f, 150.f, "%.0f", MenuConfig::Accent, ImGuiSliderFlags_None);
						ImGui::Checkbox("Bombtimer", &MiscCFG::bmbTimer);

					}
					ImGui::EndGroup();
				}
				ImGui::EndChild();
				break;
			case 3:
				ImGui::SetCursorPos(ImVec2(180, 30));
				ImGui::BeginChild("Tab1", ImVec2(235, 382));
				{
					ImGui::SetCursorPos(ImVec2(10, 10));
					ImGui::BeginGroup(); {

						ImGui::SeparatorText("Movement");
						ImGui::Checkbox("Bunnyhop", &MiscCFG::BunnyHop);
						ImGui::Checkbox("Faststop", &MiscCFG::FastStop);


						ImGui::SeparatorText("Misc");
						ImGui::Checkbox("Hitsound", &MiscCFG::HitSound);
						ImGui::SameLine();
						ImGui::Combo("##hitsounds", &MiscCFG::hitsound_type, "Neverlose\0Skeet\0Bell\0Bubble\0Cod\0");
						ImGui::Checkbox("Playerlist", &MiscCFG::PlayerList);

					}
					ImGui::EndGroup();
				}
				ImGui::EndChild();
				ImGui::SameLine(0.f, 10.f);
				ImGui::BeginChild("Tab2", ImVec2(235, 382));
				{
					ImGui::SetCursorPos(ImVec2(10, 10));
					ImGui::BeginGroup(); {
						ImGui::Checkbox("Teamcheck", &MenuConfig::TeamCheck);
						ImGui::Checkbox("OBS-Bypass", &MenuConfig::BypassOBS);
					}
					ImGui::EndGroup();
				}
				ImGui::EndChild();
				break;
			case 4:
				ImGui::SetCursorPos(ImVec2(180, 30));
				ImGui::BeginChild("Tab1", ImVec2(235, 382));
				{
					ImGui::SetCursorPos(ImVec2(10, 10));
					ImGui::BeginGroup(); {
						ImGui::Text("Empty for now working on a \nnew config system");
					}
					ImGui::EndGroup();
				}
				ImGui::EndChild();
				ImGui::SameLine(0.f, 10.f);
				ImGui::BeginChild("Tab2", ImVec2(235, 382));
				{
					ImGui::SetCursorPos(ImVec2(10, 10));
					ImGui::BeginGroup(); {
					}
					ImGui::EndGroup();
				}
				ImGui::EndChild();
				break;
			}

		}
	}
}
