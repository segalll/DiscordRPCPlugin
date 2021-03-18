#include "DiscordRPCPlugin.h"

#include <algorithm>
#include <ctime>

#include "bakkesmod/wrappers/includes.h"
#include "discord_rpc.h"

BAKKESMOD_PLUGIN(DiscordRPCPlugin, "Discord Rich Presence integration", "1.0", PLUGINTYPE_FREEPLAY)

static const char* APPLICATION_ID = "562098317547405322";

static void discordInit() {
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    Discord_Initialize(APPLICATION_ID, &handlers, 1, NULL);
}

std::string Capitalize(std::string str) {
    str[0] = std::toupper(str[0]);
    for (int i = 1; i < str.length(); i++) {
        if (str[i - 1] == ' ') {
            str[i] = std::toupper(str[i]);
        } else {
            str[i] = std::tolower(str[i]);
        }
    }
    return str;
}

enum GAMETYPE {
    GAMETYPE_REPLAY = 0,
    GAMETYPE_ONLINE = 1,
    GAMETYPE_FREEPLAY = 2,
    GAMETYPE_TRAINING = 3,
    GAMETYPE_SPECTATE = 4,
    GAMETYPE_LOCAL = 5,
    GAMETYPE_MENU = 6
};

GameState DiscordRPCPlugin::GetCurrentGameType() {
    if (gameWrapper->IsInReplay()) {
        return {GAMETYPE_REPLAY, gameWrapper->GetGameEventAsReplay()};
    } else if (gameWrapper->IsInOnlineGame()) {
        return {GAMETYPE_ONLINE, gameWrapper->GetOnlineGame()};
    } else if (gameWrapper->IsInFreeplay()) {
        return {GAMETYPE_FREEPLAY, gameWrapper->GetGameEventAsServer()};
    } else if (gameWrapper->IsInCustomTraining()) {
        return {GAMETYPE_TRAINING, gameWrapper->GetGameEventAsServer()};
    } else if (gameWrapper->IsSpectatingInOnlineGame()) {
        return {GAMETYPE_SPECTATE, gameWrapper->GetOnlineGame()};
    } else if (gameWrapper->IsInGame()) {
        return {GAMETYPE_LOCAL, gameWrapper->GetGameEventAsServer()};
    } else {
        return {GAMETYPE_MENU, NULL};
    }
}

std::string GetPlaylistName(int playlistId, int teamSize) {
    std::string size = std::to_string(teamSize);
    switch (playlistId) {
        case (1):
            return "Duel";
        case (2):
            return "Doubles";
        case (3):
            return "Standard";
        case (4):
            return "Chaos";
        case (6):
            return "Private " + size + "v" + size;
        case (7):
            return "Season " + size + "v" + size;
        case (8):
            return "Exhibition " + size + "v" + size;
        case (9):
            return "Training";
        case (10):
            return "Ranked Duel";
        case (11):
            return "Ranked Doubles";
        case (13):
            return "Ranked Standard";
        case (15):
            return "Snow Day";
        case (16):
            return "Rocket Labs";
        case (17):
            return "Hoops";
        case (18):
            return "Rumble";
        case (19):
            return "Workshop " + size + "v" + size;
        case (22):
            return "Tournament " + size + "v" + size;
        case (27):
            return "Ranked Hoops";
        case (28):
            return "Ranked Rumble";
        case (29):
            return "Ranked Dropshot";
        case (30):
            return "Ranked Snow Day";
        case (31):
            return "Ghost Hunt";
        case (32):
            return "Beach Ball";
        case (33):
            return "Spike Rush";
        case (34):
            return "Tournament " + size + "v" + size;
        case (35):
            return "Rocket Labs";
        case (37):
            return "Dropshot Rumble";
        case (38):
            return "Heatseeker";
        case (41):
            return "Boomer Ball";
        case (43):
            return "Heatseeker Doubles";
        case (44):
            return "Winter Breakaway";
        case (46):
            return "Gridiron";
        case (47):
            return "Super Cube";
        default:
            return "";
    }
}

