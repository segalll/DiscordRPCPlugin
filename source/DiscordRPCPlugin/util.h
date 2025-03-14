#pragma once

#include <algorithm>
#include <map>
#include <string>

const std::map<std::string, std::string> mapNames = {
    { "arc_darc_p",              "Starbase ARC (Aftermath)" },
    { "arc_standard_p",          "Starbase ARC" },
    { "arc_p",                   "ARCtagon (Night)" },
    { "bb_p",                    "Champions Field (NFL)" },
    { "beach_p",                 "Salty Shores" },
    { "beach_night_grs_p",       "Salty Shores (Salty Fest)" },
    { "beach_night_p",           "Salty Shores (Night)" },
    { "cs_hw_p",                 "Rivals Arena" },
    { "cs_day_p",                "Champions Field (Day)" },
    { "cs_p",                    "Champions Field (Night" },
    { "chn_stadium_day_p",       "Forbidden Temple (Day)" },
    { "chn_stadium_p",           "Forbidden Temple (Dawn)" },
    { "eurostadium_p",           "Mannfield (Day)" },
    { "eurostadium_night_p",     "Mannfield (Night)" },
    { "eurostadium_rainy_p",     "Mannfield (Stormy)" },
    { "eurostadium_snownight_p", "Mannfield (Snowy)" },
    { "eurostadium_dusk_p",      "Mannfield (Dusk)" },
    { "fni_stadiun_p",           "Forbidden Temple (Fire & Ice)" },
    { "ff_dusk_p",               "Estadio Vida (Dusk)" },
    { "farm_p",                  "Farmstead (Dusk)" },
    { "farm_night_p",            "Farmstead (Night)" },
    { "farm_grs_p",              "Farmstead (Pitched)" },
    { "farm_hw_p",               "Farmstead (Spooky)" },
    { "haunted_trainstation_p",  "Urban Central (Haunted)" },
    { "hoopsstadium_p",          "Dunk House (Night)" },
    { "hoopsstreet_p",           "The Block (Dusk)" },
    { "ko_calavera_p",           "Calavera" },
    { "ko_carbon_p",             "Carbon" },
    { "ko_quadron_p",            "Quadron" },
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
    { "music_p",                 "Neon Fields" },
    { "neotokyo_arcade_p",       "Neo Tokyo (Arcade)" },
    { "neotokyo_p",              "Tokyo Underpass" },
    { "neotokyo_standard_p",     "Neo Tokyo" },
    { "neotokyo_toon_p",         "Neo Tokyo (Comic)", },
    { "neotokyo_hax_p",          "Neo Tokyo (Hacked)", },
    { "outlaw_p",                "Deadeye Canyon" },
    { "outlaw_oasis_p",          "Deadeye Canyon (Oasis)" },
    { "park_night_p",            "Beckwith Park (Midnight)" },
    { "park_p",                  "Beckwith Park" },
    { "park_rainy_p",            "Beckwith Park (Stormy)" },
    { "park_snowy_p",            "Beckwith Park (Snowy)" },
    { "park_bman_p",             "Beckwith Park (Night)" },
    { "shattershot_p",           "Core 707" },
    { "stadium_p",               "DFH Stadium" },
    { "stadium_foggy_p",         "DFH Stadium (Stormy)" },
    { "stadium_race_day_p",      "DFH Stadium (Circuit)" },
    { "stadium_winter_p",        "DFH Stadium (Snowy)" },
    { "stadium_day_p",           "DFH Stadium (Day)" },
    { "street_p",                "Sovereign Heights (Dusk)" },
    { "swoosh_p",                "Champions Field (Nike FC)" },
    { "trainstation_dawn_p",     "Urban Central (Dawn)" },
    { "trainstation_night_p",    "Urban Central (Night)" },
    { "trainstation_p",          "Urban Central (Rainy)" },
    { "throwbackhockey_p",       "Throwback Stadium (Snowy)" },
    { "throwbackstadium_P",      "Throwback Stadium" },
    { "underwater_p",            "AquaDome" },
    { "underwater_grs_p",        "AquaDome (Salty Shallows)" },
    { "uf_day_p",                "Futura Garden (Day)" },
    { "utopiastadium_p",         "Utopia Coliseum (Day)" },
    { "utopiastadium_dusk_p",    "Utopia Coliseum (Dusk)" },
    { "utopiaStadium_lux_p",     "Utopia Coliseum (Gilded)" },
    { "utopiastadium_snow_p",    "Utopia Coliseum (Snowy)" },
    { "wasteland_p",             "Badlands (Day)" },
    { "wasteland_night_p",       "Badlands (Night)" },
    { "wasteland_s_p",           "Wasteland (Day)" },
    { "wasteland_night_s_p",     "Wasteland (Night)" },
    { "wasteland_grs_p",         "Wasteland (Pitched)" },
    { "woods_p",                 "Drift Woods (Dawn)" },
    { "woods_night_p",           "Drift Woods (Night)" },
};

