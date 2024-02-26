#pragma once
#include "..\Render.hpp"
#include "..\MenuConfig.hpp"
#include "..\Cheats.h"
#include "GetWeaponIcon.h"
#include "..\Resources\8964.h"
#define ICON_FA_EYE "\xef\x81\xae"
#include "../OS-ImGui/OS-ImGui.h"

ID3D11ShaderResourceView* winniethepooh_srv = NULL;
int winnie_h = 0, winnie_w = 0;

namespace ESP {
	struct WeaponIconSize {
		float width;
		float height;
		float offsetX;
		float offsetY;
	};
	std::unordered_map<std::string, WeaponIconSize> weaponIconSizes = {
		{"t_knife", {20.0f, 20.0f, -8.0f, 0.0f}},
		{"ct_knife", {20.0f, 20.0f, -8.0f, 0.0f}},
		{"deagle", {20.0f, 20.0f, -8.0f, 0.0f}},
		{"elite", {20.0f, 20.0f, 0.0f, 0.0f}},
		{"fiveseven", {20.0f, 20.0f, 0.0f, 0.0f}},
		{"glock", {20.0f, 20.0f, 0.0f, 0.0f}},
		{"revolver", {20.0f, 20.0f, -5.0f, 0.0f}},
		{"hkp2000", {20.0f, 20.0f, 0.0f, 0.0f}},
		{"p250", {20.0f, 20.0f, 0.0f, 0.0f}},
		{"usp_silencer", {20.0f, 20.0f, 0.0f, 0.0f}},
		{"tec9", {20.0f, 20.0f, 0.0f, 0.0f}},
		{"cz75a", {20.0f, 20.0f, 0.0f, 0.0f}},
		{"mac10", {20.0f, 20.0f, 0.0f, 0.0f}},
		{"ump45", {20.0f, 20.0f, -10.0f, 0.0f}},
		{"bizon", {20.0f, 20.0f, -10.0f, 0.0f}},
		{"mp7", {20.0f, 20.0f, -5.0f, 0.0f}},
		{"mp9", {20.0f, 20.0f, -10.0f, 0.0f}},
		{"p90", {20.0f, 20.0f, -10.0f, 0.0f}},
		{"galilar", {20.0f, 20.0f, -15.0f, 0.0f}},
		{"famas", {20.0f, 20.0f, -15.0f, 0.0f}},
		{"m4a1_silencer", {20.0f, 20.0f, -15.0f, 0.0f}},
		{"m4a1", {20.0f, 20.0f, -15.0f, 0.0f}},
		{"aug", {20.0f, 20.0f, -10.0f, 0.0f}},
		{"sg556", {20.0f, 20.0f, -15.0f, 0.0f}},
		{"ak47", {20.0f, 20.0f, -15.0f, 0.0f}},
		{"g3sg1", {20.0f, 20.0f, -15.0f, 0.0f}},
		{"scar20", {20.0f, 20.0f, -15.0f, 0.0f}},
		{"awp", {20.0f, 20.0f, -15.0f, 0.0f}},
		{"ssg08", {20.0f, 20.0f, -15.0f, 0.0f}},
		{"xm1014", {20.0f, 20.0f, -15.0f, 0.0f}},
		{"sawedoff", {20.0f, 20.0f, -15.0f, 0.0f}},
		{"mag7", {20.0f, 20.0f, -15.0f, 0.0f}},
		{"nova", {20.0f, 20.0f, -15.0f, 0.0f}},
		{"negev", {20.0f, 20.0f, -15.0f, 0.0f}},
		{"m249", {20.0f, 20.0f, -15.0f, 0.0f}},
		{"taser", {20.0f, 20.0f, 0.0f, 0.0f}},
		{"flashbang", {20.0f, 20.0f, 5.0f, 0.0f}},
		{"hegrenade", {20.0f, 20.0f, 5.0f, 0.0f}},
		{"smokegrenade", {20.0f, 20.0f, 5.0f, 0.0f}},
		{"molotov", {20.0f, 20.0f, 5.0f, 0.0f}},
		{"decoy", {20.0f, 20.0f, 5.0f, 0.0f}},
		{"incgrenade", {20.0f, 20.0f, 5.0f, 0.0f}},
		{"c4", {20.0f, 20.0f, 0.0f, 0.0f}},
	};

