#pragma once
#include <string>
#include <map>

const char* GunIcon(const std::string weapon){
    std::map<std::string, const char*> gunIcons = {
        {"ct_knife", "]"},
        {"t_knife", "["},
        {"deagle", "A"},
        {"elite", "B"},
        {"fiveseven", "C"},
        {"glock", "D"},
        {"revolver", "J"},
        {"p2000", "E"},
        {"p250", "F"},
        {"usp", "G"},
        {"tec9", "H"},
        {"cz75a", "I"},
        {"mac10", "K"},
        {"ump45", "L"},
        {"bizon", "M"},
        {"mp7", "N"},
        {"mp9", "R"},
        {"p90", "O"},
        {"galilar", "Q"},
        {"famas", "R"},
        {"m4a1_silencer", "T"},
        {"m4a1", "S"},
        {"aug", "U"},
        {"sg556", "V"},
        {"ak47", "W"},
        {"g3sg1", "X"},
        {"scar20", "Y"},
        {"awp", "Z"},
        {"ssg08", "a"},
        {"xm1014", "b"},
        {"sawedoff", "c"},
        {"mag7", "d"},
        {"nova", "e"},
        {"negev", "f"},
        {"m249", "g"},
        {"taser", "h"},
        {"flashbang", "i"},
        {"hegrenade", "j"},
        {"smokegrenade", "k"},
        {"molotov", "l"},
        {"decoy", "m"},
        {"incgrenade", "n"},
        {"c4", "o"}
    };

    auto it = gunIcons.find(weapon);
    return (it != gunIcons.end()) ? it->second : "";
}