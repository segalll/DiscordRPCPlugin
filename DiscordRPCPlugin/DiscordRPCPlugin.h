#pragma once
#pragma comment( lib, "BakkesMod.lib" )
#pragma comment( lib, "discord-rpc.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod\wrappers\GameEvent\ServerWrapper.h"

struct GameState {
	int type;
	ServerWrapper wrapper;
};

struct Data {
	int type;
	int64_t start;
	int64_t end;
	bool overtime;
	int team;
	string details;
	string state;
	string hover;
};

class DiscordRPCPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
public:
	virtual void onLoad();
	virtual void onUnload();
	void OnEnableChange(string eventName);
	void CheckGameType(Data input);
	void updateRPC(Data input);
	GameState GetCurrentGameType();
	Data HandleOnlineGame(Data input, GameState gameState);
	Data HandleReplay(Data input, GameState gameState);
	Data HandleFreeplay(Data input, GameState gameState);
	Data HandleTraining(Data input, GameState gameState);
	Data HandleSpectate(Data input, GameState gameState);
};