	ImVec4 GetBoxRect(const CEntity& Entity, int BoxType) {
		ImVec4 Rect;
		switch (BoxType) {
		case 0:
		case 2:
		case 3:
			Rect = Render::Get2DBox(Entity);
			break;
		case 1:
		case 4:
			Rect = Render::Get2DBoneRect(Entity);
			break;
		}
		return Rect;
	}


	const char* RenderWeaponIcon(const CEntity& Entity) {
		uintptr_t ClippingWeapon, WeaponData, WeaponNameAddress;
		ProcessMgr.ReadMemory(Entity.Pawn.Address + Offset::Pawn.pClippingWeapon, ClippingWeapon);
		ProcessMgr.ReadMemory(ClippingWeapon + Offset::WeaponBaseData.WeaponDataPTR, WeaponData);
		ProcessMgr.ReadMemory(WeaponData + Offset::WeaponBaseData.szName, WeaponNameAddress);
		std::string weaponName = "Invalid Weapon Name";

		if (!WeaponNameAddress) {
			weaponName = "NULL";
		}
		else {
			weaponName = Entity.Pawn.WeaponName;
		}
		std::string weaponIcon = GunIcon(weaponName);
		return weaponIcon.c_str();
	}



	void RenderPlayerESP(const CEntity& LocalEntity, const CEntity& Entity, ImVec4 Rect, int LocalPlayerControllerIndex, int Index) {
		std::string weaponIcon = GunIcon(Entity.Pawn.WeaponName);
		int distance = static_cast<int>(Entity.Pawn.Pos.DistanceTo(LocalEntity.Pawn.Pos) / 100);
		DWORD64 spottedMask = Entity.Pawn.bSpottedByMask;
		bool isSpotted = (spottedMask & (DWORD64(1) << LocalPlayerControllerIndex)) || (LocalEntity.Pawn.bSpottedByMask & (DWORD64(1) << Index));

		Render::DrawBone(Entity, ESPConfig::BoneColor, 1.3f);
		Render::ShowPenis(Entity, ESPConfig::PenisLength, ESPConfig::PenisColor, ESPConfig::PenisSize);
		Render::ShowLosLine(Entity, 50.0f, ESPConfig::EyeRayColor, 1.3f);
		Render::DrawHeadCircle(Entity, ESPConfig::HeadBoxColor);

		// box
		if (ESPConfig::FilledBox) {
			float Rounding = ESPConfig::BoxRounding;
			ImColor FlatBoxCol = ESPConfig::FilledColor;
			ImColor FlatBoxCol2 = ESPConfig::FilledColor2;
			ImColor FlatBoxVisCol = ESPConfig::BoxFilledVisColor;

			if (MenuConfig::BoxType == 2 || MenuConfig::BoxType == 3)
				Rounding = 0.f;

			if (ESPConfig::FilledVisBox) {
				Gui.RectangleFilled({ Rect.x, Rect.y }, { Rect.z, Rect.w }, isSpotted ? FlatBoxVisCol : FlatBoxCol, Rounding);
			}
			else {
				if (ESPConfig::MultiColor) {
					Gui.RectangleFilledGraident({ Rect.x, Rect.y }, { Rect.z, Rect.w }, ESPConfig::BoxColor, FlatBoxCol, FlatBoxCol2, Rounding);
				}
				else {
					Gui.RectangleFilled({ Rect.x, Rect.y }, { Rect.z, Rect.w }, FlatBoxCol, Rounding);
				}
			}
		}
		if (ESPConfig::ShowBoxESP) {
			auto Boxcolor = isSpotted && ESPConfig::VisibleCheck ? ESPConfig::VisibleColor : ESPConfig::BoxColor;
			if (MenuConfig::BoxType == 0 || MenuConfig::BoxType == 1) {
				if (ESPConfig::OutLine)
					Gui.Rectangle({ Rect.x,Rect.y }, { Rect.z,Rect.w }, ESPConfig::BoxColor & IM_COL32_A_MASK, 3, ESPConfig::BoxRounding);

				Gui.Rectangle({ Rect.x, Rect.y }, { Rect.z, Rect.w }, Boxcolor, 1.3, ESPConfig::BoxRounding);
			}

			else if (MenuConfig::BoxType == 2 || MenuConfig::BoxType == 3) {
				//Outline
				Gui.Line({ Rect.x, Rect.y }, { Rect.x + Rect.z * 0.25f, Rect.y }, ESPConfig::BoxColor & IM_COL32_A_MASK, 3);
				Gui.Line({ Rect.x, Rect.y }, { Rect.x, Rect.y + Rect.w * 0.25f }, ESPConfig::BoxColor & IM_COL32_A_MASK, 3);
				Gui.Line({ Rect.x + Rect.z, Rect.y + Rect.w }, { Rect.x + Rect.z - Rect.z * 0.25f, Rect.y + Rect.w }, ESPConfig::BoxColor & IM_COL32_A_MASK, 3);
				Gui.Line({ Rect.x + Rect.z, Rect.y + Rect.w }, { Rect.x + Rect.z, Rect.y + Rect.w - Rect.w * 0.25f }, ESPConfig::BoxColor & IM_COL32_A_MASK, 3);
				Gui.Line({ Rect.x, Rect.y + Rect.w }, { Rect.x + Rect.z * 0.25f, Rect.y + Rect.w }, ESPConfig::BoxColor & IM_COL32_A_MASK, 3);
				Gui.Line({ Rect.x, Rect.y + Rect.w }, { Rect.x, Rect.y + Rect.w - Rect.w * 0.25f }, ESPConfig::BoxColor & IM_COL32_A_MASK, 3);
				Gui.Line({ Rect.x + Rect.z, Rect.y }, { Rect.x + Rect.z - Rect.z * 0.25f, Rect.y }, ESPConfig::BoxColor & IM_COL32_A_MASK, 3);
				Gui.Line({ Rect.x + Rect.z, Rect.y }, { Rect.x + Rect.z, Rect.y + Rect.w * 0.25f }, ESPConfig::BoxColor & IM_COL32_A_MASK, 3);

				// Main Box Lines
				Gui.Line({ Rect.x, Rect.y }, { Rect.x + Rect.z * 0.25f, Rect.y }, Boxcolor, 1.3f);
				Gui.Line({ Rect.x, Rect.y }, { Rect.x, Rect.y + Rect.w * 0.25f }, Boxcolor, 1.3f);
				Gui.Line({ Rect.x + Rect.z, Rect.y }, { Rect.x + Rect.z - Rect.z * 0.25f, Rect.y }, Boxcolor, 1.3f);
				Gui.Line({ Rect.x + Rect.z, Rect.y }, { Rect.x + Rect.z, Rect.y + Rect.w * 0.25f }, Boxcolor, 1.3f);
				Gui.Line({ Rect.x, Rect.y + Rect.w }, { Rect.x + Rect.z * 0.25f, Rect.y + Rect.w }, Boxcolor, 1.3f);
				Gui.Line({ Rect.x, Rect.y + Rect.w }, { Rect.x, Rect.y + Rect.w - Rect.w * 0.25f }, Boxcolor, 1.3f);
				Gui.Line({ Rect.x + Rect.z, Rect.y + Rect.w }, { Rect.x + Rect.z - Rect.z * 0.25f, Rect.y + Rect.w }, Boxcolor, 1.3f);
				Gui.Line({ Rect.x + Rect.z, Rect.y + Rect.w }, { Rect.x + Rect.z, Rect.y + Rect.w - Rect.w * 0.25f }, Boxcolor, 1.3f);
			}
			if (ESPConfig::winniethepool ) {
				if (winniethepooh_srv == nullptr) {
					Gui.LoadTextureFromMemory(winniethepooh_image, sizeof winniethepooh_image, &winniethepooh_srv, &winnie_h, &winnie_w);
				}
				ImGui::GetBackgroundDrawList()->AddImage(winniethepooh_srv, ImVec2(Rect.x, Rect.y), { ImVec2(Rect.x,Rect.y).x + ImVec2(Rect.z,Rect.w).x,ImVec2(Rect.x,Rect.y).y + ImVec2(Rect.z,Rect.w).y });
			}
		}

		if (ESPConfig::ShowLineToEnemy) Render::LineToEnemy(Rect, ESPConfig::LineToEnemyColor, 1.2);

		if (ESPConfig::ShowWeaponESP) {
			WeaponIconSize iconSize = weaponIconSizes[Entity.Pawn.WeaponName];
			ImVec2 textPosition = { Rect.x + (Rect.z - iconSize.width) / 2 + iconSize.offsetX, Rect.y + Rect.w + 3 + iconSize.offsetY };
			if (ESPConfig::AmmoBar)
				textPosition.y += 5;

			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					ImGui::GetBackgroundDrawList()->AddText(ImGui::GetIO().Fonts->Fonts[1], 15.0f, ImVec2{ textPosition.x + i, textPosition.y + j }, ImColor(0, 0, 0, 255), weaponIcon.c_str());
				}
			}
			ImGui::GetBackgroundDrawList()->AddText(ImGui::GetIO().Fonts->Fonts[1], 15.0f, textPosition, ImColor(255, 255, 255, 255), weaponIcon.c_str());
		}


		std::string flags;
		std::string dis_str = std::to_string(distance) + "m";

		// std::string ping = std::to_string(Entity.Controller.Ping) + "ms"; // dont work
		if (ESPConfig::selectedFlags & (1 << 0))flags += dis_str + "\n";
		if ((ESPConfig::selectedFlags & (1 << 1)) && (Entity.Pawn.Health < 30))flags += "Lethal\n";
		// if (ESPConfig::selectedFlags & (1 << 2))flags += ((ESPConfig::selectedFlags & (1 << 2)) && (Entity.Controller.Ping != 0)) ? std::to_string(Entity.Controller.Ping) + "ms\n" : "Bot\n"; // dont work need to fix it
		if ((ESPConfig::selectedFlags & (1 << 2)) && Entity.Pawn.Scoped) flags += "Scoped\n";
		if ((ESPConfig::selectedFlags & (1 << 3)) && Entity.Pawn.Defusing) flags += "Defusing\n";
		if (ESPConfig::selectedFlags & (1 << 4)) flags += "$" + std::to_string(Entity.Controller.Money);
		if (ESPConfig::selectedFlags & (1 << 5) && Entity.Pawn.FlashDuration > 30) flags += "Flashed\n";

		//if (Entity.Controller.HasHelmet) flags += "HK\n"; // shit dont work
		//if (Entity.Pawn.HasHeavy) flags += "HV\n"; // shit dont work

		//if (Entity.Pawn.IsWalking) flags += "Walking\n"; // this shit is also kinda broke ima comment it out 
		//flags += "(" + std::to_string(Entity.Pawn.Pos.x) + ", " + std::to_string(Entity.Pawn.Pos.y) + ", " + std::to_string(Entity.Pawn.Pos.z) + ")" + "\n"; // this is for testing purposes 


		Gui.StrokeText(flags.c_str(), { Rect.x + Rect.z + 4, Rect.y }, ImColor(255, 255, 255, 255), 12, false);

		if (ESPConfig::ShowPlayerName) {
			float offsetY = (MenuConfig::HealthBarType == 0) ? 0 : -14;
			Gui.StrokeText(Entity.Controller.PlayerName, { Rect.x + Rect.z / 2, Rect.y - 13 + offsetY }, ImColor(255, 255, 255, 255), 14, true);
		}
	}

	void DrawPreviewBox(const ImVec2& startPos, const ImVec2& endPos, ImColor boxColor, float rounding, float thickness, bool filled) {
		if (filled) {
			ImGui::GetWindowDrawList()->AddRectFilled(startPos, endPos, boxColor, rounding, ImDrawCornerFlags_All);
		}
		else {
			ImGui::GetWindowDrawList()->AddRect(startPos, endPos, boxColor, rounding, ImDrawCornerFlags_All, thickness);
		}
	}

	void RenderPreview(ImVec2 windowSize)
	{
		if (!ESPConfig::ShowPreview)
			return;

		ImVec2 rectSize(100, 150);
		ImVec2 rectPos((windowSize.x - rectSize.x) * 0.45f, (windowSize.y - rectSize.y) * 0.3f);
		ImVec2 centerPos = ImGui::GetCursorScreenPos();
		centerPos.x += rectPos.x;
		centerPos.y += rectPos.y * -1.20f;

		if (ESPConfig::ShowEyeRay) {
			ImU32 EyeC = ESPConfig::EyeRayColor;
			ImVec2 lineStart(centerPos.x + 44, centerPos.y + 15);
			ImVec2 lineEnd(centerPos.x - 10, centerPos.y + 20);
			ImGui::GetWindowDrawList()->AddLine(lineStart, lineEnd, EyeC, 2.0f);
		}
		if (ESPConfig::ShowBoneESP) {
			ImU32 boneColor = ESPConfig::BoneColor;
			ImVec2 bones[][2] = {
				{{centerPos.x + 50, centerPos.y + 25}, {centerPos.x + 60, centerPos.y + 55}}, // Neck to Spine
				{{centerPos.x + 60, centerPos.y + 55}, {centerPos.x + 62, centerPos.y + 65}}, // Spine to Pelvis
				{{centerPos.x + 62, centerPos.y + 65}, {centerPos.x + 65, centerPos.y + 70}}, // Left Leg_Up
				{{centerPos.x + 65, centerPos.y + 70}, {centerPos.x + 60, centerPos.y + 100}}, // Left Leg_Mid
				{{centerPos.x + 60, centerPos.y + 100}, {centerPos.x + 68, centerPos.y + 145}}, // Left Leg_Down
				{{centerPos.x + 62, centerPos.y + 65}, {centerPos.x + 35, centerPos.y + 100}}, // Right Leg_Up
				{{centerPos.x + 35, centerPos.y + 100}, {centerPos.x + 47, centerPos.y + 130}}, // Right Leg_Down
				{{centerPos.x + 50, centerPos.y + 25}, {centerPos.x + 60, centerPos.y + 30}}, // Left Scapula
				{{centerPos.x + 60, centerPos.y + 30}, {centerPos.x + 45, centerPos.y + 55}}, // Left Arm_Up
				{{centerPos.x + 45, centerPos.y + 55}, {centerPos.x + 25, centerPos.y + 45}}, // Left Arm_Down
				{{centerPos.x + 50, centerPos.y + 25}, {centerPos.x + 40, centerPos.y + 30}}, // Right Scapula
				{{centerPos.x + 40, centerPos.y + 30}, {centerPos.x + 27, centerPos.y + 53}}, // Right Arm_Up
				{{centerPos.x + 27, centerPos.y + 53}, {centerPos.x + 20, centerPos.y + 45}}  // Right Arm_Down
			};

			for (const auto& bone : bones) {
				ImGui::GetWindowDrawList()->AddLine(bone[0], bone[1], boneColor, 1.8f);
			}
		}

		if (ESPConfig::ShowHeadBox) {
			switch (ESPConfig::HeadBoxStyle) {
			case 0:
				ImGui::GetWindowDrawList()->AddCircle({ centerPos.x + 44, centerPos.y + 17 }, 12.0f, ESPConfig::HeadBoxColor, 0, 1.8f);
				break;
			case 1:
				ImGui::GetWindowDrawList()->AddCircleFilled({ centerPos.x + 44, centerPos.y + 17 }, 12.0f, ESPConfig::HeadBoxColor, 0);
			default:
				break;
			}
		}

		if (ESPConfig::FilledBox) {
			ImVec2 rectStartPos;
			ImVec2 rectEndPos;
			ImColor filledBoxColor = { ESPConfig::FilledColor.Value.x, ESPConfig::FilledColor.Value.y, ESPConfig::FilledColor.Value.z, ESPConfig::FilledColor.Value.w };
			ImColor filledBoxColor2 = { ESPConfig::FilledColor2.Value.x, ESPConfig::FilledColor2.Value.y, ESPConfig::FilledColor2.Value.z, ESPConfig::FilledColor2.Value.w };

			rectStartPos = centerPos;
			rectEndPos = { rectStartPos.x + rectSize.x, rectStartPos.y + rectSize.y };

			if (MenuConfig::BoxType == 0 || MenuConfig::BoxType == 2)
			{
				if (ESPConfig::MultiColor)
					ImGui::GetWindowDrawList()->AddRectFilledMultiColorRounded(rectStartPos, rectEndPos, ImGui::GetColorU32(ImGuiCol_ChildBg), filledBoxColor, filledBoxColor, filledBoxColor2, filledBoxColor2, ESPConfig::BoxRounding, ImDrawCornerFlags_All);
			}
			else if (MenuConfig::BoxType == 1)
			{
				rectStartPos = { centerPos.x + 20, centerPos.y + 15 };
				rectEndPos = { rectStartPos.x + 50, rectStartPos.y + 132 };
				if (ESPConfig::MultiColor)
					ImGui::GetWindowDrawList()->AddRectFilledMultiColorRounded(rectStartPos, rectEndPos, ImGui::GetColorU32(ImGuiCol_ChildBg), filledBoxColor, filledBoxColor, filledBoxColor2, filledBoxColor2, ESPConfig::BoxRounding, ImDrawCornerFlags_All);
			}
		}

		if (ESPConfig::ShowBoxESP) {
			ImVec2 rectStartPos;
			ImVec2 rectEndPos;
			ImColor boxColor = ESPConfig::BoxColor;
			rectStartPos = centerPos;
			rectEndPos = { rectStartPos.x + rectSize.x, rectStartPos.y + rectSize.y };

			switch (MenuConfig::BoxType) {
			case 0:
				DrawPreviewBox(rectStartPos, rectEndPos, boxColor, ESPConfig::BoxRounding, 1.3f, false);
				break;
			case 1:
				rectStartPos = { centerPos.x + 20, centerPos.y + 15 };
				rectEndPos = { rectStartPos.x + 50, rectStartPos.y + 132 };
				DrawPreviewBox(rectStartPos, rectEndPos, boxColor, ESPConfig::BoxRounding, 1.0f, false);
				break;
			case 2:
			case 3: {
				float width = rectSize.x * 0.25f;
				float height = rectSize.y * 0.25f;
				ImGui::GetWindowDrawList()->AddLine(rectStartPos, { rectStartPos.x + width, rectStartPos.y }, boxColor, 1.3f);
				ImGui::GetWindowDrawList()->AddLine(rectStartPos, { rectStartPos.x, rectStartPos.y + height }, boxColor, 1.3f);
				ImGui::GetWindowDrawList()->AddLine({ rectStartPos.x + rectSize.x, rectStartPos.y + rectSize.y }, { rectStartPos.x + rectSize.x - width, rectStartPos.y + rectSize.y }, boxColor, 1.3f);
				ImGui::GetWindowDrawList()->AddLine({ rectStartPos.x + rectSize.x, rectStartPos.y + rectSize.y }, { rectStartPos.x + rectSize.x, rectStartPos.y + rectSize.y - height }, boxColor, 1.3f);
				ImGui::GetWindowDrawList()->AddLine({ rectStartPos.x, rectStartPos.y + rectSize.y }, { rectStartPos.x + width, rectStartPos.y + rectSize.y }, boxColor, 1.3f);
				ImGui::GetWindowDrawList()->AddLine({ rectStartPos.x, rectStartPos.y + rectSize.y }, { rectStartPos.x, rectStartPos.y + rectSize.y - height }, boxColor, 1.3f);
				ImGui::GetWindowDrawList()->AddLine({ rectStartPos.x + rectSize.x, rectStartPos.y }, { rectStartPos.x + rectSize.x - width, rectStartPos.y }, boxColor, 1.3f);
				ImGui::GetWindowDrawList()->AddLine({ rectStartPos.x + rectSize.x, rectStartPos.y }, { rectStartPos.x + rectSize.x, rectStartPos.y + height }, boxColor, 1.3f);
			}
				  break;
			}
		}
		if (ESPConfig::ShowHealthBar) {
			ImU32 greenColor = IM_COL32(0, 255, 0, 255);
			ImVec2 HBPos = centerPos;
			ImVec2 HBSize = rectSize;
			if (MenuConfig::BoxType == 1 || MenuConfig::BoxType == 3) {
				HBPos = { centerPos.x + 20, centerPos.y + 15 };
				HBSize = { rectSize.x - 2, rectSize.y - 18 };
			}
			if (MenuConfig::HealthBarType == 0) {
				ImVec2 HBS(HBPos.x - 6, HBPos.y);
				ImVec2 HBE(HBPos.x - 3, HBPos.y + HBSize.y);
				ImGui::GetWindowDrawList()->AddRectFilled(HBS, HBE, greenColor, 0.0f, ImDrawCornerFlags_All);
			}
		}
		if (ESPConfig::ArmorBar) {
			ImU32 blue = IM_COL32(0, 128, 255, 255);
			ImVec2 pos = centerPos, size = rectSize;
			if (MenuConfig::BoxType == 1 || MenuConfig::BoxType == 3) {
				pos = { centerPos.x + 20, centerPos.y + 15 };
				size = { rectSize.x - 2, rectSize.y - 18 };
			}
			if (ESPConfig::ShowHealthBar) {
				pos.x -= 4;
				size.x -= 4;
			}
			ImVec2 start(pos.x - 6, pos.y), end(pos.x - 3, pos.y + size.y);
			ImGui::GetWindowDrawList()->AddRectFilled(start, end, blue, 0.0f, ImDrawCornerFlags_All);
		}

		if (ESPConfig::AmmoBar) {

			ImVec2 ABS = ImVec2(centerPos.x, centerPos.y + rectSize.y + (MenuConfig::BoxType == 0 || MenuConfig::BoxType == 2 ? 2 : 0));
			ImVec2 ABE = ImVec2(centerPos.x + rectSize.x - (MenuConfig::BoxType == 0 || MenuConfig::BoxType == 2 ? 0 : 30), centerPos.y + rectSize.y + (MenuConfig::BoxType == 0 || MenuConfig::BoxType == 2 ? 5 : 3));
			ImGui::GetWindowDrawList()->AddRectFilled(ABS, ABE, IM_COL32(255, 255, 0, 255), 0.0f, ImDrawCornerFlags_All);
		}




		if (ESPConfig::ShowLineToEnemy) {
			ImVec2 LineStart = { centerPos.x + rectSize.x / 2 , centerPos.y };
			ImVec2 LineEnd = { LineStart.x, LineStart.y - 50 * (ESPConfig::LinePos == 0) + (Gui.Window.Size.x / 2) * (ESPConfig::LinePos == 1) + LineStart.y + 200 * (ESPConfig::LinePos == 2) };

			ImGui::GetWindowDrawList()->AddLine(LineStart, LineEnd, ESPConfig::LineToEnemyColor, 1.8f);
		}

		if (ESPConfig::ShowPlayerName) {
			if (MenuConfig::BoxType == 1 || MenuConfig::BoxType == 3) {
				centerPos.x -= 3;
				centerPos.y += 15;
			}
			ImVec2 textPos(centerPos.x + 28, centerPos.y - (MenuConfig::HealthBarType == 1 ? 22 : 18));
			ImGui::GetWindowDrawList()->AddText(textPos, IM_COL32(255, 255, 255, 255), "Player");
		}


		if (ESPConfig::ShowWeaponESP) {
			ImVec2 textPos(0, 0);
			if (ESPConfig::AmmoBar) centerPos.y += 5;
			if (MenuConfig::BoxType == 1 || MenuConfig::BoxType == 3) centerPos.y -= 17;

			textPos = { centerPos.x + 26, centerPos.y + (MenuConfig::HealthBarType == 2 ? 155 : 150) };
			ImGui::GetWindowDrawList()->AddText(ImGui::GetIO().Fonts->Fonts[1], 20.f, textPos, IM_COL32(255, 255, 255, 255), "W");
		}
	}
}