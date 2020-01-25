#include "DiscordRPCPlugin.h"
#include "discord_rpc.h"
#include "bakkesmod\wrappers\GameEvent\ServerWrapper.h"
#include "bakkesmod\wrappers\ReplayServerWrapper.h"
#include "bakkesmod\wrappers\GameEvent\GameSettingPlaylistWrapper.h"
#include "bakkesmod\wrappers\GameEvent\TrainingEditorWrapper.h"
#include "bakkesmod\wrappers\GameEvent\SaveData\TrainingEditorSaveDataWrapper.h"
#include "bakkesmod\wrappers\GameEvent\GameEventWrapper.h"
#include <ctime>
#include <algorithm>

BAKKESMOD_PLUGIN(DiscordRPCPlugin, "Discord Rich Presence Integration", "1.0", PLUGINTYPE_FREEPLAY)

static const char* APPLICATION_ID = "562098317547405322";

static void discordInit()
{
	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	Discord_Initialize(APPLICATION_ID, &handlers, 1, NULL);
}

string Capitalize(string str) {
	str[0] = toupper(str[0]);
	for (int i = 1; i < str.length(); i++) {
		if (str[i - 1] == ' ') {
			str[i] = toupper(str[i]);
		} else {
			str[i] = tolower(str[i]);
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
	GAMETYPE_MENU = 5
};

GameState DiscordRPCPlugin::GetCurrentGameType() {
	if (gameWrapper->IsInReplay()) {
		return { GAMETYPE_REPLAY, gameWrapper->GetGameEventAsReplay() };
	} else if (gameWrapper->IsInOnlineGame()) {
		return { GAMETYPE_ONLINE, gameWrapper->GetOnlineGame() };
	} else if (gameWrapper->IsInFreeplay()) {
		return { GAMETYPE_FREEPLAY, gameWrapper->GetGameEventAsServer() };
	} else if (gameWrapper->IsInCustomTraining()) {
		return { GAMETYPE_TRAINING, gameWrapper->GetGameEventAsServer() };
	} else if (gameWrapper->IsSpectatingInOnlineGame()) {
		return { GAMETYPE_SPECTATE, gameWrapper->GetOnlineGame() };
	} else {
		return { GAMETYPE_MENU, NULL };
	}
}

string GetPlaylistName(int playlistId, int teamSize) {
	string size = to_string(teamSize);
	switch (playlistId) {
		case(1):
			return "Casual Duel";
			break;
		case(2):
			return "Casual Doubles";
			break;
		case(3):
			return "Casual Standard";
			break;
		case(4):
			return "Casual Chaos";
			break;
		case(6):
			return "Private " + size + "v" + size;
			break;
		case(10):
			return "Ranked Duel";
			break;
		case(11):
			return "Ranked Doubles";
			break;
		case(12):
			return "Ranked Solo Standard";
			break;
		case(13):
			return "Ranked Standard";
			break;
		case(14):
			return "Mutator Mashup";
			break;
		case(22):
			return "Tournament " + size + "v" + size;
			break;
		case(27):
			return "Ranked Hoops";
			break;
		case(28):
			return "Ranked Rumble";
			break;
		case(29):
			return "Ranked Dropshot";
			break;
		case(30):
			return "Ranked Snowday";
			break;
		default:
			return "";
			break;
	}
}

string GetRank(int tierID) {
	switch (tierID) {
		case(0):
			return "Unranked";
			break;
		case(1):
			return "Bronze 1";
			break;
		case(2):
			return "Bronze 2";
			break;
		case(3):
			return "Bronze 3";
			break;
		case(4):
			return "Silver 1";
			break;
		case(5):
			return "Silver 2";
			break;
		case(6):
			return "Silver 3";
			break;
		case(7):
			return "Gold 1";
			break;
		case(8):
			return "Gold 2";
			break;
		case(9):
			return "Gold 3";
			break;
		case(10):
			return "Platinum 1";
			break;
		case(11):
			return "Platinum 2";
			break;
		case(12):
			return "Platinum 3";
			break;
		case(13):
			return "Diamond 1";
			break;
		case(14):
			return "Diamond 2";
			break;
		case(15):
			return "Diamond 3";
			break;
		case(16):
			return "Champion 1";
			break;
		case(17):
			return "Champion 2";
			break;
		case(18):
			return "Champion 3";
			break;
		case(19):
			return "Grand Champion";
			break;
		default:
			return "";
			break;
	}
}

string GetMap(string mapName) {
	transform(mapName.begin(), mapName.end(), mapName.begin(), ::tolower);
	if (mapName == "arc_p") return "ARCtagon";
	else if (mapName == "arc_standard_p") return "Starbase ARC";
	else if (mapName == "beach_p") return "Salty Shores";
	else if (mapName == "beach_night_p") return "Salty Shores (Night)";
	else if (mapName == "chn_stadium_p") return "Forbidden Temple";
	else if (mapName == "cs_day_p") return "Champions Field (Day)";
	else if (mapName == "cs_hw_p") return "Rivals Arena";
	else if (mapName == "cs_p") return "Champions Field";
	else if (mapName == "eurostadium_night_p") return "Mannfield (Night)";
	else if (mapName == "eurostadium_p") return "Mannfield";
	else if (mapName == "eurostadium_rainy_p") return "Mannfield (Stormy)";
	else if (mapName == "eurostadium_snownight_p") return "Mannfield (Snowy)";
	else if (mapName == "farm_p") return "Farmstead";
	else if (mapName == "farm_night_p") return "Farmstead (Night)";
	else if (mapName == "farm_upsidedown_p") return "Farmstead (The Upside Down)";
	else if (mapName == "hoopsstadium_p") return "Dunk House";
	else if (mapName == "labs_circlepillars_p") return "Pillars";
	else if (mapName == "labs_cosmic_p") return "Cosmic (Old)";
	else if (mapName == "labs_cosmic_v4_p") return "Cosmic";
	else if (mapName == "labs_doublegoal_p") return "Double Goal (Old)";
	else if (mapName == "labs_doublegoal_v2_p") return "Double Goal";
	else if (mapName == "labs_octagon_02_p") return "Octagon";
	else if (mapName == "labs_octagon_p") return "Octagon (Old)";
	else if (mapName == "labs_underpass_p") return "Underpass (Old)";
	else if (mapName == "labs_underpass_v0_p") return "Underpass";
	else if (mapName == "labs_utopia_p") return "Utopia Retro";
	else if (mapName == "neotokyo_p") return "Tokyo Underpass";
	else if (mapName == "neotokyo_standard_p") return "Neo Tokyo";
	else if (mapName == "park_night_p") return "Beckwith Park (Midnight)";
	else if (mapName == "park_p") return "Beckwith Park";
	else if (mapName == "park_rainy_p") return "Beckwith Park (Stormy)";
	else if (mapName == "shattershot_p") return "Core 707";
	else if (mapName == "stadium_day_p") return "DFH Stadium (Day)";
	else if (mapName == "stadium_foggy_p") return "DFH Stadium (Stormy)";
	else if (mapName == "stadium_p") return "DFH Stadium";
	else if (mapName == "stadium_winter_p") return "DFH Stadium (Snowy)";
	else if (mapName == "throwbackstadium_p") return "Throwback Stadium";
	else if (mapName == "trainstation_dawn_p") return "Urban Central (Dawn)";
	else if (mapName == "trainstation_night_p") return "Urban Central (Night)";
	else if (mapName == "trainstation_p") return "Urban Central";
	else if (mapName == "haunted_trainstation_p") return "Urban Central (Haunted)";
	else if (mapName == "underwater_p") return "AquaDome";
	else if (mapName == "utopiastadium_dusk_p") return "Utopia Coliseum (Dusk)";
	else if (mapName == "utopiastadium_p") return "Utopia Coliseum";
	else if (mapName == "utopiastadium_snow_p") return "Utopia Coliseum (Snowy)";
	else if (mapName == "wasteland_night_p") return "Badlands (Night)";
	else if (mapName == "wasteland_night_s_p") return "Wasteland (Night)";
	else if (mapName == "wasteland_p") return "Badlands";
	else if (mapName == "wasteland_s_p") return "Wasteland";
	else return "logo";
}

string FormatMap(string map) {
	transform(map.begin(), map.end(), map.begin(), ::tolower);
	transform(map.begin(), map.end(), map.begin(), [](char ch) {
		return ch == ' ' ? '-' : ch;
	});
	map.erase(remove(map.begin(), map.end(), '('), map.end());
	map.erase(remove(map.begin(), map.end(), ')'), map.end());
	return map;
}

Data DiscordRPCPlugin::HandleOnlineGame(Data input, GameState gameState) {
	Data output = input;
	
	GameSettingPlaylistWrapper playlistWrapper = gameState.wrapper.GetPlaylist();

	string gameType;
	if (playlistWrapper.memory_address != NULL) {
		gameType = GetPlaylistName(playlistWrapper.GetPlaylistId(), gameState.wrapper.GetMaxTeamSize());
	}
	
	int teamGoals[2] = { 0, 0 };
	if (!gameState.wrapper.GetTeams().Get(0).IsNull() && !gameState.wrapper.GetTeams().Get(1).IsNull()) {
		teamGoals[0] = gameState.wrapper.GetTeams().Get(0).GetScore();
		teamGoals[1] = gameState.wrapper.GetTeams().Get(1).GetScore();
	}
	CarWrapper localPlayer = gameWrapper->GetLocalCar();
	if (!localPlayer.IsNull()) {
		output.team = localPlayer.GetPRI().GetTeamNum();
	}
	if (gameType.find("Ranked") != string::npos) {
		if (!localPlayer.IsNull() && playlistWrapper.memory_address != NULL) {
			SteamID playerID = localPlayer.GetPRI().GetUniqueId();
			int playlistID = playlistWrapper.GetPlaylistId();
			MMRWrapper mmrWrapper = gameWrapper->GetMMRWrapper();
			SkillRank rank = mmrWrapper.GetPlayerRank(playerID, playlistID);
			float mmr = mmrWrapper.GetPlayerMMR(playerID, playlistID);
			string rankStr = GetRank(rank.Tier);
			if (rankStr.length() > 0) {
				output.smallHover = rankStr + " (MMR: " + to_string((int) mmr) + ")";
				transform(rankStr.begin(), rankStr.end(), rankStr.begin(), ::tolower);
				rankStr.erase(remove_if(rankStr.begin(), rankStr.end(), ::isspace), rankStr.end());
				output.smallImage = rankStr;
			}
		}
	}

	string plyGoals;
	string enemyGoals;
	if (output.team == 0) {
		plyGoals = to_string(teamGoals[0]);
		enemyGoals = to_string(teamGoals[1]);
	}
	else if (output.team == 1) {
		enemyGoals = to_string(teamGoals[0]);
		plyGoals = to_string(teamGoals[1]);
	}
	
	if (gameState.wrapper.GetbMatchEnded()) {
		output.details = "In " + gameType + " lobby";
		output.start = NULL;
		output.remaining = NULL;
		if (plyGoals > enemyGoals) {
			output.state = "Won ";
		}
		else {
			output.state = "Lost ";
		}
		output.state += "(" + plyGoals + " - " + enemyGoals + ")";
	}
	else {
		output.details = "Playing " + gameType;
		if (plyGoals > enemyGoals) {
			output.state = "Winning ";
		}
		else if (plyGoals < enemyGoals) {
			output.state = "Losing ";
		}
		else {
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
		}
		else {
			if (!gameState.wrapper.GetbUnlimitedTime()) {
				output.remaining = gameState.wrapper.GetSecondsRemaining() + 1;
			}
		}
	}
	string map = GetMap(gameWrapper->GetCurrentMap());
	output.hover = map;
	output.image = FormatMap(map);
	return output;
}

Data DiscordRPCPlugin::HandleReplay(Data input, GameState gameState) {
	Data output = input;
	
	GameSettingPlaylistWrapper playlistWrapper = gameState.wrapper.GetPlaylist();
	string gameType;
	if (playlistWrapper.memory_address != NULL) {
		gameType = GetPlaylistName(playlistWrapper.GetPlaylistId(), gameState.wrapper.GetMaxTeamSize());
	}
	
	int teamGoals[2] = { 0, 0 };
	if (!gameState.wrapper.GetTeams().Get(0).IsNull() && !gameState.wrapper.GetTeams().Get(1).IsNull()) {
		teamGoals[0] = gameState.wrapper.GetTeams().Get(0).GetScore();
		teamGoals[1] = gameState.wrapper.GetTeams().Get(1).GetScore();
	}
	output.state = "(Blue) " + to_string(teamGoals[0]) + " - " + to_string(teamGoals[1]) + " (Red)";
	
	output.details = "Watching " + gameType;

	if (gameState.wrapper.GetbOverTime()) {
		output.remaining = NULL;
		if (!input.overtime) {
			output.overtime = true;
			output.start = time(0);
		}
		output.state += " (OT)";
	}
	else {
		if (!gameState.wrapper.GetbUnlimitedTime()) {
			output.start = time(0);
			output.remaining = output.start + gameState.wrapper.GetSecondsRemaining();
		}
	}

	string map = GetMap(gameWrapper->GetCurrentMap());
	output.hover = map;
	output.image = FormatMap(map);

	return output;
}

Data DiscordRPCPlugin::HandleFreeplay(Data input, GameState gameState) {
	Data output = input;

	output.details = "In freeplay";

	string map = GetMap(gameWrapper->GetCurrentMap());
	output.hover = map;
	output.image = FormatMap(map);

	return output;
}

Data DiscordRPCPlugin::HandleTraining(Data input, GameState gameState) {
	Data output = input;
	TrainingEditorWrapper tew = gameState.wrapper.memory_address;
	if (!tew.IsNull()) {
			TrainingEditorSaveDataWrapper tesdw = tew.GetTrainingData().GetTrainingData();
			int currentShot = tew.GetActiveRoundNumber() + 1;
			string currentName = Capitalize(tesdw.GetTM_Name().ToString());
			output.details = "In custom training";
			output.state = currentName + " (Shot " + to_string(currentShot) + "/" + to_string(tew.GetTotalRounds()) + ")";
	}

	string map = GetMap(gameWrapper->GetCurrentMap());
	output.hover = map;
	output.image = FormatMap(map);

	return output;
}

Data DiscordRPCPlugin::HandleSpectate(Data input, GameState gameState) {
	Data output = input;
	
	GameSettingPlaylistWrapper playlistWrapper = gameState.wrapper.GetPlaylist();
	string gameType;
	if (playlistWrapper.memory_address != NULL) {
		gameType = GetPlaylistName(playlistWrapper.GetPlaylistId(), gameState.wrapper.GetMaxTeamSize());
	}
	
	int teamGoals[2] = { 0, 0 };
	if (!gameState.wrapper.GetTeams().Get(0).IsNull() && !gameState.wrapper.GetTeams().Get(1).IsNull()) {
		teamGoals[0] = gameState.wrapper.GetTeams().Get(0).GetScore();
		teamGoals[1] = gameState.wrapper.GetTeams().Get(1).GetScore();
	}
	output.state = "(Blue) " + to_string(teamGoals[0]) + " - " + to_string(teamGoals[1]) + " (Red)";
	
	output.details = "Watching " + gameType;

	if (gameState.wrapper.GetbOverTime()) {
		output.remaining = NULL;
		if (!input.overtime) {
			output.overtime = true;
			output.start = time(0);
		}
		output.state += " (OT)";
	}
	else {
		if (!gameState.wrapper.GetbUnlimitedTime()) {
			output.remaining = gameState.wrapper.GetSecondsRemaining() + 1;
		}
	}

	string map = GetMap(gameWrapper->GetCurrentMap());
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
			case(GAMETYPE_ONLINE): {
				if (cvarManager->getCvar("rpc_show_online_games").getBoolValue()) {
					output = DiscordRPCPlugin::HandleOnlineGame(output, gameState);
				}
				break;
			}
			case(GAMETYPE_REPLAY): {
				if (cvarManager->getCvar("rpc_show_replays").getBoolValue()) {
					output = DiscordRPCPlugin::HandleReplay(output, gameState);
				}
				break;
			}
			case(GAMETYPE_FREEPLAY): {
				if (cvarManager->getCvar("rpc_show_freeplay").getBoolValue()) {
					output = DiscordRPCPlugin::HandleFreeplay(output, gameState);
				}
				break;
			}
			case(GAMETYPE_TRAINING): {
				if (cvarManager->getCvar("rpc_show_training").getBoolValue()) {
					output = DiscordRPCPlugin::HandleTraining(output, gameState);
				}
				break;
			}
			case(GAMETYPE_SPECTATE): {
				if (cvarManager->getCvar("rpc_show_spectating").getBoolValue()) {
					output = DiscordRPCPlugin::HandleSpectate(output, gameState);
				}
				break;
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
	gameWrapper->SetTimeout(bind(&DiscordRPCPlugin::CheckGameType, this, input), 1.0f);
}

void DiscordRPCPlugin::OnEnableChange(string eventName) {
	cvarManager->log(string("OnEnableChange") + eventName);
	bool enabled = cvarManager->getCvar("rpc_enabled").getBoolValue();
	if (enabled) {
		discordInit();
	}
	else {
		Discord_Shutdown();
	}
}

void DiscordRPCPlugin::onLoad()
{
	cvarManager->registerCvar("rpc_enabled", "1", "Enable Discord RPC").addOnValueChanged(bind(&DiscordRPCPlugin::OnEnableChange, this, placeholders::_1));
	cvarManager->registerCvar("rpc_show_online_games", "1", "Should show online games in Rich Presence");
	cvarManager->registerCvar("rpc_show_replays", "1", "Should show replay viewing in Rich Presence");
	cvarManager->registerCvar("rpc_show_freeplay", "1", "Should show freeplay in Rich Presence");
	cvarManager->registerCvar("rpc_show_training", "1", "Should show custom training in Rich Presence");
	cvarManager->registerCvar("rpc_show_spectating", "1", "Should show spectating online games in Rich Presence");
	discordInit();
	Data input = {
		-1, // int type
		time(0), // int64_t start
		NULL, // int64_t remaining
		false, // bool overtime
		0, // int team
		"", // string details
		"", // string state
		"", // string image
		"", // string hover
		"", // string smallImage
		"" // string smallHover
	};
	CheckGameType(input);
}

void DiscordRPCPlugin::onUnload()
{
	Discord_Shutdown();
}