std::string GetRank(int tierID) {
    switch (tierID) {
        case (0):
            return "Unranked";
        case (1):
            return "Bronze 1";
        case (2):
            return "Bronze 2";
        case (3):
            return "Bronze 3";
        case (4):
            return "Silver 1";
        case (5):
            return "Silver 2";
        case (6):
            return "Silver 3";
        case (7):
            return "Gold 1";
        case (8):
            return "Gold 2";
        case (9):
            return "Gold 3";
        case (10):
            return "Platinum 1";
        case (11):
            return "Platinum 2";
        case (12):
            return "Platinum 3";
        case (13):
            return "Diamond 1";
        case (14):
            return "Diamond 2";
        case (15):
            return "Diamond 3";
        case (16):
            return "Champion 1";
        case (17):
            return "Champion 2";
        case (18):
            return "Champion 3";
        case (19):
            return "Grand Champion 1";
        case (20):
            return "Grand Champion 2";
        case (21):
            return "Grand Champion 3";
        case (22):
            return "Supersonic Legend";
        default:
            return "";
    }
}

std::string GetMap(std::string mapName) {
    std::transform(mapName.begin(), mapName.end(), mapName.begin(), ::tolower);
    if (mapName == "arc_p") return "ARCtagon";
    if (mapName == "arc_standard_p") return "Starbase ARC";
    if (mapName == "beach_p") return "Salty Shores";
    if (mapName == "beach_night_p") return "Salty Shores (Night)";
    if (mapName == "chn_stadium_p") return "Forbidden Temple";
    if (mapName == "cs_day_p") return "Champions Field (Day)";
    if (mapName == "cs_hw_p") return "Rivals Arena";
    if (mapName == "cs_p") return "Champions Field";
    if (mapName == "eurostadium_night_p") return "Mannfield (Night)";
    if (mapName == "eurostadium_p") return "Mannfield";
    if (mapName == "eurostadium_rainy_p") return "Mannfield (Stormy)";
    if (mapName == "eurostadium_snownight_p") return "Mannfield (Snowy)";
    if (mapName == "farm_p") return "Farmstead";
    if (mapName == "farm_night_p") return "Farmstead (Night)";
    if (mapName == "farm_upsidedown_p") return "Farmstead (The Upside Down)";
    if (mapName == "hoopsstadium_p") return "Dunk House";
    if (mapName == "labs_circlepillars_p") return "Pillars";
    if (mapName == "labs_cosmic_p") return "Cosmic (Old)";
    if (mapName == "labs_cosmic_v4_p") return "Cosmic";
    if (mapName == "labs_doublegoal_p") return "Double Goal (Old)";
    if (mapName == "labs_doublegoal_v2_p") return "Double Goal";
    if (mapName == "labs_octagon_02_p") return "Octagon";
    if (mapName == "labs_octagon_p") return "Octagon (Old)";
    if (mapName == "labs_underpass_p") return "Underpass (Old)";
    if (mapName == "labs_underpass_v0_p") return "Underpass";
    if (mapName == "labs_utopia_p") return "Utopia Retro";
    if (mapName == "neotokyo_p") return "Tokyo Underpass";
    if (mapName == "neotokyo_standard_p") return "Neo Tokyo";
    if (mapName == "park_night_p") return "Beckwith Park (Midnight)";
    if (mapName == "park_p") return "Beckwith Park";
    if (mapName == "park_rainy_p") return "Beckwith Park (Stormy)";
    if (mapName == "shattershot_p") return "Core 707";
    if (mapName == "stadium_day_p") return "DFH Stadium (Day)";
    if (mapName == "stadium_foggy_p") return "DFH Stadium (Stormy)";
    if (mapName == "stadium_p") return "DFH Stadium";
    if (mapName == "stadium_winter_p") return "DFH Stadium (Snowy)";
    if (mapName == "throwbackstadium_p") return "Throwback Stadium";
    if (mapName == "trainstation_dawn_p") return "Urban Central (Dawn)";
    if (mapName == "trainstation_night_p") return "Urban Central (Night)";
    if (mapName == "trainstation_p") return "Urban Central";
    if (mapName == "haunted_trainstation_p") return "Urban Central (Haunted)";
    if (mapName == "underwater_p") return "AquaDome";
    if (mapName == "utopiastadium_dusk_p") return "Utopia Coliseum (Dusk)";
    if (mapName == "utopiastadium_p") return "Utopia Coliseum";
    if (mapName == "utopiastadium_snow_p") return "Utopia Coliseum (Snowy)";
    if (mapName == "wasteland_night_p") return "Badlands (Night)";
    if (mapName == "wasteland_night_s_p") return "Wasteland (Night)";
    if (mapName == "wasteland_p") return "Badlands";
    if (mapName == "wasteland_s_p") return "Wasteland";
    return "logo";
}

std::string FormatMap(std::string map) {
    std::transform(map.begin(), map.end(), map.begin(), ::tolower);
    std::transform(map.begin(), map.end(), map.begin(), [](char ch) {
        return ch == ' ' ? '-' : ch;
    });
    map.erase(remove(map.begin(), map.end(), '('), map.end());
    map.erase(remove(map.begin(), map.end(), ')'), map.end());
    return map;
}

