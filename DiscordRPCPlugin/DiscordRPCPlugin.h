#pragma once
#pragma comment( lib, "BakkesMod.lib" )
#pragma comment( lib, "discord-rpc.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"

class DiscordRPCPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
public:
	virtual void onLoad();
	virtual void onUnload();
	void updateRPC(string prevCheck, int64_t startTime, int64_t endTime, bool prevOvertime, int localPlayerTeam);
	void OnEnableChange(string eventName);
	struct GameState;
	GameState GetCurrentGameType();
};