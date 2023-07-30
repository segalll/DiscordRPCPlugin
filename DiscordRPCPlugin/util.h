#pragma once

#include <algorithm>
#include <map>
#include <string>

const std::map<std::string, std::string> mapsMap = {
    { "arc_darc_p",              "Starbase ARC (Aftermath)" },
    { "arc_p",                   "ARCtagon" },
    { "chn_stadium_day_p",       "Forbidden Temple (Day)" },
    { "chn_stadium_p",           "Forbidden Temple" },
    { "eurostadium_night_p",     "Mannfield (Night)" },
    { "eurostadium_p",           "Mannfield" },
    { "eurostadium_rainy_p",     "Mannfield (Stormy)" },
    { "farm_night_p",            "Farmstead (Night)" },
    { "haunted_trainstation_p",  "Urban Central (Haunted)" },
    { "hoopsstadium_p",          "Dunk House" },
    { "labs_basin_p",            "Basin" },
    { "labs_circlepillars_p",    "Pillars" },
    { "labs_corridor_p",         "Corridor" },
    { "labs_cosmic_p",           "Cosmic (Old)" },
    { "labs_cosmic_v4_p",        "Cosmic (New)" },
    { "labs_doublegoal_p",       "Double Goal (Old)" },
    { "labs_doublegoal_v2_p",    "Double Goal (New)" },
    { "labs_galleon_mast_p",     "Galleon Retro" },
    { "labs_galleon_p",          "Galleon" },
    { "labs_holyfield_p",        "Loophole" },
    { "labs_octagon_02_p",       "Octagon (New)" },
    { "labs_octagon_p",          "Octagon (Old)" },
    { "labs_pillarglass_p",      "Hourglass" },
    { "labs_pillarheat_p",       "Barricade" },
    { "labs_pillarwings_p",      "Colossus" },
    { "labs_underpass_P",        "Underpass" },
    { "labs_utopia_P",           "Utopia Retro" },
    { "neotokyo_p",              "Tokyo Underpass" },
    { "neotokyo_standard_p",     "Neo Tokyo" },
    { "neotokyo_toon_p",         "Neo Tokyo (Comic)", },
    { "park_night_p",            "Beckwith Park (Midnight)" },
    { "park_p",                  "Beckwith Park" },
    { "park_rainy_p",            "Beckwith Park (Stormy)" },
    { "park_snowy_p",            "Beckwith Park (Snowy)" },
    { "shattershot_p",           "Core 707" },
    { "stadium_foggy_p",         "DFH Stadium (Stormy)" },
    { "stadium_p",               "DFH Stadium" },
    { "stadium_race_day_p",      "DFH Stadium (Circuit)" },
    { "stadium_winter_p",        "DFH Stadium (Snowy)" },
    { "trainstation_dawn_p",     "Urban Central (Dawn)" },
    { "trainstation_night_p",    "Urban Central (Night)" },
    { "trainstation_p",          "Urban Central" },
    { "underwater_p",            "AquaDome" },
    { "utopiastadium_dusk_p",    "Utopia Coliseum (Dusk)" },
    { "utopiastadium_p",         "Utopia Coliseum" },
    { "utopiastadium_snow_p",    "Utopia Coliseum (Snowy)" },
    { "wasteland_night_p",       "Badlands (Night)" },
    { "wasteland_p",             "Badlands" },
    { "arc_standard_p",          "Starbase ARC" },
    { "bb_p",                    "Champions Field (NFL)" },
    { "beach_p",                 "Salty Shores" },
    { "beach_night_p",           "Salty Shores (Night)" },
    { "cs_day_p",                "Champions Field (Day)" },
    { "cs_hw_p",                 "Rivals Arena" },
    { "cs_p",                    "Champions Field" },
    { "eurostadium_snownight_p", "Mannfield (Snowy)" },
    { "farm_p",                  "Farmstead" },
    { "music_p",                 "Neon Fields" },
    { "outlaw_p",                "Deadeye Canyon" },
    { "park_bman_p",             "Beckwith Park" },
    { "stadium_day_p",           "DFH Stadium (Day)" },
    { "throwbackhockey_p",       "Throwback Stadium (Snowy)" },
    { "throwbackstadium_P",      "Throwback Stadium" },
    { "wasteland_night_s_p",     "Wasteland (Night)" },
    { "wasteland_s_p",           "Wasteland" }
};

std::string getMap(std::string mapKey) {
    std::transform(mapKey.begin(), mapKey.end(), mapKey.begin(), ::tolower);
    if (mapsMap.find(mapKey) == mapsMap.end()) {
        return "logo";
    }
    return mapsMap.at(mapKey);
}