Data DiscordRPCPlugin::HandleGame(Data input, GameState gameState) {
    Data output = input;

    GameSettingPlaylistWrapper playlistWrapper = gameState.wrapper.GetPlaylist();

    std::string gameType;
    if (playlistWrapper.memory_address != NULL) {
        gameType = GetPlaylistName(playlistWrapper.GetPlaylistId(), gameState.wrapper.GetMaxTeamSize());
    }

    if (gameType == "Training") {
        output = HandleTraining(input, gameState);
        return output;
    }

    int teamGoals[2] = {0, 0};
    if (!gameState.wrapper.GetTeams().Get(0).IsNull() && !gameState.wrapper.GetTeams().Get(1).IsNull()) {
        teamGoals[0] = gameState.wrapper.GetTeams().Get(0).GetScore();
        teamGoals[1] = gameState.wrapper.GetTeams().Get(1).GetScore();
    }
    CarWrapper localPlayer = gameWrapper->GetLocalCar();
    if (!localPlayer.IsNull()) {
        output.team = localPlayer.GetPRI().GetTeamNum();
    }
    if (gameType.find("Ranked") != std::string::npos) {
        if (!localPlayer.IsNull() && playlistWrapper.memory_address != NULL) {
            auto uidWrapper = gameWrapper->GetUniqueID();
            int playlistID = playlistWrapper.GetPlaylistId();
            MMRWrapper mmrWrapper = gameWrapper->GetMMRWrapper();
            SkillRank rank = mmrWrapper.GetPlayerRank(uidWrapper, playlistID);
            float mmr = mmrWrapper.GetPlayerMMR(uidWrapper, playlistID);
            std::string rankStr = GetRank(rank.Tier);
            if (rankStr.length() > 0) {
                output.smallHover = rankStr + " (MMR: " + std::to_string((int)mmr) + ")";
                transform(rankStr.begin(), rankStr.end(), rankStr.begin(), ::tolower);
                rankStr.erase(remove_if(rankStr.begin(), rankStr.end(), ::isspace), rankStr.end());
                output.smallImage = rankStr;
            }
        }
    }

    std::string plyGoals;
    std::string enemyGoals;
    if (output.team == 0) {
        plyGoals = std::to_string(teamGoals[0]);
        enemyGoals = std::to_string(teamGoals[1]);
    } else if (output.team == 1) {
        enemyGoals = std::to_string(teamGoals[0]);
        plyGoals = std::to_string(teamGoals[1]);
    }

    if (gameState.wrapper.GetbMatchEnded()) {
        output.details = "In " + gameType + " lobby";
        output.start = NULL;
        output.remaining = NULL;
        if (plyGoals > enemyGoals) {
            output.state = "Won ";
        } else {
            output.state = "Lost ";
        }
        output.state += "(" + plyGoals + " - " + enemyGoals + ")";
    } else {
        output.details = "Playing " + gameType;
        if (plyGoals > enemyGoals) {
            output.state = "Winning ";
        } else if (plyGoals < enemyGoals) {
            output.state = "Losing ";
        } else {
            output.state = "Tied ";
        }
        output.state += plyGoals + " - " + enemyGoals;

        if (gameState.wrapper.GetbOverTime()) {
            output.remaining = NULL;
            if (!input.overtime) {
                output.overtime = true;
                output.start = time(0);
            }
            output.state += " (OT)";
        } else {
            if (!gameState.wrapper.GetbUnlimitedTime()) {
                output.remaining = gameState.wrapper.GetSecondsRemaining() + 1;
            }
        }
    }
    std::string map = GetMap(gameWrapper->GetCurrentMap());
    output.hover = map;
    output.image = FormatMap(map);
    return output;
}

