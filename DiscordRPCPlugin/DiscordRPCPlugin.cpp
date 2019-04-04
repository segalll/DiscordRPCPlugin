#include "DiscordRPCPlugin.h"
#include "discord_rpc.h"
#include "bakkesmod\wrappers\GameEvent\ServerWrapper.h"
#include "bakkesmod\wrappers\ReplayServerWrapper.h"
#include "bakkesmod\wrappers\GameEvent\GameSettingPlaylistWrapper.h"
#include "bakkesmod\wrappers\GameEvent\TrainingEditorWrapper.h"
#include "bakkesmod\wrappers\GameEvent\SaveData\TrainingEditorSaveDataWrapper.h"
#include "bakkesmod\wrappers\GameEvent\GameEventWrapper.h"
#include "time.h"

BAKKESMOD_PLUGIN(DiscordRPCPlugin, "Discord RPC plugin", "1.0", PLUGINTYPE_THREADED)

static const char* APPLICATION_ID = "562098317547405322";

static void discordInit()
{
	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	Discord_Initialize(APPLICATION_ID, &handlers, 1, NULL);
}

struct DiscordRPCPlugin::GameState {
	string name;
	ServerWrapper wrapper;
};

string Capitalize(string str) {
	str[0] = toupper(str[0]);
	for (int i = 1; i < str.length(); i++) {
		if (str[i - 1] == ' ') {
			str[i] = toupper(str[i]);
		}
	}
	return str;
}

DiscordRPCPlugin::GameState DiscordRPCPlugin::GetCurrentGameType() {
	if (gameWrapper->IsInReplay()) {
		return { "replay", gameWrapper->GetGameEventAsReplay() };
	} else if (gameWrapper->IsInOnlineGame()) {
		return { "online", gameWrapper->GetOnlineGame() };
	} else if (gameWrapper->IsInFreeplay()) {
		return { "freeplay", gameWrapper->GetGameEventAsServer() };
	} else if (gameWrapper->IsInCustomTraining()) {
		return { "training", gameWrapper->GetGameEventAsServer() };
	} else if (gameWrapper->IsSpectatingInOnlineGame()) {
		return { "spectate", gameWrapper->GetOnlineGame() };
	} else {
		return { "menu", NULL };
	}
}

