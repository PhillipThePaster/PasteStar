#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <chrono>
#include <map>
#include <Windows.h>
#include "Entity.h"
#include "Utils/Format.h"
#include "OS-ImGui/imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "OS-ImGui/imgui/imgui_internal.h"
#include "TriggerBot.h"

namespace Render
{


	void DrawFovCircle(const CEntity& LocalEntity) {
		Vec2 CenterPoint = Gui.Window.Size / 2;
		float Radius = tan(AimControl::AimFov / 180.f * M_PI / 2.f) * Gui.Window.Size.x;
		Gui.Circle(CenterPoint, Radius, ImColor(0, 0, 0, static_cast<int>(MenuConfig::FovCircleColor.Value.w * 255)), 4);
		Gui.Circle(CenterPoint, Radius, MenuConfig::FovCircleColor, 2);
	}

	void DrawCrossHair(ImDrawList* drawList, const ImVec2& pos, ImU32 color) noexcept {
		float Thickness = CrosshairsCFG::Thickness;
		int BorderWidth = 2, DotSize = 1, gap = CrosshairsCFG::Gap / 2, outlineGap = gap - 1;

		ImVec2 offset1{ CrosshairsCFG::DotSize,CrosshairsCFG::DotSize };
		ImVec2 offset2{ CrosshairsCFG::DotSize + 1,CrosshairsCFG::DotSize + 1 };

		if (CrosshairsCFG::drawOutLine) {
			if (CrosshairsCFG::drawDot) drawList->AddRectFilled(ImVec2(pos.x - offset1.x, pos.y - offset1.y), ImVec2(pos.x + offset2.x, pos.y + offset2.y), color & IM_COL32_A_MASK);//dot
			if (CrosshairsCFG::drawOutLine) {
				drawList->AddRectFilled(ImVec2(pos.x - (outlineGap + BorderWidth + CrosshairsCFG::HorizontalLength), pos.y - Thickness), ImVec2(pos.x - outlineGap, pos.y + 1 + Thickness), color & IM_COL32_A_MASK);//left
				drawList->AddRectFilled(ImVec2(pos.x + (outlineGap + DotSize), pos.y - Thickness), ImVec2(pos.x + (outlineGap + DotSize + BorderWidth + CrosshairsCFG::HorizontalLength), pos.y + 1 + Thickness), color & IM_COL32_A_MASK);//right
				if (!CrosshairsCFG::tStyle) drawList->AddRectFilled(ImVec2(pos.x - Thickness, pos.y - (outlineGap + BorderWidth + CrosshairsCFG::VerticalLength)), ImVec2(pos.x + 1 + Thickness, pos.y - outlineGap), color & IM_COL32_A_MASK);//top
				drawList->AddRectFilled(ImVec2(pos.x - Thickness, pos.y + outlineGap + DotSize), ImVec2(pos.x + 1 + Thickness, pos.y + (outlineGap + DotSize + BorderWidth + CrosshairsCFG::VerticalLength)), color & IM_COL32_A_MASK);//bottom
			}
		}
		if (CrosshairsCFG::drawDot) drawList->AddRectFilled(ImVec2(pos.x - offset1.x + DotSize, pos.y - offset1.y + DotSize), ImVec2(pos.x + offset1.x, pos.y + offset1.y), color);// dot
		drawList->AddRectFilled(ImVec2(pos.x - (gap + CrosshairsCFG::HorizontalLength), pos.y - Thickness + 1), ImVec2(pos.x - gap, pos.y + Thickness), color);// left
		drawList->AddRectFilled(ImVec2(pos.x + gap + DotSize, pos.y - Thickness + 1), ImVec2(pos.x + (gap + DotSize + CrosshairsCFG::HorizontalLength), pos.y + Thickness), color);// right
		if (!CrosshairsCFG::tStyle) drawList->AddRectFilled(ImVec2(pos.x - Thickness + 1, pos.y - (gap + CrosshairsCFG::VerticalLength)), ImVec2(pos.x + Thickness, pos.y - gap), color);// top
		drawList->AddRectFilled(ImVec2(pos.x - Thickness + 1, pos.y + gap + DotSize), ImVec2(pos.x + Thickness, pos.y + (gap + DotSize + CrosshairsCFG::VerticalLength)), color);// bottom
	}

}