Data DiscordRPCPlugin::HandleReplay(Data input, GameState gameState) {
    Data output = input;

    GameSettingPlaylistWrapper playlistWrapper = gameState.wrapper.GetPlaylist();
    std::string gameType;
    if (playlistWrapper.memory_address != NULL) {
        gameType = GetPlaylistName(playlistWrapper.GetPlaylistId(), gameState.wrapper.GetMaxTeamSize());
    }

    int teamGoals[2] = {0, 0};
    if (!gameState.wrapper.GetTeams().Get(0).IsNull() && !gameState.wrapper.GetTeams().Get(1).IsNull()) {
        teamGoals[0] = gameState.wrapper.GetTeams().Get(0).GetScore();
        teamGoals[1] = gameState.wrapper.GetTeams().Get(1).GetScore();
    }
    output.state = "(Blue) " + std::to_string(teamGoals[0]) + " - " + std::to_string(teamGoals[1]) + " (Red)";

    output.details = "Watching " + gameType;

    if (gameState.wrapper.GetbOverTime()) {
        output.remaining = NULL;
        if (!input.overtime) {
            output.overtime = true;
            output.start = time(0);
        }
        output.state += " (OT)";
    } else {
        if (!gameState.wrapper.GetbUnlimitedTime()) {
            output.start = time(0);
            output.remaining = output.start + gameState.wrapper.GetSecondsRemaining();
        }
    }

    std::string map = GetMap(gameWrapper->GetCurrentMap());
    output.hover = map;
    output.image = FormatMap(map);

    return output;
}

Data DiscordRPCPlugin::HandleFreeplay(Data input, GameState gameState) {
    Data output = input;

    output.details = "In freeplay";

    std::string map = GetMap(gameWrapper->GetCurrentMap());
    output.hover = map;
    output.image = FormatMap(map);

    return output;
}

Data DiscordRPCPlugin::HandleTraining(Data input, GameState gameState) {
    Data output = input;
    
    TrainingEditorWrapper tew = gameState.wrapper.memory_address;
    
    if (!tew.IsNull()) {
        if (tew.GetTrainingFileName().ToString() == "") {
            int currentShot = gameState.wrapper.GetRoundNum() + 1;
            output.details = "In training";
            cvarManager->log("c");
            output.state = "Shot " + std::to_string(currentShot) + "/10"; // yeah I know I shouldn't hardcode the 10 but it works as of 3/17/21
            cvarManager->log("d");
        } else {
            auto gesdw = tew.GetTrainingData();
            if (gesdw.memory_address != NULL) {
                auto tesdw = tew.GetTrainingData().GetTrainingData();
                if (tesdw.memory_address != NULL) {
                    int currentShot = tew.GetActiveRoundNumber() + 1;
                    std::string currentName = Capitalize(tesdw.GetTM_Name().ToString());
                    output.details = "In custom training";
                    output.state = currentName + " (Shot " + std::to_string(currentShot) + "/" + std::to_string(tew.GetTotalRounds()) + ")";
                }
            }
            
        }
    }
    
    cvarManager->log("e");
    std::string map = GetMap(gameWrapper->GetCurrentMap());
    output.hover = map;
    output.image = FormatMap(map);

    return output;
}

Data DiscordRPCPlugin::HandleSpectate(Data input, GameState gameState) {
    Data output = input;

    GameSettingPlaylistWrapper playlistWrapper = gameState.wrapper.GetPlaylist();
    std::string gameType;
    if (playlistWrapper.memory_address != NULL) {
        gameType = GetPlaylistName(playlistWrapper.GetPlaylistId(), gameState.wrapper.GetMaxTeamSize());
    }

    int teamGoals[2] = {0, 0};
    if (!gameState.wrapper.GetTeams().Get(0).IsNull() && !gameState.wrapper.GetTeams().Get(1).IsNull()) {
        teamGoals[0] = gameState.wrapper.GetTeams().Get(0).GetScore();
        teamGoals[1] = gameState.wrapper.GetTeams().Get(1).GetScore();
    }
    output.state = "(Blue) " + std::to_string(teamGoals[0]) + " - " + std::to_string(teamGoals[1]) + " (Red)";

    output.details = "Watching " + gameType;

    if (gameState.wrapper.GetbOverTime()) {
        output.remaining = NULL;
        if (!input.overtime) {
            output.overtime = true;
            output.start = time(0);
        }
        output.state += " (OT)";
    } else {
        if (!gameState.wrapper.GetbUnlimitedTime()) {
            output.remaining = gameState.wrapper.GetSecondsRemaining() + 1;
        }
    }

    std::string map = GetMap(gameWrapper->GetCurrentMap());
    output.hover = map;
    output.image = FormatMap(map);

    return output;
}