std::string mapToImageKey(std::string map) {
    std::transform(map.begin(), map.end(), map.begin(), ::tolower);
    std::transform(map.begin(), map.end(), map.begin(), [](char ch) {
        return ch == ' ' ? '-' : ch;
    });
    map.erase(std::remove(map.begin(), map.end(), '('), map.end());
    map.erase(std::remove(map.begin(), map.end(), ')'), map.end());
    return map;
}

std::array<std::string, 23> ranks = {
    "Unranked",
    "Bronze 1",
    "Bronze 2",
    "Bronze 3",
    "Silver 1",
    "Silver 2",
    "Silver 3",
    "Gold 1",
    "Gold 2",
    "Gold 3",
    "Platinum 1",
    "Platinum 2",
    "Platinum 3",
    "Diamond 1",
    "Diamond 2",
    "Diamond 3",
    "Champion 1",
    "Champion 2",
    "Champion 3",
    "Grand Champion 1",
    "Grand Champion 2",
    "Grand Champion 3",
    "Supersonic Legend"
};

std::string getRank(int tierId) {
    if (tierId >= 0 && tierId < 23) {
        return ranks[tierId];
    }
    return "";
}

std::string rankToImageKey(std::string rank) {
    std::transform(rank.begin(), rank.end(), rank.begin(), ::tolower);
    rank.erase(std::remove_if(rank.begin(), rank.end(), ::isspace), rank.end());
    return rank;
}

std::string getPlaylistName(int playlistId, int teamSize) {
    std::string format = teamSize + "v" + teamSize;

    if (playlistId == 0) { return "Casual"; }
    if (playlistId == 1) { return "Duel"; }
    if (playlistId == 2) { return "Doubles"; }
    if (playlistId == 3) { return "Standard"; }
    if (playlistId == 4) { return "Chaos"; }
    if (playlistId == 6) { return "Private " + format; }
    if (playlistId == 7) { return "Season " + format; }
    if (playlistId == 8) { return "Exhibition " + format; }
    if (playlistId == 9) { return "Training"; }
    if (playlistId == 10) { return "Ranked Duel"; }
    if (playlistId == 11) { return "Ranked Doubles"; }
    if (playlistId == 13) { return "Ranked Standard"; }
    if (playlistId == 15) { return "Snow Day"; }
    if (playlistId == 16) { return "Rocket Labs"; }
    if (playlistId == 17) { return "Hoops"; }
    if (playlistId == 18) { return "Rumble"; }
    if (playlistId == 19) { return "Workshop " + format; }
    if (playlistId == 22) { return "Tournament " + format; }
    if (playlistId == 23) { return "Breakout"; }
    if (playlistId == 26) { return "FACEIT " + format; }
    if (playlistId == 27) { return "Ranked Hoops"; }
    if (playlistId == 28) { return "Ranked Rumble"; }
    if (playlistId == 29) { return "Ranked Dropshot"; }
    if (playlistId == 30) { return "Ranked Snow Day"; }
    if (playlistId == 31) { return "Ghost Hunt"; }
    if (playlistId == 32) { return "Beach Ball"; }
    if (playlistId == 33) { return "Spike Rush"; }
    if (playlistId == 34) { return "Tournament " + format; }
    if (playlistId == 35) { return "Rocket Labs"; }
    if (playlistId == 37) { return "Dropshot Rumble"; }
    if (playlistId == 38) { return "Heatseeker"; }
    if (playlistId == 41) { return "Boomer Ball"; }
    if (playlistId == 43) { return "Heatseeker Doubles"; }
    if (playlistId == 44) { return "Winter Breakaway"; }
    if (playlistId == 46) { return "Gridiron"; }
    if (playlistId == 47) { return "Super Cube"; }
    if (playlistId == 48) { return "Tactical Rumble"; }
    if (playlistId == 49) { return "Spring Loaded"; }
    if (playlistId == 50) { return "Speed Demon"; }
    if (playlistId == 52) { return "Rumble BM"; } // idk what this is
    if (playlistId == 54) { return "Knockout"; }
    if (playlistId == 55) { return "confidential_thirdwheel_test"; }
    if (playlistId == 62) { return "Magnus Futbal"; }

    return "Unknown";
}

std::string capitalize(std::string s) {
    s[0] = std::toupper(s[0]);
    for (int i = 1; i < s.length(); i++) {
        if (s[i - 1] == ' ') {
            s[i] = std::toupper(s[i]);
        }
        else {
            s[i] = std::tolower(s[i]);
        }
    }
    return s;
}