#pragma once
#include "..\Render.hpp"
#include "..\MenuConfig.hpp"
#include "..\Cheats.h"
#include "GetWeaponIcon.h"
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
		{"p2000", {20.0f, 20.0f, 0.0f, 0.0f}},
		{"p250", {20.0f, 20.0f, 0.0f, 0.0f}},
		{"usp", {20.0f, 20.0f, 0.0f, 0.0f}},
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



	void DrawHeadCircle(const CEntity& Entity, ImColor Color) {
		Vec2 CenterPos = Entity.GetBone().BonePosList[BONEINDEX::head].ScreenPos;
		float Radius = abs(Entity.GetBone().BonePosList[BONEINDEX::head].ScreenPos.y - Entity.GetBone().BonePosList[BONEINDEX::neck_0].ScreenPos.y) + 2;
		 Gui.Circle(CenterPos, Radius, Color, 1.2);
	}

	void LineToEnemy(ImVec4 Rect, ImColor Color, float Thickness) {
		const auto lineStart = ImVec2(Rect.x + Rect.z / 2, Rect.y + Rect.w);
		const auto lineEnd = ImVec2(Gui.Window.Size.x / 2, ESPConfig::LinePos == 0 ? 0 : (ESPConfig::LinePos == 1 ? Gui.Window.Size.y / 2 : Gui.Window.Size.y));

		Gui.Line(lineStart, lineEnd, Color, Thickness);
	}
	ImVec4 Get2DBox(const CEntity& Entity){
		BoneJointPos Head = Entity.GetBone().BonePosList[BONEINDEX::head];

		Vec2 Size, Pos;
		Size.y = (Entity.Pawn.ScreenPos.y - Head.ScreenPos.y) * 1.09;
		Size.x = Size.y * 0.6;

		Pos = ImVec2(Entity.Pawn.ScreenPos.x - Size.x / 2, Head.ScreenPos.y - Size.y * 0.08);

		return ImVec4{ Pos.x,Pos.y,Size.x,Size.y };
	}
	void ShowLosLine(const CEntity& Entity, const float Length, ImColor Color, float Thickness){
		if (!ESPConfig::ShowEyeRay)
			return;

		Vec2 StartPoint, EndPoint;
		Vec3 Temp;
		BoneJointPos Head = Entity.GetBone().BonePosList[BONEINDEX::head];

		StartPoint = Head.ScreenPos;

		float LineLength = cos(Entity.Pawn.ViewAngle.x * M_PI / 180) * Length;

		Temp.x = Head.Pos.x + cos(Entity.Pawn.ViewAngle.y * M_PI / 180) * LineLength;
		Temp.y = Head.Pos.y + sin(Entity.Pawn.ViewAngle.y * M_PI / 180) * LineLength;
		Temp.z = Head.Pos.z - sin(Entity.Pawn.ViewAngle.x * M_PI / 180) * Length;

		if (!gGame.View.WorldToScreen(Temp, EndPoint))
			return;

		Gui.Line(StartPoint, EndPoint, Color, Thickness);
	}
	void DrawBone(const CEntity& Entity, ImColor Color, float Thickness) {
		for (const auto& jointList : BoneJointList::List) {
			BoneJointPos prev, curr;
			for (auto index : jointList) {
				curr = Entity.GetBone().BonePosList[index];
				if (prev.Pos != Vec3() && prev.IsVisible && curr.IsVisible)
					Gui.Line(prev.ScreenPos, curr.ScreenPos, Color, Thickness);
				prev = curr;
			}
		}
	}

	class HealthBar {
	private:
		using TimePoint_ = std::chrono::steady_clock::time_point;
	private:
		const int ShowBackUpHealthDuration = 500;
		float MaxHealth = 0.f;
		float CurrentHealth = 0.f;
		float LastestBackupHealth = 0.f;
		ImVec2 RectPos{};
		ImVec2 RectSize{};
		bool InShowBackupHealth = false;
		TimePoint_ BackupHealthTimePoint{};
		int MaxAmmo = 0;
		int CurrentAmmo = 0;
		int MaxArmor = 0;
		int CurrentArmor = 0;
	public:
		HealthBar() {}

		void HealthBarV(float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size, const ImColor& HealthColorTop);
		void ArmorBarV(float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size, bool ShowNum, const ImColor& ArmourColor);

		void AmmoBarH(float MaxAmmo, float CurrentAmmo, ImVec2 Pos, ImVec2 Size, const ImColor& AmmoColor);

	private:
		ImColor Mix(ImColor Col_1, ImColor Col_2, float t);
		ImColor BackupHealthColor = ImColor(255, 255, 255, 220);
		ImColor BGC = ImColor(20, 20, 20, 220);
		ImColor ArmorColor = ImColor(0, 128, 255, 255);
		ImColor ArmorWithHelmetColor = ImColor(255, 0, 255, 255);
	};

	void HealthBar::HealthBarV(float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size, const ImColor& HealthColor)
	{
		ImDrawList* DrawList = ImGui::GetBackgroundDrawList();

		this->MaxHealth = MaxHealth;
		this->RectPos = Pos;
		this->RectSize = Size;

		float AnimationSpeed = 1.2f;
		float Proportion = CurrentHealth / MaxHealth;
		float Height = RectSize.y * Proportion;

		if (CurrentHealth > LastestBackupHealth) {
			LastestBackupHealth += AnimationSpeed;
			if (LastestBackupHealth > CurrentHealth)
				LastestBackupHealth = CurrentHealth;
		}
		else {
			LastestBackupHealth -= AnimationSpeed;
			if (LastestBackupHealth < CurrentHealth)
				LastestBackupHealth = CurrentHealth;
		}

		ImColor Color = Mix(HealthColor, ESPConfig::HealthbarMin, Proportion);

		DrawList->AddRectFilled(RectPos, { RectPos.x + RectSize.x, RectPos.y + RectSize.y }, BGC, 1);// background
		DrawList->AddRectFilled({ RectPos.x, RectPos.y + RectSize.y - LastestBackupHealth / MaxHealth * RectSize.y }, { RectPos.x + RectSize.x, RectPos.y + RectSize.y }, Color, 1, 0);// main bar
		DrawList->AddRect(RectPos, { RectPos.x + RectSize.x, RectPos.y + RectSize.y }, BGC, 0, 3); // frame

		ImVec2 textPos = { RectPos.x - (4), RectPos.y + RectSize.y - LastestBackupHealth / MaxHealth * RectSize.y };

		if (CurrentHealth < 100) {
			std::string displayText = std::to_string(static_cast<int>(CurrentHealth));
			Gui.StrokeText(displayText, textPos, ImColor(255, 255, 255, 255), 10, false);
		}
		
	}

	void HealthBar::ArmorBarV(float MaxArmor, float CurrentArmor, ImVec2 Pos, ImVec2 Size, bool ShowNum , const ImColor& ArmourColor)
	{
		auto InRange = [&](float value, float min, float max) -> bool
			{
				return value > min && value <= max;
			};

		ImDrawList* DrawList = ImGui::GetBackgroundDrawList();

		this->MaxArmor = MaxArmor;
		this->CurrentArmor = CurrentArmor;
		this->RectPos = Pos;
		this->RectSize = Size;

		float Proportion = CurrentArmor / MaxArmor;

		float Height = RectSize.y * Proportion;

		
		DrawList->AddRectFilled(RectPos,{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },BGC, 5, 15);// background
		DrawList->AddRectFilled({ RectPos.x,RectPos.y + RectSize.y - Height },{ RectPos.x + RectSize.x,RectPos.y + RectSize.y }, ArmourColor, 0);// main bar
		DrawList->AddRect(RectPos,{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },BGC, 0, 15, 1); // frame

		if (ShowNum)
		{
			if (CurrentArmor < MaxArmor)
			{
				std::string armor_str = Format("%.f", CurrentArmor);
				Vec2 Pos = { RectPos.x,RectPos.y + RectSize.y - Height };
				Gui.StrokeText(armor_str, Pos, ImColor(255, 255, 255), 13.f, true);
			}
		}
	}

	void HealthBar::AmmoBarH(float MaxAmmo, float CurrentAmmo, ImVec2 Pos, ImVec2 Size, const ImColor& AmmoColor){
		ImDrawList* DrawList = ImGui::GetBackgroundDrawList();

		this->MaxAmmo = MaxAmmo;
		this->CurrentAmmo = CurrentAmmo;
		this->RectPos = Pos;
		this->RectSize = Size;

		float Proportion = CurrentAmmo / MaxAmmo;

		float Width = RectSize.x * Proportion;

		DrawList->AddRectFilled(RectPos,{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },BGC, 5, 15); // background
		DrawList->AddRectFilled(RectPos,{ RectPos.x + Width,RectPos.y + RectSize.y },AmmoColor, 0); // main bar
		DrawList->AddRect(RectPos,{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },BGC, 0, 15, 1); // frame
	}

	ImColor HealthBar::Mix(ImColor Col_1, ImColor Col_2, float t)
	{
		ImColor Col;
		Col.Value.x = t * Col_1.Value.x + (1 - t) * Col_2.Value.x;
		Col.Value.y = t * Col_1.Value.y + (1 - t) * Col_2.Value.y;
		Col.Value.z = t * Col_1.Value.z + (1 - t) * Col_2.Value.z;
		Col.Value.w = Col_1.Value.w;
		return Col;
	}

	void DrawHealthBar(DWORD Sign, float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size, const ImColor& HealthColor)
	{
		static std::map<DWORD, HealthBar> HealthBarMap;
		if (!HealthBarMap.count(Sign))
			HealthBarMap.insert({ Sign,HealthBar() });

		if (HealthBarMap.count(Sign))
			HealthBarMap[Sign].HealthBarV(MaxHealth, CurrentHealth, Pos, Size, HealthColor);
	}

	void DrawAmmoBar(DWORD Sign, float MaxAmmo, float CurrentAmmo, ImVec2 Pos, ImVec2 Size, const ImColor& AmmoColor)
	{
		static std::map<DWORD, HealthBar> HealthBarMap;
		if (!HealthBarMap.count(Sign))
			HealthBarMap.insert({ Sign,HealthBar() });

		if (HealthBarMap.count(Sign))
			HealthBarMap[Sign].AmmoBarH(MaxAmmo, CurrentAmmo, Pos, Size, AmmoColor);
	}

	void DrawArmorBar(DWORD Sign, float MaxArmor, float CurrentArmor, ImVec2 Pos, ImVec2 Size ,const ImColor& ArmourColor)
	{
		static std::map<DWORD, HealthBar> HealthBarMap;
		if (!HealthBarMap.count(Sign))
			HealthBarMap.insert({ Sign,HealthBar() });

		if (HealthBarMap.count(Sign))
			HealthBarMap[Sign].ArmorBarV(MaxArmor, CurrentArmor, Pos, Size, ESPConfig::ShowArmorNum, ArmourColor);
	}



	void RenderPlayerESP(const CEntity& LocalEntity, const CEntity& Entity, ImVec4 Rect, int LocalPlayerControllerIndex, int Index) {	
		std::string weaponIcon = GunIcon(Entity.Pawn.WeaponName);
		int distance = static_cast<int>(Entity.Pawn.Pos.DistanceTo(LocalEntity.Pawn.Pos) / 100);
		DWORD64 spottedMask = Entity.Pawn.bSpottedByMask;
		bool isSpotted = (spottedMask & (DWORD64(1) << LocalPlayerControllerIndex)) || (LocalEntity.Pawn.bSpottedByMask & (DWORD64(1) << Index));

		if (ESPConfig::ShowBoneESP) DrawBone(Entity, ESPConfig::BoneColor, 1.3f);
		ShowLosLine(Entity, 50.0f, ESPConfig::EyeRayColor, 1.3f);
		if (ESPConfig::ShowHeadBox) DrawHeadCircle(Entity, ESPConfig::HeadBoxColor);


		auto FillCol = isSpotted && ESPConfig::VisibleCheck ? ESPConfig::FilledVisColor : ESPConfig::FilledColor;
		ImU32 colorTop = IM_COL32(ImColor(FillCol).Value.x * 255, ImColor(FillCol).Value.y * 255, ImColor(FillCol).Value.z * 255, 0);
		ImU32 colorBottom = IM_COL32(ImColor(FillCol).Value.x * 255, ImColor(FillCol).Value.y * 255, ImColor(FillCol).Value.z * 255, 130);

		if (ESPConfig::FilledBox) {
			if (ESPConfig::BoxFillType == 0)
				Gui.RectangleFilled({ Rect.x, Rect.y }, { Rect.z, Rect.w }, FillCol);
			if (ESPConfig::BoxFillType == 1)
				ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor({ Rect.x, Rect.y }, { Rect.x + Rect.z, Rect.y + Rect.w }, colorTop, colorTop, colorBottom, colorBottom);
		}


		// box

		if (ESPConfig::ShowBoxESP) {
			auto Boxcolor = isSpotted && ESPConfig::VisibleCheck ? ESPConfig::VisibleColor : ESPConfig::BoxColor;
			if (MenuConfig::BoxType == 0 || MenuConfig::BoxType == 1) {
				if (ESPConfig::OutLine)
					Gui.Rectangle({ Rect.x,Rect.y }, { Rect.z,Rect.w }, ESPConfig::BoxColor & IM_COL32_A_MASK, 3, 0.f);

				Gui.Rectangle({ Rect.x, Rect.y }, { Rect.z, Rect.w }, Boxcolor, 1.3, 0.f);
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
		}

		if (ESPConfig::ShowLineToEnemy) LineToEnemy(Rect, ESPConfig::LineToEnemyColor, 1.2);

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
		if (ESPConfig::selectedFlags & (1 << 4)) flags += "$" + std::to_string(Entity.Controller.Money) + "\n";
		if (ESPConfig::selectedFlags & (1 << 5) && Entity.Pawn.FlashDuration > 30) flags += "Flashed\n";
		//if (Entity.Controller.HasHelmet) flags += "HK\n"; // shit dont work
		//if (Entity.Pawn.HasHeavy) flags += "HV\n"; // shit dont work
		//flags += "(" + std::to_string(Entity.Pawn.Pos.x) + ", " + std::to_string(Entity.Pawn.Pos.y) + ", " + std::to_string(Entity.Pawn.Pos.z) + ")" + "\n"; // this is for testing purposes 
		ImVec2 flagpos (4,0);
		if (ESPConfig::ArmorBar)
			flagpos.x += 4;
		Gui.StrokeText(flags.c_str(), { Rect.x + Rect.z + flagpos.x, Rect.y }, ImColor(255, 255, 255, 255), 12, false);

		if (ESPConfig::ShowPlayerName) {
			float offsetY = (MenuConfig::HealthBarType == 0) ? 0 : -14;
			Gui.StrokeText(Entity.Controller.PlayerName, { Rect.x + Rect.z / 2, Rect.y - 13 + offsetY }, ImColor(255, 255, 255, 255), 14, true);
		}
	}

}