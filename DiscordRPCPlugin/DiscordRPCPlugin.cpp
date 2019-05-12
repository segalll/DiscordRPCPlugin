#include "DiscordRPCPlugin.h"
#include "discord_rpc.h"
#include "bakkesmod\wrappers\GameEvent\ServerWrapper.h"
#include "bakkesmod\wrappers\ReplayServerWrapper.h"
#include "bakkesmod\wrappers\GameEvent\GameSettingPlaylistWrapper.h"
#include "bakkesmod\wrappers\GameEvent\TrainingEditorWrapper.h"
#include "bakkesmod\wrappers\GameEvent\SaveData\TrainingEditorSaveDataWrapper.h"
#include "bakkesmod\wrappers\GameEvent\GameEventWrapper.h"
#include "time.h"

BAKKESMOD_PLUGIN(DiscordRPCPlugin, "Discord Rich Presence Integration", "1.0", PLUGINTYPE_THREADED)

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

Data DiscordRPCPlugin::HandleOnlineGame(Data input, GameState gameState) {
	Data output = input;
	GameSettingPlaylistWrapper playlistWrapper = gameState.wrapper.GetPlaylist();
	string teamSize = to_string(gameState.wrapper.GetMaxTeamSize());
	string gameType;
	int teamGoals[2] = { 0, 0 };
	if (!gameState.wrapper.GetTeams().Get(0).IsNull() && !gameState.wrapper.GetTeams().Get(1).IsNull()) {
		teamGoals[0] = gameState.wrapper.GetTeams().Get(0).GetScore();
		teamGoals[1] = gameState.wrapper.GetTeams().Get(1).GetScore();
	}
	switch (playlistWrapper.GetPlaylistId()) {
		case(1):
			gameType = "Casual Duel";
			break;
		case(2):
			gameType = "Casual Doubles";
			break;
		case(3):
			gameType = "Casual Standard";
			break;
		case(4):
			gameType = "Casual Chaos";
			break;
		case(6):
			gameType = "Private " + teamSize + "v" + teamSize;
			break;
		case(10):
			gameType = "Ranked Duel";
			break;
		case(11):
			gameType = "Ranked Doubles";
			break;
		case(12):
			gameType = "Ranked Solo Standard";
			break;
		case(13):
			gameType = "Ranked Standard";
			break;
		case(14):
			gameType = "Mutator Mashup";
			break;
		case(22):
			gameType = "Tournament " + teamSize + "v" + teamSize;
			break;
		case(27):
			gameType = "Ranked Hoops";
			break;
		case(28):
			gameType = "Ranked Rumble";
			break;
		case(29):
			gameType = "Ranked Dropshot";
			break;
		case(30):
			gameType = "Ranked Snowday";
			break;
	}
	CarWrapper localPlayer = gameWrapper->GetLocalCar();
	if (!localPlayer.IsNull()) {
		output.team = localPlayer.GetPRI().GetTeamNum();
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
		output.end = NULL;
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
			output.end = NULL;
			if (!input.overtime) {
				output.overtime = true;
				output.start = time(0);
			}
			output.state += " (OT)";
		}
		else {
			if (!gameState.wrapper.GetbUnlimitedTime()) {
				output.start = time(0);
				output.end = output.start + gameState.wrapper.GetSecondsRemaining();
				cvarManager->log("Input: " + to_string(input.end - input.start));
				cvarManager->log("Output: " + to_string(output.end - output.start));
			}
		}
	}
	return output;
}