void DiscordRPCPlugin::updateRPC(string prevCheck = "", int64_t startTime = time(0), int64_t endTime = NULL, bool prevOvertime = false, int localPlayerTeam = 0) {
	if (!cvarManager->getCvar("enableRPC").getBoolValue()) return;

	DiscordRPCPlugin::GameState gameState = GetCurrentGameType();
	string details;
	string hover;
	string state;
	char detailBuffer[128];
	char hoverBuffer[128];
	char stateBuffer[128];
	if (prevCheck != gameState.name) {
		startTime = time(0);
	}
	if (gameState.name == "menu") {
		prevCheck = "menu";
		details = "On main menu";
	}
	if (!gameState.wrapper.IsNull()) {
		if (gameState.name == "online" && cvarManager->getCvar("showOnlineGames").getBoolValue()) {
			prevCheck = gameState.name;
			GameSettingPlaylistWrapper playlistWrapper = gameState.wrapper.GetPlaylist();
			string teamSize = to_string(gameState.wrapper.GetMaxTeamSize());
			string gameType;
			if (!gameState.wrapper.GetTeams().Get(0).IsNull() && !gameState.wrapper.GetTeams().Get(1).IsNull()) {
				int teamGoals[2] = { 0, 0 };
				teamGoals[0] = gameState.wrapper.GetTeams().Get(0).GetScore();
				teamGoals[1] = gameState.wrapper.GetTeams().Get(1).GetScore();
				if (playlistWrapper.GetbRanked()) {
					gameType = "Ranked";
				}
				else if (playlistWrapper.IsPrivateMatch()) {
					gameType = "Private";
				}
				else if (playlistWrapper.GetbStandard()) {
					gameType = "Casual";
				}

				if (gameState.wrapper.GetbMatchEnded()) {
					details = "In " + gameType + " " + teamSize + "v" + teamSize + " lobby";
					startTime = NULL;
					CarWrapper localPlayer = gameWrapper->GetLocalCar();
					if (!localPlayer.IsNull()) {
						localPlayerTeam = localPlayer.GetPRI().GetTeamNum();
					}
					string plyGoals;
					string enemyGoals;
					if (localPlayerTeam == 0) {
						plyGoals = to_string(teamGoals[0]);
						enemyGoals = to_string(teamGoals[1]);
					}
					else if (localPlayerTeam == 1) {
						enemyGoals = to_string(teamGoals[0]);
						plyGoals = to_string(teamGoals[1]);
					}
					if (plyGoals > enemyGoals) {
						state = "Won ";
					}
					else {
						state = "Lost ";
					}
					state += "(" + plyGoals + " - " + enemyGoals + ")";
				}
				else {
					details = "Playing " + gameType + " " + teamSize + "v" + teamSize;
					CarWrapper localPlayer = gameWrapper->GetLocalCar();
					if (!localPlayer.IsNull()) {
						localPlayerTeam = localPlayer.GetPRI().GetTeamNum();
					}
					string plyGoals;
					string enemyGoals;
					if (localPlayerTeam == 0) {
						plyGoals = to_string(teamGoals[0]);
						enemyGoals = to_string(teamGoals[1]);
					}
					else if (localPlayerTeam == 1) {
						enemyGoals = to_string(teamGoals[0]);
						plyGoals = to_string(teamGoals[1]);
					}
					if (plyGoals > enemyGoals) {
						state = "Winning ";
					}
					else if (plyGoals < enemyGoals) {
						state = "Losing ";
					}
					else {
						state = "Tied ";
					}
					state += plyGoals + " - " + enemyGoals;

					if (gameState.wrapper.GetbOverTime()) {
						endTime = NULL;
						if (!prevOvertime) {
							prevOvertime = true;
							startTime = time(0);
						}
						state += " (OT)";
					}
					else {
						if (!gameState.wrapper.GetbUnlimitedTime()) {
							if (gameState.wrapper.GetWaitTimeRemaining() == 0) {
								startTime = time(0);
								endTime = startTime + gameState.wrapper.GetSecondsRemaining();
							}
						}
					}
				}
			}
		}
		else if (gameState.name == "replay" && cvarManager->getCvar("showReplayViewing").getBoolValue()) {
			prevCheck = gameState.name;
			details = "hi";
			GameSettingPlaylistWrapper playlistWrapper = gameState.wrapper.GetPlaylist();
			string teamSize = to_string(gameState.wrapper.GetMaxTeamSize());
			if (playlistWrapper.GetbRanked()) {
				details = "Watching Ranked " + teamSize + "v" + teamSize;
			}
			else if (playlistWrapper.IsPrivateMatch()) {
				details = "Watching Private " + teamSize + "v" + teamSize;
			}
			else if (playlistWrapper.GetbStandard()) {
				details = "Watching Casual " + teamSize + "v" + teamSize;
			}
			
			if (!gameState.wrapper.GetTeams().Get(0).IsNull() && !gameState.wrapper.GetTeams().Get(1).IsNull()) {
				int teamGoals[2] = { 0, 0 };
				teamGoals[0] = gameState.wrapper.GetTeams().Get(0).GetScore();
				teamGoals[1] = gameState.wrapper.GetTeams().Get(1).GetScore();
				state = "(Blue) " + to_string(teamGoals[0]) + " - " + to_string(teamGoals[1]) + " (Red)";

				if (gameState.wrapper.GetbOverTime()) {
					endTime = NULL;
					if (!prevOvertime) {
						prevOvertime = true;
						startTime = time(0);
					}
					state += " (OT)";
				}
				else {
					if (!gameState.wrapper.GetbUnlimitedTime()) {
						if (gameState.wrapper.GetWaitTimeRemaining() == 0) {
							startTime = time(0);
							endTime = startTime + gameState.wrapper.GetSecondsRemaining();
						}
					}
				}
			}
		}
		else if (gameState.name == "freeplay" && cvarManager->getCvar("showFreeplay").getBoolValue()) {
			prevCheck = gameState.name;
			details = "In freeplay";
		}
		else if (gameState.name == "training" && cvarManager->getCvar("showCustomTraining").getBoolValue()) {
			prevCheck = gameState.name;
			TrainingEditorWrapper tew = gameState.wrapper.memory_address;
			TrainingEditorSaveDataWrapper tesdw = tew.GetTrainingData().GetTrainingData();
			int currentShot = tew.GetActiveRoundNumber() + 1;
			string currentName = Capitalize(tesdw.GetTM_Name().ToString());
			details = "In custom training";
			hover = currentName + " (Shot " + to_string(currentShot) + "/" + to_string(tew.GetTotalRounds()) + ")";
		}
		else if (gameState.name == "spectate" && cvarManager->getCvar("showOnlineSpectating").getBoolValue()) {
			prevCheck = gameState.name;
			GameSettingPlaylistWrapper playlistWrapper = gameState.wrapper.GetPlaylist();
			string teamSize = to_string(gameState.wrapper.GetMaxTeamSize());
			if (playlistWrapper.GetbRanked()) {
				details = "Spectating Ranked " + teamSize + "v" + teamSize;
			}
			else if (playlistWrapper.IsPrivateMatch()) {
				details = "Spectating Private " + teamSize + "v" + teamSize;
			}
			else if (playlistWrapper.GetbStandard()) {
				details = "Spectating Casual " + teamSize + "v" + teamSize;
			}

			int teamGoals[2] = { 0, 0 };
			teamGoals[0] = gameState.wrapper.GetReplayDirector().GetReplay().GetTeam0Score();
			teamGoals[1] = gameState.wrapper.GetReplayDirector().GetReplay().GetTeam1Score();
			state = "(Blue) " + to_string(teamGoals[0]) + " - " + to_string(teamGoals[1]) + " (Red)";

			if (gameState.wrapper.GetbOverTime()) {
				if (!prevOvertime) {
					prevOvertime = true;
					startTime = time(0);
				}
				state += " in overtime";
			}
			else {
				if (!gameState.wrapper.GetbUnlimitedTime()) {
					startTime = time(0);
					endTime = startTime + gameState.wrapper.GetSecondsRemaining();
				}
			}
		}
	}
	strcpy_s(detailBuffer, details.c_str());
	strcpy_s(hoverBuffer, hover.c_str());
	strcpy_s(stateBuffer, state.c_str());
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.state = stateBuffer;
	discordPresence.details = detailBuffer;
	discordPresence.startTimestamp = startTime;
	discordPresence.endTimestamp = endTime;
	discordPresence.largeImageKey = "logo";
	discordPresence.largeImageText = hoverBuffer;
	Discord_UpdatePresence(&discordPresence);
	gameWrapper->SetTimeout(bind(&DiscordRPCPlugin::updateRPC, this, prevCheck, startTime, endTime, prevOvertime, localPlayerTeam), 0.5f);
}

void DiscordRPCPlugin::OnEnableChange(string eventName) {
	cvarManager->log(string("OnEnableChange") + eventName);
	bool enabled = cvarManager->getCvar("enableRPC").getBoolValue();
	if (enabled) {
		discordInit();
		updateRPC();
	}
	else {
		Discord_Shutdown();
	}
}

void DiscordRPCPlugin::onLoad()
{
	cvarManager->registerCvar("enableRPC", "1", "Enable Discord RPC").addOnValueChanged(bind(&DiscordRPCPlugin::OnEnableChange, this, placeholders::_1));
	cvarManager->registerCvar("showOnlineGames", "1", "Should show online games in Rich Presence");
	cvarManager->registerCvar("showReplayViewing", "1", "Should show replay viewing in Rich Presence");
	cvarManager->registerCvar("showFreeplay", "1", "Should show freeplay in Rich Presence");
	cvarManager->registerCvar("showCustomTraining", "1", "Should show custom training in Rich Presence");
	cvarManager->registerCvar("showOnlineSpectating", "1", "Should show spectating online games in Rich Presence");
	discordInit();
	updateRPC();
}

void DiscordRPCPlugin::onUnload()
{
	Discord_Shutdown();
}
