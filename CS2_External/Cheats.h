#pragma once
#include "Game.h"
#include "Entity.h"
#include "Features\Aimbot\Legitbot.hpp"

#include "Radar/Radar.h"
#include "TriggerBot.h"
#include "Features/Misc.h"

#include "Features\Aimbot\Legitbot.hpp"
namespace Cheats
{
	void KeyCheckThread();

	void RenderCrossHair(ImDrawList* drawList) noexcept;

	void RadarSetting(Base_Radar& Radar);

	void Run();
}

struct {
	ImFont* normal15px = nullptr;
} fonts;