Data DiscordRPCPlugin::HandleReplay(Data input, GameState gameState) {
	Data output = input;
	GameSettingPlaylistWrapper playlistWrapper = gameState.wrapper.GetPlaylist();
	string teamSize = to_string(gameState.wrapper.GetMaxTeamSize());
	if (playlistWrapper.GetbRanked()) {
		output.details = "Watching Ranked " + teamSize + "v" + teamSize;
	}
	else if (playlistWrapper.IsPrivateMatch()) {
		output.details = "Watching Private " + teamSize + "v" + teamSize;
	}
	else if (playlistWrapper.GetbStandard()) {
		output.details = "Watching Casual " + teamSize + "v" + teamSize;
	}
	int teamGoals[2] = { 0, 0 };
	if (!gameState.wrapper.GetTeams().Get(0).IsNull() && !gameState.wrapper.GetTeams().Get(1).IsNull()) {
		teamGoals[0] = gameState.wrapper.GetTeams().Get(0).GetScore();
		teamGoals[1] = gameState.wrapper.GetTeams().Get(1).GetScore();
	}
	output.state = "(Blue) " + to_string(teamGoals[0]) + " - " + to_string(teamGoals[1]) + " (Red)";

	if (gameState.wrapper.GetbOverTime()) {
		output.end = NULL;
		if (!input.overtime) {
			output.overtime = true;
			output.start = time(0);
		}
		output.state += " (OT)";
	}
	else {
		if (!gameState.wrapper.GetbUnlimitedTime()) {
			output.start = time(0);
			output.end = output.start + gameState.wrapper.GetSecondsRemaining();
		}
	}
	return output;
}

Data DiscordRPCPlugin::HandleFreeplay(Data input, GameState gameState) {
	Data output = input;
	output.details = "In freeplay";
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
			output.hover = currentName + " (Shot " + to_string(currentShot) + "/" + to_string(tew.GetTotalRounds()) + ")";
	}
	return output;
}

Data DiscordRPCPlugin::HandleSpectate(Data input, GameState gameState) {
	Data output = input;
	GameSettingPlaylistWrapper playlistWrapper = gameState.wrapper.GetPlaylist();
	string teamSize = to_string(gameState.wrapper.GetMaxTeamSize());
	if (playlistWrapper.GetbRanked()) {
		output.details = "Spectating Ranked " + teamSize + "v" + teamSize;
	}
	else if (playlistWrapper.IsPrivateMatch()) {
		output.details = "Spectating Private " + teamSize + "v" + teamSize;
	}
	else if (playlistWrapper.GetbStandard()) {
		output.details = "Spectating Casual " + teamSize + "v" + teamSize;
	}

	int teamGoals[2] = { 0, 0 };
	if (!gameState.wrapper.GetTeams().Get(0).IsNull() && !gameState.wrapper.GetTeams().Get(1).IsNull()) {
		teamGoals[0] = gameState.wrapper.GetTeams().Get(0).GetScore();
		teamGoals[1] = gameState.wrapper.GetTeams().Get(1).GetScore();
	}
	output.state = "(Blue) " + to_string(teamGoals[0]) + " - " + to_string(teamGoals[1]) + " (Red)";

	if (gameState.wrapper.GetbOverTime()) {
		output.end = NULL;
		if (!input.overtime) {
			output.overtime = true;
			output.start = time(0);
		}
		output.state += " (OT)";
	}
	else {
		if (!gameState.wrapper.GetbUnlimitedTime()) {
			output.start = time(0);
			output.end = output.start + gameState.wrapper.GetSecondsRemaining();
		}
	}
	return output;
}
void DiscordRPCPlugin::CheckGameType(Data input) {
	if (!cvarManager->getCvar("rpc_enabled").getBoolValue()) return;
	GameState gameState = GetCurrentGameType();
	Data output = input;
	if (input.type != gameState.type) {
		output.type = gameState.type;
		output.start = time(0);
		output.end = NULL;
	}
	if (gameState.type == GAMETYPE_MENU) {
		output.details = "On main menu";
		output.state = "";
		output.hover = "";
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
	char hoverBuffer[128];
	strcpy_s(detailBuffer, input.details.c_str());
	strcpy_s(stateBuffer, input.state.c_str());
	strcpy_s(hoverBuffer, input.hover.c_str());
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.state = stateBuffer;
	discordPresence.details = detailBuffer;
	discordPresence.startTimestamp = input.start;
	discordPresence.endTimestamp = input.end;
	discordPresence.largeImageKey = "logo";
	discordPresence.largeImageText = hoverBuffer;
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
	Data input = {6, time(0), NULL, false, 0, "", "", ""}; //int type; int64_t start; int64_t end; bool overtime; int team; string details; string state; string hover;
	CheckGameType(input);
}

void DiscordRPCPlugin::onUnload()
{
	Discord_Shutdown();
}