const std::vector<std::string> rankNames = {
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

const std::map<int32_t, std::string> playlistNames = {
    { 0,    "Casual" },
    { 1,    "Duel" },
    { 2,    "Doubles" },
    { 3,    "Standard" },
    { 4,    "Chaos" },
    { 6,    "Private" },
    { 7,    "Season" },
    { 8,    "Exhibition" },
    { 9,    "Training" },
    { 10,   "Ranked Duel" },
    { 11,   "Ranked Doubles" },
    { 13,   "Ranked Standard" },
    { 15,   "Snow Day" },
    { 16,   "Rocket Labs" }, // Experimental
    { 17,   "Hoops" },
    { 18,   "Rumble" },
    { 19,   "Workshop" },
    { 20,   "Custom Training" }, // UGCTrainingEditor (Game no longer uses this for the editor, they now use "21" for all custom training)
    { 21,   "Custom Training" }, // UGCTraining
    { 22,   "Tournament" }, // Tournament (User created custom tournaments)
    { 23,   "Dropshot" },
    { 24,   "Lan" },
    { 26,   "FACEIT" },
    { 27,   "Ranked Hoops" },
    { 28,   "Ranked Rumble" },
    { 29,   "Ranked Dropshot" },
    { 30,   "Ranked Snow Day" },
    { 31,   "Ghost Hunt" },
    { 32,   "Beach Ball" },
    { 33,   "Spike Rush" },
    { 34,   "Tournament" }, // AutoTournament (Psyonix tournaments)
    { 35,   "Rocket Labs" }, // RocketLabs (Different MapSet from Experimental)
    { 37,   "Dropshot Rumble" },
    { 38,   "Heatseeker" },
    { 39,   "Co-Op Vs AI" },
    { 41,   "Heatseeker" },
    { 43,   "Heatseeker Doubles" },
    { 44,   "Winter Breakaway" },
    { 46,   "Gridiron" },
    { 47,   "Cubic" },
    { 48,   "Tactical Rumble" },
    { 49,   "Spring Loaded" },
    { 50,   "Speed Demon" },
    { 52,   "Gotham City Rumble" },
    { 54,   "Knockout" },
    { 55,   "confidential_thirdwheel_test" }, // April fools playlist, name can change every year depending on what the event is.
    { 62,   "Nike FC Showdown" },
    { 64,   "Haunted Heatseeker" },
    { 66,   "Heatseeker Ricochet" },
    { 67,   "Spooky Cube" },
    { 68,   "G-Force Frenzy" }
};

const std::vector<int32_t> formatThesePlaylists = {
    6, // Private
    7, // Season
    8, // Exhibition 
    19, // Workshop
    22, // Custom Tournament
    26, // FACEIT 
    34 // Psyonix Tournament
};

std::string capitalize(std::string s) {
    s[0] = std::toupper(s[0]);

    for (size_t i = 1; i < s.length(); i++) {
        if (s[i - 1] == ' ') {
            s[i] = std::toupper(s[i]);
        }
        else {
            s[i] = std::tolower(s[i]);
        }
    }

    return s;
}

void toLower(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), tolower);
}

void toUpper(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), toupper);
}

std::string getMap(std::string mapKey) {
    if (!mapKey.empty()) {
        toLower(mapKey);

        if (mapNames.contains(mapKey)) {
            return mapNames.at(mapKey);
        }
    }

    return "logo";
}

std::string mapToImageKey(std::string map) {
    if (!map.empty()) {
        toLower(map);

        std::transform(map.begin(), map.end(), map.begin(), [](char ch) {
            return ((ch == ' ') ? '-' : ch);
        });

        map.erase(std::remove(map.begin(), map.end(), '('), map.end());
        map.erase(std::remove(map.begin(), map.end(), ')'), map.end());
    }

    return map;
}

std::string getRank(int tierId) {
    if ((tierId >= 0) && (tierId < 23)) {
        return rankNames.at(tierId);
    }

    return "";
}

std::string rankToImageKey(std::string rank) {
    toLower(rank);
    rank.erase(std::remove_if(rank.begin(), rank.end(), ::isspace), rank.end());
    return rank;
}

std::string getPlaylistName(int playlistId, int teamSize) {
    if (playlistNames.contains(playlistId)) {
        std::string format = (teamSize + "v" + teamSize);
        std::string playlistName = playlistNames.at(playlistId);

        if (std::find(formatThesePlaylists.begin(), formatThesePlaylists.end(), playlistId) != formatThesePlaylists.end()) {
            return (playlistName + " " + format);
        }

        return playlistName;
    }

    return "Unknown";
}