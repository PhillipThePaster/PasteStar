#pragma once
#define _USE_MATH_DEFINES
#define MAXV 10000e9
#include <math.h>
#include <iostream>

#include "..\..\Game.h"
#include "..\..\Entity.h"
#include "..\..\MenuConfig.hpp"
#include "..\..\View.hpp"
#include "..\..\Features/RCS.h"


// im going to recode most of how this works cause the code is shit but im adding weapon groups / individual weapon settings  -- Phillip

inline int GetWeaponGroup(const std::string& weaponName)
{
    std::unordered_map<std::string, int> weaponGroups = {
        {"knife" , 0},
        {"ak47", 1}, {"aug", 1}, {"famas", 1},{"galilar", 1}, {"m4a1_silencer", 1}, {"m4a1", 1}, {"sg553", 1},
        {"mag7", 2}, {"sawedoff", 2}, {"nova", 2}, {"xm1014", 2},
        {"cz75a", 3}, {"elite", 3}, {"fiveseven", 3},{"glock", 3}, {"hkp2000", 3}, {"p250", 3}, {"revolver", 3},{"tec9", 3}, {"usp_silencer", 3},
        {"deagle", 4}, {"revolver", 4},
        {"awp", 5}, {"g3sg1", 5}, {"scar20", 5}, {"ssg08", 5},
        {"mac10", 6}, {"mp7", 6}, {"mp9", 6}, {"mp5sd", 6},{"ump45", 6}, {"bizon", 6}, {"p90", 6},
        {"smokegrenade", 7}, {"flashbang", 7}, {"molotov", 7},{"fire_grenade", 7}
    };


    if (weaponGroups.count(weaponName)) {
        return weaponGroups[weaponName];
    }
    else {
        return 0;
    }
}

namespace AimControl
{
    extern bool Rage;

    inline int HotKey = VK_LBUTTON;
    inline int AimBullet = 0;
    inline bool ScopeOnly = false;
    inline bool AutoShot = false;
    inline bool IgnoreFlash = false;
    inline float AimFov = 5;
    inline float AimFovMin = 0.f;
    inline float Smooth = 2.0f;
    inline unsigned int AimPos = 7;
    inline std::vector<int> HitboxList{ BONEINDEX::head };
    inline std::vector<int> HotKeyList{ VK_LMENU, VK_LBUTTON, VK_RBUTTON, VK_XBUTTON1, VK_XBUTTON2, VK_CAPITAL, VK_LSHIFT, VK_LCONTROL };

    inline bool HasTarget = false;

    inline void SetHotKey(int Index) {
        HotKey = HotKeyList.at(Index);
    }

    inline void switchToggle(){
        MenuConfig::AimAlways = !MenuConfig::AimAlways;
    }

    // doesnt work -Phillip
    inline void AutoPistol(int weaponGroup) {
        while ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0 && weaponGroup == 3)
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            Sleep(10);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        }
        return;
    }
   

    inline void AimBot(const CEntity& Local, Vec3 LocalPos, std::vector<Vec3>& AimPosList){
          int weaponGroup = GetWeaponGroup(Local.Pawn.WeaponName);
        if (MenuConfig::ShowMenu || weaponGroup == 7)
            return;

        //int isFired;
        //ProcessMgr.ReadMemory(Local.Pawn.Address + Offset::Pawn.iShotsFired, isFired);
        //if (!isFired && !AimLock)


        if (AimControl::ScopeOnly && !Local.Pawn.Scoped && (weaponGroup == 5 || 1))
            return;      
        
        if (!IgnoreFlash && Local.Pawn.FlashDuration > 0.f)
            return;

        int ListSize = AimPosList.size();
        float BestNorm = MAXV;

        float Yaw, Pitch;
        float Distance, Norm, Length;
        Vec2 Angles{ 0,0 };
        int ScreenCenterX = Gui.Window.Size.x / 2;
        int ScreenCenterY = Gui.Window.Size.y / 2;
        float TargetX = 0.f;
        float TargetY = 0.f;

        Vec2 ScreenPos;

        for (int i = 0; i < ListSize; i++)
        {
            Vec3 OppPos;

            OppPos = AimPosList[i] - LocalPos;

            Distance = sqrt(pow(OppPos.x, 2) + pow(OppPos.y, 2));

            Length = sqrt(Distance * Distance + OppPos.z * OppPos.z);

            // RCS by @Tairitsu
            // im going to recode the full aimbot+rcs at some point -Phillip
            if (MenuConfig::RCS ){
                RCS::UpdateAngles(Local, Angles);
                float rad = Angles.x * RCS::RCSScale.x / 360.f * M_PI;
                float si = sinf(rad);
                float co = cosf(rad);

                float z = OppPos.z * co + Distance * si;
                float d = (Distance * co - OppPos.z * si) / Distance;

                rad = -Angles.y * RCS::RCSScale.y / 360.f * M_PI;
                si = sinf(rad);
                co = cosf(rad);

                float x = (OppPos.x * co - OppPos.y * si) * d;
                float y = (OppPos.x * si + OppPos.y * co) * d;

                OppPos = Vec3{ x, y, z };

                AimPosList[i] = LocalPos + OppPos;
            }

            Yaw = atan2f(OppPos.y, OppPos.x) * 57.295779513 - Local.Pawn.ViewAngle.y;
            Pitch = -atan(OppPos.z / Distance) * 57.295779513 - Local.Pawn.ViewAngle.x;
            Norm = sqrt(pow(Yaw, 2) + pow(Pitch, 2));

            if (Norm < BestNorm)
                BestNorm = Norm;

            gGame.View.WorldToScreen(Vec3(AimPosList[i]), ScreenPos);
        }

        if (Norm >= AimFov || Norm <= AimFovMin)
            return;


        if (!Smooth){
            mouse_event(MOUSEEVENTF_MOVE, (DWORD)(TargetX), (DWORD)(TargetY), NULL, NULL);
            return;
        }

        float SmoothValue = Smooth != 0.0f ? Smooth : 1.5f;
        TargetX = (ScreenPos.x - ScreenCenterX) / SmoothValue;
        TargetY = (ScreenPos.y != ScreenCenterY && ScreenPos.y != 0) ? (ScreenPos.y - ScreenCenterY) / SmoothValue : 0;

        mouse_event(MOUSEEVENTF_MOVE, (DWORD)(TargetX), (DWORD)(TargetY), NULL, NULL);
    }
}