void DiscordRPCPlugin::CheckGameType(Data input) {
    if (!cvarManager->getCvar("rpc_enabled").getBoolValue()) return;
    GameState gameState = GetCurrentGameType();
    Data output = input;
    if (input.type != gameState.type) {
        output.type = gameState.type;
        output.start = time(0);
        output.remaining = NULL;
    }
    if (gameState.type == GAMETYPE_MENU) {
        output.details = "On main menu";
        output.state = "";
        output.image = "logo";
        output.hover = "";
        output.smallImage = "";
        output.smallHover = "";
    }
    if (!gameState.wrapper.IsNull()) {
        switch (gameState.type) {
            case (GAMETYPE_ONLINE): {
                if (cvarManager->getCvar("rpc_show_online_games").getBoolValue()) {
                    output = DiscordRPCPlugin::HandleGame(output, gameState);
                }
                break;
            }
            case (GAMETYPE_REPLAY): {
                if (cvarManager->getCvar("rpc_show_replays").getBoolValue()) {
                    output = DiscordRPCPlugin::HandleReplay(output, gameState);
                }
                break;
            }
            case (GAMETYPE_FREEPLAY): {
                if (cvarManager->getCvar("rpc_show_freeplay").getBoolValue()) {
                    output = DiscordRPCPlugin::HandleFreeplay(output, gameState);
                }
                break;
            }
            case (GAMETYPE_TRAINING): {
                if (cvarManager->getCvar("rpc_show_training").getBoolValue()) {
                    output = DiscordRPCPlugin::HandleTraining(output, gameState);
                }
                break;
            }
            case (GAMETYPE_SPECTATE): {
                if (cvarManager->getCvar("rpc_show_spectating").getBoolValue()) {
                    output = DiscordRPCPlugin::HandleSpectate(output, gameState);
                }
                break;
            }
            case (GAMETYPE_LOCAL): {
                if (cvarManager->getCvar("rpc_show_local_games").getBoolValue()) {
                    output = DiscordRPCPlugin::HandleGame(output, gameState);
                }
            }
        }
    }

    updateRPC(output);
}

void DiscordRPCPlugin::updateRPC(Data input) {
    char detailBuffer[128];
    char stateBuffer[128];
    char imageBuffer[128];
    char hoverBuffer[128];
    char smallImageBuffer[128];
    char smallHoverBuffer[128];
    strcpy_s(detailBuffer, input.details.c_str());
    strcpy_s(stateBuffer, input.state.c_str());
    strcpy_s(imageBuffer, input.image.c_str());
    strcpy_s(hoverBuffer, input.hover.c_str());
    strcpy_s(smallImageBuffer, input.smallImage.c_str());
    strcpy_s(smallHoverBuffer, input.smallHover.c_str());
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = stateBuffer;
    discordPresence.details = detailBuffer;
    discordPresence.startTimestamp = input.start;
    if (input.remaining != NULL) {
        discordPresence.endTimestamp = time(0) + input.remaining;
    }
    discordPresence.largeImageKey = imageBuffer;
    discordPresence.largeImageText = hoverBuffer;
    discordPresence.smallImageKey = smallImageBuffer;
    discordPresence.smallImageText = smallHoverBuffer;
    Discord_UpdatePresence(&discordPresence);
    gameWrapper->SetTimeout(std::bind(&DiscordRPCPlugin::CheckGameType, this, input), 1.0f);
}

void DiscordRPCPlugin::OnEnableChange(std::string eventName) {
    cvarManager->log(std::string("OnEnableChange") + eventName);
    bool enabled = cvarManager->getCvar("rpc_enabled").getBoolValue();
    if (enabled) {
        discordInit();
    } else {
        Discord_Shutdown();
    }
}

void DiscordRPCPlugin::onLoad() {
    cvarManager->registerCvar("rpc_enabled", "1", "Enable Discord RPC").addOnValueChanged(bind(&DiscordRPCPlugin::OnEnableChange, this, std::placeholders::_1));
    cvarManager->registerCvar("rpc_show_online_games", "1", "Show online games in Rich Presence");
    cvarManager->registerCvar("rpc_show_replays", "1", "Show replay viewing in Rich Presence");
    cvarManager->registerCvar("rpc_show_freeplay", "1", "Show freeplay in Rich Presence");
    cvarManager->registerCvar("rpc_show_training", "1", "Show custom training in Rich Presence");
    cvarManager->registerCvar("rpc_show_spectating", "1", "Show spectating online games in Rich Presence");
    cvarManager->registerCvar("rpc_show_local_games", "1", "Show local games in Rich Presence");
    discordInit();
    Data input = {
        -1,       // int type
        time(0),  // int64_t start
        NULL,     // int64_t remaining
        false,    // bool overtime
        0,        // int team
        "",       // string details
        "",       // string state
        "",       // string image
        "",       // string hover
        "",       // string smallImage
        ""        // string smallHover
    };
    CheckGameType(input);
}

void DiscordRPCPlugin::onUnload() {
    Discord_Shutdown();
}
